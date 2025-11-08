#include<SFML/Graphics.hpp>
#include<UI.h>
#include<Game.h>
#include<string>

UI::UI(Game& game, std::string title) : game(game),
    window(sf::VideoMode({ game.windowWeight(), game.windowHeight() }), title) {
    
    window.setVerticalSyncEnabled(true);
    setupUI();
}

void UI::setupUI() {
   
}

void UI::run() {
    int weight = game.windowHeight();
    int height = game.windowWeight();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        window.clear();
        window.display();
    }
}

    