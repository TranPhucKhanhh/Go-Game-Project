#include<SFML/Graphics.hpp>
#include<UI/UI.h>
#include<game/Game.h>
#include<core/AssetManager.h>
#include<model/UICfg.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<string>
#include<iostream>
#include<vector>
#include <thread>
#include <atomic>
#include <mutex>

UI::~UI() {
    stopAIThread();
}

UI::UI(Game& game, const AssetManager& _asset_manager, const sf::Vector2u& window_size, const sf::Vector2u& min_size, const std::string& title)
    : game(game), asset_manager(_asset_manager), window(sf::VideoMode(window_size), title),
    ui_cfg(window_size, min_size,window, _asset_manager),
    loading(_asset_manager, ui_cfg),
    menu(_asset_manager, ui_cfg), in_game(_asset_manager, game, ui_cfg),
    game_setting(_asset_manager, game, ui_cfg) {

    window.setVerticalSyncEnabled(true);
    window.setMinimumSize(ui_cfg.min_window_size);

	std::cerr << "UI initialized successfully." << std::endl;
}

void UI::startAIAsync()
{
    if (ai_running.load(std::memory_order_acquire))
        return;

    if (ai_thread.joinable())
        ai_thread.join();

    ai_running.store(true, std::memory_order_release);
    ai_finished.store(false, std::memory_order_release);

    ai_thread = std::thread([this]()
        {
            // Protect Game because main thread is drawing/reading game state.
            {
                std::lock_guard<std::mutex> lk(game_mutex);
                game.placeStoneAI(); // unchanged AI code
            }

            ai_finished.store(true, std::memory_order_release);
            ai_running.store(false, std::memory_order_release);
        });
}

void UI::stopAIThread()
{
    if (ai_thread.joinable())
        ai_thread.join();
}

void UI::switchGameState(const std::string &respond) {
    if (respond == "ToMenu") {
        game.getGameCfg().prev_state = game.getGameCfg().state;
        game.getGameCfg().state = GameState::Menu;
		menu.enter();
    }
    else if (respond == "GameNewOption") {
        game.getGameCfg().prev_state = game.getGameCfg().state;
        game.getGameCfg().state = GameState::Setting;
        game_setting.setPanel(SettingPanel::NewGame);
        game_setting.enter();
    }
    else if (respond == "StartNewGame") {
        game.getGameCfg().prev_state = game.getGameCfg().state;
        game.getGameCfg().state = GameState::Playing;
        in_game.enter();
    }
    else if (respond == "OpenSetting") {
        game.getGameCfg().prev_state = game.getGameCfg().state;
        game.getGameCfg().state = GameState::Setting;
        game_setting.enter();
        game_setting.setPanel(SettingPanel::Setting);
    }
    else if (respond == "LoadNewGame") {
        game.getGameCfg().prev_state = game.getGameCfg().state;
        game.getGameCfg().state = GameState::Playing;
        in_game.enter();
    }
    else if (respond == "GoBack") {
        std::swap(game.getGameCfg().prev_state, game.getGameCfg().state);

		if (game.getGameCfg().state == GameState::Playing) switchGameState("LoadNewGame");
        else if (game.getGameCfg().state == GameState::Menu) switchGameState("ToMenu");
    }
}

void UI::run() {
    sf::View uiView(sf::FloatRect( { 0.f, 0.f }, (sf::Vector2f) window.getSize()));
	ui_cfg.updateMusic(asset_manager);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* mouse_move = event->getIf<sf::Event::MouseMoved>()) ui_cfg.mouse_pos = mouse_move->position;

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                ui_cfg.window_size = window.getSize();
                std::cerr << "Window resize: " << ui_cfg.window_size.x << " " << ui_cfg.window_size.y << std::endl;

                // Set the view in window to the exact current window's pixel
                uiView.setSize((sf::Vector2f)ui_cfg.window_size);
                uiView.setCenter({ uiView.getSize().x * 0.5f, uiView.getSize().y * 0.5f });

                uiView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));

                window.setView(uiView);
            }

            // For debugging mouse position
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    std::cerr << "mouse pos: " << ui_cfg.mouse_pos.x << " " << ui_cfg.mouse_pos.y << std::endl;
                    std::cerr << "mous pos2: " << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
                }
            }

            // Handle the event for GameState
            GameState current_state = game.getGameState();
            respond = "";

            if (current_state == GameState::Menu) {
                menu.eventHandle(*event, respond);
            }
            else if (current_state == GameState::Playing) {
                std::scoped_lock lock(game_mutex);
                in_game.eventHandle(*event, respond);
            }
            else if (current_state == GameState::Setting) {
                game_setting.eventHandle(*event, respond);
            }

            if (respond == "GameNewOption" || respond == "StartNewGame" || respond == "OpenSetting" 
                || respond == "LoadNewGame" || respond == "GoBack") {
				UI::switchGameState(respond);
            }
            else if (respond == "GameExit") {
                window.close();
            }
            else if (respond == "StartAI") {
                startAIAsync();
            }
        }

        if (ai_finished.exchange(false, std::memory_order_acq_rel)) {
            // AI has made its move; update UI safely on main thread
            {
                std::lock_guard<std::mutex> lk(game_mutex);

                ui_cfg.stone_place_sound.play();
                ui_cfg.AI_in_turn = false;

                // refresh in-game UI elements after AI move
                in_game.updateHistoryScroll();
                in_game.updateHeaderBar();
                in_game.updateScoreBox(game.getScore());
            }
        }

        GameState current_state = game.getGameState();
        if (current_state == GameState::Playing) {
            std::scoped_lock lock(game_mutex);
            in_game.draw();
        }
        else {
            switch (current_state) {
            case GameState::Menu:
                menu.draw();
                break;
            case GameState::Setting:
                game_setting.draw();
                break;
            case GameState::Playing:
                // handled above
                break;
            }
        }

        window.display();   
    }
}

    