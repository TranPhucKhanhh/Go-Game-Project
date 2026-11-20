#include<game/Game.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<JSON/json.hpp>
#include<filesystem>
#include<string.h>
#include<algorithm>
#include<cmath>
#include<stdexcept>

using json = nlohmann::json;

void Game::initializeClock() {
	black_seconds = white_seconds = game_config.main_time_seconds;
	clock_paused = true;
	timed_out_player = CellState::Empty;
	clock_updated_at = std::chrono::steady_clock::now();
}

void Game::updateClock() {
	const auto now = std::chrono::steady_clock::now();
	const double elapsed = std::chrono::duration<double>(now - clock_updated_at).count();
	clock_updated_at = now;
	if (clock_paused || game_end || scoring_active || game_config.main_time_seconds <= 0) return;
	double& remaining = state.current_player == CellState::Black ? black_seconds : white_seconds;
	remaining = std::max(0.0, remaining - elapsed);
	if (remaining <= 0.0) {
		timed_out_player = state.current_player;
		resigned_player = CellState::Empty;
		game_end = true;
	}
}

void Game::setClockPaused(bool paused) {
	updateClock(); // Settle the old running interval before changing pause state.
	clock_paused = paused;
}

void Game::initializeScoring() {
	scoring_active = score_confirmed = false;
	scoring_pass_floor = 0;
	dead_stones.assign(game_config.board_size, std::vector<bool>(game_config.board_size, false));
}

void Game::beginScoringIfNeeded() {
	if (!game_end && history.getMoveListSize() - scoring_pass_floor >= 2 && history.checkConsecutivePass()) {
		scoring_active = true;
		clock_paused = true;
		clock_updated_at = std::chrono::steady_clock::now();
	}
}

void Game::toggleDeadGroup(int x, int y) {
	if (!scoring_active) return;
	const GroupInfo group = state.current_board.inspectGroup(x, y);
	if (group.stones.empty()) return;
	const bool marked = !dead_stones[x][y];
	for (const Cell& cell : group.stones) dead_stones[cell.x][cell.y] = marked;
}

void Game::confirmScore() {
	if (!scoring_active) return;
	scoring_active = false;
	score_confirmed = true;
	game_end = true;
}

void Game::resumePlay() {
	if (!scoring_active) return;
	scoring_active = false;
	scoring_pass_floor = history.getMoveListSize();
	dead_stones.assign(game_config.board_size, std::vector<bool>(game_config.board_size, false));
	clock_updated_at = std::chrono::steady_clock::now();
}

void Game::start() {
	//Init first state with board_size size, black to move
	state.initializer(game_config.board_size);
	resigned_player = CellState::Empty;
	game_end = false;
	initializeClock();
	initializeScoring();
	std::cout << "Game initializer\n";
	history.clear();
	if (game_config.game_mode != GameMode::PvP) {
		GameMode level = game_config.game_mode;
		AI.startGame(game_config.board_size, level, game_config.komi);
		if (game_config.AI_side == CellState::Black) {
			setClockPaused(false);
			Game::placeStoneAI();
			setClockPaused(true);
		}
	}
}

void Game::pass() {
	updateClock();
	if (game_end || scoring_active) return;
	Move move(-1, -1, state.current_player, 1);
	std::vector<Cell> capture;
	if (game_config.game_mode != GameMode::PvP) AI.syncMove(move);
	state.setNextPlayer();
	history.addMove(move, state.current_board, capture);
	clock_updated_at = std::chrono::steady_clock::now();
	beginScoringIfNeeded();
	if (game_config.game_mode != GameMode::PvP && !game_end && !scoring_active) {
		Game::placeStoneAI();
	}
}

void Game::undo() {
	updateClock();
	if (game_end || scoring_active) return;
	if (game_config.game_mode != GameMode::PvP) {
		history.undoMoveAIMode(state.current_board, state.current_player, AI);
		scoring_pass_floor = std::min(scoring_pass_floor, history.getMoveListSize());
		clock_updated_at = std::chrono::steady_clock::now();
		return;
	}
	history.undoMove(state.current_board, state.current_player);
	scoring_pass_floor = std::min(scoring_pass_floor, history.getMoveListSize());
	clock_updated_at = std::chrono::steady_clock::now();
}

void Game::redo() {
	updateClock();
	if (game_end || scoring_active) return;
	if (game_config.game_mode != GameMode::PvP) {
		history.redoMoveAIMode(state.current_board, state.current_player, AI);
		clock_updated_at = std::chrono::steady_clock::now();
		beginScoringIfNeeded();
		return;
	}
	history.redoMove(state.current_board, state.current_player);
	clock_updated_at = std::chrono::steady_clock::now();
	beginScoringIfNeeded();
}

