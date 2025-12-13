#pragma once
#include<game/History.h>
#include<iostream>
#include<algorithm>

static std::string conv[19] = { "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T"};

bool GameHistory::checkSuperKO(const Board& current, MoveVerdict& last_move_verdict) {
	for (Board b : board) {
		if (current == b) {
			//std::cout << "OH FUCK\n";
			last_move_verdict = MoveVerdict::SuperKO;
			return 1;
		}
	}
	return 0;
}

void GameHistory::addMove(const Move& m, const Board& current_board, const std::vector<Cell>& turn_capture) {
	move.push_back(m);
	capture.push_back(turn_capture);
	undo.clear();
	restore.clear();
	board.push_back(current_board);
}

void GameHistory::undoMove(Board& current_board, CellState& current_player) {
	if (move.empty()) return;
	if (current_player == CellState::Black) current_player = CellState::White;
	else current_player = CellState::Black;

	Move m = move.back(); move.pop_back();
	undo.push_back(m);

	if (m.pass == 1) return;

	std::vector<Cell> restore_list = capture.back(); capture.pop_back();
	restore.push_back(restore_list);
	
	current_board[m.x][m.y] = CellState::Empty;
	for (auto i : restore_list) {
		int x = i.x;
		int y = i.y;
		current_board[x][y] = i.state;
	}
	board.pop_back();
}

void GameHistory::redoMove(Board& current_board, CellState& current_player) {
	if (undo.empty()) return;
	if (current_player == CellState::Black) current_player = CellState::White;
	else current_player = CellState::Black;

	Move m = undo.back(); undo.pop_back();
	move.push_back(m);

	if (m.pass == 1) return;

	std::vector<Cell> capture_list = restore.back(); restore.pop_back();
	capture.push_back(capture_list);

	current_board[m.x][m.y] = m.player;
	for (auto i : capture_list) {
		int x = i.x;
		int y = i.y;
		current_board[x][y] = CellState::Empty;
	}
	board.push_back(current_board);
}

void GameHistory::clear() {
	board.clear();
	capture.clear();
	move.clear();
	restore.clear();
	undo.clear();
}

bool GameHistory::checkConsecutivePass() {
	if (move.size() <= 1) return 0;
	return (move.back().pass == 1 && move[int(move.size()) - 2].pass == 1);
}

void GameHistory::loadFromMoveList(const std::vector<Move>& move_list, Board& current_board, CellState& current_player) {
	GameHistory::clear();
	for (const auto& m : move_list) {
		std::vector<Cell> turn_capture;
		current_player = m.player == CellState::Black ? CellState::White : CellState::Black;
		if (m.pass == 0) current_board.placeStoneWithoutValidating(m, current_board, turn_capture);
		for (const Cell& i : turn_capture) {
			current_board[i.x][i.y] = i.state;
		}
		GameHistory::addMove(m, current_board, turn_capture);
	}
}

void GameHistory::loadPreviewFromMoveList(const std::vector<Move>& move_list, Board& current_board) {
	for (const auto& m : move_list) {
		std::vector<Cell> turn_capture;
		if (m.pass == 0) current_board.placeStoneWithoutValidating(m, current_board, turn_capture);
		for (const Cell& i : turn_capture) {
			current_board[i.x][i.y] = i.state;
		}
	}
}

std::vector<Move> GameHistory::getMoveHistory() {
	return move;
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

std::string GameHistory::getLastMove(const int board_size) {
	if (move.empty()) return "";
	Move m = move.back();
	std::string s = "";
	std::string lastPlayer = (m.player == CellState::Black ? "b" : "w");
	if (m.pass) {
		s = lastPlayer + " pass";
		return s;
	}
	s = lastPlayer + " " + conv[m.x] + intToString(board_size - m.y);
	//std::cout << s << "\n";
	//std::cout << m.x << " " << m.y << "\n";
	return s;
}

std::vector<std::string> GameHistory::getFullMove(const int board_size) {
	if (move.empty()) return { };

	std::vector<std::string> vec;

	for (const auto& m : move) {
		std::string s = "";
		std::string lastPlayer = (m.player == CellState::Black ? "b" : "w");
		if (m.pass) {
			s = lastPlayer + " pass";
			vec.push_back(s);
			continue;
		}
		s = lastPlayer + " " + conv[m.x] + intToString(board_size - m.y);
		vec.push_back(s);
		//std::cout << s << "\n";
		//std::cout << m.x << " " << m.y << "\n";
	}

	return vec;
}

int GameHistory::getMoveListSize() {
	return move.size();
}

Board GameHistory::getKthBoard(const int k) {
	return board[k];
}