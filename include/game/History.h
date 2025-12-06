#pragma once
#include<game/Board.h>
#include<model/CellState.h>
#include<model/Move.h>
#include<model/Cell.h>

class GameHistory {
public:
	void clear();
	bool checkConsecutivePass();
	bool checkSuperKO(const Board& current_board);
	void undoMove(Board& current_board, CellState& current_player);
	void redoMove(Board& current_board, CellState& current_player);
	void addMove(const Move& move, const Board& current_board, const std::vector<Cell>& turn_capture);
	void loadFromMoveList(const std::vector<Move>& move_list, Board& current_board, CellState& current_player);
	void loadPreviewFromMoveList(const std::vector<Move>& move_list, Board& current_board);
	std::string getLastMove();
	std::vector<Move> getMoveHistory();
	int getMoveListSize();
	Board getKthBoard(const int k);
private:
	//Board history, use in checking Super-KO
	//Planning to upgrade to Zobrist hashing later on to improve speed
	std::vector<Board> board;

	//Capture history, use in undo, can use board but this is faster
	std::vector<std::vector<Cell>> capture;

	//Move history, use in undo
	std::vector<Move> move;

	//Reverse version of capture history, use in redo
	std::vector<std::vector<Cell>> restore;

	//Undo history, use in redo
	std::vector<Move> undo;
};