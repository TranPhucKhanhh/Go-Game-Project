#include<SFML/Graphics.hpp>
#include<Component.h>
#include<UI.h>
#include<Game.h>
#include<string>
#include<iostream>
#include<vector>

UI::UI(Game& game, std::string title, sf::Vector2u window_size, sf::Vector2u min_size) : game(game), window_size(window_size),
    window(sf::VideoMode({ window_size.x, window_size.y}), title) {

    window.setVerticalSyncEnabled(true);
    window.setMinimumSize(min_size);
    setupUI();
}

void UI::setupUI() {
    // Load fonts
    font_manager.load("StackSansNotch-Bold", "/fonts/Stack_Sans_Notch/static/StackSansNotch-Bold.ttf");
    font_manager.load("StackSansNotch-Regular", "/fonts/Stack_Sans_Notch/static/StackSansNotch-Regular.ttf");
    
    font_manager.load("Rubik", "/fonts/Rubik_80s_Fade/Rubik80sFade-Regular.ttf");

    font_manager.load("Roboto-Slab-Bold", "/fonts/Roboto_Slab/static/RobotoSlab-Bold.ttf");

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

            if (const auto* mouse_move = event->getIf<sf::Event::MouseMoved>()) mouse_pos = mouse_move->position;

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                window_size = window.getSize();
                std::cerr << "Window resize: " << window_size.x << " " << window_size.y << std::endl;

                // Set the view in window to the exact current window's pixel
                uiView.setSize((sf::Vector2f)window_size);
                uiView.setCenter({ uiView.getSize().x * 0.5f, uiView.getSize().y * 0.5f });
                
                uiView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));

                window.setView(uiView);
            }

            // For debugging mouse position
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    std::cerr << "mouse pos: " << mouse_pos.x << " " << mouse_pos.y << std::endl;
                }
            }

            // Handle the event for GameState
            GameState current_state = game.getGameState();
            switch (current_state) {
            case GameState::Menu:
                menu.eventHandle(*event, mouse_pos, window_size);
                break;
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

    