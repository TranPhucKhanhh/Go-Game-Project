#pragma once

#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<core/AssetManager.h>
#include<model/UICfg.h>
#include<game/Game.h>
#include<vector>
#include<string>

class InGame {
public:
    InGame(const AssetManager& _asset_manager, Game& _game, UICfg& ui_cfg);

    void enter();

    void eventHandle(const sf::Event& event, std::string& respond);

    void resize();

    void draw();
private:
    BoardUI board;
    sf::RectangleShape side_panel;
    //sf::RectangleShape header_bar;
    sf::RectangleShape footer_bar;
    TextBox header_bar;

    std::unordered_map<std::string, SimpleButton> buttons;
    std::unordered_map<std::string, sf::Text> texts;

    Game& game;
	UICfg& ui_cfg;
    const AssetManager& asset_manager;

    void resize(const sf::Vector2u& window_size);

    // Screen UI value
    float padding;
    float inner_padding;
    sf::Vector2f canvas_size;
    float side_panel_size_x;
    float board_size;
    float status_bar_size_y;
};