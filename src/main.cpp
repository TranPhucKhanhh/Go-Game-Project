#include <Game.h>
#include<UI.h>

int main()
{
	Game game(700, 400); // Set the default screen 

	UI ui(game, "Go-Game");

	ui.run();
	return 0;
}