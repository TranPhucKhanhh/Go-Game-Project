#pragma once
#include <model/CellState.h>
struct Cell {
	int x, y;
	CellState state;
	Cell() {}
	Cell(int x, int y, CellState state) :
		x(x), y(y), state(state) {}

};