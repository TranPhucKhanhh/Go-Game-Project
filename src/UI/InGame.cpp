#include<SFML/Graphics.hpp>
#include<UI/InGame.h>
#include<UI/Component.h>
#include<model/UICfg.h>
#include<model/CellState.h>
#include<model/MoveVerdict.h>
#include<game/Game.h>
#include<algorithm>
#include<iostream>
#include<cmath>
#include <cstdlib>
#include <ctime>

static const std::string win_mess[] = {"Winner winner chicken dinner", "Your the Champion!", "We've got a WINNER!!", "EZ Game Amirite?"};

static void updateSpriteSize(sf::Sprite& _sprite, const float& _size) {
    _sprite.setScale({ 1.f, 1.f });

    sf::Vector2f _sprite_size = _sprite.getLocalBounds().size;

    float _new_scale = std::min(_size / _sprite_size.x, _size / _sprite_size.y);

    _sprite.setScale({ _new_scale , _new_scale });
}

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
    header_bar(_asset_manager.getFont("Spicy-Sale")),
    exit_button("Exit Game", _asset_manager.getFont("Spicy-Sale"), ui_cfg),
    undo_button("Undo", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    redo_button("Redo", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    pass_button("Pass", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    new_button("New Game", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    reset_button("Reset Game", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    resign_button("Resign", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    out_his_button("Close history", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    save_button("Save game", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    setting_button("Setting", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg),
    sound_button(ui_cfg),
    music_button(ui_cfg),
    white_score_box(_asset_manager.getFont("StackSansNotch-Regular")),
    black_score_box(_asset_manager.getFont("StackSansNotch-Regular")),
    save_file_input(_asset_manager.getFont("RobotoSlab-Bold")),
    error_notification(_asset_manager.getFont("RobotoSlab-Bold")),
    win_notification(_asset_manager.getFont("RobotoSlab-Bold")),
    mode_box(_asset_manager.getFont("StackSansNotch-Bold")),
    board(_asset_manager),
    white_stone_image(_asset_manager.getTexture("dummy")),
    black_stone_image(_asset_manager.getTexture("dummy")),
    history_scroll(ui_cfg)
{
   
    // Set the respond message for each button
    exit_button.updateRespondStr("Exit");
	undo_button.updateRespondStr("Undo");
	redo_button.updateRespondStr("Redo");
	pass_button.updateRespondStr("Pass");
    resign_button.updateRespondStr("Resign");
    out_his_button.updateRespondStr("Close History");
	new_button.updateRespondStr("New");
	reset_button.updateRespondStr("Reset");
	save_button.updateRespondStr("Save");
    setting_button.updateRespondStr("Setting");
    sound_button.updateRespondStr("Toggle Sound");
    music_button.updateRespondStr("Toggle Music");

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
    TextureEffectButton _cancel("Cancel", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
    _cancel.updateRespondStr("CloseNoti");
    _cancel.updateTextEffectColor(sf::Color::White);
    _cancel.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-red"));
    _cancel.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-red"));
    _cancel.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));
    TextureEffectButton _save("Save", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
    _save.updateRespondStr("SaveCurrentGame");
    _save.updateTextEffectColor(sf::Color::White);
    _save.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-green"));
    _save.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-green"));
    _save.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-green"));
    
    save_file_input.addSelection(_cancel);
    save_file_input.addSelection(_save);

    error_notification.updateTitleStr("ERROR");
    error_notification.container_color = sf::Color(255, 92, 92);
    _cancel.updateStr("Close");
    error_notification.addSelection(_cancel);

    win_notification.container_color = sf::Color(170, 255, 0);
    TextureEffectButton _new("New Game", _asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
    _new.updateRespondStr("New");
    _new.updateTextEffectColor(sf::Color::White);
    _new.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-green"));
    _new.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-green"));
    _new.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-green"));
    win_notification.addSelection(_cancel);
    win_notification.addSelection(_new);

	// initialize the history
    history_scroll.updatePreviewSize(7);

    // update the random
    std::srand(std::time(0));

    // Update button effect
    exit_button.updateTextEffectColor(sf::Color::White);
    exit_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_gradient-red"));
    exit_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gloss-red"));
    exit_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_flat-red"));

    undo_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    undo_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    undo_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    undo_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    redo_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    redo_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    redo_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    redo_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    pass_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    pass_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    pass_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    pass_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    new_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    new_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    new_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    new_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    reset_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    reset_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    reset_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    reset_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    resign_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    resign_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    resign_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    resign_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    out_his_button.updateTextEffectColor(sf::Color::Black);
    out_his_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border-yellow"));
    out_his_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_flat-yellow"));
    out_his_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss-yellow"));

    save_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    save_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    save_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    save_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));

    setting_button.updateTextEffectColor(sf::Color::Black, sf::Color::Black, sf::Color::White);
    setting_button.updateIdleTex(asset_manager.getTexture("button_rectangle_flat"));
    setting_button.updateHoverTex(asset_manager.getTexture("button_rectangle_border"));
    setting_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));
}

void InGame::enter() {
    // Initailize the board
    board.updateCellNumber(game.getGameCfg().board_size);
    board.updateBoardUI(ui_cfg.board_design, asset_manager);
    board.updateStoneUI(ui_cfg.stone_design, asset_manager);

    // Update preview iamge of Black and White player
    white_stone_image.setTexture(asset_manager.getTexture("white-stone-" + ui_cfg.stone_design), true);
    black_stone_image.setTexture(asset_manager.getTexture("black-stone-" + ui_cfg.stone_design), true);

    // Run first time preventing bug
    updateEventHandle();
    
    save_file_input.updateOnScreen(false);
    error_notification.updateOnScreen(false);
    win_notification.updateOnScreen(false);
    if (!game.isGameEnd()) game_playable = true;
	
    // update sound and music button
    updateSoundMusic();

    // Update history from game
	history_scroll.clearContent();
	std::vector<std::string> _move_list = game.getMoveList();
	for (size_t i = 0; i < _move_list.size(); i++) {
		std::string _str = std::to_string(i + 1) + ". " + _move_list[i];
        TextButton _move_btn(_str, asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
		_move_btn.updateRespondStr("|" + std::to_string(i + 1));
        _move_btn.updateColor({ 91, 164, 252 });
		history_scroll.updateContent(_move_btn);
	}
    history_scroll.updateIndex(std::max(0, (int)game.getMoveListSize() - (int)history_scroll.getPreviewSize())); // Set the current index to the last move
	
    // update the mode
    if (game.getGameCfg().game_mode == GameMode::PvP) mode_box.updateStr("Mode: PvP");
    else if (game.getGameCfg().game_mode == GameMode::AIEasy) mode_box.updateStr("Mode: AI easy");
    else if (game.getGameCfg().game_mode == GameMode::AIMedium) mode_box.updateStr("Mode: AI medium");
	else if (game.getGameCfg().game_mode == GameMode::AIHard) mode_box.updateStr("Mode: AI hard");

    history_preview_index = -1;
    updateHeaderBar();
    updateScoreBox(game.getScore());

    resize();
}

void InGame::updateSoundMusic() {
    if (ui_cfg.background_music_volume == 0) music_button.updateTexture(asset_manager.getTexture("music-no"));
    else music_button.updateTexture(asset_manager.getTexture("music"));

    if (ui_cfg.sound_effect_volume == 0) sound_button.updateTexture(asset_manager.getTexture("sound-no"));
    else sound_button.updateTexture(asset_manager.getTexture("sound"));

    ui_cfg.music.setVolume(ui_cfg.background_music_volume);
    ui_cfg.updateSoundEffectVolume(ui_cfg.sound_effect_volume);
}

void InGame::updateHeaderBar() {
    if (game.isGameEnd()) {
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
        else { // Surely this won't happen but leaving it here cost nothing :D
            header_bar.updateStr("DRAW");
            header_bar.updateBoxColor(sf::Color::Magenta);
            header_bar.updateTextColor(sf::Color::Black);
        }
    }
    else if (game.getCurrentPlayer() == CellState::White) {
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

void InGame::updateScoreBox(const std::pair<float, float>& _score) {
    white_score_box.updateStr(convert_to_string(_score.second));
    black_score_box.updateStr(convert_to_string(_score.first));

    float _tmp = std::min(white_score_box.getTextSizeFit(0.9f), black_score_box.getTextSizeFit(0.9f));
    white_score_box.updateTextSize(_tmp);
    black_score_box.updateTextSize(_tmp);
}

void InGame::updateHistoryScroll() {
    if (history_scroll.getContentSize() == game.getMoveListSize()) return;
    if (history_scroll.getIndex() < std::max(0, (int)game.getMoveListSize() - (int)history_scroll.getPreviewSize()))
        history_scroll.updateIndex(history_scroll.getIndex() + 1);
    std::string _str = std::to_string(game.getMoveListSize()) + ". " + game.getLastMove();
    TextButton _move_btn(_str, asset_manager.getFont("StackSansNotch-Regular"), ui_cfg);
    _move_btn.updateRespondStr("|" + std::to_string(game.getMoveListSize()));
    _move_btn.updateColor({ 91, 164, 252 });
    history_scroll.updateContent(_move_btn);
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

    if (win_notification.onScreen()) {
        win_notification.eventHandle(event, ui_cfg, event_respond);
        if (event_respond == "New") {
            respond = "GameNewOption";
        }
        return;
    }

    if (game_playable) {
        if (event.is<sf::Event::MouseMoved>()) {
            board.hoverStone(ui_cfg.mouse_pos, game);
        }
        if (event.is<sf::Event::MouseButtonReleased>()) {
            board.placeStone(ui_cfg.mouse_pos, game);

            if (game.getLastMoveVerdict() == MoveVerdict::Valid) {
                ui_cfg.stone_place_sound.play();
            }

            updateHistoryScroll();
            updateHeaderBar();
            updateScoreBox(game.getScore());
        }
    }

    sound_button.eventHandle(event, ui_cfg, event_respond);
    music_button.eventHandle(event, ui_cfg, event_respond);
    
    new_button.eventHandle(event, ui_cfg, event_respond);
    reset_button.eventHandle(event, ui_cfg, event_respond);
    save_button.eventHandle(event, ui_cfg, event_respond);
    setting_button.eventHandle(event, ui_cfg, event_respond);

    exit_button.eventHandle(event, ui_cfg, event_respond);
    undo_button.eventHandle(event, ui_cfg, event_respond);
    redo_button.eventHandle(event, ui_cfg, event_respond);
    pass_button.eventHandle(event, ui_cfg, event_respond);
    resign_button.eventHandle(event, ui_cfg, event_respond);
    if (history_preview_index != -1) out_his_button.eventHandle(event, ui_cfg, event_respond);

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
            if (!game.isGameEnd()) game_playable = true;
            history_preview_index = -1;
            updateHeaderBar();
            updateScoreBox(game.getScore());
        }
    }
    if (event_respond == "Undo" && game_playable && !game.isGameEnd()) {
        game.undo();
        updateHeaderBar();
        updateScoreBox(game.getScore());

		// updat the history scroll
        history_scroll.deleteLastContent();

    }
    else if (event_respond == "Redo" && game_playable && !game.isGameEnd()) {
        game.redo();
        updateHeaderBar();
        updateScoreBox(game.getScore());
        updateHistoryScroll();
    }
    else if (event_respond == "Pass" && game_playable && !game.isGameEnd()) {
        game.pass();
        updateHeaderBar();
        updateScoreBox(game.getScore());
        updateHistoryScroll();
    }
    else if (event_respond == "Reset") {
        game.reset();
		history_scroll.clearContent();
        game_playable = true; 
        history_preview_index = -1;
        updateHeaderBar();
        updateScoreBox(game.getScore());
    }
    else if (event_respond == "New") {
        respond = "GameNewOption";
    }
    else if (event_respond == "Save" && game_playable && !game.isGameEnd()) {
        save_file_input.clearStr();
        save_file_input.updateOnScreen(true);
	}
    else if (event_respond == "Setting") {
        respond = "OpenSetting";
    }
    else if (event_respond == "Close History") {
        if (!game.isGameEnd()) game_playable = true;
        history_preview_index = -1;
        updateHeaderBar();
        updateScoreBox(game.getScore());
    }
    else if (event_respond == "Resign") {
        game.resign();
    }
    else if (event_respond == "Toggle Music") {
        if (ui_cfg.pre_background_music_volume == 0 && ui_cfg.background_music_volume == 0) {
            ui_cfg.background_music_volume = 25;
        }
        else if (ui_cfg.background_music_volume == 0) {
            std::swap(ui_cfg.background_music_volume, ui_cfg.pre_background_music_volume);
        }
        else {
            ui_cfg.pre_background_music_volume = ui_cfg.background_music_volume;
            ui_cfg.background_music_volume = 0;
        }
        updateSoundMusic();
    }
    else if (event_respond == "Toggle Sound") {
        if (ui_cfg.pre_sound_effect_volume == 0 && ui_cfg.sound_effect_volume == 0) {
            ui_cfg.sound_effect_volume = 25;
        }
        else if (ui_cfg.sound_effect_volume == 0) {
            std::swap(ui_cfg.sound_effect_volume, ui_cfg.pre_sound_effect_volume);
        }
        else {
            ui_cfg.pre_sound_effect_volume = ui_cfg.sound_effect_volume;
            ui_cfg.sound_effect_volume = 0;
        }
        updateSoundMusic();
    }
    else if (event_respond == "Exit") {
        respond = "GameExit";
    }
    

    // Check if the game reach the end stage
    if (game.isGameEnd() && game_playable) {
        game_playable = false;

        updateHeaderBar();

        int random_num = (std::rand() % std::size(win_mess));
        win_notification.updateOnScreen(true);
        win_notification.updateTitleStr(win_mess[random_num]);
        if (game.getResignPlayer() == CellState::White) {
            win_notification.updateNotificationStr("Black wins - White resigned");
        }
        else if (game.getResignPlayer() == CellState::Black) {
            win_notification.updateNotificationStr("White wins - Black resigned");
        }
        else {
            std::pair<float, float> _score = game.getScore();
            if (_score.first < _score.second) {
                win_notification.updateNotificationStr("White wins by " + convert_to_string(_score.second- _score.first) + " points");
            }
            else if (_score.first > _score.second) {
                win_notification.updateNotificationStr("Black wins by " + convert_to_string(_score.first - _score.second ) + " points");
            }
            else {
                win_notification.updateNotificationStr("The creator of this game");
            }
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

    ui_cfg.window.draw(black_stone_image);
    ui_cfg.window.draw(white_stone_image);

    sound_button.draw(ui_cfg.window);
    music_button.draw(ui_cfg.window);

    exit_button.draw(ui_cfg.window);
    new_button.draw(ui_cfg.window);
    reset_button.draw(ui_cfg.window);
    save_button.draw(ui_cfg.window);
    setting_button.draw(ui_cfg.window);

    undo_button.draw(ui_cfg.window);
    redo_button.draw(ui_cfg.window);
    pass_button.draw(ui_cfg.window);
    resign_button.draw(ui_cfg.window);
    if (history_preview_index != -1) out_his_button.draw(ui_cfg.window);

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

    if (win_notification.onScreen()) {
        win_notification.draw(ui_cfg.window);
    }
}

void InGame::mode_panel_resize(const float& _height) {
    mode_panel.setSize({ side_panel_size_x, _height });
    mode_panel.setFillColor(sf::Color(242, 176, 109));
    mode_panel.setOrigin(mode_panel.getLocalBounds().getCenter());
    mode_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2, 
        header_bar.getPos().y - status_bar_size_y / 2.f + _height /2.f});

    mode_box.updateBoxPos(mode_panel.getPosition());
    mode_box.updateBoxSize({ mode_panel.getSize().x * 0.8f, mode_panel.getSize().y * 0.8f });
    mode_box.updateTextSizeFit(1.f);
}

void InGame::option_panel_resize(const float& _height) {
    float _button_padding = side_panel_size_x / 15.f;
    float _inner_button_padding = _button_padding / 1.5f;

    option_panel.setSize({ side_panel_size_x, _height });
    option_panel.setFillColor(sf::Color::Transparent);
    option_panel.setOrigin(option_panel.getLocalBounds().getCenter());
    option_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        mode_panel.getPosition().y + mode_panel.getSize().y / 2.f + option_panel.getSize().y / 2.f + inner_padding });

    sf::Vector2f _button_size = { side_panel_size_x - _button_padding * 2, (_height - 2 * _button_padding - 3 * _inner_button_padding) / 4.f };

    new_button.updateSize(_button_size);
    new_button.updatePos({ option_panel.getPosition().x, option_panel.getPosition().y - _height/2.f + _button_size.y/2.f + _button_padding});
    
    reset_button.updateSize(_button_size);
    reset_button.updatePos({ option_panel.getPosition().x, new_button.getPos().y + _button_size.y + _inner_button_padding });
    
    save_button.updateSize(_button_size);
    save_button.updatePos({ option_panel.getPosition().x, reset_button.getPos().y + _button_size.y + _inner_button_padding });
    
    setting_button.updateSize(_button_size);
    setting_button.updatePos({ option_panel.getPosition().x, save_button.getPos().y + _button_size.y + _inner_button_padding });

    float _ratio = 0.95;
    float _button_text_size = std::min({ new_button.getTextSizeFit(_ratio), reset_button.getTextSizeFit(_ratio), save_button.getTextSizeFit(_ratio), setting_button.getTextSizeFit(_ratio)});

    new_button.updateTextSize(_button_text_size);
    reset_button.updateTextSize(_button_text_size);
    save_button.updateTextSize(_button_text_size);
    setting_button.updateTextSize(_button_text_size);
}

void InGame::history_panel_resize(const float& _height) {
    history_panel.setSize({ side_panel_size_x, _height });
    history_panel.setFillColor(sf::Color(242, 176, 109));
    history_panel.setOrigin(history_panel.getLocalBounds().getCenter());
    history_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        option_panel.getPosition().y + option_panel.getSize().y / 2.f + history_panel.getSize().y / 2.f + inner_padding });


    history_scroll.updateSize({ side_panel_size_x, _height });
    history_scroll.updatePos({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2,
        option_panel.getPosition().y + option_panel.getSize().y / 2.f + history_panel.getSize().y / 2.f + inner_padding });
}

void InGame::control_panel_resize(const float& _height) {
    /*control_panel.setSize({ control_bar_size_x, _height });
    control_panel.setFillColor(sf::Color(242, 176, 109));
    control_panel.setOrigin(control_panel.getLocalBounds().getCenter());
    control_panel.setPosition({ board.getPos().x - board_size / 2 - inner_padding - control_bar_size_x / 2,
        board.getPos().y + board_size/2.f - _height/2.f});*/

    float _button_padding = side_panel_size_x / 10.f;

    sf::Vector2f _button_size = { control_bar_size_x, (_height - _button_padding * 4) / 4 };

    resign_button.updateSize(_button_size);
    resign_button.updatePos({ board.getPos().x - board_size / 2 - inner_padding - control_bar_size_x / 2 ,board.getPos().y + board_size / 2.f - _button_size.y / 2.f });

    pass_button.updateSize(_button_size);
    pass_button.updatePos({ resign_button.getPos().x ,resign_button.getPos().y - _button_padding - _button_size.y});

    redo_button.updateSize(_button_size);
    redo_button.updatePos({ pass_button.getPos().x ,pass_button.getPos().y - _button_padding - _button_size.y });

    undo_button.updateSize(_button_size);
    undo_button.updatePos({ redo_button.getPos().x ,redo_button.getPos().y - _button_padding - _button_size.y });

    out_his_button.updateSize(_button_size);
    out_his_button.updatePos({ undo_button.getPos().x ,undo_button.getPos().y - _button_padding - _button_size.y });

    float _button_text_size = std::min({ undo_button.getTextSizeFit(0.9f), redo_button.getTextSizeFit(0.9f), pass_button.getTextSizeFit(0.9f), resign_button.getTextSizeFit(0.9f)});

    undo_button.updateTextSize(_button_text_size);
    redo_button.updateTextSize(_button_text_size);
    pass_button.updateTextSize(_button_text_size);
    resign_button.updateTextSize(_button_text_size);
    out_his_button.updateTextSizeFit(0.9f);
}

void InGame::score_bar_resize(const float &_height) {
    float _margin = _height / 3.5;

    sf::Vector2f _box_size = { control_bar_size_x, (_height - _margin) / 2.f};
    black_score_box.updateBoxSize(_box_size);
    black_score_box.updateBoxPos({ pass_button.getPos().x, header_bar.getPos().y - status_bar_size_y/2.f + _box_size.y/2.f});
    white_score_box.updateBoxSize(_box_size);
    white_score_box.updateBoxPos({ pass_button.getPos().x, black_score_box.getPos().y + _margin + _box_size.y});

    float _size = _margin;
    updateSpriteSize(white_stone_image, _size);
    white_stone_image.setOrigin(white_stone_image.getLocalBounds().getCenter());
    white_stone_image.setPosition({ black_score_box.getPos().x, white_score_box.getPos().y + _box_size.y / 2.f + _size/2.f });

    updateSpriteSize(black_stone_image, _size);
    black_stone_image.setOrigin(black_stone_image.getLocalBounds().getCenter());
    black_stone_image.setPosition({ black_score_box.getPos().x, black_score_box.getPos().y + _box_size.y / 2.f + _size/2.f });

    float _box_text_size = std::min(black_score_box.getTextSizeFit(0.9f), white_score_box.getTextSizeFit(0.9f));
    black_score_box.updateTextSize(_box_text_size);
    white_score_box.updateTextSize(_box_text_size);
}

void InGame::resize() {
    float min_scale = std::min(ui_cfg.window_size.x, ui_cfg.window_size.y);
    
    // Initial value
    padding = min_scale / 20;
    canvas_size = { ui_cfg.window_size.x - 10.f, ui_cfg.window_size.y - padding * 2 };
    inner_padding = (min_scale - padding) / 30;
    status_bar_size_y = std::min(750.f, std::max(canvas_size.y / 17, 50.f));
    side_panel_size_x = std::min(ui_cfg.window_size.y / 4.f, std::max(canvas_size.x / 7, 100.f));
    control_bar_size_x = side_panel_size_x * 0.55;
    board_size = std::min(canvas_size.y - status_bar_size_y - inner_padding, canvas_size.x - inner_padding*2 - side_panel_size_x - control_bar_size_x);

    float _tmp = (ui_cfg.window_size.x - board_size - inner_padding - side_panel_size_x) / 2;
    float _total_height_panel = status_bar_size_y + inner_padding + board_size;

    // Reize the middle component: header bar & board
    board.updateSize( board_size );
    board.updatePos({ ui_cfg.window_size.x / 2.f - (side_panel_size_x- control_bar_size_x)/2.f, ui_cfg.window_size.y / 2.f + (inner_padding + status_bar_size_y) / 2.f});
    board.update();
    
    header_bar.updateBoxSize({ board_size, status_bar_size_y });
    header_bar.updateBoxPos({ board.getPos().x, board.getPos().y - board_size / 2.f - inner_padding - status_bar_size_y / 2.f});
	header_bar.updateTextSizeFit(0.9);

    // Resize the sound and music button
    sound_button.updateSize({ padding * 0.9f, padding * 0.9f });
    sound_button.updatePos({ board.getPos().x+board_size/2.f-sound_button.getSize().x/2.f, header_bar.getPos().y - status_bar_size_y / 2.f - sound_button.getSize().y / 2.f});
    
    music_button.updateSize({ padding * 0.9f, padding * 0.9f });
    music_button.updatePos({ sound_button.getPos().x - sound_button.getSize().x - inner_padding, sound_button.getPos().y});

    // Resize other component
    mode_panel_resize( (_total_height_panel-inner_padding*2) * 1/7  );
    option_panel_resize((_total_height_panel - inner_padding * 2) * 3 / 7 );
    history_panel_resize((_total_height_panel - inner_padding * 2) * 3 / 7);
    
    control_panel_resize(_total_height_panel * 0.5);
    score_bar_resize(_total_height_panel * 0.3);

    // Notification resize
    float _tmp_noti_size = std::max(std::min(ui_cfg.window_size.x * 3 / 20.f, ui_cfg.window_size.y / 4.f), 180.f);
    sf::Vector2f _noti_size = { _tmp_noti_size * 5 / 3.f, _tmp_noti_size };
    save_file_input.updateSize(_noti_size);
    save_file_input.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });

    error_notification.updateSize(_noti_size);
    error_notification.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });

    win_notification.updateSize(_noti_size);
    win_notification.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });

    exit_button.updateSize({ (float)side_panel_size_x, padding * 0.9f });
    exit_button.updatePos({ (float)mode_panel.getPosition().x,  mode_panel.getPosition().y - mode_panel.getSize().y / 2.f - exit_button.getSize().y / 2.f });
    exit_button.updateTextSizeFit(0.9f);
}