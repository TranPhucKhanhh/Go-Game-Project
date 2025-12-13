#pragma once
#include <model/Move.h>
#include <AI/KatagoLink.h>
#include <model/GameState.h>
class AI {
public:
	AI();
	void syncMove(const Move& v);
	Move genMove(const CellState& current_player, CellState& resigned_player);
	void startGame(const int init_board_size, const GameMode init_level);
	void resetGame();
	//Return value
	bool isThinking() const { return is_thinking; }
private:
	KatagoLink katago;
	int board_size;
	GameMode level;
	bool is_thinking = 0;
};