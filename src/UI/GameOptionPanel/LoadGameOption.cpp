#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<string.h>
#include<algorithm>

void GameOption::drawLoadPanel() {
	if (board_preview_show) {
		board_preview.updateCellNumber(game.getCurrentBoard().size());
		board_preview.draw(ui_cfg.window, game.getCurrentBoard());
		delete_button.draw(ui_cfg.window);
		board_preview_title.draw(ui_cfg.window);
	}
	refresh_button.draw(ui_cfg.window);
	load_game_scroll.draw(ui_cfg.window);
}

void GameOption::eventHandleLoadPanel(const sf::Event& event, std::string& respond) {
	std::string event_respond = "";
	refresh_button.eventHandle(event, ui_cfg, event_respond);
	load_game_scroll.eventHandle(event, ui_cfg, event_respond);
	delete_button.eventHandle(event, ui_cfg, event_respond);

	if (event_respond == "Refresh") {
		GameOption::loadGameFile();
	}
	else if (event_respond != "" && event_respond[0] == '-') {
		std::string game_name = event_respond.substr(1);

		game.loadPreviewGame(game_name);
		file_to_load = game_name;

		board_preview_show = true;

		board_preview.updateCellNumber(game.getCurrentBoard().size());
		GameOption::resizeLoadPanel();
	}
	else if (event_respond == "Delete") {
		if (board_preview_show) {
			
			board_preview_show = false;
			game.deleteSaveGame(file_to_load);
			file_to_load = "";

			loadGameFile();
		}
	}

	board_preview_title.updateStr(file_to_load);
	board_preview_title.updateTextSizeFitWithScale(0.9);
}

void GameOption::resizeLoadPanel() {
	float _margin = canvas_size.x / 15.f;
	float _inner_margin = _margin / 2.f;

	sf::Vector2f _button_size = { (canvas_size.x - _margin * 2) / 5.f , _margin };
	sf::Vector2f _load_size = { (canvas_size.x - _margin * 2) / 3.f , canvas_size.y - _margin - _inner_margin * 2 - _button_size.y - start_game_button.getSize().y - new_game_panel.getSize().y};
	float _board_size = std::min( canvas_size.x - _margin * 2 - _inner_margin - _load_size.x, canvas_size.y - _margin * 2 - _inner_margin - _button_size.y);

	refresh_button.updateSize(_button_size);
	refresh_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _margin + _button_size.x / 2.f, new_game_panel.getPos().y + new_game_panel.getSize().y / 2.f + _margin + _button_size.y / 2.f});
	refresh_button.updateTextSizeFit(0.9f);

	load_game_scroll.updateSize(_load_size);
	load_game_scroll.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _margin + _load_size.x / 2.f, refresh_button.getPos().y + refresh_button.getSize().y/2.f + _inner_margin + _load_size.y / 2.f });

	board_preview.updateSize(_board_size);
	board_preview.updatePos({load_game_scroll.getPos().x + _load_size.x /2.f + _inner_margin + _board_size / 2.f, load_game_scroll.getPos().y});
	board_preview.updateStoneTexture(asset_manager.getTexture("white-stone-default"), asset_manager.getTexture("black-stone-default"));
	board_preview.updateTexture(asset_manager.getTexture("board-minimal"));
	board_preview.update();

	delete_button.updateSize(_button_size);
	delete_button.updatePos({ board_preview.getPos().x - board_preview.getSize().x / 2.f + _button_size.x/2.f, board_preview.getPos().y - board_preview.getSize().y / 2.f - _button_size.y / 2.f - _inner_margin });
	delete_button.updateTextSizeFit(0.9f);

	board_preview_title.updateBoxSize({ _board_size,  _inner_margin});
	board_preview_title.updateBoxPos({ board_preview.getPos().x - board_preview.getSize().x / 2.f + _board_size / 2.f, board_preview.getPos().y - board_preview.getSize().y / 2.f - _inner_margin/2 });
}

void GameOption::loadGameFile() {
	std::vector<std::string> save_files = game.loadAllSaveGameName();
	load_game_scroll.clearContent();
	for (int i = 0; i < save_files.size(); i++) {
		TextButton _tmp(save_files[i], asset_manager.getFont("StackSansNotch-Regular"));
		_tmp.updateRespondStr('-' + save_files[i]);
		load_game_scroll.udpateContent(_tmp);
	}
	load_game_scroll.updateIndex(0);
	load_game_scroll.updateState();
}