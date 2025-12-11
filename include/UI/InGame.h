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

    BaseButton sound_button, music_button;
    TextButton new_button, save_button, reset_button, setting_button;
    TextButton undo_button, redo_button, pass_button, resign_button, out_his_button;

    TextBox white_score_box;
    TextBox black_score_box;
    TextBox mode_box;
    sf::Sprite white_stone_image, black_stone_image;

    TextInputNotification save_file_input;
	BaseNotification error_notification;

	Scroll history_scroll;

    Game& game;
	UICfg& ui_cfg;
    const AssetManager& asset_manager;

    int history_preview_index = -1;

    bool game_playable;

    void resize(const sf::Vector2u& window_size);

    void updateHeaderBar();
    void updateSoundMusic();
    void updateScoreBox(const std:: pair<float, float> &_score);
    void updateHistoryScroll();
    void mode_panel_resize(const float& _height);
    void control_panel_resize(const float& _height);
    void option_panel_resize(const float& _height);
    void history_panel_resize(const float& _height);
    void score_bar_resize(const float& _height);

    // Screen UI value
    float padding;
    float inner_padding;
    sf::Vector2f canvas_size;
    float side_panel_size_x, control_bar_size_x;
    float board_size;
    float status_bar_size_y;
};