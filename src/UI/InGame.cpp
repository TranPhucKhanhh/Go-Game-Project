#include<SFML/Graphics.hpp>
#include<UI/InGame.h>
#include<UI/Component.h>
#include<model/UICfg.h>
#include<model/CellState.h>
#include<game/Game.h>
#include<algorithm>
#include<iostream>

InGame::InGame(const AssetManager& _asset_manager, Game& _game, UICfg& ui_cfg) : 
    asset_manager(_asset_manager), game(_game), ui_cfg(ui_cfg),
    header_bar(_asset_manager.getFont("Roboto-Slab-Bold")),
    undo_button("Undo", _asset_manager.getFont("StackSansNotch-Regular")),
    redo_button("Redo", _asset_manager.getFont("StackSansNotch-Regular")), 
    pass_button("Pass", _asset_manager.getFont("StackSansNotch-Regular")),
    new_button("New Game", _asset_manager.getFont("StackSansNotch-Regular")),
    reset_button("Reset Gane", _asset_manager.getFont("StackSansNotch-Regular")),
    save_button("Save game", _asset_manager.getFont("StackSansNotch-Regular")) {
   
	undo_button.updateRespondStr("Undo");
	redo_button.updateRespondStr("Redo");
	pass_button.updateRespondStr("Pass");
	new_button.updateRespondStr("New");
	reset_button.updateRespondStr("Reset");
	save_button.updateRespondStr("Save");


    updateHeaderBar();

    resize();

    std::cerr << "Load InGame UI successfully." << std::endl;
}

void InGame::enter() {
    board.updateCellNumber(game.getGameCfg().board_size);
    sf::Event _d = sf::Event::Closed{};
    std::string _dt = "Test";
    eventHandle(_d, _dt);
    resize();
}

void InGame::updateHeaderBar() {
    if (game.getCurrentPlayer() == CellState::White) {
        header_bar.updateStr("WHITE TO MOVE");
        header_bar.updateBoxColor(sf::Color::White);
        header_bar.updateTextColor(sf::Color::Black);
    }
    else {
        header_bar.updateStr("BLACK TO MOVE");
        header_bar.updateBoxColor(sf::Color::Black);
        header_bar.updateTextColor(sf::Color::White);
    }
}

void InGame::eventHandle(const sf::Event& event, std::string& respond) {
    if (event.is<sf::Event::Resized>()) {
        resize();
    }
    if (event.is<sf::Event::MouseMoved>()) {
        board.hoverStone(ui_cfg.mouse_pos, game);
    }
    if (event.is<sf::Event::MouseButtonReleased>()) {
        board.placeStone(ui_cfg.mouse_pos, game);

        updateHeaderBar();
    }

	std::string event_respond = "";
    new_button.eventHandle(event, ui_cfg, event_respond);
    reset_button.eventHandle(event, ui_cfg, event_respond);
    save_button.eventHandle(event, ui_cfg, event_respond);

    undo_button.eventHandle(event, ui_cfg, event_respond);
    redo_button.eventHandle(event, ui_cfg, event_respond);
    pass_button.eventHandle(event, ui_cfg, event_respond);

    if (event_respond != "") {
		std::cerr << "clicked: " << event_respond << std::endl;
	}

    if (event_respond == "Undo") {
        game.undo();
        updateHeaderBar();
    }
    else if (event_respond == "Redo") {
        game.redo();
        updateHeaderBar();
    }
    else if (event_respond == "Pass") {
        game.pass();
        updateHeaderBar();
    }
    else if (event_respond == "Reset") {
        game.reset();
        updateHeaderBar();
    }
    else if (event_respond == "New") {
        respond = "GameNewOption";
    }
}

void InGame::draw() {
    ui_cfg.window.clear(sf::Color(240, 217, 181));

    //ui_cfg.window.draw(side_panel);
	ui_cfg.window.draw(mode_panel);
    ui_cfg.window.draw(option_panel);
    ui_cfg.window.draw(control_panel);
    ui_cfg.window.draw(history_panel);
    ui_cfg.window.draw(footer_bar);

    new_button.draw(ui_cfg.window);
    reset_button.draw(ui_cfg.window);
    save_button.draw(ui_cfg.window);

    undo_button.draw(ui_cfg.window);
    redo_button.draw(ui_cfg.window);
    pass_button.draw(ui_cfg.window);

    header_bar.draw(ui_cfg.window);

    board.draw(ui_cfg.window, game.getCurrentBoard());
}

void InGame::mode_panel_resize(const float& _total_height_panel) {
    mode_panel.setSize({ side_panel_size_x, _total_height_panel / 10.f });
    mode_panel.setFillColor(sf::Color(242, 176, 109));
    mode_panel.setOrigin(mode_panel.getLocalBounds().getCenter());
    mode_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2, ui_cfg.window_size.y / 2.f - _total_height_panel / 2.f + _total_height_panel / 20.f });
}

