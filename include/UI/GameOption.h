#pragma once

#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<core/AssetManager.h>
#include<model/UICfg.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<game/Game.h>

class GameOption {
public:
    GameOption(const AssetManager& _asset_manager, Game& _game, UICfg& ui_cfg);

    void enter();

    void eventHandle(const sf::Event& event, std::string& respond);

    void resize();

    void draw();
private:
    
    Game& game;
    UICfg& ui_cfg;
    const AssetManager& asset_manager;

    sf::RectangleShape canvas;
    SimpleButton start_game_button;
    
    SimpleButton new_game_panel;
    sf::Text game_mode_title;
    SimpleButton pvp_mode_button;
    SimpleButton ai_easy_button;
    SimpleButton ai_medium_button;
    SimpleButton ai_hard_button;
    sf::Text choose_side_title;
    SimpleButton black_side_button;
    SimpleButton white_side_button;
    sf::Text choose_board_size_title;
    SimpleButton board_size_9_button;
    SimpleButton board_size_13_button;
    SimpleButton board_size_19_button;

    SimpleButton load_game_panel;
    SimpleButton setting_panel;
    SimpleButton customize_panel;

    float margin;
    sf::Vector2f canvas_size;
    SettingPanel panel = SettingPanel::NewGame;
    GameMode game_mode;
    CellState side_chosen;
    unsigned int board_size_chosen;


    void resize(const sf::Vector2u& window_size);

    void resizeNewPanel();
    void drawNewPanel();
    void eventHandleNewPanel(const sf::Event& event, std::string& respond);

    void resizeLoadPanel();
    void drawLoadPanel();
    void eventHandleLoadPanel(const sf::Event& event, std::string& respond);
    
    void resizeSettingPanel();
    void drawSettingPanel();
    void eventHandleSettingPanel(const sf::Event& event, std::string& respond);
    
    void resizeCustomizePanel();
    void drawCustomizePanel();
    void eventHandleCustomizePanel(const sf::Event& event, std::string& respond);
};