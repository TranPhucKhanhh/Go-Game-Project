#include<game/History.h>
#include<iostream>

bool GameHistory::checkSuperKO(const Board& current) {
	for (Board b : board) {
		if (current == b) {
			//std::cout << "OH FUCK\n";
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