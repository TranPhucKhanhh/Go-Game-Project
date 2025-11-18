#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<model/CellState.h>
#include<algorithm>

GameOption::GameOption(const AssetManager& _asset_manager, Game& _game, UICfg& ui_cfg) :
	asset_manager(_asset_manager), game(_game), ui_cfg(ui_cfg),
	start_game_button("Start a new game", asset_manager.getFont("StackSansNotch-Regular")),

	new_game_panel("New game", asset_manager.getFont("StackSansNotch-Regular")),
	game_mode_title(asset_manager.getFont("Momo"), "Select game mode:"),
	pvp_mode_button("PvP", asset_manager.getFont("StackSansNotch-Bold")),
	ai_easy_button("AI easy", asset_manager.getFont("StackSansNotch-Bold")),
	ai_medium_button("AI medium", asset_manager.getFont("StackSansNotch-Bold")),
	ai_hard_button("AI hard", asset_manager.getFont("StackSansNotch-Bold")),
	choose_side_title(asset_manager.getFont("Momo"), "Choose your side:"),
	black_side_button("Black", asset_manager.getFont("StackSansNotch-Bold")),
	white_side_button("White", asset_manager.getFont("StackSansNotch-Bold")),
	choose_board_size_title(asset_manager.getFont("Momo"), "Choose your board size:"),
	board_size_9_button("9 x 9", asset_manager.getFont("StackSansNotch-Bold")),
	board_size_13_button("13 x 13", asset_manager.getFont("StackSansNotch-Bold")),
	board_size_19_button("19 x 19", asset_manager.getFont("StackSansNotch-Bold")),

	load_game_panel("Load game", asset_manager.getFont("StackSansNotch-Regular")),
	setting_panel("Setting", asset_manager.getFont("StackSansNotch-Regular")),
	customize_panel("Customization", asset_manager.getFont("StackSansNotch-Regular"))

{

	// Set the respond message when clicked
	start_game_button.updateRespondStr("StartGame");
	new_game_panel.updateRespondStr("NewGame");
	load_game_panel.updateRespondStr("LoadGame");
	setting_panel.updateRespondStr("Setting");
	customize_panel.updateRespondStr("Customize");
	pvp_mode_button.updateRespondStr("pvpMode");
	ai_easy_button.updateRespondStr("AIeasyMode");
	ai_medium_button.updateRespondStr("AIMediumMode");
	ai_hard_button.updateRespondStr("AIHardMode");
	black_side_button.updateRespondStr("BlackSide");
	white_side_button.updateRespondStr("WhiteSide");
	board_size_9_button.updateRespondStr("9x9");
	board_size_13_button.updateRespondStr("13x13");
	board_size_19_button.updateRespondStr("19x19");

	// Initialzize the basic color and outline
	canvas.setFillColor(sf::Color({ 254, 159, 77 }));
	canvas.setOutlineThickness(3);
	canvas.setOutlineColor(sf::Color::Black);

	game_mode_title.setFillColor(sf::Color::Black);
	choose_side_title.setFillColor(sf::Color::Black);
	choose_board_size_title.setFillColor(sf::Color::Black);

	resize();

	std::cerr << "Load game Option UI successfully." << std::endl;
}

void GameOption::enter() {
	game_mode = GameMode::PvP;
	side_chosen = CellState::Black;
	board_size_chosen = 19;
	resize();
}

void GameOption::draw() {
	ui_cfg.window.clear(sf::Color(240, 217, 181));

	ui_cfg.window.draw(canvas);

	new_game_panel.updateOpacity(100);
	load_game_panel.updateOpacity(100);
	setting_panel.updateOpacity(100);
	customize_panel.updateOpacity(100);

	if (panel == SettingPanel::NewGame) {
		new_game_panel.updateOpacity(255);
		drawNewPanel();
	}
	else if (panel == SettingPanel::LoadGame) {
		load_game_panel.updateOpacity(255);
		drawLoadPanel();
	}
	else if (panel == SettingPanel::Setting) {
		setting_panel.updateOpacity(255);
		drawSettingPanel();
	}
	else if (panel == SettingPanel::Customization) {
		customize_panel.updateOpacity(255);
		drawCustomizePanel();
	}

	start_game_button.draw(ui_cfg.window);
	new_game_panel.draw(ui_cfg.window);
	load_game_panel.draw(ui_cfg.window);
	setting_panel.draw(ui_cfg.window);
	customize_panel.draw(ui_cfg.window);
}

