#pragma once
#include<AI/AI.h>
#include<string>
#include<iostream>

static std::string conv[19] = { "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T" };

static void parseResponse(std::string& s) {
	std::string res = "";
	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] == '\n' || s[i] == '\r') continue;
		if (s[i] == ' ') {
			if (i > 0 && (s[i - 1] == '\n' || s[i - 1] == ' ' || s[i - 1] == '\r')) continue;
		}
		res += s[i];
	}
	if (res.size() && res.back() == ' ') res.pop_back();
	s = res;
}

static std::string intToString(int n) {
	std::string s = "";
	while (n) {
		s += (n % 10) + '0';
		n /= 10;
	}
	std::reverse(s.begin(), s.end());
	return s;
}

AI::AI() {
	const std::string base = KATAGO_DIR;
	const std::string exe = base + "\\katago.exe";
	const std::string model = base + "\\model.bin.gz";
	const std::string cfg = base + "\\default_gtp.cfg";
	try {

		katago.start(
			".",
			exe,        // FULL PATH
			model,      // FULL PATH
			cfg,        // FULL PATH
			true
		);
	}
	catch (const std::exception& e) {
		std::cerr << "KATAGO_DIR = " << KATAGO_DIR << "\n";

		std::cerr << "Katago start failed: " << e.what() << std::endl;
		return;
	}
}

void AI::startGame(const int init_board_size, const GameMode init_level) {
	board_size = init_board_size;
	level = init_level;
	katago.send("komi 7.5");
	katago.readResponse();
	katago.send("kata-set-rules chinese");
	katago.readResponse();
	katago.send("boardsize " + intToString(board_size));
	katago.readResponse();
	katago.send("clear_board");
	katago.readResponse();
	katago.send("kata-set-rule ko POSITIONAL");
	katago.readResponse();
	if (level == GameMode::AIEasy) katago.send("kata-set-param maxVisits 1");
	if (level == GameMode::AIMedium) katago.send("kata-set-param maxVisits 10");
	if (level == GameMode::AIHard) katago.send("kata-set-param maxVisits 100");
	katago.readResponse();
}

void AI::resetGame() {
	katago.send("komi 7.5");
	katago.readResponse();
	katago.send("kata-set-rules chinese");
	katago.readResponse();
	katago.send("boardsize " + intToString(board_size));
	katago.readResponse();
	katago.send("clear_board");
	katago.readResponse(); 
	katago.send("kata-set-rule ko POSITIONAL");
	katago.readResponse();
	if (level == GameMode::AIEasy) katago.send("kata-set-param maxVisits 1");
	if (level == GameMode::AIMedium) katago.send("kata-set-param maxVisits 10");
	if (level == GameMode::AIHard) katago.send("kata-set-param maxVisits 100");
	katago.readResponse();
}

static std::string moveToString(const Move& m, const int board_size) {
	std::string res = "play ";
	res += (m.player == CellState::Black ? "B" : "W");
	res += " ";
	if (m.pass) {
		res += "pass";
		return res;
	}
	//(0,0) -> A(board_size)
	//(1,0) -> B(board_size)
	//(0,1) -> A(board_size - 1)
	res += conv[m.x] + intToString(board_size - m.y);
	return res;
}

void AI::syncMove(const Move& m) {
	std::string s = moveToString(m, board_size);
	katago.send(s);
	katago.readResponse();
}

static int stringToInt(const std::string s) {
	int res = 0;
	for (char c : s) {
		res = res * 10 + c - '0';
	}
	return res;
}

static Move stringToMove(const CellState& current_player, std::string& s, int board_size) {
	Move m;
	m.x = s[2] - 'A' - (s[2] >= 'J' ? 1 : 0);
	m.y = board_size - stringToInt(s.substr(3));
	std::cout << s.size() << "\n";
	//std::cout << s.substr(3) << " " << board_size - stringToInt(s.substr(3)) << "\n";
	m.pass = 0;
	m.player = current_player;
	return m;
}

Move AI::genMove(const CellState& current_player, CellState& resigned_player) {
	std::string s = "genmove ";
	s += (current_player == CellState::Black ? "B" : "W");
	katago.send(s);
	std::string response = katago.readResponse();
	parseResponse(response);
	std::cout << response << "\n";
	if (response == "= resign") {
		resigned_player = current_player;
		return Move(-1, -1, CellState::Empty, 0);
	}
	if (response == "= pass") {
		return Move(-1, -1, CellState::Empty, 1);
	}
	return stringToMove(current_player, response, board_size);
}

void AI::undo() {
	katago.send("undo");
	katago.readResponse();
}

void AI::loadSavedGame(const std::vector<Move>& move_history, const int init_board_size, const GameMode init_level) {
	AI::startGame(init_board_size, init_level);
	for (const Move& m : move_history) {
		AI::syncMove(m);
		//AI::showBoard();
	}
}

void AI::showBoard() {
	katago.send("showboard");
	std::string s = katago.readResponse();
	std::cout << s;
}