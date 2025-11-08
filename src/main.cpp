#include <Game.h>
#include<UI.h>

int main()
{
	Game game(1280, 720); // Set the default screen 

	UI ui(game, "TestGit");

	ui.run();
	return 0;
}