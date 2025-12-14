#pragma once
#include <game/Board.h>
#include <model/Cell.h>
#include <model/CellState.h>
#include <iostream>

struct InGameState {
	CellState current_player;
	Board current_board;
	InGameState() {}
	void initializer(int board_size) {
		this->current_player = CellState::Black;
		this->current_board = Board(board_size);
	}
	void setNextPlayer() {
		CellState next = this->current_player;
		if (next == CellState::Black) next = CellState::White;
		else next = CellState::Black;
		this->current_player = next;
	}
	CellState getNextPlayer() {
		CellState next = this->current_player;
		if (next == CellState::Black) next = CellState::White;
		else next = CellState::Black;
		return next;
	}
	void placeWithoutValidating(const Move& move, std::vector<Cell>& capture, MoveVerdict& last_move_verdict) {
		current_board.placeStoneWithoutValidating(move, current_board, capture);
		if (capture.size()) last_move_verdict = MoveVerdict::Capture;
		else last_move_verdict = MoveVerdict::Valid;
		//std::cout << "captured size: " << capture.size() << "\n";
		for (auto i : capture) {
			current_board[i.x][i.y] = CellState::Empty;
		}
	}
	bool validateMove(const Move& move, std::vector<Cell>& capture, MoveVerdict& last_move_verdict) {
		return current_board.validateMove(move, current_board, capture, last_move_verdict);
	}
	void printPlayer() {
		std::cout << "Current player is: ";
		std::cout << (this->current_player == CellState::Black ? "Black" : "White") << "\n";
	}
};