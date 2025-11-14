#pragma once

#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<core/AssetManager.h>
#include<game.h>
#include<vector>
#include<string>

class InGame {
public:
    InGame(const AssetManager& asset_manager, const GameCfg& game_config);

    void build(const sf::Vector2u& window_size);

    void eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos, const sf::Vector2u& window_size);

    void draw(sf::RenderWindow& window);
private:
    Board board;
    std::unordered_map<std::string, SimpleButton> buttons;
    std::unordered_map<std::string, sf::Text> texts;

    const GameCfg& game_config;
    const AssetManager& asset_manager;

    void resize(const sf::Vector2u& window_size);

    // Screen UI value
    float margin;
    float canva_size;
};