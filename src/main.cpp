#include<game/Game.h>
#include<UI/UI.h>
#include<model/UICfg.h>
#include<core/AssetManager.h>
#include<iostream>

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

    // Run UI
    UI ui(game, asset_manager, { WINDOW_START_WIDTH, WINDOW_START_HEIGHT }, { WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT }, TITLE);
    ui.run();

    return 0;
}