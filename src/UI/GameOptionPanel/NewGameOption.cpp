#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<model/CellState.h>
#include<algorithm>
#include<cmath>
#include<iomanip>
#include<sstream>
#include<initializer_list>
#include<stdexcept>

void GameOption::drawNewPanel() {
	ui_cfg.window.draw(game_mode_title);

	pvp_mode_button.updateForceTex(0);
	ai_easy_button.updateForceTex(0);
	ai_medium_button.updateForceTex(0);
	ai_hard_button.updateForceTex(0);
	white_side_button.updateForceTex(0);
	black_side_button.updateForceTex(0);
	board_size_9_button.updateForceTex(0);
	board_size_13_button.updateForceTex(0);
	board_size_19_button.updateForceTex(0);

	if (game_mode == GameMode::PvP) {
		pvp_mode_button.updateForceTex(2);
	}
	else if (game_mode == GameMode::AIEasy) {
		ai_easy_button.updateForceTex(2);
	}
	else if (game_mode == GameMode::AIMedium) {
		ai_medium_button.updateForceTex(2);
	}
	else if (game_mode == GameMode::AIHard) {
		ai_hard_button.updateForceTex(2);
	}

	if (side_chosen == CellState::Black) {
		white_side_button.updateForceTex(2);
	}
	else  {
		black_side_button.updateForceTex(2);
	}

	if (board_size_chosen == 9) {
		board_size_9_button.updateForceTex(2);
	}
	else if (board_size_chosen == 13) {
		board_size_13_button.updateForceTex(2);
	}
	else if (board_size_chosen == 19) {
		board_size_19_button.updateForceTex(2);
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
	ui_cfg.window.draw(time_control_title);
	untimed_button.updateForceTex(main_time_chosen == 0 ? 2 : 0);
	minute_1_button.updateForceTex(main_time_chosen == 60 ? 2 : 0);
	minute_5_button.updateForceTex(main_time_chosen == 300 ? 2 : 0);
	minute_10_button.updateForceTex(main_time_chosen == 600 ? 2 : 0);
	untimed_button.draw(ui_cfg.window);
	minute_1_button.draw(ui_cfg.window);
	minute_5_button.draw(ui_cfg.window);
	minute_10_button.draw(ui_cfg.window);
	std::ostringstream komi_text;
	komi_text << std::fixed << std::setprecision(1) << komi_chosen;
	komi_button.updateStr("Komi: " + komi_text.str() + " (edit)");
	komi_button.updateTextSizeFit(0.85f);
	komi_button.draw(ui_cfg.window);
}

void GameOption::eventHandleNewPanel(const sf::Event& event, std::string& respond) {
	std::string event_respond = "";
	if (komi_input.onScreen()) {
		komi_input.eventHandle(event, ui_cfg, event_respond);
		if (event_respond == "ApplyKomi") {
			try {
				const std::string input = komi_input.getInputStr();
				std::size_t used = 0;
				const float value = std::stof(input, &used);
				if (used != input.size() || !std::isfinite(value) || value < -50.f || value > 50.f ||
					std::abs(value * 2.f - std::round(value * 2.f)) > 0.0001f)
					throw std::invalid_argument("Invalid komi");
				komi_chosen = value;
				komi_input.updateOnScreen(false);
			}
			catch (const std::exception&) {
				komi_input.updateTitleStr("Invalid komi");
				komi_input.updateNotificationStr("Enter -50 to 50, in steps of 0.5");
			}
		}
		return;
	}

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
	untimed_button.eventHandle(event, ui_cfg, event_respond);
	minute_1_button.eventHandle(event, ui_cfg, event_respond);
	minute_5_button.eventHandle(event, ui_cfg, event_respond);
	minute_10_button.eventHandle(event, ui_cfg, event_respond);
	komi_button.eventHandle(event, ui_cfg, event_respond);
	if (event_respond == "Time0") main_time_chosen = 0;
	else if (event_respond == "Time60") main_time_chosen = 60;
	else if (event_respond == "Time300") main_time_chosen = 300;
	else if (event_respond == "Time600") main_time_chosen = 600;
	else if (event_respond == "EditKomi") {
		komi_input.clearStr();
		komi_input.updateTitleStr("Edit komi");
		komi_input.updateNotificationStr("Enter -50 to 50, in steps of 0.5");
		komi_input.updateOnScreen(true);
	}

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
		side_chosen = CellState::Black;
	}
	else if (event_respond == "BlackSide") {
		side_chosen = CellState::White;
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
	const float pad = canvas_size.x * 0.055f;
	const float gap = pad * 0.5f;
	const float left = canvas.getPosition().x - canvas_size.x / 2.f + pad;
	const float top = canvas.getPosition().y - canvas_size.y / 2.f + canvas_size.y * 0.14f;
	const int row_count = game_mode == GameMode::PvP ? 4 : 5;
	const float row_height = canvas_size.y * 0.72f / row_count;
	int row = 0;
	auto layout_row = [&](sf::Text& title, std::initializer_list<TextureEffectButton*> buttons) {
		const float y = top + row++ * row_height;
		title.setCharacterSize(static_cast<unsigned int>(canvas_size.y * 0.027f));
		title.setPosition({ left, y });
		const float width = (canvas_size.x - 2.f * pad - gap * (buttons.size() - 1)) / buttons.size();
		int index = 0;
		for (auto* button : buttons) {
			button->updateSize({ width, row_height * 0.40f });
			button->updatePos({ left + width / 2.f + index++ * (width + gap), y + row_height * 0.58f });
			button->updateTextSizeFit(0.85f);
		}
	};
	layout_row(game_mode_title, { &pvp_mode_button, &ai_easy_button, &ai_medium_button, &ai_hard_button });
	if (game_mode != GameMode::PvP)
		layout_row(choose_side_title, { &black_side_button, &white_side_button });
	layout_row(choose_board_size_title, { &board_size_9_button, &board_size_13_button, &board_size_19_button });
	layout_row(time_control_title, { &untimed_button, &minute_1_button, &minute_5_button, &minute_10_button });
	komi_button.updateSize({ (canvas_size.x - 2.f * pad) * 0.65f, row_height * 0.45f });
	komi_button.updatePos({ canvas.getPosition().x, top + row * row_height + row_height * 0.45f });
	komi_button.updateTextSizeFit(0.85f);
	komi_input.updateSize({ canvas_size.x * 0.80f, canvas_size.y * 0.38f });
	komi_input.updatePos(canvas.getPosition());
}
