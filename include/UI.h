    #pragma once
    #include<SFML/Graphics.hpp>
    #include<Game.h>
    #include<string>

    class UI {
    public:
        UI(Game& game, std::string title);

        void run();

        void setupUI();
    private:
        Game& game;
        sf::RenderWindow window;

        Player cur_player;
    };