#pragma once
#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<game/Game.h>
#include<core/AssetManager.h>
#include<vector>
#include<string>

class Menu {
public:
    Menu(GameCfg& _game_config) : game_config(_game_config) {};

    void build(const sf::Vector2u& window_size, const AssetManager& asset_manager);

    void eventHandle(const sf::Event &event, const sf::Vector2i &mouse_pos, const sf::Vector2u &window_size);

    void draw(sf::RenderWindow& window);
private:
    std::unordered_map<std::string, SimpleButton> buttons;
    std::unordered_map<std::string, sf::Text> texts;

    GameCfg& game_config;

    void resize(const sf::Vector2u& window_size);  
};