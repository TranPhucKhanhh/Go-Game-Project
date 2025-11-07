#pragma once
#include<SFML/Graphics.hpp>
#include<Game.h>
#include<string>

class UI {
public:
    UI(Game& game, std::string title);

    void Run();

    void SetupUI();
private:
    Game game;
    sf::RenderWindow window;

    Player cur_player;
};