void Game::reset() {
	//Init same as the start of the game
	state.initializer(game_config.board_size);
	//Clear all undo, redo, and board history
	history.clear();   
	resigned_player =  CellState::Empty;
	game_end = false;
	initializeClock();
	initializeScoring();
	if (game_config.game_mode != GameMode::PvP) {
		GameMode level = game_config.game_mode;
		AI.startGame(game_config.board_size, level, game_config.komi);
		if (game_config.AI_side == CellState::Black) {
			setClockPaused(false);
			Game::placeStoneAI();
			setClockPaused(true);
		}
	}
}

void Game::placeStone(int x, int y) {
	updateClock();
	last_move_verdict = MoveVerdict::Invalid;
	if (game_end || scoring_active) return;
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
	clock_updated_at = std::chrono::steady_clock::now();
	//std::cout << "Placing stone in: " << x << " " << y << "\n";
	//state.printPlayer();
}

void Game::placeStoneAI() {
	updateClock();
	if (game_end || scoring_active) return;
	Move move = AI.genMove(state.current_player, resigned_player);
	// Charge the full search interval before accepting the result, even when rendering was blocked.
	updateClock();
	if (game_end) return;
	//std::cout << move.x << " " << move.y << "\n";
	if (resigned_player != CellState::Empty) {
		game_end = 1;
		return;
	}
	if (move.pass) {
		std::vector<Cell> capture;
		history.addMove(move, state.current_board, capture);
		state.setNextPlayer();
		beginScoringIfNeeded();
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
	if (scoring_active || score_confirmed) {
		const AreaScore score = getScoringBreakdown();
		return { static_cast<float>(score.black_stones + score.black_territory),
			score.white_stones + score.white_territory + game_config.komi };
	}
	int black_score = 0;
	int white_score = 0;
	float KOMI = game_config.komi;
	state.current_board.calculateScore(state.current_board, black_score, white_score);
	if (resigned_player == CellState::Black) return std::make_pair(-1, white_score + KOMI);
	if (resigned_player == CellState::White) return std::make_pair(black_score, -1);
	return std::make_pair(black_score, white_score + KOMI);
}

std::pair<float, float> Game::getScoreFromBoard(Board board) {
	int _black_score = 0;
	int _white_score = 0;
	const float KOMI = game_config.komi;

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
		{"game_mode", config.game_mode},
		{"komi", config.komi},
		{"main_time_seconds", config.main_time_seconds}
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
	config.komi = j.value("komi", 7.5f);
	config.main_time_seconds = j.value("main_time_seconds", 0);
	if (!std::isfinite(config.komi) || config.komi < -50.f || config.komi > 50.f ||
		std::abs(config.komi * 2.f - std::round(config.komi * 2.f)) > 0.0001f ||
		config.main_time_seconds < 0 || config.main_time_seconds > 86400)
		throw std::invalid_argument("Invalid saved komi or time limit");
}

bool Game::saveGame(const std::string& name) {
	updateClock();
	std::vector<Move> move_history = history.getMoveHistory();
	json j_move_history = move_history;
	json j_config = Game::game_config;
	j_config["clock"] = {{"black_seconds", black_seconds}, {"white_seconds", white_seconds},
		{"timed_out_player", timed_out_player}};
	j_config["game_end"] = game_end;
	j_config["resigned_player"] = resigned_player;
	json marked = json::array();
	for (int x = 0; x < state.current_board.size(); ++x)
		for (int y = 0; y < state.current_board.size(); ++y)
			if (dead_stones[x][y]) marked.push_back({{"x", x}, {"y", y}});
	j_config["scoring"] = {{"active", scoring_active}, {"confirmed", score_confirmed},
		{"pass_floor", scoring_pass_floor}, {"dead_stones", marked}};
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
	GameCfg loaded_config = json_config;
	if (loaded_config.board_size != 9 && loaded_config.board_size != 13 && loaded_config.board_size != 19) return false;
	for (const Move& move : move_history) {
		if (move.player != CellState::Black && move.player != CellState::White) return false;
		if (!move.pass && (move.x < 0 || move.y < 0 || move.x >= loaded_config.board_size || move.y >= loaded_config.board_size))
			return false;
	}
	double loaded_black = loaded_config.main_time_seconds;
	double loaded_white = loaded_config.main_time_seconds;
	CellState loaded_timeout = CellState::Empty;
	const CellState loaded_resignation = json_config.value("resigned_player", CellState::Empty);
	const bool loaded_end = json_config.value("game_end", false);
	if (json_config.contains("clock")) {
		const auto& clock = json_config.at("clock");
		loaded_black = clock.at("black_seconds").get<double>();
		loaded_white = clock.at("white_seconds").get<double>();
		loaded_timeout = clock.value("timed_out_player", CellState::Empty);
	}
	if (!std::isfinite(loaded_black) || !std::isfinite(loaded_white) || loaded_black < 0 || loaded_white < 0 ||
		loaded_black > loaded_config.main_time_seconds || loaded_white > loaded_config.main_time_seconds ||
		(loaded_timeout != CellState::Empty && loaded_timeout != CellState::Black && loaded_timeout != CellState::White) ||
		(loaded_resignation != CellState::Empty && loaded_resignation != CellState::Black && loaded_resignation != CellState::White) ||
		(loaded_timeout == CellState::Black && (loaded_black > 0 || loaded_config.main_time_seconds == 0)) ||
		(loaded_timeout == CellState::White && (loaded_white > 0 || loaded_config.main_time_seconds == 0)))
		return false;
	bool loaded_scoring = false, loaded_confirmed = false;
	int loaded_floor = 0;
	std::vector<std::vector<bool>> loaded_dead(loaded_config.board_size,
		std::vector<bool>(loaded_config.board_size, false));
	if (json_config.contains("scoring")) {
		const auto& scoring = json_config.at("scoring");
		loaded_scoring = scoring.at("active").get<bool>();
		loaded_confirmed = scoring.at("confirmed").get<bool>();
		loaded_floor = scoring.at("pass_floor").get<int>();
		if (loaded_floor < 0 || loaded_floor > static_cast<int>(move_history.size()) ||
			(loaded_scoring && (loaded_confirmed || loaded_end)) || (loaded_confirmed && !loaded_end)) return false;
		const auto& marked = scoring.at("dead_stones");
		if (!marked.is_array()) return false;
		for (const auto& point : marked) {
			const int x = point.at("x").get<int>(), y = point.at("y").get<int>();
			if (x < 0 || y < 0 || x >= loaded_config.board_size || y >= loaded_config.board_size ||
				loaded_dead[x][y] || (!loaded_scoring && !loaded_confirmed)) return false;
			loaded_dead[x][y] = true;
		}
	}
	// Reconstruct and check the scoring mask before replacing the current match.
	Board loaded_board(loaded_config.board_size);
	CellState loaded_player = CellState::Black;
	GameHistory loaded_history;
	loaded_history.loadFromMoveList(move_history, loaded_board, loaded_player);
	if ((loaded_scoring || loaded_confirmed) &&
		(!loaded_history.checkConsecutivePass() || static_cast<int>(move_history.size()) - loaded_floor < 2 ||
		 loaded_timeout != CellState::Empty || loaded_resignation != CellState::Empty)) return false;
	for (int x = 0; x < loaded_board.size(); ++x) {
		for (int y = 0; y < loaded_board.size(); ++y) {
			if (!loaded_dead[x][y]) continue;
			const GroupInfo group = loaded_board.inspectGroup(x, y);
			if (group.stones.empty()) return false;
			for (const Cell& stone : group.stones)
				if (!loaded_dead[stone.x][stone.y]) return false;
		}
	}
	game_config = loaded_config;
	state.current_board = std::move(loaded_board);
	state.current_player = loaded_player;
	history = std::move(loaded_history);
	initializeClock();
	initializeScoring();
	scoring_active = loaded_scoring;
	score_confirmed = loaded_confirmed;
	scoring_pass_floor = loaded_floor;
	dead_stones = std::move(loaded_dead);
	black_seconds = loaded_black;
	white_seconds = loaded_white;
	timed_out_player = loaded_timeout;
	resigned_player = loaded_resignation;
	game_end = loaded_end ||
		resigned_player != CellState::Empty || timed_out_player != CellState::Empty;
	beginScoringIfNeeded();
	// Timed saves with a zero active clock cannot resume play, even if an old result field is missing.
	if (!game_end && !scoring_active && game_config.main_time_seconds > 0 && getRemainingSeconds(state.current_player) <= 0) {
		timed_out_player = state.current_player;
		game_end = true;
	}
	if (game_config.game_mode != GameMode::PvP && !game_end) {
		AI.loadSavedGame(move_history, game_config.board_size, game_config.game_mode, game_config.komi);
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

std::string Game::getKthMove(const int k) {
	return history.getKthMove(k, game_config.board_size);
}

Board Game::getKthBoard(const int k) {
	return previewBoard = history.getKthBoard(k);
}

std::string Game::getLastMove() {
	return history.getLastMove(game_config.board_size);
}

void Game::resign() {
	updateClock();
	if (game_end || scoring_active) return;
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
