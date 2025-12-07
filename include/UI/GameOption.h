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
    
	void setPanel(const SettingPanel &_panel) { panel = _panel; }
private:
    
    Game& game;
    UICfg& ui_cfg;
    const AssetManager& asset_manager;

    sf::RectangleShape canvas;
    TextButton start_game_button, go_back_button;
    
	// New game panel components
    TextButton new_game_panel;
    sf::Text game_mode_title;
    TextButton pvp_mode_button, ai_easy_button, ai_medium_button, ai_hard_button;
    sf::Text choose_side_title;
    TextButton black_side_button, white_side_button;
    sf::Text choose_board_size_title;
    TextButton board_size_9_button, board_size_13_button, board_size_19_button;

    // Load game panel components
    TextButton load_game_panel;
    Scroll load_game_scroll;
	TextButton refresh_button,delete_button;
    BoardUI board_preview;
    bool board_preview_show;
	TextBox board_preview_title;
	std::string file_to_load;

	// Setting panel components
    TextButton setting_panel;
    Scroll music_scroll;
	sf::Text music_scroll_title, music_volume_title, sound_effect_volume_title;
    Slider background_music_slide, sound_effect_slide;

    TextButton customize_panel;

    float margin;
    sf::Vector2f canvas_size;
    SettingPanel panel = SettingPanel::NewGame;
    GameMode game_mode;
    CellState side_chosen;
    unsigned int board_size_chosen;

    void resizeNewPanel();
    void drawNewPanel();
    void eventHandleNewPanel(const sf::Event& event, std::string& respond);

    void resizeLoadPanel();
    void drawLoadPanel();
    void loadGameFile();
    void eventHandleLoadPanel(const sf::Event& event, std::string& respond);
    
    void resizeSettingPanel();
    void drawSettingPanel();
    void eventHandleSettingPanel(const sf::Event& event, std::string& respond);
    
    void resizeCustomizePanel();
    void drawCustomizePanel();
    void eventHandleCustomizePanel(const sf::Event& event, std::string& respond);
};