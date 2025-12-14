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
	resigned_player = CellState::Empty;
	game_end = false;
	std::cout << "Game initializer\n";
}

void Game::pass() {
	Move move(-1, -1, state.current_player, 1);
	std::vector<Cell> capture;
	state.setNextPlayer();
	history.addMove(move, state.current_board, capture);
	if (history.checkConsecutivePass()) game_end = true;
	if (game_config.game_mode != GameMode::PvP) {
		Game::placeStoneAI();
	}
}

void Game::undo() {
	if (game_config.game_mode != GameMode::PvP) {
		history.undoMoveAIMode(state.current_board, state.current_player, AI);
		return;
	}
	history.undoMove(state.current_board, state.current_player);
}

void Game::redo() {
	if (game_config.game_mode != GameMode::PvP) {
		history.redoMoveAIMode(state.current_board, state.current_player, AI);
		return;
	}
	history.redoMove(state.current_board, state.current_player);
}

void Game::reset() {
	//Init same as the start of the game
	state.initializer(game_config.board_size);
	if (game_config.game_mode != GameMode::PvP) {
		GameMode level = game_config.game_mode;
		AI.startGame(game_config.board_size, level);
		if (game_config.AI_side == CellState::Black) {
			Game::placeStoneAI();
		}
	}
	//Clear all undo, redo, and board history
	history.clear();   
	resigned_player =  CellState::Empty;
	game_end = false;
}

void Game::placeStone(int x, int y) {
	//std::cout << x << " " << y << "\n";
	Move move(x, y, state.current_player, 0);
	std::vector<Cell> capture;
	if (state.validateMove(move, capture, last_move_verdict) == 0) return;
	for (auto i : capture) {
		//std::cout << i.x << " " << i.y << " " << (i.state == CellState::Black ? "Black" : "White") << "\n";
		state.current_board[i.x][i.y] = CellState::Empty;
	}
	if (history.checkSuperKO(state.current_board, last_move_verdict)) {
		state.current_board[x][y] = CellState::Empty;
		for (auto i : capture) {
			state.current_board[i.x][i.y] = i.state;
		}
		return;
	}
	if (game_config.game_mode != GameMode::PvP) AI.syncMove(move);
	history.addMove(move, state.current_board, capture);
	state.setNextPlayer();
	//std::cout << "Placing stone in: " << x << " " << y << "\n";
	//state.printPlayer();
}

void Game::placeStoneAI() {
	Move move = AI.genMove(state.current_player, resigned_player);
	//std::cout << move.x << " " << move.y << "\n";
	if (resigned_player != CellState::Empty) {
		game_end = 1;
		return;
	}
	if (move.pass) {
		std::vector<Cell> capture;
		history.addMove(move, state.current_board, capture);
		if (history.checkConsecutivePass()) game_end = true;
		state.setNextPlayer();
		return;
	}
	std::vector<Cell> capture;
	state.placeWithoutValidating(move, capture, last_move_verdict);
	history.addMove(move, state.current_board, capture);
	state.setNextPlayer();
	//AI.showBoard();
	//Game::print();
}

std::pair<float, float> Game::getScore() {
	int black_score = 0;
	int white_score = 0;
	float KOMI = game_config.komi;
	state.current_board.calculateScore(state.current_board, black_score, white_score);
	if (resigned_player == CellState::Empty) return std::make_pair(black_score, white_score + KOMI);
	if (resigned_player == CellState::Black) return std::make_pair(-1, white_score + KOMI);
	if (resigned_player == CellState::White) return std::make_pair(black_score, -1);
}

std::pair<float, float> Game::getScoreFromBoard(Board board) {
	int _black_score = 0;
	int _white_score = 0;
	const float KOMI = 7.5;

	board.calculateScore(board, _black_score, _white_score);
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
		{"AI_side", config.AI_side},
		{"board_size", config.board_size},
		{"prev_state", config.prev_state},
		{"state", config.state},
		{"game_mode", config.game_mode}
	};
}

void from_json(const json& j, Move& move) {
	move.player = j.at("player").get<CellState>();
	move.pass = j.at("pass").get<bool>();
	move.x = j.at("x").get<int>();
	move.y = j.at("y").get<int>();
}

void from_json(const json& j, GameCfg& config) {
	config.AI_side = j.at("AI_side").get<CellState>();
	config.board_size = j.at("board_size").get<int>();
	config.prev_state = j.at("prev_state").get<GameState>();
	config.state = j.at("state").get<GameState>();
	config.game_mode = j.at("game_mode").get<GameMode>();
}

bool Game::saveGame(const std::string& name) {
	std::vector<Move> move_history = history.getMoveHistory();
	json j_move_history = move_history;
	json j_config = Game::game_config;
	namespace fs = std::filesystem;
	if (!fs::exists(SAVEGAME_DIR)) {
		fs::create_directories(SAVEGAME_DIR);
	}
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
	if (game_config.game_mode != GameMode::PvP) {
		AI.loadSavedGame(move_history, game_config.board_size, game_config.game_mode);
		//AI.showBoard();
	}
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
	return previewBoard = history.getKthBoard(k);
}

std::string Game::getLastMove() {
	return history.getLastMove(game_config.board_size);
}

void Game::resign() {
	resigned_player = state.current_player;
	game_end = 1;
}

//debug purpose
void Game::print() {
	state.printPlayer();
	state.current_board.printBoard(state.current_board);
}

std::string Game::getAILastMove() {
	return last_AI_move;
}

Board Game::getExampleBoard() {
	//std::cout << "Hello\n";
	if (example_board.size() != 13) {
		example_board = Board(13);
		//Place some stones for preview
		for (int i = 0; i < 13; i++) {
			if (i % 2) {
				example_board[i][i] = CellState::White;
			}
			else {
				example_board[i][i] = CellState::Black;
			}
		}
	}
	return example_board;
}