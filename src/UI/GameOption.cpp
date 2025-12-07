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
	go_back_button("Back", asset_manager.getFont("StackSansNotch-Regular")),
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
	refresh_button("Refresh", asset_manager.getFont("StackSansNotch-Regular")),
	delete_button("Delete", asset_manager.getFont("StackSansNotch-Regular")),
	board_preview_title(asset_manager.getFont("Momo")),
	board_preview(_asset_manager.getFont("Momo")),

	setting_panel("Setting", asset_manager.getFont("StackSansNotch-Regular")),
	music_scroll_title(asset_manager.getFont("Momo"), "Choose your music: " + ui_cfg.music_name),
	music_volume_title(asset_manager.getFont("Momo"), "Music Volume: " + std::to_string((int)ui_cfg.background_music_volume) + "%"),
	sound_effect_volume_title(asset_manager.getFont("Momo"), "Sound effect Volume: " + std::to_string((int)ui_cfg.background_music_volume) + "%"),

	customize_panel("Customization", asset_manager.getFont("StackSansNotch-Regular")),
	customize_board_preview(_asset_manager.getFont("Momo")),
	board_design_title(asset_manager.getFont("Momo"), "Choose your board design: " + ui_cfg.board_design),
	stone_design_title(asset_manager.getFont("Momo"), "Choose your stone design: " + ui_cfg.stone_design),
	design_preview_title(asset_manager.getFont("Momo"), "Board design preview")

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
	refresh_button.updateRespondStr("Refresh");
	go_back_button.updateRespondStr("GoBack");
	delete_button.updateRespondStr("Delete");

	// Initialzize the basic color and outline
	canvas.setFillColor(sf::Color({ 254, 159, 77 }));
	canvas.setOutlineThickness(3);
	canvas.setOutlineColor(sf::Color::Black);

	go_back_button.updateOutlineThickness(3);
	go_back_button.updateOutlineColor(sf::Color::Black);

	game_mode_title.setFillColor(sf::Color::Black);
	choose_side_title.setFillColor(sf::Color::Black);
	choose_board_size_title.setFillColor(sf::Color::Black);

	board_preview_title.updateBoxColor(sf::Color(255, 255, 255, 0));
	board_preview_title.updateTextColor(sf::Color::Black);

	music_scroll_title.setFillColor(sf::Color::Black);
	music_volume_title.setFillColor(sf::Color::Black);
	sound_effect_volume_title.setFillColor(sf::Color::Black);

	board_design_title.setFillColor(sf::Color::Black);
	stone_design_title.setFillColor(sf::Color::Black);
	design_preview_title.setFillColor(sf::Color::Black);

	board_preview.updateBoardUI(ui_cfg.board_design, asset_manager);
	board_preview.updateStoneUI(ui_cfg.stone_design, asset_manager);
	customize_board_preview.updateBoardUI(ui_cfg.board_design, asset_manager);
	customize_board_preview.updateStoneUI(ui_cfg.stone_design, asset_manager);

	// Initial value
	load_game_scroll.updatePreviewSize(7);
	loadGameFile();
	background_music_slide.value = ui_cfg.background_music_volume;
	sound_effect_slide.value = ui_cfg.sound_effect_volume;
	music_scroll.updatePreviewSize(5);
	TextButton _music1("Puzzle-Dreams", asset_manager.getFont("StackSansNotch-Regular")); _music1.updateRespondStr("~Puzzle-Dreams");
	TextButton _music2("The-Spunky-Princess", asset_manager.getFont("StackSansNotch-Regular")); _music2.updateRespondStr("~The-Spunky-Princess");
	music_scroll.updateContent(_music1);
	music_scroll.updateContent(_music2);
	
	customize_board_preview.updateCellNumber(13);
	board_design_scroll.updatePreviewSize(4);
	stone_design_scroll.updatePreviewSize(4);
	for (int i = 0; i < std::size(BoardDesign); ++i) {
		TextButton _button(BoardDesign[i], asset_manager.getFont("StackSansNotch-Regular")); 
		_button.updateRespondStr("[" + BoardDesign[i]);
		board_design_scroll.updateContent(_button);
	}

	for (int i = 0; i < std::size(StoneDesign); ++i) {
		TextButton _button(StoneDesign[i], asset_manager.getFont("StackSansNotch-Regular"));
		_button.updateRespondStr("]" + StoneDesign[i]);
		stone_design_scroll.updateContent(_button);
	}

	resize();

	std::cerr << "Load game Option UI successfully." << std::endl;
}

void GameOption::enter() {
	board_preview_show = false;
	game_mode = GameMode::PvP;
	side_chosen = CellState::Black;
	board_size_chosen = 19;
	sf::Event _d = sf::Event::Closed{};
	std::string _dt = "Test";
	eventHandle(_d, _dt);
	board_design_chosen = ui_cfg.board_design; 
	stone_design_chosen = ui_cfg.stone_design;
	music_scroll.updateIndex(0);
	board_design_scroll.updateIndex(0);
	stone_design_scroll.updateIndex(0);
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

	if (panel == SettingPanel::NewGame || panel == SettingPanel::LoadGame) {
		start_game_button.draw(ui_cfg.window);
	}
	go_back_button.draw(ui_cfg.window);
	new_game_panel.draw(ui_cfg.window);
	load_game_panel.draw(ui_cfg.window);
	setting_panel.draw(ui_cfg.window);
	customize_panel.draw(ui_cfg.window);
	go_back_button.draw(ui_cfg.window);
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
	go_back_button.eventHandle(event, ui_cfg, event_respond);

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
		if (game_mode == GameMode::PvP) {
			game.getGameCfg().board_size = board_size_chosen;
			game.start();
			game.reset();
			respond = "StartNewGame";
		}
		else {
			std::cerr << "Other game mode has't implemented yet!" << std::endl;
		}
	}
	else if (event_respond == "LoadNewGame") {
		if (board_preview_show) {
			game.loadGame(file_to_load);
			respond = "LoadNewGame";
		}
	}
	else if (event_respond == "GoBack") {
		respond = "GoBack";
	}

	// Change the start game button text according to the panel
	if (event_respond == "NewGame" || event_respond == "LoadGame") {
		if (panel == SettingPanel::NewGame) {
			start_game_button.updateStr("New Game");
			start_game_button.updateRespondStr("StartGame");
		}
		else {
			start_game_button.updateStr("Load Game");
			start_game_button.updateRespondStr("LoadNewGame");
		}
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
	go_back_button.updateSize({ _button_panel_size.x, std::min(margin * 0.7f, _button_panel_size.y * 0.7f)});
	go_back_button.updatePos({ new_game_panel.getPos().x, new_game_panel.getPos().y - _button_panel_size .y/2.f - go_back_button.getSize().y/2.f - 7.f});

	float _ratio = 0.9;
	float _min_button_text_size = std::min({ new_game_panel.getTextSizeFit(_ratio),load_game_panel.getTextSizeFit(_ratio),setting_panel.getTextSizeFit(_ratio),customize_panel.getTextSizeFit(_ratio) });

	start_game_button.updateTextSizeFit(_ratio);
	go_back_button.updateTextSizeFit(_ratio);
	new_game_panel.updateTextSize(_min_button_text_size);
	load_game_panel.updateTextSize(_min_button_text_size);
	setting_panel.updateTextSize(_min_button_text_size);
	customize_panel.updateTextSize(_min_button_text_size);

	resizeNewPanel();
	resizeLoadPanel();
	resizeSettingPanel();
	resizeCustomizePanel();
}
