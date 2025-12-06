#pragma once
#include<game/Game.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<JSON/json.hpp>
#include<filesystem>
#include<string.h>

using json = nlohmann::json;

void Game::start() {
	//Init first state with board_size size, black to move
	state.initializer(game_config.board_size);
	game_end = false;
	std::cout << "Game initializer\n";
}

void Game::pass() {
	Move move(-1, -1, state.current_player, 1);
	std::vector<Cell> capture;
	state.setNextPlayer();
	history.addMove(move, state.current_board, capture);
	if (history.checkConsecutivePass()) game_end = true;
}

void Game::undo() {
	history.undoMove(state.current_board, state.current_player);
}

void Game::redo() {
	history.redoMove(state.current_board, state.current_player);
}

void Game::reset() {
	//Init same as the start of the game
	state.initializer(game_config.board_size);
	//Clear all undo, redo, and board history
	history.clear();   
	game_end = false;
}

void Game::placeStone(int x, int y) {
	Move move(x, y, state.current_player, 0);
	std::vector<Cell> capture;
	if (state.validateMove(move, capture) == 0) return;
	for (auto i : capture) {
		//std::cout << i.x << " " << i.y << " " << (i.state == CellState::Black ? "Black" : "White") << "\n";
		state.current_board[i.x][i.y] = CellState::Empty;
	}
	if (history.checkSuperKO(state.current_board)) {
		state.current_board[x][y] = CellState::Empty;
		for (auto i : capture) {
			state.current_board[i.x][i.y] = i.state;
		}
		return;
	}
	history.addMove(move, state.current_board, capture);
	state.setNextPlayer();
	//std::cout << "Placing stone in: " << x << " " << y << "\n";
	//state.printPlayer();
}

std::pair<float, float> Game::getScore() {
	int _black_score = 0;
	int _white_score = 0;
	const float KOMI = 7.5;

	state.current_board.calculateScore(state.current_board, _black_score, _white_score);
	return std::make_pair(_black_score, _white_score + KOMI);
}

void to_json(json& j, const Move& move) {
	j = json{
		{"player", move.player},
		{"pass", move.pass},
		{"x", move.x},
		{"y", move.y}
	};
}

void to_json(json& j, const GameCfg& config) {
	j = json{
		{"board_size", config.board_size},
		{"prev_state", config.prev_state},
		{"state", config.state},
		{"game_mode", config.game_mode},
		{"sound_theme", config.sound_theme},
		{"board_design", config.board_design},
		{"stone_design", config.stone_design},
		{"background_music", config.background_music},
		{"placing_stone", config.placing_stone},
		{"capturing_stone", config.capturing_stone},
		{"end_game_sound", config.end_game_sound}
	};
}

void from_json(const json& j, Move& move) {
	move.player = j.at("player").get<CellState>();
	move.pass = j.at("pass").get<bool>();
	move.x = j.at("x").get<int>();
	move.y = j.at("y").get<int>();
}

void from_json(const json& j, GameCfg& config) {
	config.board_size = j.at("board_size").get<int>();
	config.prev_state = j.at("prev_state").get<GameState>();
	config.state = j.at("state").get<GameState>();
	config.game_mode = j.at("game_mode").get<GameMode>();
	config.sound_theme = j.at("sound_theme").get<SoundTheme>();
	config.board_design = j.at("board_design").get<BoardDesign>();
	config.stone_design = j.at("stone_design").get<StoneDesign>();
	config.background_music = j.at("background_music").get<int>();
	config.placing_stone = j.at("placing_stone").get<int>();
	config.capturing_stone = j.at("capturing_stone").get<int>();
	config.end_game_sound = j.at("end_game_sound").get<int>();
}

bool Game::saveGame(const std::string& name) {
	std::vector<Move> move_history = history.getMoveHistory();
	json j_move_history = move_history;
	json j_config = Game::game_config;

	std::string path = std::string(SAVEGAME_DIR) + '/' + name;
	std::ofstream fout(path);
	if (!fout.is_open()) {
		std::cout << "Cannot save file: " << name << "\n";
		return 0;
	}
	fout << j_config.dump(4) << "\n";
	fout << j_move_history.dump(4) << "\n";
	fout.close();
	return 1;
}

bool Game::loadGame(const std::string& name) {
	std::string path = std::string(SAVEGAME_DIR) + '/' + name;
	std::ifstream fin(path);
	if (!fin.is_open()) {
		std::cout << "Cannot load file: " << name << "\n";
		return 0;
	}
	json json_move_history;
	json json_config;
	fin >> json_config;
	fin >> json_move_history;
	std::vector<Move> move_history = json_move_history;
	game_config = json_config;
	Game::reset();
	history.loadFromMoveList(move_history, state.current_board, state.current_player);
	fin.close();
	return 1;
}

bool Game::loadPreviewGame(const std::string& name) {
	std::string path = std::string(SAVEGAME_DIR) + '/' + name;
	std::ifstream fin(path);
	if (!fin.is_open()) {
		std::cout << "Cannot load file to preview: " << name << "\n";
		return 0;
	}
	json json_move_history;
	json json_config;
	fin >> json_config;
	fin >> json_move_history;
	std::vector<Move> move_history = json_move_history;
	previewBoard = Board(json_config.at("board_size").get<int>());
	previewBoard.loadPreviewFromMoveList(move_history, previewBoard);
	fin.close();
	return 1;
}

std::vector<std::string> Game::loadAllSaveGameName() {
	std::vector<std::string> save_game_names;
	if (!std::filesystem::exists(SAVEGAME_DIR) || !std::filesystem::is_directory(SAVEGAME_DIR)) {
		std::cerr << "Error: Folder '" << SAVEGAME_DIR << "' does not exist or is not a directory." << std::endl;
		
		return save_game_names;
	}

	for (const auto& entry : std::filesystem::directory_iterator(SAVEGAME_DIR)) {
		if (entry.is_regular_file()) {
			save_game_names.push_back(entry.path().filename().string());
		}
	}
	return save_game_names;
}

bool Game::deleteSaveGame(const std::string& name) {
	std::string path = std::string(SAVEGAME_DIR) + '/' + name;
	if (!std::filesystem::exists(path)) {
		std::cout << "Cannot delete file: " << name << " (file does not exist)\n";
		return 0;
	}
	std::error_code ec;
	if (!std::filesystem::remove(path, ec)) {
		std::cout << "Error deleting file: " << ec.message() << "\n";
		return 0;
	}
	return 1;
}

int Game::getMoveListSize() {
	return history.getMoveListSize();
}

Board Game::getKthBoard(const int k) {
	return history.getKthBoard(k);
}

std::string Game::getLastMove() {
	return history.getLastMove(game_config.board_size);
}

//debug purpose
void Game::print() {
	state.printPlayer();
	state.current_board.printBoard(state.current_board);
}