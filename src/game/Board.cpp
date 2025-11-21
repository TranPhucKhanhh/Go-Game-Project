#pragma once
#include <game/Board.h>
#include <model/Move.h>
#include <model/Cell.h>
#include <vector>
#include <iostream>

//Direction arrays, used in dfs and bfs
static int dx[4] = { 1,-1,0,0 };
static int dy[4] = { 0,0,1,-1 };
//Visited array, used in dfs and bfs
static std::vector<std::vector<bool>> visited;
static std::vector<std::vector<bool>> captured_cell;

static bool isOutOfRange(const Move& move, int board_size) {
	return move.x < 0 || move.y < 0 || move.x >= board_size || move.y >= board_size;
}

static bool isOutOfRange(int x, int y, int board_size) {
	return x < 0 || y < 0 || x >= board_size || y >= board_size;
}

static bool isOccupied(const Move& move,const Board& board) {
	return board[move.x][move.y] != CellState::Empty;
}

static int countLiberty(int x, int y,const Board& board,const CellState& type) {
	if (isOutOfRange(x, y, board.size())) return 0;
	if (board[x][y] == CellState::Empty) return 1;
	if (board[x][y] != type) return 0;
	if (visited[x][y]) return 0;
	visited[x][y] = 1;
	int liberty = 0;
	for (int i = 0; i < 4; i++) {
		int u = x + dx[i];
		int v = y + dy[i];
		liberty += countLiberty(u, v, board, type);
	}
	return liberty;
}

static int countLiberty(const Move& move,const Board& board) {
	visited.assign(board.size(), std::vector<bool>(board.size(), 0));
	int x = move.x;
	int y = move.y;
	CellState type = move.player;
	int liberty = countLiberty(x, y, board, type);
	return liberty;
}

static void getCapture(int x, int y, const Board& board, const CellState& type, std::vector<Cell>& capture) {
	if (isOutOfRange(x, y, board.size())) return;
	if (board[x][y] != type) return;
	if (captured_cell[x][y]) return;
	captured_cell[x][y] = 1;
	capture.push_back(Cell(x, y, board[x][y]));
	for (int i = 0; i < 4; i++) {
		int u = x + dx[i];
		int v = y + dy[i];
		getCapture(u, v, board, type, capture);
	}
}

static void getCapture(const Move& move, const Board& board, std::vector<Cell>& capture) {
	captured_cell.assign(board.size(), std::vector<bool>(board.size(), 0));
	int x = move.x;
	int y = move.y;
	CellState type = move.player;
	//std::cout << "Move: " << x << " " << y << " ";
	//std::cout << (type == CellState::Black ? "Black" : "White") << "\n";
	if (type == CellState::Black) type = CellState::White;
	else type = CellState::Black;
	for (int i = 0; i < 4; i++) {
		int u = x + dx[i];
		int v = y + dy[i];
		if (isOutOfRange(u, v, board.size())) continue;
		if (captured_cell[u][v]) continue;
		visited.assign(board.size(), std::vector<bool>(board.size(), 0));
		int tmp = countLiberty(u, v, board, type);
		//Note: this count Liberty DOES NOT reset visited state so reset it manually
		//std::cout << u << " " << v << " " << tmp << "\n";
		if (tmp > 0 || captured_cell[u][v]) continue;
		getCapture(u, v, board, type, capture);
	}
	//std::cout << capture.size() << "\n";
}

static bool canCapture(const Move& move,const Board& board, std::vector<Cell>& capture) {
	getCapture(move, board, capture);
	return capture.size() > 0;
}

bool Board::validateMove(const Move& move, Board& board, std::vector<Cell>& capture) {
	if (isOutOfRange(move, board.size())) return 0;
	if (isOccupied(move, board)) return 0;

	//Temporary add move to board
	//Will be removed if it is invalid, and keep if it is valid
	//Super-KO will be checked later
	board[move.x][move.y] = move.player;
	//printBoard(board);
	//Self-suicide check
	int liberty = countLiberty(move, board);
	if (liberty != 0) {
		getCapture(move, board, capture);
		return 1;
	}
	//Place in no liberty but can capture is also valid
	if (canCapture(move, board, capture)) return 1;
	board[move.x][move.y] = CellState::Empty;
	return 0;
}

static int getScore(const Board& board, int x, int y, int& mask) {
	if (isOutOfRange(x, y, board.size())) return 0;
	if (board[x][y] != CellState::Empty) {
		if (board[x][y] == CellState::Black) mask |= 1;
		else mask |= 2;
		return 1;
	}
	if (visited[x][y]) return 0;
	visited[x][y] = 1;
	int res = 1;
	for (int i = 0; i < 4; i++) {
		int u = x + dx[i];
		int v = y + dy[i];
		res += getScore(board, u, v, mask);
	}
	return res;
}

void Board::calculateScore(const Board& board, int& black_score, int& white_score) {
	visited.assign(board.size(), std::vector<bool>(board.size(), 0));
	for (int i = 0; i < (*this).size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			if (visited[i][j]) continue;
			int mask = 0;
			int score = getScore(board, i, j, mask);
			if (mask == 3) continue;
			if (mask == 1) black_score += score;
			if (mask == 2) white_score += score;
		}
	}
}