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
    TextureEffectButton start_game_button, go_back_button;
    
	// New game panel components
    TextureEffectButton new_game_panel;
    sf::Text game_mode_title;
    TextureEffectButton pvp_mode_button, ai_easy_button, ai_medium_button, ai_hard_button;
    sf::Text choose_side_title;
    TextureEffectButton black_side_button, white_side_button;
    sf::Text choose_board_size_title;
    TextureEffectButton board_size_9_button, board_size_13_button, board_size_19_button;

    // Load game panel components
    TextureEffectButton load_game_panel;
    TextureEffectButton refresh_button, delete_button;
    Scroll load_game_scroll;
    BoardUI board_preview;
	TextBox board_preview_title;
	std::string file_to_load;
    bool board_preview_show;

	// Setting panel components
    TextureEffectButton setting_panel;
    Scroll music_scroll, effect_scroll;
	sf::Text music_scroll_title, effect_scroll_title, music_volume_title, sound_effect_volume_title;
    Slider background_music_slide, sound_effect_slide;

	// Customize panel components
    TextureEffectButton customize_panel;
    sf::Text board_design_title, stone_design_title, design_preview_title;
	Scroll board_design_scroll, stone_design_scroll;
	BoardUI customize_board_preview;
	std::string board_design_chosen, stone_design_chosen;

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

    // helper functions
    void updateLoadNewButton();

    // Special function to prevent bug in event handling
    void updateEventHandle() {
        sf::Event _d = sf::Event::Closed{};
        std::string _dt = "Test";
        eventHandle(_d, _dt);
    }
};