#include<SFML/Graphics.hpp>
#include<UI/InGame.h>
#include<UI/Component.h>
#include<game.h>
#include<algorithm>
#include<iostream>

InGame::InGame(const AssetManager& _asset_manager, const GameCfg& _game_config) : asset_manager(_asset_manager), game_config(_game_config) {

}

void InGame::build(const sf::Vector2u& window_size) {
    resize(window_size);
}

void InGame::eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos, const sf::Vector2u& window_size) {
    if (event.is<sf::Event::Resized>()) {
        resize(window_size);
    }
    if (event.is<sf::Event::MouseMoved>()) {
        board.hoverStone(mouse_pos);
    }
}

void InGame::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 217, 181));
    window.draw(debug);
    window.draw(side_panel);
    window.draw(header_bar);
    window.draw(footer_bar);
    board.draw(window);
}

void InGame::resize(const sf::Vector2u& window_size) {
    float min_scale = std::min(window_size.x, window_size.y);
    
    padding = min_scale / 10;
    canvas_size = { window_size.x - padding, window_size.y - padding };
    inner_padding = (min_scale - padding) / 30;
    side_panel_size_x = std::min( 500.f, std::max(canvas_size.x / 7, 100.f));
    status_bar_size_y = std::min(500.f, std::max(canvas_size.y / 17, 75.f));
    board_size = std::min(canvas_size.y - (status_bar_size_y+inner_padding) * 2, canvas_size.x - inner_padding - side_panel_size_x);

    float _tmp = (window_size.x - board_size - inner_padding - side_panel_size_x) / 2;
    std::cerr << "board_size: " << board_size << " inner_padding; " << inner_padding << std::endl;
    std::cerr << "board x: " << _tmp + board_size / 2.f << " board y: " << window_size.y / 2.f << std::endl;

    board.updateCellNumber(19);
    board.updateSize( board_size );
    board.updatePos({_tmp + board_size / 2, window_size.y / 2.f});
    board.updateStoneTexture(asset_manager.getTexture("stone-default"));
    board.updateTexture(asset_manager.getTexture("board-minimal"));
    board.update();

    debug.setSize(canvas_size);
    debug.setFillColor(sf::Color::White);
    debug.setOrigin(debug.getLocalBounds().getCenter());
    debug.setPosition({window_size.x/2.f,window_size.y/2.f});

    side_panel.setSize({ side_panel_size_x, status_bar_size_y * 2 + inner_padding * 2 + board_size });
    side_panel.setFillColor(sf::Color(242, 176, 109));
    side_panel.setOrigin(side_panel.getLocalBounds().getCenter());
    side_panel.setPosition({ board.getPos().x + board_size / 2 + inner_padding + side_panel_size_x / 2, window_size.y / 2.f});

    header_bar.setSize({ board_size, status_bar_size_y });
    header_bar.setFillColor(sf::Color(242, 176, 109));
    header_bar.setOrigin(header_bar.getLocalBounds().getCenter());
    header_bar.setPosition({ _tmp + board_size / 2, window_size.y / 2.f - board_size / 2.f - inner_padding - status_bar_size_y  / 2});

    footer_bar.setSize({ board_size, status_bar_size_y });
    footer_bar.setFillColor(sf::Color(242, 176, 109));
    footer_bar.setOrigin(footer_bar.getLocalBounds().getCenter());
    footer_bar.setPosition({ _tmp + board_size / 2, window_size.y / 2.f + board_size / 2.f + inner_padding + status_bar_size_y  / 2});
}