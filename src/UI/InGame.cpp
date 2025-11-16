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
    header_bar(_asset_manager.getFont("Roboto-Slab-Bold")) {
   
    resize();

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

    std::cerr << "Load InGame UI successfully." << std::endl;
}

void InGame::enter() {
    resize();
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

        if (game.getCurrentPlayer() == CellState::White) {
            header_bar.updateStr("WHITE TO MOVE");
            header_bar.updateBoxColor(sf::Color(255, 255, 255));
            header_bar.updateTextColor(sf::Color(0, 0, 0));
        }
        else {
            header_bar.updateStr("BLACK TO MOVE");
            header_bar.updateBoxColor(sf::Color(0, 0, 0));
            header_bar.updateTextColor(sf::Color(255, 255, 255));
        }
    }
}

void InGame::draw() {
    ui_cfg.window.clear(sf::Color(240, 217, 181));
    ui_cfg.window.draw(side_panel);
	header_bar.draw(ui_cfg.window);
    ui_cfg.window.draw(footer_bar);

    board.draw(ui_cfg.window, game.getCurrentBoard());
}

void InGame::resize() {
    float min_scale = std::min(ui_cfg.window_size.x, ui_cfg.window_size.y);
    
    padding = min_scale / 10;
    canvas_size = { ui_cfg.window_size.x - padding, ui_cfg.window_size.y - padding };
    inner_padding = (min_scale - padding) / 30;
    side_panel_size_x = std::min( 500.f, std::max(canvas_size.x / 7, 100.f));
    status_bar_size_y = std::min(500.f, std::max(canvas_size.y / 17, 75.f));
    board_size = std::min(canvas_size.y - (status_bar_size_y+inner_padding) * 2, canvas_size.x - inner_padding - side_panel_size_x);

    float _tmp = (ui_cfg.window_size.x - board_size - inner_padding - side_panel_size_x) / 2;

    board.updateCellNumber(game.getGameCfg().board_size);
    board.updateSize( board_size );
    board.updatePos({_tmp + board_size / 2, ui_cfg.window_size.y / 2.f});
    board.updateStoneTexture(asset_manager.getTexture("white-stone-default"), asset_manager.getTexture("black-stone-default"));
    board.updateTexture(asset_manager.getTexture("board-minimal"));
    board.update();

    side_panel.setSize({ side_panel_size_x, status_bar_size_y * 2 + inner_padding * 2 + board_size });
    side_panel.setFillColor(sf::Color(242, 176, 109));
    side_panel.setOrigin(side_panel.getLocalBounds().getCenter());
    side_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2, ui_cfg.window_size.y / 2.f});

    header_bar.updateBoxSize({ board_size, status_bar_size_y });
    header_bar.updateTextSize(50.f);
    header_bar.updateBoxPos({ _tmp + board_size / 2, ui_cfg.window_size.y / 2.f - board_size / 2.f - inner_padding - status_bar_size_y  / 2});

    footer_bar.setSize({ board_size, status_bar_size_y });
    footer_bar.setFillColor(sf::Color(242, 176, 109));
    footer_bar.setOrigin(footer_bar.getLocalBounds().getCenter());
    footer_bar.setPosition({ _tmp + board_size / 2, ui_cfg.window_size.y / 2.f + board_size / 2.f + inner_padding + status_bar_size_y  / 2});
}