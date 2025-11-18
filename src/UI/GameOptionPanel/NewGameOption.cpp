#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<model/CellState.h>
#include<algorithm>

void GameOption::drawNewPanel() {
	ui_cfg.window.draw(game_mode_title);

	pvp_mode_button.updateOpacity(100);
	ai_easy_button.updateOpacity(100);
	ai_medium_button.updateOpacity(100);
	ai_hard_button.updateOpacity(100);
	white_side_button.updateOpacity(100);
	black_side_button.updateOpacity(100);
	board_size_9_button.updateOpacity(100);
	board_size_13_button.updateOpacity(100);
	board_size_19_button.updateOpacity(100);

	if (game_mode == GameMode::PvP) {
		pvp_mode_button.updateOpacity(255);
	}
	else if (game_mode == GameMode::AIEasy) {
		ai_easy_button.updateOpacity(255);
	}
	else if (game_mode == GameMode::AIMedium) {
		ai_medium_button.updateOpacity(255);
	}
	else if (game_mode == GameMode::AIHard) {
		ai_hard_button.updateOpacity(255);
	}

	if (side_chosen == CellState::White) {
		white_side_button.updateOpacity(255);
	}
	else  {
		black_side_button.updateOpacity(255);
	}

	if (board_size_chosen == 9) {
		board_size_9_button.updateOpacity(255);
	}
	else if (board_size_chosen == 13) {
		board_size_13_button.updateOpacity(255);
	}
	else if (board_size_chosen == 19) {
		board_size_19_button.updateOpacity(255);
	}


	pvp_mode_button.draw(ui_cfg.window);
	ai_easy_button.draw(ui_cfg.window);
	ai_medium_button.draw(ui_cfg.window);
	ai_hard_button.draw(ui_cfg.window);

	if (game_mode != GameMode::PvP) {
		ui_cfg.window.draw(choose_side_title);
		white_side_button.draw(ui_cfg.window);
		black_side_button.draw(ui_cfg.window);
	}

	ui_cfg.window.draw(choose_board_size_title);
	board_size_9_button.draw(ui_cfg.window);
	board_size_13_button.draw(ui_cfg.window);
	board_size_19_button.draw(ui_cfg.window);
}

void GameOption::eventHandleNewPanel(const sf::Event& event, std::string& respond) {
	std::string event_respond = "";

	if (game_mode != GameMode::PvP) {
		black_side_button.eventHandle(event, ui_cfg, event_respond);
		white_side_button.eventHandle(event, ui_cfg, event_respond);
	}
	
	pvp_mode_button.eventHandle(event, ui_cfg, event_respond);
	ai_medium_button.eventHandle(event, ui_cfg, event_respond);
	ai_hard_button.eventHandle(event, ui_cfg, event_respond);
	ai_easy_button.eventHandle(event, ui_cfg, event_respond);

	board_size_9_button.eventHandle(event, ui_cfg, event_respond);
	board_size_13_button.eventHandle(event, ui_cfg, event_respond);
	board_size_19_button.eventHandle(event, ui_cfg, event_respond);

	if (event_respond == "pvpMode") {
		game_mode = GameMode::PvP;
		resizeNewPanel();
	}
	else if (event_respond == "AIeasyMode") {
		game_mode = GameMode::AIEasy;
		resizeNewPanel();
	}
	else if (event_respond == "AIMediumMode") {
		game_mode = GameMode::AIMedium;
		resizeNewPanel();
	}
	else if (event_respond == "AIHardMode") {
		game_mode = GameMode::AIHard;
		resizeNewPanel();
	}
	else if (event_respond == "WhiteSide") {
		side_chosen = CellState::White;
	}
	else if (event_respond == "BlackSide") {
		side_chosen = CellState::Black;
	}
	else if (event_respond == "9x9") {
		board_size_chosen = 9;
	}
	else if (event_respond == "13x13") {
		board_size_chosen = 13;
	}
	else if (event_respond == "19x19") {
		board_size_chosen = 19;
	}
}

