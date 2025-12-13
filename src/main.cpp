#include<game/Game.h>
#include<UI/UI.h>
#include<model/UICfg.h>
#include<core/AssetManager.h>
#include<iostream>
#include<AI/KatagoLink.h>

const unsigned int WINDOW_START_WIDTH = 1280u;
const unsigned int WINDOW_START_HEIGHT = 720u;

const unsigned int WINDOW_MIN_WIDTH = 400u;
const unsigned int WINDOW_MIN_HEIGHT = 650u;

const std::string TITLE = "Go game";

int main()
{
    // Load all the asset 
	AssetManager asset_manager;

    // Load fonts
    asset_manager.loadFontFromFolder("fonts");
    asset_manager.loadTextureFromFolder("icon");
    asset_manager.loadTextureFromFolder("board-background");
    asset_manager.loadTextureFromFolder("stone");
    asset_manager.loadTextureFromFolder("component-texture");
    asset_manager.loadMusicFromFolder("music");
    asset_manager.loadSoundFromFolder("sound-effect");

	std::cerr << "Load all assets successfully." << std::endl;

    // Start the game
    Game game;

	game.start();
    //try {
    //    std::string base = KATAGO_DIR;
    //    std::string exe = base + "\\katago.exe";
    //    std::string model = base + "\\model.bin.gz";
    //    std::string cfg = base + "\\default_gtp.cfg";

    //    KatagoLink kg;
    //    kg.start(
    //        ".",        // current directory doesn't matter anymore
    //        exe,        // FULL PATH
    //        model,      // FULL PATH
    //        cfg,        // FULL PATH
    //        true
    //    );
    //}
    //catch (const std::exception& e) {
    //    std::cerr << "KATAGO_DIR = " << KATAGO_DIR << "\n";

    //    std::cerr << "Katago start failed: " << e.what() << std::endl;
    //    return 0;
    //}

    //std::string base = KATAGO_DIR;
    //std::string exe = base + "\\katago.exe";
    //std::string model = base + "\\model.bin.gz";
    //std::string cfg = base + "\\default_gtp.cfg";

    //KatagoLink kg;
    //kg.start(
    //    ".",        // current directory doesn't matter anymore
    //    exe,        // FULL PATH
    //    model,      // FULL PATH
    //    cfg,        // FULL PATH
    //    true
    //);

    //kg.send("boardsize 19");
    //std::cerr << kg.readResponse() << "\n";

    //kg.send("clear_board");
    //std::cerr << kg.readResponse() << "\n";

    //kg.send("genmove b");
    //std::cerr << kg.readResponse() << "\n";

    //kg.send("Hoho");
    //std::cerr << kg.readResponse() << "\n";

    //Run UI
	UI ui(game, asset_manager, { WINDOW_START_WIDTH, WINDOW_START_HEIGHT }, { WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT }, TITLE);
	ui.run();
	
    return 0;
}