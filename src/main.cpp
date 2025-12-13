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
    asset_manager.loadFont("StackSansNotch-Bold", "/fonts/Stack_Sans_Notch/static/StackSansNotch-Bold.ttf");
    asset_manager.loadFont("StackSansNotch-Regular", "/fonts/Stack_Sans_Notch/static/StackSansNotch-Regular.ttf");
    asset_manager.loadFont("Rubik", "/fonts/Rubik_80s_Fade/Rubik80sFade-Regular.ttf");
    asset_manager.loadFont("Roboto-Slab-Bold", "/fonts/Roboto_Slab/static/RobotoSlab-Bold.ttf");
    asset_manager.loadFont("Momo", "/fonts/Momo_Trust_Display/MomoTrustDisplay-Regular.ttf");

    // Load texture
    asset_manager.loadTexture("dummy", "/dummy.png");
    asset_manager.loadTexture("white-stone-Default", "/stone/white-stone-Default.png");
    asset_manager.loadTexture("black-stone-Default", "/stone/black-stone-Default.png");
    asset_manager.loadTexture("white-stone-Fruit", "/stone/white-stone-Fruit.png");
    asset_manager.loadTexture("black-stone-Fruit", "/stone/black-stone-Fruit.png");
    asset_manager.loadTexture("white-stone-Animal1", "/stone/white-stone-Animal1.png");
    asset_manager.loadTexture("black-stone-Animal1", "/stone/black-stone-Animal1.png");
    asset_manager.loadTexture("white-stone-Animal2", "/stone/white-stone-Animal2.png");
    asset_manager.loadTexture("black-stone-Animal2", "/stone/black-stone-Animal2.png");
    asset_manager.loadTexture("white-stone-Animal3", "/stone/white-stone-Animal3.png");
    asset_manager.loadTexture("black-stone-Animal3", "/stone/black-stone-Animal3.png");
    // Load music
    asset_manager.loadMusic("Puzzle-Dreams", "/music/Puzzle-Dreams.ogg");
    asset_manager.loadMusic("The-Spunky-Princess", "/music/The-Spunky-Princess.ogg");
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