void InGame::option_panel_resize(const float& _total_height_panel) {
    float _button_padding = side_panel_size_x / 15.f;
    float _inner_button_padding = _button_padding / 1.5f;

    option_panel.setSize({ side_panel_size_x, _total_height_panel / 6.f });
    option_panel.setFillColor(sf::Color(242, 176, 109));
    option_panel.setOrigin(option_panel.getLocalBounds().getCenter());
    option_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        mode_panel.getPosition().y + mode_panel.getSize().y / 2.f + option_panel.getSize().y / 2.f + inner_padding });

    sf::Vector2f _button_size = { side_panel_size_x - _button_padding * 2, (option_panel.getSize().y - 2 * _button_padding - 3 * _inner_button_padding) / 3.f };

    new_button.updateSize(_button_size);
    new_button.updatePos({ option_panel.getPosition().x, option_panel.getPosition().y - _button_size.y - _inner_button_padding});
    
    reset_button.updateSize(_button_size);
    reset_button.updatePos({ option_panel.getPosition().x, option_panel.getPosition().y });
    
    save_button.updateSize(_button_size);
    save_button.updatePos({ option_panel.getPosition().x, option_panel.getPosition().y + _inner_button_padding + _button_size.y});
    
    float _button_text_size = std::min({ new_button.getTextSizeFit(0.8), reset_button.getTextSizeFit(0.8), save_button.getTextSizeFit(0.8) });

    new_button.updateTextSize(_button_text_size);
    reset_button.updateTextSize(_button_text_size);
    save_button.updateTextSize(_button_text_size);
}

void InGame::control_panel_resize(const float& _total_height_panel) {
    control_panel.setSize({ side_panel_size_x, _total_height_panel / 8.f });
    control_panel.setFillColor(sf::Color(242, 176, 109));
    control_panel.setOrigin(control_panel.getLocalBounds().getCenter());
    control_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        option_panel.getPosition().y + option_panel.getSize().y / 2.f + control_panel.getSize().y / 2.f + inner_padding });

    float _button_padding = side_panel_size_x / 15.f;
    float _inner_button_padding = _button_padding / 1.5;

    sf::Vector2f _button_size = { (side_panel_size_x - _inner_button_padding * 4) / 3.f, control_panel.getSize().y - _button_padding * 2 };

    undo_button.updateSize(_button_size);
    undo_button.updatePos({ control_panel.getPosition().x - _button_size.x - _inner_button_padding ,control_panel.getPosition().y });

    redo_button.updateSize(_button_size);
    redo_button.updatePos({ control_panel.getPosition().x ,control_panel.getPosition().y });

    pass_button.updateSize(_button_size);
    pass_button.updatePos({ control_panel.getPosition().x + _button_size.x + _inner_button_padding ,control_panel.getPosition().y });

    float _button_text_size = std::min({ undo_button.getTextSizeFit(0.8), redo_button.getTextSizeFit(0.8), pass_button.getTextSizeFit(0.8) });

    undo_button.updateTextSize(_button_text_size);
    redo_button.updateTextSize(_button_text_size);
    pass_button.updateTextSize(_button_text_size);
}

void InGame::history_panel_resize(const float& _total_height_panel) {
    history_panel.setSize({ side_panel_size_x,
        _total_height_panel - inner_padding * 3 - option_panel.getSize().y - mode_panel.getSize().y - control_panel.getSize().y });
    history_panel.setFillColor(sf::Color(242, 176, 109));
    history_panel.setOrigin(history_panel.getLocalBounds().getCenter());
    history_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        control_panel.getPosition().y + control_panel.getSize().y / 2.f + history_panel.getSize().y / 2.f + inner_padding });
}

void InGame::resize() {
    float min_scale = std::min(ui_cfg.window_size.x, ui_cfg.window_size.y);
    
    padding = min_scale / 10;
    canvas_size = { ui_cfg.window_size.x - padding, ui_cfg.window_size.y - padding };
    inner_padding = (min_scale - padding) / 30;
    side_panel_size_x = std::min(ui_cfg.window_size.y / 4.f, std::max(canvas_size.x / 7, 100.f));
    status_bar_size_y = std::min(500.f, std::max(canvas_size.y / 17, 75.f));
    board_size = std::min(canvas_size.y - (status_bar_size_y+inner_padding) * 2, canvas_size.x - inner_padding - side_panel_size_x);

    float _tmp = (ui_cfg.window_size.x - board_size - inner_padding - side_panel_size_x) / 2;
    float _total_height_panel = status_bar_size_y * 2 + inner_padding * 2 + board_size;

    board.updateSize( board_size );
    board.updatePos({_tmp + board_size / 2, ui_cfg.window_size.y / 2.f});
    board.updateStoneTexture(asset_manager.getTexture("white-stone-default"), asset_manager.getTexture("black-stone-default"));
    board.updateTexture(asset_manager.getTexture("board-minimal"));
    board.update();

    header_bar.updateBoxSize({ board_size, status_bar_size_y });
    header_bar.updateBoxPos({ _tmp + board_size / 2, ui_cfg.window_size.y / 2.f - board_size / 2.f - inner_padding - status_bar_size_y  / 2});
	header_bar.updateTextSizeFit(0.9);
    
    //side_panel.setSize({ side_panel_size_x, _total_height_panel });
    //side_panel.setFillColor(sf::Color::Transparent);
    //side_panel.setOrigin(side_panel.getLocalBounds().getCenter());
    //side_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2, ui_cfg.window_size.y / 2.f});

    mode_panel_resize(_total_height_panel);
    option_panel_resize(_total_height_panel);
    control_panel_resize(_total_height_panel);
    history_panel_resize(_total_height_panel);

    footer_bar.setSize({ board_size, status_bar_size_y });
    footer_bar.setFillColor(sf::Color(242, 176, 109));
    footer_bar.setOrigin(footer_bar.getLocalBounds().getCenter());
    footer_bar.setPosition({ _tmp + board_size / 2, ui_cfg.window_size.y / 2.f + board_size / 2.f + inner_padding + status_bar_size_y  / 2});
}