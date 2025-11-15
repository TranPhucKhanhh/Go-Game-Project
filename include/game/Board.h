#pragma once
#include<model/CellState.h>
#include<model/Cell.h>
#include<model/Move.h>
#include<vector>
#include<iostream>

class Board {
public:
	Board() = default;
	Board(int board_size) :
		grid(std::vector<std::vector<CellState>>(board_size, std::vector<CellState>(board_size, CellState::Empty))) {
	}
	int size() {
		return grid.size();
	}
	int size() const {
		return grid.size();
	}
	//Allow to use [][] for easy access
	std::vector<CellState>& operator[](int i) { return grid[i]; }
	const std::vector<CellState>& operator[](int i) const { return grid[i]; }
	//Allow to use = to assign value
	//This is move value
	Board& operator = (Board&& a) noexcept {
		this->grid = std::move(a.grid);
		return *this;
	}
	//This is copy value
	Board& operator = (const Board& a) noexcept {
		this->grid = a.grid;
		return *this;
	}
	//Allow to use == to compare value
	bool operator == (const Board& a) const {
		for (int i = 0; i < a.grid.size(); i++)
			for (int j = 0; j < a.grid.size(); j++)
				if (grid[i][j] != a.grid[i][j]) return 0;
		return 1;
	}
	//This is copy constructor
	Board(const Board& other) noexcept = default;
	//This is move constructor
	Board(Board&& other) noexcept = default;

	//ValidateMove, if move is valid update the board of InGameState
	//Super-KO is NOT check here
	//Return the capture list for future use
	bool validateMove(const Move& move, Board& board, std::vector<Cell>& capture);

	void calculateScore(const Board& board, int& black_score, int& white_score);

	//Debug purpose, no use
	void printBoard(const Board& board) {
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board.size(); j++) {
				if (board[i][j] != CellState::Black && board[i][j] != CellState::White) {
					std::cout << "_" << " ";
					continue;
				}
				std::cout << (board[i][j] == CellState::Black ? "B" : "W") << " ";
			}
			std::cout << "\n";
		}
	}
private:
	std::vector<std::vector<CellState>> grid;
};
