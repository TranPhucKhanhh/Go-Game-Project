#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<algorithm>

void GameOption::drawCustomizePanel() {
	ui_cfg.window.draw(board_design_title);
	ui_cfg.window.draw(stone_design_title);
	ui_cfg.window.draw(design_preview_title);

	board_design_scroll.draw(ui_cfg.window);
	stone_design_scroll.draw(ui_cfg.window);
	customize_board_preview.draw(ui_cfg.window, game.getExampleBoard());
}

void GameOption::eventHandleCustomizePanel(const sf::Event& event, std::string& respond) {
	std::string event_respond = "";
	board_design_scroll.eventHandle(event, ui_cfg, event_respond);
	stone_design_scroll.eventHandle(event, ui_cfg, event_respond);

	if (event_respond.size() > 0 && event_respond[0] == '[') {
		std::string _board_design = event_respond.substr(1);
		customize_board_preview.updateBoardUI(_board_design, asset_manager);
		board_preview.updateBoardUI(_board_design, asset_manager);
		ui_cfg.board_design = _board_design;
	}

	if (event_respond.size() > 0 && event_respond[0] == ']') {
		std::string _stone_design = event_respond.substr(1);
		customize_board_preview.updateStoneUI(_stone_design, asset_manager);
		board_preview.updateStoneUI(_stone_design, asset_manager);
		ui_cfg.stone_design = _stone_design;
	}
}

void GameOption::resizeCustomizePanel() {
	float _margin = canvas_size.x / 15.f;
	float _inner_margin = _margin / 2.f;
	float _component_size_y = _margin / 2.f;
	sf::Vector2f _scroll_size = { (canvas_size.x - _margin * 2) / 3.f , canvas_size.y / 4.f };

	board_design_title.setCharacterSize(_component_size_y);
	board_design_title.setPosition({ canvas.getPosition().x - canvas_size.x / 2.f + _margin, new_game_panel.getPos().y + new_game_panel.getSize().y / 2.f + _margin });

	board_design_scroll.updateSize(_scroll_size);
	board_design_scroll.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _margin + _scroll_size.x / 2.f, board_design_title.getPosition().y + _inner_margin + _scroll_size.y / 2.f + _component_size_y / 2.f });

	stone_design_title.setCharacterSize(_component_size_y);
	stone_design_title.setPosition({ board_design_title.getPosition().x, board_design_scroll.getPos().y + _component_size_y / 2.f + _inner_margin + _scroll_size.y / 2.f });

	stone_design_scroll.updateSize(_scroll_size);
	stone_design_scroll.updatePos({ board_design_scroll.getPos().x, stone_design_title.getPosition().y + _inner_margin + _scroll_size.y / 2.f + _component_size_y / 2.f });

	float _board_size = std::min({ canvas_size.x - _scroll_size.x - 2 * _margin - _inner_margin, canvas.getPosition().y + canvas_size.y / 2.f - (stone_design_scroll.getPos().y - _scroll_size.y / 2.f) - _margin });

	std::cerr << "Board size: " << canvas_size.x - _scroll_size.x - 2 * _margin - _inner_margin << " " << canvas.getPosition().y + canvas_size.y / 2.f - stone_design_scroll.getPos().y - _scroll_size.y / 2.f - _margin << std::endl;
	std::cerr << "Board size: " << _board_size << std::endl;
	customize_board_preview.updateSize(_board_size);
	customize_board_preview.updatePos({ canvas.getPosition().x + canvas_size.x / 2.f - _margin - _board_size / 2.f,
		stone_design_title.getPosition().y + _inner_margin + _board_size / 2.f + _component_size_y / 2.f });
	customize_board_preview.update();

	design_preview_title.setCharacterSize(_component_size_y);
	design_preview_title.setOrigin(design_preview_title.getLocalBounds().getCenter());
	design_preview_title.setPosition({ customize_board_preview.getPos().x, customize_board_preview.getPos().y + _board_size / 2.f + _component_size_y / 2.f + _inner_margin / 2.f });
}