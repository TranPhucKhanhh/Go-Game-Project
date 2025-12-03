#pragma once
#include<SFML/Graphics.hpp>
#include<game/Game.h>
#include<UI/Component.h>
#include<UI/Menu.h>
#include<UI/InGame.h>
#include<UI/GameOption.h>
#include<UI/Loading.h>
#include<model/UICfg.h>
#include<core/AssetManager.h>

class UI {
public:
    UI(Game& game, const AssetManager& asset_manager, const sf::Vector2u& window_size, const sf::Vector2u& min_size, const std::string& title);

    // UI initialize
    void run();

private:
    UICfg ui_cfg;
    sf::RenderWindow window;

    Game& game;
    const AssetManager& asset_manager;

    Loading loading;
    Menu menu;
    InGame in_game;
    GameOption game_setting;

	std::string respond;
};