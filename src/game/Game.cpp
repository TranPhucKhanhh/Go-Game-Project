#pragma once
#include<game/Game.h>
#include<vector>
#include<iostream>

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

	state.current_board.calculateScore(state.current_board, _black_score, _white_score);
	return std::make_pair(_black_score, _white_score);
}

void Game::print() {
	state.printPlayer();
	state.current_board.printBoard(state.current_board);
}