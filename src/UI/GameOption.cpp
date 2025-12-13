#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<model/CellState.h>
#include<algorithm>

GameOption::GameOption(const AssetManager& _asset_manager, Game& _game, UICfg& ui_cfg) :
	asset_manager(_asset_manager), game(_game), ui_cfg(ui_cfg),

	start_game_button("Start a new game", asset_manager.getFont("Spicy-Sale"), ui_cfg),
	exit_button("Exit game", asset_manager.getFont("Spicy-Sale"), ui_cfg),
	new_game_panel("New game", asset_manager.getFont("RobotoSlab-Bold"), ui_cfg),
	go_back_button("Back", asset_manager.getFont("Spicy-Sale"), ui_cfg),
	game_mode_title(asset_manager.getFont("RobotoSlab-Bold"), "Select game mode:"),
	pvp_mode_button("PvP", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	ai_easy_button("AI easy", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	ai_medium_button("AI medium", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	ai_hard_button("AI hard", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	choose_side_title(asset_manager.getFont("RobotoSlab-Bold"), "Choose your side:"),
	black_side_button("Black", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	white_side_button("White", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	choose_board_size_title(asset_manager.getFont("RobotoSlab-Bold"), "Choose your board size:"),
	board_size_9_button("9 x 9", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	board_size_13_button("13 x 13", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),
	board_size_19_button("19 x 19", asset_manager.getFont("StackSansNotch-Bold"), ui_cfg),

	load_game_panel("Load game", asset_manager.getFont("RobotoSlab-Bold"), ui_cfg),
	refresh_button("Refresh", asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
	delete_button("Delete", asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
	board_preview_title(asset_manager.getFont("Momo")),
	board_preview(_asset_manager),
	load_game_scroll(ui_cfg),

	setting_panel("Setting", asset_manager.getFont("RobotoSlab-Bold"), ui_cfg),
	music_scroll_title(asset_manager.getFont("Momo"), "Choose your music: " + ui_cfg.music_name),
	effect_scroll_title(asset_manager.getFont("Momo"), "In-game sound effect: " + ui_cfg.sound_effect_name),
	music_volume_title(asset_manager.getFont("Momo"), "Music Volume: " + std::to_string((int)ui_cfg.background_music_volume) + "%"),
	sound_effect_volume_title(asset_manager.getFont("Momo"), "Sound effect Volume: " + std::to_string((int)ui_cfg.sound_effect_volume) + "%"),
	background_music_slide(_asset_manager, ui_cfg),
	sound_effect_slide(_asset_manager, ui_cfg),
	music_scroll(ui_cfg),
	effect_scroll(ui_cfg),

	customize_panel("Customization", asset_manager.getFont("RobotoSlab-Bold"), ui_cfg),
	customize_board_preview(_asset_manager),
	board_design_title(asset_manager.getFont("Momo"), "Choose your board design: " + ui_cfg.board_design),
	stone_design_title(asset_manager.getFont("Momo"), "Choose your stone design: " + ui_cfg.stone_design),
	design_preview_title(asset_manager.getFont("Momo"), "Board design preview"),
	board_design_scroll(ui_cfg),
	stone_design_scroll(ui_cfg)

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
	exit_button.updateRespondStr("Exit");

	// Initialzize the basic color and outline
	canvas.setFillColor({ 221,221,221 });
	canvas.setOutlineThickness(3);
	canvas.setOutlineColor(sf::Color::Black);

	game_mode_title.setFillColor(sf::Color::Black);
	choose_side_title.setFillColor(sf::Color::Black);
	choose_board_size_title.setFillColor(sf::Color::Black);

	board_preview_title.updateBoxColor(sf::Color(255, 255, 255, 0));
	board_preview_title.updateTextColor(sf::Color::Black);

	music_scroll_title.setFillColor(sf::Color::Black);
	effect_scroll_title.setFillColor(sf::Color::Black);
	music_volume_title.setFillColor(sf::Color::Black);
	sound_effect_volume_title.setFillColor(sf::Color::Black);

	board_design_title.setFillColor(sf::Color::Black);
	stone_design_title.setFillColor(sf::Color::Black);
	design_preview_title.setFillColor(sf::Color::Black);

	board_preview.updateBoardUI(ui_cfg.board_design, asset_manager);
	board_preview.updateStoneUI(ui_cfg.stone_design, asset_manager);
	customize_board_preview.updateBoardUI(ui_cfg.board_design, asset_manager);
	customize_board_preview.updateStoneUI(ui_cfg.stone_design, asset_manager);

	// Button Effect
	go_back_button.updateTextEffectColor(sf::Color::White);
	go_back_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-red"));
	go_back_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-red"));
	go_back_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	start_game_button.updateTextEffectColor(sf::Color::White);
	start_game_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-red"));
	start_game_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-red"));
	start_game_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	exit_button.updateTextEffectColor(sf::Color::White);
	exit_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-red"));
	exit_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-red"));
	exit_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	new_game_panel.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
	new_game_panel.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
	new_game_panel.updateHoverTex(asset_manager.getTexture("button_rectangle_flat-yellow"));
	new_game_panel.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	load_game_panel.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
	load_game_panel.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
	load_game_panel.updateHoverTex(asset_manager.getTexture("button_rectangle_flat-yellow"));
	load_game_panel.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	setting_panel.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
	setting_panel.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
	setting_panel.updateHoverTex(asset_manager.getTexture("button_rectangle_flat-yellow"));
	setting_panel.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	customize_panel.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
	customize_panel.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
	customize_panel.updateHoverTex(asset_manager.getTexture("button_rectangle_flat-yellow"));
	customize_panel.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

	pvp_mode_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	pvp_mode_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	pvp_mode_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	pvp_mode_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	ai_easy_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	ai_easy_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	ai_easy_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	ai_easy_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	ai_medium_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	ai_medium_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	ai_medium_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	ai_medium_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	ai_hard_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	ai_hard_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	ai_hard_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	ai_hard_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	white_side_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	white_side_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	white_side_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	white_side_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	black_side_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	black_side_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	black_side_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	black_side_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	board_size_9_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	board_size_9_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	board_size_9_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	board_size_9_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	board_size_13_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	board_size_13_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	board_size_13_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	board_size_13_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	board_size_19_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	board_size_19_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	board_size_19_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	board_size_19_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	refresh_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	refresh_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	refresh_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	refresh_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	delete_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
	delete_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
	delete_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
	delete_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

	// Initial value
	load_game_scroll.updatePreviewSize(7);
	loadGameFile();
	background_music_slide.value = ui_cfg.background_music_volume;
	sound_effect_slide.value = ui_cfg.sound_effect_volume;
	
	music_scroll.updatePreviewSize(5);
	for (int i = 0; i < std::size(Music); i++) {
		TextButton _music1(Music[i], asset_manager.getFont("StackSansNotch-Regular"), ui_cfg); 
		_music1.updateRespondStr("~" + Music[i]);
		_music1.updateColor({ 91, 164, 252 });
		music_scroll.updateContent(_music1);
	}
	effect_scroll.updatePreviewSize(5);
	for (int i = 0; i < std::size(SoundEffect); i++) {
		TextButton _effect(SoundEffect[i], asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
		_effect.updateRespondStr("`" + SoundEffect[i]);
		_effect.updateColor({ 91, 164, 252 });
		effect_scroll.updateContent(_effect);
	}
	
	customize_board_preview.updateCellNumber(13);
	board_design_scroll.updatePreviewSize(4);
	stone_design_scroll.updatePreviewSize(4);
	for (int i = 0; i < std::size(BoardDesign); ++i) {
		TextButton _button(BoardDesign[i], asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
		_button.updateRespondStr("[" + BoardDesign[i]);
		_button.updateColor({ 91, 164, 252 });
		board_design_scroll.updateContent(_button);
	}

	for (int i = 0; i < std::size(StoneDesign); ++i) {
		TextButton _button(StoneDesign[i], asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
		_button.updateRespondStr("]" + StoneDesign[i]);
		_button.updateColor({ 91, 164, 252 });
		stone_design_scroll.updateContent(_button);
	}

	resize();

	std::cerr << "Load game Option UI successfully." << std::endl;
}

void GameOption::updateLoadNewButton() {
	if (panel == SettingPanel::NewGame) {
		start_game_button.updateStr("New Game");
		start_game_button.updateRespondStr("StartGame");
	}
	else {
		start_game_button.updateStr("Load Game");
		start_game_button.updateRespondStr("LoadNewGame");
	}
}

void GameOption::enter() {
	board_preview_show = false;
	game_mode = GameMode::PvP;
	side_chosen = CellState::Black;
	board_size_chosen = 19;
	board_design_chosen = ui_cfg.board_design; 
	stone_design_chosen = ui_cfg.stone_design;
	music_scroll.updateIndex(0);
	effect_scroll.updateIndex(0);
	board_design_scroll.updateIndex(0);
	stone_design_scroll.updateIndex(0);
	// Update scroll value according to the game music & sound 
	music_volume_title.setString("Music Volume: " + std::to_string((int)ui_cfg.background_music_volume) + "%");
	sound_effect_volume_title.setString("Sound Effect Volume: " + std::to_string((int)ui_cfg.sound_effect_volume) + "%");
	background_music_slide.value = ui_cfg.background_music_volume;
	sound_effect_slide.value = ui_cfg.sound_effect_volume;
	updateLoadNewButton();
	updateEventHandle();
	resize();
}

void GameOption::draw() {
	ui_cfg.window.clear(sf::Color(240, 217, 181));

	ui_cfg.window.draw(canvas);

	new_game_panel.updateForceTex(0);
	load_game_panel.updateForceTex(0);
	setting_panel.updateForceTex(0);
	customize_panel.updateForceTex(0);
	if (panel == SettingPanel::NewGame) {
		new_game_panel.updateForceTex(2);
		drawNewPanel();
	}
	else if (panel == SettingPanel::LoadGame) {
		load_game_panel.updateForceTex(2);
		drawLoadPanel();
	}
	else if (panel == SettingPanel::Setting) {
		setting_panel.updateForceTex(2);
		drawSettingPanel();
	}
	else if (panel == SettingPanel::Customization) {
		customize_panel.updateForceTex(2);
		drawCustomizePanel();
	}

	if (panel == SettingPanel::NewGame || panel == SettingPanel::LoadGame) {
		start_game_button.draw(ui_cfg.window);
	}
	go_back_button.draw(ui_cfg.window);
	exit_button.draw(ui_cfg.window);
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
	go_back_button.eventHandle(event, ui_cfg, event_respond);
	exit_button.eventHandle(event, ui_cfg, event_respond);

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
	else if (event_respond == "Exit") {
		respond = "GameExit";
	}

	// Change the start game button text according to the panel
	if (event_respond == "NewGame" || event_respond == "LoadGame") {
		updateLoadNewButton();
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
	exit_button.updateSize(go_back_button.getSize());
	exit_button.updatePos({ customize_panel.getPos().x , go_back_button.getPos().y});
	exit_button.updateTextSizeFit(0.9f);

	float _ratio = 1;
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
