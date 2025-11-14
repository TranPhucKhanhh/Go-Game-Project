#include <Game.h>
#include<UI/UI.h>

const unsigned int WINDOW_START_WIDTH = 1280u;
const unsigned int WINDOW_START_HEIGHT = 720u;

const unsigned int WINDOW_MIN_WIDTH = 400u;
const unsigned int WINDOW_MIN_HEIGHT = 650u;

int main()
{
	Game game;

	UI ui(game, "Go Game", { WINDOW_START_WIDTH, WINDOW_START_HEIGHT }, { WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT });

	ui.run();
	return 0;
}