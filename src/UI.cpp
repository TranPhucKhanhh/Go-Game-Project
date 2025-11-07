#include<SFML/Graphics.hpp>
#include<UI.h>
#include<Game.h>
#include<string>

UI::UI(Game& game, std::string title) : game(game),
    window(sf::VideoMode({ game.WindowWeight(), game.WindowHeight() }), title) {
    
    window.setVerticalSyncEnabled(true);
    SetupUI();
}

void UI::SetupUI() {
   
}

void UI::Run() {
    int weight = game.WindowHeight();
    int height = game.WindowWeight();

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

    