#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<UI/UI.h>
#include<Game.h>
#include<string>
#include<iostream>
#include<vector>

UI::UI(Game& game, std::string title, sf::Vector2u window_size, sf::Vector2u min_size) : game(game), window_size(window_size), window(sf::VideoMode(window_size), title), in_game(asset_manager, game.getGameCfg()) {

    window.setVerticalSyncEnabled(true);
    window.setMinimumSize(min_size);
    setupUI();
}

void UI::setupUI() {
    // Load fonts
    asset_manager.loadFont("StackSansNotch-Bold", "/fonts/Stack_Sans_Notch/static/StackSansNotch-Bold.ttf");
    asset_manager.loadFont("StackSansNotch-Regular", "/fonts/Stack_Sans_Notch/static/StackSansNotch-Regular.ttf");
    asset_manager.loadFont("Rubik", "/fonts/Rubik_80s_Fade/Rubik80sFade-Regular.ttf");
    asset_manager.loadFont("Roboto-Slab-Bold", "/fonts/Roboto_Slab/static/RobotoSlab-Bold.ttf");
    asset_manager.loadFont("Momo", "/fonts/Momo_Trust_Display/MomoTrustDisplay-Regular.ttf");

    // Load texture
    asset_manager.loadTexture("board-default", "/board-background/board-default.png");
    asset_manager.loadTexture("board-minimal", "/board-background/board-minimal.png");
    asset_manager.loadTexture("stone-default", "/stone/stone-default.png");

    menu.build(window_size, asset_manager);
    in_game.build(window_size);
}

void UI::run() {
    sf::View uiView(sf::FloatRect( { 0.f, 0.f }, (sf::Vector2f) window.getSize()));

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
                    std::cerr << "mous pos2: " << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
                }
            }

            // Handle the event for GameState
            GameState current_state = game.getGameState();
            switch (current_state) {
            case GameState::Menu:
                menu.eventHandle(*event, mouse_pos, window_size);
                break;
            case GameState::Playing:
                in_game.eventHandle(*event, mouse_pos, window_size);
            }
        }

        GameState current_state = game.getGameState();
        switch (current_state) {
            case GameState::Menu:
                menu.draw(window);
                break;
            case GameState::Playing:
                in_game.draw(window);
                break;
        }

        window.display();   
    }
}

    