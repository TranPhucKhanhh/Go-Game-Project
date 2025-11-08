    #pragma once
    #include<SFML/Graphics.hpp>
    #include<Game.h>
    #include<string>

    class UI {
    public:
        UI(Game& game, std::string title);

        // UI initialize
        void run();
        void setupUI();

        // UI for menu
        void drawMenu();
    private:
        Game& game;
        sf::RenderWindow window;

        Player cur_player;
    };