#include<SFML/Graphics.hpp>
#include<Component.h>
#include<UI.h>
#include<Game.h>
#include<string>
#include<iostream>
#include<vector>

UI::UI(Game& game, std::string title, sf::Vector2u window_size) : game(game), window_size(window_size),
    window(sf::VideoMode({ window_size.x, window_size.y}), title) {

    window.setVerticalSyncEnabled(true);
    setupUI();
}

void UI::setupUI() {
    // Load fonts
    font_manager.load("arial", "/fonts/ARIAL.TTF");

    menu.build(window_size, font_manager);
}

void UI::run() {
    sf::View uiView(sf::FloatRect( { 0.f, 0.f }, (sf::Vector2f) window.getSize()));
    uiView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                window_size = window.getSize();
                std::cerr << "Window resize: " << window_size.x << " " << window_size.y << std::endl;

                // Set the view in window to the exact current window's pixel
                uiView.setSize((sf::Vector2f)window_size);
                uiView.setCenter({ uiView.getSize().x * 0.5f, uiView.getSize().y * 0.5f });
                
                uiView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));

                window.setView(uiView);

                GameState current_state = game.getGameState();
                switch (current_state) {
                    case GameState::Menu:
                        menu.resize(window_size);
                        break;
                }
            }

            if (const auto* e = event->getIf<sf::Event::MouseMoved>()) {
                mouse_pos = e->position;

                GameState current_state = game.getGameState();
                switch (current_state) {
                    case GameState::Menu:
                        menu.checkHover(mouse_pos);
                        break;
                }
            }

            // For debugging mouse position
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    std::cerr << "mouse pos: " << mouse_pos.x << " " << mouse_pos.y << std::endl;
                }
            }
        }

        GameState current_state = game.getGameState();
        switch (current_state) {
            case GameState::Menu:
                menu.draw(window);
                break;
        }

        window.display();   
    }
}

    