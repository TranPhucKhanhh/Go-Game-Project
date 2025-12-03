#include<SFML/Graphics.hpp>
#include<UI/GameOption.h>
#include<core/AssetManager.h>
#include<model/SettingPanel.h>
#include<model/GameState.h>
#include<algorithm>

void GameOption::drawLoadPanel() {
	if (board_preview_show) {
		board_preview.updateCellNumber(game.getTempBoard().size());
		board_preview.draw(ui_cfg.window, game.getTempBoard());
	}
	load_test_button.draw(ui_cfg.window);
}

void GameOption::eventHandleLoadPanel(const sf::Event& event, std::string& respond) {
	std::string event_respond = "";
	load_test_button.eventHandle(event, ui_cfg, event_respond);
	if (event_respond == "LoadTest1") {
		game.loadPreviewGame("save1");
		board_preview_show = true;
		game.getTempBoard().printBoard(game.getTempBoard());

		board_preview.updateCellNumber(game.getTempBoard().size());
		GameOption::resizeLoadPanel();
	}
}

void GameOption::resizeLoadPanel() {
	float _margin = canvas_size.x / 15.f;
	float _inner_margin = _margin / 2.f;
	float _component_size_y = _margin / 2.f;

	sf::Vector2f _button_size = { (canvas_size.x - _margin * 2 - _inner_margin * 3) / 4.f , _component_size_y * 2 };
	float _board_size = std::min( canvas_size.x - _margin * 4, canvas_size.y - _margin * 4 - _button_size.y );

	load_test_button.updateSize(_button_size);
	load_test_button.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _margin + _button_size.x / 2.f, new_game_panel.getPos().y + new_game_panel.getSize().y / 2.f + _margin + _button_size.y / 2.f});
	load_test_button.updateTextSizeFit(0.9f);

	board_preview.updateSize(_board_size);
	board_preview.updatePos({ canvas.getPosition().x - canvas_size.x / 2.f + _margin + _board_size / 2.f, load_test_button.getPos().y + _inner_margin + _board_size / 2.f});
	board_preview.updateStoneTexture(asset_manager.getTexture("white-stone-default"), asset_manager.getTexture("black-stone-default"));
	board_preview.updateTexture(asset_manager.getTexture("board-minimal"));
	board_preview.update();
}