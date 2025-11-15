#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<model/CellState.h>
#include<game/Game.h>
#include<iostream>
#include<cmath>

void BoardUI::updateSize(const float& size) {
    canvas_size = size;
    canvas_padding = canvas_size / 20;
    board_size = canvas_size - canvas_padding * 2;
    grid_size = board_size / (board_cell_number + 1);
    board_padding = grid_size;

    sf::Color c = predict_stone.getFillColor();
    c.a = 200;
    predict_stone.setFillColor(c);
}

void BoardUI::draw(sf::RenderWindow& window, const Board& current_board) {
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

    if (hoverOnStone) {
        window.draw(predict_stone);
    }

    for (int i = 0; i < board_cell_number; i++) {
        for (int j = 0; j < board_cell_number; j++) {
            if (current_board[i][j] != CellState::Empty) {
                stone.setPosition({start_position.x + grid_size * i, start_position.y + grid_size * j });
                
                if (current_board[i][j] == CellState::White) stone.setTexture(&white_texture);
                else stone.setTexture(&black_texture);

                window.draw(stone);
            }
        }
    }
}

void BoardUI::hoverStone(const sf::Vector2i& mouse_pos, const Game& game) {
    if (board.getGlobalBounds().contains((sf::Vector2f)mouse_pos)) {
        sf::Vector2f _tmp = { mouse_pos.x - start_position.x + board_padding - grid_size / 2, mouse_pos.y - start_position.y + board_padding - grid_size / 2};
        _tmp = { std::max(0.f, _tmp.x), std::max(0.f, _tmp.y) };        
        float _x = abs(_tmp.x - (std::floor(_tmp.x / grid_size) * grid_size + grid_size / 2));
        float _y = abs(_tmp.y - (std::floor(_tmp.y / grid_size) * grid_size + grid_size / 2));
        
        if (_x > grid_size * inner_percent || _y > grid_size * inner_percent) {
            hoverOnStone = 0;
            return;
        }
        
        int max_board_size = game.getBoardSize();

        pos.x = std::min(max_board_size-1, (int) std::floor(_tmp.x / grid_size));
        pos.y = std::min(max_board_size-1, (int) std::floor(_tmp.y / grid_size));
        if (game.getCurrentBoard()[pos.x][pos.y] != CellState::Empty) {
            hoverOnStone = 0;
            return;
        }

        if (game.getCurrentPlayer() == CellState::White) predict_stone.setTexture(&white_texture);
        else predict_stone.setTexture(&black_texture);

        predict_stone.setPosition({ start_position.x + grid_size * pos.x, start_position.y + grid_size * pos.y });
        hoverOnStone = 1;
    }
    else {
        hoverOnStone = 0;
    }
    
}

void BoardUI::placeStone(const sf::Vector2i& mouse_pos, Game& game) {
    if (hoverOnStone) {
        game.placeStone(pos.x, pos.y);
    }
}

void BoardUI::update() {
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

    stone.setRadius(grid_size * 0.45);
    stone.setOrigin(stone.getLocalBounds().getCenter());

    predict_stone.setRadius(grid_size * 0.45);
    predict_stone.setOrigin(stone.getLocalBounds().getCenter());
}