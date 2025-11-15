#pragma once
#include<SFML/Graphics.hpp>
#include<game/Game.h>
#include<UI/Component.h>
#include<UI/Menu.h>
#include<UI/InGame.h>

class UI {
public:
    UI(Game& game, std::string title, sf::Vector2u window_size, sf::Vector2u min_size);

    // UI initialize
    void run();
    void setupUI();

private:
    Game& game;
    Menu menu;
    InGame in_game;

    sf::RenderWindow window;
    AssetManager asset_manager;

    sf::Vector2i mouse_pos;
    sf::Vector2u window_size;
};