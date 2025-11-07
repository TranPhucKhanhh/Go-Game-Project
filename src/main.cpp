#include <Game.h>
#include<UI.h>

int main()
{
	GameCfg game_cfg(700, 400); // window size
	Game game(game_cfg);

	UI ui(game, "Go-Game");

	ui.Run();
	return 0;
}