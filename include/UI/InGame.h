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
    sf::RectangleShape mode_panel;
    sf::RectangleShape option_panel;
    sf::RectangleShape control_panel;
    sf::RectangleShape history_panel;
    sf::RectangleShape footer_bar;
    TextBox header_bar;

    SimpleButton new_button;
    SimpleButton save_button;
    SimpleButton reset_button;

    SimpleButton undo_button;
    SimpleButton redo_button;
    SimpleButton pass_button;

    Game& game;
	UICfg& ui_cfg;
    const AssetManager& asset_manager;

    void resize(const sf::Vector2u& window_size);

    void updateHeaderBar();
    void mode_panel_resize(const float& _total_height_panel);
    void control_panel_resize(const float& _total_height_panel);
    void option_panel_resize(const float& _total_height_panel);
    void history_panel_resize(const float& _total_height_panel);

    // Screen UI value
    float padding;
    float inner_padding;
    sf::Vector2f canvas_size;
    float side_panel_size_x;
    float board_size;
    float status_bar_size_y;
};