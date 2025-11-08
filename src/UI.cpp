#include<SFML/Graphics.hpp>
#include<Component.h>
#include<UI.h>
#include<Game.h>
#include<string>

UI::UI(Game& game, std::string title) : game(game),
    window(sf::VideoMode({ game.windowWeight(), game.windowHeight() }), title) {
    
    window.setVerticalSyncEnabled(true);
    setupUI();
}

void UI::setupUI() {
    // Check fonts
    font_manager.load("arial", "/fonts/ARIAL.TTF");
}

void UI::drawMenu() {
    window.clear(sf::Color(255, 204, 153));
    int window_weight = game.windowWeight();
    int window_height = game.windowHeight();

    float button_weight = 400;
    float button_height = 100;
    SimpleButton start_button({ window_weight / 2.f - button_weight / 2.f, window_height / 2.f }, 
        {button_weight, button_height}, 100, "Start", font_manager.get("arial"));

    start_button.update(window);
    if (start_button.isClicked(window))
        std::cout << "Clicked!\n";
    start_button.draw(window);
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

        if (game.getGameState() == GameState::Menu) {
            drawMenu();
        }



        window.display();
    }
}

    