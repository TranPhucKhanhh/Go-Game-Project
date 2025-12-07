#include<SFML/Graphics.hpp>
#include<UI/InGame.h>
#include<UI/Component.h>
#include<model/UICfg.h>
#include<model/CellState.h>
#include<game/Game.h>
#include<algorithm>
#include<iostream>
#include<cmath>

static std::string convert_to_string(const float& num) {
    int val = floor(num);
    float epsilon = 0.3;

    std::string _re = std::to_string(val);
    if (num - val > epsilon) {
        _re += '.';
        _re += '5';
    }
    return _re;
}

InGame::InGame(const AssetManager& _asset_manager, Game& _game, UICfg& ui_cfg) : 
    asset_manager(_asset_manager), game(_game), ui_cfg(ui_cfg),
    header_bar(_asset_manager.getFont("Roboto-Slab-Bold")),
    undo_button("Undo", _asset_manager.getFont("StackSansNotch-Regular")),
    redo_button("Redo", _asset_manager.getFont("StackSansNotch-Regular")), 
    pass_button("Pass", _asset_manager.getFont("StackSansNotch-Regular")),
    new_button("New Game", _asset_manager.getFont("StackSansNotch-Regular")),
    reset_button("Reset Gane", _asset_manager.getFont("StackSansNotch-Regular")),
    save_button("Save game", _asset_manager.getFont("StackSansNotch-Regular")),
    white_score_box(_asset_manager.getFont("StackSansNotch-Regular")),
    black_score_box(_asset_manager.getFont("StackSansNotch-Regular")),
    save_file_input(_asset_manager.getFont("StackSansNotch-Regular")),
    error_notification(_asset_manager.getFont("StackSansNotch-Regular")),
    mode_box(_asset_manager.getFont("StackSansNotch-Regular")) {
   
	undo_button.updateRespondStr("Undo");
	redo_button.updateRespondStr("Redo");
	pass_button.updateRespondStr("Pass");
	new_button.updateRespondStr("New");
	reset_button.updateRespondStr("Reset");
	save_button.updateRespondStr("Save");

    white_score_box.updateTextColor(sf::Color::Black);
    black_score_box.updateBoxColor(sf::Color::Black);
    black_score_box.updateTextColor(sf::Color::White);
    white_score_box.updateBoxColor(sf::Color::White);

	mode_box.updateBoxColor(sf::Color({ 0, 0, 0, 0 }));
    mode_box.updateTextColor(sf::Color::Black);

    resize();

    std::cerr << "Load InGame UI successfully." << std::endl;

    // initialize the notification
    save_file_input.updateTitleStr("Save game file");
    save_file_input.updateNotificationStr("Input file name (max 20 words):");
    save_file_input.updateMaxInputLength(20);
    TextButton _cancel("Cancel", _asset_manager.getFont("StackSansNotch-Regular"));
    _cancel.updateRespondStr("CloseNoti");
    TextButton _save("Save", _asset_manager.getFont("StackSansNotch-Regular"));
    _save.updateRespondStr("SaveCurrentGame");
    save_file_input.addSelection(_cancel);
    save_file_input.addSelection(_save);

    error_notification.updateTitleStr("ERROR");
    error_notification.container_color = sf::Color(255, 92, 92);
    TextButton _close("Close", _asset_manager.getFont("StackSansNotch-Regular"));
    _close.updateRespondStr("CloseNoti");
    error_notification.addSelection(_close);

	// initialize the history
    history_scroll.updatePreviewSize(7);
}

void InGame::enter() {
    board.updateCellNumber(game.getGameCfg().board_size);
    sf::Event _d = sf::Event::Closed{};
    std::string _dt = "Test";
    eventHandle(_d, _dt);
    
    save_file_input.updateOnScreen(false);
    game_playable = true;
	
	history_scroll.clearContent();
	std::vector<std::string> _move_list = game.getMoveList();
	for (size_t i = 0; i < _move_list.size(); i++) {
		std::string _str = std::to_string(i + 1) + ". " + _move_list[i];
        TextButton _move_btn(_str, asset_manager.getFont("StackSansNotch-Regular"));
		_move_btn.updateRespondStr("|" + std::to_string(i + 1));
		history_scroll.updateContent(_move_btn);
	}
    history_scroll.updateIndex(std::max(0, (int)game.getMoveListSize() - (int)history_scroll.getPreviewSize())); // Set the current index to the last move
	
    if (game.getGameCfg().game_mode == GameMode::PvP) mode_box.updateStr("Mode: PvP");
    else if (game.getGameCfg().game_mode == GameMode::AIEasy) mode_box.updateStr("Mode: AI easy");
    else if (game.getGameCfg().game_mode == GameMode::AIMedium) mode_box.updateStr("Mode: AI medium");
	else if (game.getGameCfg().game_mode == GameMode::AIHard) mode_box.updateStr("Mode: AI hard");

    updateHeaderBar();
    updateScoreBox(game.getScore());

    resize();
}