void GameOption::eventHandle(const sf::Event& event, std::string& respond) {
	if (event.is<sf::Event::Resized>()) {
		resize();
	}
	std::string event_respond = "";
	start_game_button.eventHandle(event, ui_cfg, event_respond);
	new_game_panel.eventHandle(event, ui_cfg, event_respond);
	load_game_panel.eventHandle(event, ui_cfg, event_respond);
	setting_panel.eventHandle(event, ui_cfg, event_respond);
	customize_panel.eventHandle(event, ui_cfg, event_respond);

	if (panel == SettingPanel::NewGame) {
		eventHandleNewPanel(event, event_respond);
	}
	else if (panel == SettingPanel::LoadGame) {
		eventHandleLoadPanel(event, event_respond);
	}
	else if (panel == SettingPanel::Setting) {
		eventHandleSettingPanel(event, event_respond);
	}
	else if (panel == SettingPanel::Customization) {
		eventHandleCustomizePanel(event, event_respond);
	}

	if (event_respond == "NewGame") {
		panel = SettingPanel::NewGame;
	}
	else if (event_respond == "LoadGame") {
		panel = SettingPanel::LoadGame;
	}
	else if (event_respond == "Setting") {
		panel = SettingPanel::Setting;
	}
	else if (event_respond == "Customize") {
		panel = SettingPanel::Customization;
	}
	else if (event_respond == "StartGame") {
		std::cerr << "start a game pls" << std::endl;
	}
}


void GameOption::resize() {

	// Calculate the basic size of the screen
	float min_scale = std::min(ui_cfg.window_size.x, ui_cfg.window_size.y);

	margin = min_scale / 10;
	canvas_size = { std::min( (ui_cfg.window_size.y - margin * 2) * 1.f, ui_cfg.window_size.x - margin * 2), ui_cfg.window_size.y - margin * 2};

	// Resize the canvas
	canvas.setSize(canvas_size);
	canvas.setOrigin(canvas.getLocalBounds().getCenter());
	canvas.setPosition({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });

	sf::Vector2f _button_panel_size = { canvas_size.x / 4.f, canvas_size.y / 10.f };

	new_game_panel.updateSize(_button_panel_size);
	new_game_panel.updatePos({ canvas.getPosition().x - canvas_size.x * 0.375f, canvas.getPosition().y - canvas_size.y / 2.f + _button_panel_size.y / 2.f });
	load_game_panel.updateSize(_button_panel_size);
	load_game_panel.updatePos({ canvas.getPosition().x - canvas_size.x * 0.125f, canvas.getPosition().y - canvas_size.y / 2.f + _button_panel_size.y / 2.f });
	setting_panel.updateSize(_button_panel_size);
	setting_panel.updatePos({ canvas.getPosition().x + canvas_size.x * 0.125f, canvas.getPosition().y - canvas_size.y / 2.f + _button_panel_size.y / 2.f });
	customize_panel.updateSize(_button_panel_size);
	customize_panel.updatePos({ canvas.getPosition().x + canvas_size.x * 0.375f, canvas.getPosition().y - canvas_size.y / 2.f + _button_panel_size.y / 2.f });

	start_game_button.updateSize(_button_panel_size);
	start_game_button.updatePos({ canvas.getPosition().x, canvas.getPosition().y + canvas_size.y / 2.f - _button_panel_size.y / 2.f});

	float _ratio = 0.9;
	float _min_button_text_size = std::min({ new_game_panel.getTextSizeFit(_ratio),load_game_panel.getTextSizeFit(_ratio),setting_panel.getTextSizeFit(_ratio),customize_panel.getTextSizeFit(_ratio) });

	start_game_button.updateTextSizeFit(_ratio);
	new_game_panel.updateTextSize(_min_button_text_size);
	load_game_panel.updateTextSize(_min_button_text_size);
	setting_panel.updateTextSize(_min_button_text_size);
	customize_panel.updateTextSize(_min_button_text_size);

	resizeNewPanel();
	resizeLoadPanel();
	resizeSettingPanel();
	resizeCustomizePanel();
}
