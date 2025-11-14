#pragma once
#include <Board.h>
#include <Cell.h>
#include <CellState.h>
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
	bool validateMove(const Move& move, std::vector<Cell>& capture) {
		return current_board.validateMove(move, current_board, capture);
	}
	void printPlayer() {
		std::cout << "Current player is: ";
		std::cout << (this->current_player == CellState::Black ? "Black" : "White") << "\n";
	}
};