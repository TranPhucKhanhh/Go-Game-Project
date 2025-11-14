#include <Game.h>
#include<UI.h>

int main()
{
	Game game(1280, 720); // Set the default screen 

	//UI ui(game, "Go Game");

	//ui.run();

	//Use console to debug the logic, can be removed
	game.start();
	game.placeStone(1, 1);
	game.pass();
	game.placeStone(2, 2);
	game.pass();
	game.placeStone(2, 0);
	game.pass();
	game.placeStone(3, 1);
	game.print();

	game.placeStone(1, 2);
	game.pass();
	game.placeStone(2, 3);
	game.pass();
	game.placeStone(3, 2);
	game.pass();
	game.print();
	game.placeStone(2, 1);
	game.print();

	game.placeStone(2, 2);
	game.print();

	game.pass();
	game.pass();
	return 0;
}