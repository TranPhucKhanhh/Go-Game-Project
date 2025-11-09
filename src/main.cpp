#include <Game.h>
#include<UI.h>

int main()
{
	Game game;

	UI ui(game, "Go Game", {1280u, 720u});

	ui.run();
	return 0;
}