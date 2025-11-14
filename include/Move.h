#pragma once
#include <CellState.h>

struct Move {
	int x, y;
	CellState player;
	bool pass;

	Move() {}
	Move(int x, int y, CellState player, bool pass) : 
		x(x), y(y), player(player), pass(pass) {}
};