void GameOption::resizeNewPanel() {
	float _margin = canvas_size.x / 15.f;
	float _inner_margin = _margin / 2.f;
	float _component_size_y = _margin / 2.f;

	std::cerr << _margin << " " << _inner_margin << std::endl;

	sf::Vector2f _mode_button_size = { (canvas_size.x - _margin * 2 - _inner_margin * 3) / 4.f , _component_size_y * 2 };

	game_mode_title.setCharacterSize(_component_size_y);
	//game_mode_title.setOrigin(game_mode_title.getLocalBounds().getCenter());
	game_mode_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, new_game_panel.getPos().y + new_game_panel.getSize().y / 2.f + _margin });

	pvp_mode_button.updateSize(_mode_button_size);
	pvp_mode_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x / 2.f + _margin, game_mode_title.getPosition().y + _component_size_y + _margin });

	ai_easy_button.updateSize(_mode_button_size);
	ai_easy_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x / 2.f + _margin + _inner_margin + _mode_button_size.x, game_mode_title.getPosition().y + _component_size_y + _margin });

	ai_medium_button.updateSize(_mode_button_size);
	ai_medium_button.updatePos({ canvas.getPosition().x + canvas_size.x / 2.f - _mode_button_size.x / 2.f - _margin - _inner_margin - _mode_button_size.x, game_mode_title.getPosition().y + _component_size_y + _margin });

	ai_hard_button.updateSize(_mode_button_size);
	ai_hard_button.updatePos({ canvas.getPosition().x + canvas_size.x / 2.f - _mode_button_size.x / 2.f - _margin , game_mode_title.getPosition().y + _component_size_y + _margin });

	float _ratio = 0.9;
	float _min_button_text_size = std::min({ pvp_mode_button.getTextSizeFit(_ratio),ai_easy_button.getTextSizeFit(_ratio),
		ai_medium_button.getTextSizeFit(_ratio),ai_hard_button.getTextSizeFit(_ratio) });

	pvp_mode_button.updateTextSize(_min_button_text_size);
	ai_easy_button.updateTextSize(_min_button_text_size);
	ai_medium_button.updateTextSize(_min_button_text_size);
	ai_hard_button.updateTextSize(_min_button_text_size);

	float _start_tmp;
	if (game_mode != GameMode::PvP) {
		choose_side_title.setCharacterSize(_component_size_y);
		choose_side_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, ai_hard_button.getPos().y + ai_hard_button.getSize().y / 2.f + _inner_margin });

		white_side_button.updateSize(_mode_button_size);
		white_side_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x / 2.f + _margin, choose_side_title.getPosition().y + _component_size_y + _margin });

		black_side_button.updateSize(_mode_button_size);
		black_side_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x / 2.f + _margin + _inner_margin + _mode_button_size.x, choose_side_title.getPosition().y + _component_size_y + _margin });

		_min_button_text_size = std::min(white_side_button.getTextSizeFit(_ratio), black_side_button.getTextSizeFit(_ratio));
		white_side_button.updateTextSize(_min_button_text_size);
		black_side_button.updateTextSize(_min_button_text_size);

		_start_tmp = white_side_button.getPos().y + _mode_button_size.y / 2.f + _inner_margin;
	}
	else {
		_start_tmp = ai_easy_button.getPos().y + _mode_button_size.y / 2.f + _inner_margin;
	}

	std::cerr << _start_tmp << std::endl;

	choose_board_size_title.setCharacterSize(_component_size_y);
	choose_board_size_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin , _start_tmp });

	board_size_9_button.updateSize(_mode_button_size);
	board_size_9_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x / 2.f + _margin, choose_board_size_title.getPosition().y + _component_size_y + _margin });

	board_size_13_button.updateSize(_mode_button_size);
	board_size_13_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x * 1.5f + _margin + _inner_margin, choose_board_size_title.getPosition().y + _component_size_y + _margin });

	board_size_19_button.updateSize(_mode_button_size);
	board_size_19_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _mode_button_size.x * 2.5f + _margin + _inner_margin * 2, choose_board_size_title.getPosition().y + _component_size_y + _margin });

	_min_button_text_size = std::min({ board_size_9_button.getTextSizeFit(_ratio), board_size_13_button.getTextSizeFit(_ratio), board_size_19_button.getTextSizeFit(_ratio)});
	board_size_9_button.updateTextSize(_min_button_text_size);
	board_size_13_button.updateTextSize(_min_button_text_size);
	board_size_19_button.updateTextSize(_min_button_text_size);
}