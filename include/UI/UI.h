#pragma once
#include<SFML/Graphics.hpp>
#include<game/Game.h>
#include<UI/Component.h>
#include<UI/Menu.h>
#include<UI/InGame.h>
#include<UI/GameOption.h>
#include<UI/Loading.h>
#include<model/UICfg.h>
#include<core/AssetManager.h>
#include <thread>
#include <atomic>
#include <mutex>

class UI {
public:
    UI(Game& game, const AssetManager& asset_manager, const sf::Vector2u& window_size, const sf::Vector2u& min_size, const std::string& title);

    // UI initialize
    void run();

    ~UI();

private:
    UICfg ui_cfg;
    sf::RenderWindow window;

    Game& game;
    const AssetManager& asset_manager;

    Loading loading;
    Menu menu;
    InGame in_game;
    GameOption game_setting;

	std::string respond;

	void switchGameState(const std::string& respond);

    // Handle AI thread
    std::thread ai_thread;
    std::atomic<bool> ai_running{ false };
    std::atomic<bool> ai_finished{ false };

    std::mutex game_mutex;     // protects Game access (main thread + AI thread)

    void startAIAsync();
    void stopAIThread();
};