void InGame::updateHeaderBar() {
    if (game_playable) {
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
}

void InGame::updateScoreBox(const std::pair<float, float> &_score) {
    white_score_box.updateStr(convert_to_string(_score.second));
    black_score_box.updateStr(convert_to_string(_score.first));

    footer_bar_resize();
}

void InGame::eventHandle(const sf::Event& event, std::string& respond) {
    std::string event_respond = "";
    
    if (event.is<sf::Event::Resized>()) {
        resize();
    }

    if (error_notification.onScreen()) {
        error_notification.eventHandle(event, ui_cfg, event_respond);
        return;
	}

    if (save_file_input.onScreen()) {
        save_file_input.eventHandle(event, ui_cfg, event_respond);
        if (event_respond == "SaveCurrentGame") {
			int check = game.saveGame(save_file_input.getInputStr());
            if (check == 0) {
				error_notification.updateNotificationStr("Failed to save file");
				error_notification.updateOnScreen(true);
            }
            else {
				save_file_input.updateOnScreen(false);
            }
        }
        return;
    }

    if (game_playable) {
        if (event.is<sf::Event::MouseMoved>()) {
            board.hoverStone(ui_cfg.mouse_pos, game);
        }
        if (event.is<sf::Event::MouseButtonReleased>()) {
            board.placeStone(ui_cfg.mouse_pos, game);

            if (history_scroll.getContentSize() < game.getMoveListSize()) {
                if (history_scroll.getIndex() < std::max(0, (int)game.getMoveListSize() - (int)history_scroll.getPreviewSize())) 
                    history_scroll.updateIndex(history_scroll.getIndex() + 1);
                std::string _str = std::to_string(game.getMoveListSize()) + ". " + game.getLastMove();
				TextButton _move_btn(_str, asset_manager.getFont("StackSansNotch-Regular"));
                _move_btn.updateRespondStr("|" + std::to_string(game.getMoveListSize()));
                history_scroll.updateContent(_move_btn);
			}
            updateHeaderBar();
            updateScoreBox(game.getScore());
        }
    }
    
    new_button.eventHandle(event, ui_cfg, event_respond);
    reset_button.eventHandle(event, ui_cfg, event_respond);
    save_button.eventHandle(event, ui_cfg, event_respond);

    undo_button.eventHandle(event, ui_cfg, event_respond);
    redo_button.eventHandle(event, ui_cfg, event_respond);
    pass_button.eventHandle(event, ui_cfg, event_respond);

	history_scroll.eventHandle(event, ui_cfg, event_respond);

    if (event_respond != "") {
		std::cerr << "clicked: " << event_respond << std::endl;
	}

    if (event_respond.size() >= 1 && event_respond[0] == '|') {
        // Clicked the move in the history
        int k = std::stoi(event_respond.substr(1));

        if (k < game.getMoveListSize()) {
            game.getKthBoard(k-1);
            game_playable = false;
            history_preview_index = k-1;
            updateScoreBox(game.getScoreFromBoard(game.getPreviewBoard()));

            header_bar.updateStr(history_scroll.getContentStr(k-1));
            header_bar.updateBoxColor(sf::Color::Yellow);
            header_bar.updateTextColor(sf::Color::Black);
            return;
        }
        else {
            game_playable = true;
            history_preview_index = -1;
            updateHeaderBar();
            updateScoreBox(game.getScore());
        }
    }
    if (event_respond == "Undo" && game_playable) {
        game.undo();
        updateHeaderBar();
        updateScoreBox(game.getScore());
    }
    else if (event_respond == "Redo" && game_playable) {
        game.redo();
        updateHeaderBar();
        updateScoreBox(game.getScore());
    }
    else if (event_respond == "Pass" && game_playable) {
        game.pass();
        updateHeaderBar();
        updateScoreBox(game.getScore());
    }
    else if (event_respond == "Reset" && game_playable) {
        game.reset();
		history_scroll.clearContent();
        updateHeaderBar();
        updateScoreBox(game.getScore());
    }
    else if (event_respond == "New") {
        respond = "GameNewOption";
    }
    else if (event_respond == "Save" && game_playable) {
        save_file_input.clearStr();
        save_file_input.updateOnScreen(true);
	}

    // Check if the game reach the end stage
    if (game.isGameEnd() && game_playable) {
        game_playable = false;

        std::pair<float, float> _score = game.getScore();
        
        if (_score.first < _score.second) {
            header_bar.updateStr("WHITE WINS");
            header_bar.updateBoxColor(sf::Color::White);
            header_bar.updateTextColor(sf::Color::Black);
        }
        else if (_score.first > _score.second) {
            header_bar.updateStr("BLACK WINS");
            header_bar.updateBoxColor(sf::Color::Black);
            header_bar.updateTextColor(sf::Color::White);
        }
        else {
            header_bar.updateStr("DRAW");
            header_bar.updateBoxColor(sf::Color::Magenta);
            header_bar.updateTextColor(sf::Color::Black);
        }
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

    white_score_box.draw(ui_cfg.window);
    black_score_box.draw(ui_cfg.window);

    if (history_preview_index != -1) board.draw(ui_cfg.window, game.getPreviewBoard());
    else board.draw(ui_cfg.window, game.getCurrentBoard());

    history_scroll.draw(ui_cfg.window);
    mode_box.draw(ui_cfg.window);

    if (save_file_input.onScreen()) {
        save_file_input.draw(ui_cfg.window);
    }

    if (error_notification.onScreen()) {
        error_notification.draw(ui_cfg.window);
	}
}

void InGame::mode_panel_resize(const float& _total_height_panel) {
    mode_panel.setSize({ side_panel_size_x, _total_height_panel / 10.f });
    mode_panel.setFillColor(sf::Color(242, 176, 109));
    mode_panel.setOrigin(mode_panel.getLocalBounds().getCenter());
    mode_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2, ui_cfg.window_size.y / 2.f - _total_height_panel / 2.f + _total_height_panel / 20.f });

    mode_box.updateBoxPos(mode_panel.getPosition());
    mode_box.updateBoxSize({ mode_panel.getSize().x * 0.8f, mode_panel.getSize().y * 0.8f });
    mode_box.updateTextSizeFit(1.f);
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


    history_scroll.updateSize({ side_panel_size_x,
        _total_height_panel - inner_padding * 3 - option_panel.getSize().y - mode_panel.getSize().y - control_panel.getSize().y });
    history_scroll.updatePos({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        control_panel.getPosition().y + control_panel.getSize().y / 2.f + history_panel.getSize().y / 2.f + inner_padding });
}

