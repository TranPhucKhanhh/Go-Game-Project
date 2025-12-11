#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<core/AssetManager.h>
#include<model/CellState.h>
#include<game/Game.h>
#include<iostream>
#include<cmath>

static char charPos[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'};

static float min(const float& a, const float& b) {
    if (a < b) return a;
    return b;
}

BoardUI::BoardUI(const AssetManager & _asset_manager) :
    font(_asset_manager.getFont("Momo")), stone(_asset_manager.getTexture("dummy")), predict_stone(_asset_manager.getTexture("dummy")) {
    board.setFillColor(sf::Color::White);
}

void BoardUI::updateSize(const float& size) {
    canvas_size = size;
    canvas_padding = canvas_size / 25;
    board_size = canvas_size - canvas_padding * 0;
    grid_size = board_size / (board_cell_number + 1);
    board_padding = grid_size;

    predict_stone.setColor(sf::Color(255,255,255, 200));
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
        line.setFillColor(line_color);
        line.setPosition({ start_position.x + i * grid_size, start_position.y });
        window.draw(line);
        
        // draw Letter
        sf::Text letter_text(font, charPos[i], (grid_size * 0.6f));
		letter_text.setFillColor(text_color);
		letter_text.setPosition({ start_position.x + i * grid_size - letter_text.getLocalBounds().size.x / 2, start_position.y - grid_size});
        window.draw(letter_text);

        letter_text.setPosition({ start_position.x + i * grid_size - letter_text.getLocalBounds().size.x / 2, position.y + board_size/2.f - grid_size * 0.85f});
        window.draw(letter_text);
    }
    // draw horizontal lines
    for (int i = 0; i < board_cell_number; i++) {
        sf::RectangleShape line({ line_length + line_thickness * (i==board_cell_number-1), line_thickness});
        line.setFillColor(line_color);
        line.setPosition({ start_position.x, start_position.y + i * grid_size });
        window.draw(line);

        // draw number
        sf::Text number_text(font, std::to_string(board_cell_number - i), (grid_size * 0.6f));
        number_text.setFillColor(text_color);
		number_text.setOrigin(number_text.getLocalBounds().getCenter());
        number_text.setPosition({ start_position.x - grid_size/2.f, start_position.y + i * grid_size});
        window.draw(number_text);

        number_text.setPosition({ start_position.x - grid_size * 1.5f + board_size, start_position.y + i * grid_size});
        window.draw(number_text);
    }

    if (hoverOnStone) {
        window.draw(predict_stone);
    }

    for (int i = 0; i < board_cell_number; i++) {
        for (int j = 0; j < board_cell_number; j++) {
            if (current_board[i][j] != CellState::Empty) {
                
                if (current_board[i][j] == CellState::White) stone.setTexture(white_texture, true);
                else stone.setTexture(black_texture, true);

                BoardUI::updateStone();
                stone.setPosition({start_position.x + grid_size * i, start_position.y + grid_size * j });
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


        if (game.getCurrentPlayer() == CellState::White) predict_stone.setTexture(white_texture, true);
        else predict_stone.setTexture(black_texture, true);
        BoardUI::updateStone();

        predict_stone.setPosition({ start_position.x + grid_size * pos.x, start_position.y + grid_size * pos.y });
        hoverOnStone = 1;
    }
    else {
        hoverOnStone = 0;
    }
}

void BoardUI::placeStone(const sf::Vector2i& mouse_pos, Game& game) const {
    if (hoverOnStone) {
        game.placeStone(pos.x, pos.y);
    }
}

void BoardUI::update() {
    start_position = { position.x - board_size / 2.f + board_padding, position.y - board_size / 2.f + board_padding };
    
    //canvas.setFillColor(sf::Color(242, 176, 109));
    //canvas.setSize({ canvas_size, canvas_size });
    //canvas.setOrigin(canvas.getLocalBounds().getCenter());
    //canvas.setPosition(position);

    board.setSize({ board_size, board_size });
    board.setOrigin(board.getLocalBounds().getCenter());
    board.setPosition(position);
    //board_background.setSmooth(true);
    //board.setTexture(&board_background);
    
    BoardUI::updateStone();
}

void BoardUI::updateStone() {
    BoardUI::updateRadiusSprite(stone, grid_size * 0.9);
    stone.setOrigin({ stone.getLocalBounds().size.x / 2.f,stone.getLocalBounds().size.y / 2.f });

    BoardUI::updateRadiusSprite(predict_stone, grid_size * 0.9);
    predict_stone.setOrigin({ predict_stone.getLocalBounds().size.x / 2.f,predict_stone.getLocalBounds().size.y / 2.f });
}

void BoardUI::updateRadiusSprite(sf::Sprite& _sprite, const float& _size) {
    _sprite.setScale({ 1.f, 1.f });

    sf::Vector2f _sprite_size = _sprite.getLocalBounds().size;

    float _new_scale = min(_size / _sprite_size.x, _size / _sprite_size.y);

    _sprite.setScale({ _new_scale , _new_scale });
}

void BoardUI::updateBoardUI(const std::string& _design, const AssetManager &asset_manager) {
    board.setTexture(nullptr);
    if (_design == "White color") {
        text_color = line_color = sf::Color(0, 0, 0, 200);
        board.setFillColor(sf::Color(255,255,255));
    }
    else if (_design == "Cyan color") {
        text_color = line_color = sf::Color(0, 0, 0, 200);
        board.setFillColor(sf::Color(200, 255, 255));
    }
    else if (_design == "Green color") {
        text_color = line_color = sf::Color(0, 0, 0, 200);
        board.setFillColor(sf::Color(200, 255, 200));
    }
    else if (_design == "Black color") {
        text_color = line_color = sf::Color(200, 200, 200, 200);
        board.setFillColor(sf::Color(50, 50, 50));
    }
    else if (_design == "Yellow color") {
        text_color = line_color = sf::Color(0, 0, 0, 200);
        board.setFillColor(sf::Color(255, 255, 200));
	}
    else {
        text_color = line_color = sf::Color(0, 0, 0, 200);
        board.setFillColor(sf::Color(255, 255, 255, 255));
        board.setTexture(&asset_manager.getTexture(_design));
    }
}

void BoardUI::updateStoneUI(const std::string& _design, const AssetManager &asset_manager) {
    white_texture = asset_manager.getTexture("white-stone-" + _design);
    black_texture = asset_manager.getTexture("black-stone-" + _design);
}