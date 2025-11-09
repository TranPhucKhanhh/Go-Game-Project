#pragma once
#include<SFML/Graphics.hpp>
#include<Game.h>
#include<Component.h>
#include<Menu.h>
#include<FontManager.h>

class UI {
public:
    UI(Game& game, std::string title, sf::Vector2u window_size, sf::Vector2u min_size);

    // UI initialize
    void run();
    void setupUI();

private:
    Game& game;
    Menu menu;
    Player cur_player;

    sf::RenderWindow window;
    FontManager font_manager;
    sf::Vector2i mouse_pos;
    sf::Vector2u window_size;
};