void InGame::footer_bar_resize() {
    float _tmp = (ui_cfg.window_size.x - board_size - inner_padding - side_panel_size_x) / 2;

    float _box_size = board_size / 4;
    black_score_box.updateBoxSize({ _box_size, status_bar_size_y });
    black_score_box.updateBoxPos({ _tmp + _box_size / 2.f, ui_cfg.window_size.y / 2.f + board_size / 2.f + inner_padding + status_bar_size_y / 2.f });
    white_score_box.updateBoxSize({ _box_size, status_bar_size_y });
    white_score_box.updateBoxPos({ _tmp - _box_size / 2.f + board_size, ui_cfg.window_size.y / 2.f + board_size / 2.f + inner_padding + status_bar_size_y / 2.f });

    float _box_text_size = std::min(black_score_box.getTextSizeFit(0.9f), white_score_box.getTextSizeFit(0.9f));
    black_score_box.updateTextSize(_box_text_size);
    white_score_box.updateTextSize(_box_text_size);
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
    
    // Notification resize
	float _tmp_noti_size = std::max(std::min(ui_cfg.window_size.x * 3 / 20.f, ui_cfg.window_size.y / 4.f), 180.f);
	sf::Vector2f _noti_size = { _tmp_noti_size * 5 / 3.f, _tmp_noti_size };
    save_file_input.updateSize(_noti_size);
    save_file_input.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });

    error_notification.updateSize(_noti_size);
    error_notification.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });

    mode_panel_resize(_total_height_panel);
    option_panel_resize(_total_height_panel);
    control_panel_resize(_total_height_panel);
    history_panel_resize(_total_height_panel);

    footer_bar.setSize({ board_size, status_bar_size_y });
    footer_bar.setFillColor(sf::Color(242, 176, 109));
    footer_bar.setOrigin(footer_bar.getLocalBounds().getCenter());
    footer_bar.setPosition({ _tmp + board_size / 2, ui_cfg.window_size.y / 2.f + board_size / 2.f + inner_padding + status_bar_size_y  / 2});

    footer_bar_resize();
}