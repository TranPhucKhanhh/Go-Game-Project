#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<Game.h>
#include<iostream>
#include<cmath>

// debug
int board_state[21][21];

void Board::updateSize(const float& size) {
    canvas_size = size;
    canvas_padding = canvas_size / 20;
    board_size = canvas_size - canvas_padding * 2;
    grid_size = board_size / (board_cell_number + 1);
    board_padding = grid_size;
}

void Board::draw(sf::RenderWindow& window) {
    window.draw(canvas);
    window.draw(board);
    
    // draw lines on board
    float line_thickness = std::min(board_size / 150, 5.f);
    float line_length = board_size - board_padding * 2;
    // draw vetical lines
    for (int i = 0; i < board_cell_number; i++) {
        sf::RectangleShape line({ line_thickness, line_length + line_thickness * (i == board_cell_number - 1) });
        line.setFillColor(sf::Color(0, 0, 0, 200));
        line.setPosition({ start_position.x + i * grid_size, start_position.y });
        window.draw(line);
    }
    // draw horizontal lines
    for (int i = 0; i < board_cell_number; i++) {
        sf::RectangleShape line({ line_length + line_thickness * (i==board_cell_number-1), line_thickness});
        line.setFillColor(sf::Color(0, 0, 0, 200));
        line.setPosition({ start_position.x, start_position.y + i * grid_size });
        window.draw(line);
    }

    if (pos.x != -1) {
        sf::Color c = stone.getFillColor();
        c.a = 200; 
        stone.setFillColor(c);

        stone.setPosition({ start_position.x + grid_size * (pos.x - 1), start_position.y + grid_size * (pos.y - 1) });
        window.draw(stone);

        c.a = 255;
        stone.setFillColor(c);
    }

    for (int i = 1; i <= board_cell_number; i++) {
        for (int j = 1; j <= board_cell_number; j++) {
            if (board_state[i][j]) {
                stone.setPosition({start_position.x + grid_size * (i-1), start_position.y + grid_size * (j-1) });
                window.draw(stone);
            }
        }
    }
}

void Board::hoverStone(const sf::Vector2i& mouse_pos) {
    if (board.getGlobalBounds().contains((sf::Vector2f)mouse_pos)) {
        sf::Vector2f _tmp = { mouse_pos.x - start_position.x + board_padding - grid_size / 2, mouse_pos.y - start_position.y + board_padding - grid_size / 2};
        _tmp = { std::max(0.f, _tmp.x), std::max(0.f, _tmp.y) };        
        float _x = abs(_tmp.x - (std::floor(_tmp.x / grid_size) * grid_size + grid_size / 2));
        float _y = abs(_tmp.y - (std::floor(_tmp.y / grid_size) * grid_size + grid_size / 2));
        
        if (_x > grid_size * inner_percent || _y > grid_size * inner_percent) {
            pos.x = -1;
            return;
        }
        
        pos.x = std::min(19, (int) std::floor(_tmp.x / grid_size) + 1);
        pos.y = std::min(19, (int) std::floor(_tmp.y / grid_size) + 1);
        if (board_state[pos.x][pos.y]) pos.x = -1;
    }
    else {
        pos.x = -1;
    }
    
}

void Board::update() {
    start_position = { position.x - board_size / 2.f + board_padding, position.y - board_size / 2.f + board_padding };
    
    canvas.setFillColor(sf::Color(242, 176, 109));
    canvas.setSize({ canvas_size, canvas_size });
    canvas.setOrigin(canvas.getLocalBounds().getCenter());
    canvas.setPosition(position);

    board.setSize({ board_size, board_size });
    board.setOrigin(board.getLocalBounds().getCenter());
    board.setPosition(position);
    //board_background.setSmooth(true);
    //board.setTexture(&board_background);
    board.setFillColor(sf::Color::White);

    stone.setRadius(grid_size * 0.6);
    stone.setOrigin(stone.getLocalBounds().getCenter());
}