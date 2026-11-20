#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<core/AssetManager.h>
#include<model/CellState.h>
#include<game/Game.h>
#include<algorithm>
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

void BoardUI::drawStarPoints(sf::RenderWindow& window) const {
    if (board_cell_number != 9 && board_cell_number != 13 && board_cell_number != 19) return;

    const int near_index = board_cell_number == 9 ? 2 : 3;
    const int middle = board_cell_number / 2;
    const int far_index = board_cell_number - 1 - near_index;
    const float radius = std::max(2.f, grid_size * 0.09f);
    sf::CircleShape point(radius);
    point.setOrigin({ radius, radius });
    point.setFillColor(line_color);

    for (int x : { near_index, middle, far_index }) {
        for (int y : { near_index, middle, far_index }) {
            // Small boards use four corner points and the center; 19x19 uses all nine.
            if (board_cell_number != 19 && ((x == middle) != (y == middle))) continue;
            point.setPosition({ start_position.x + x * grid_size, start_position.y + y * grid_size });
            window.draw(point);
        }
    }
}

void BoardUI::draw(sf::RenderWindow& window, const Board& current_board,
    const std::optional<Move>& last_move, bool show_hover) {
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

    drawStarPoints(window);

    if (show_hover && hoverOnStone) {
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

    if (last_move && !last_move->pass &&
        (last_move->player == CellState::Black || last_move->player == CellState::White) &&
        last_move->x >= 0 && last_move->y >= 0 &&
        last_move->x < current_board.size() && last_move->y < current_board.size() &&
        current_board[last_move->x][last_move->y] == last_move->player) {
        const float radius = grid_size * 0.19f;
        sf::CircleShape marker(radius);
        marker.setOrigin({ radius, radius });
        marker.setPosition({ start_position.x + last_move->x * grid_size,
            start_position.y + last_move->y * grid_size });
        marker.setFillColor(sf::Color::Transparent);

        // A white ring with a black outer rim remains visible on every stone design.
        marker.setOutlineColor(sf::Color::Black);
        marker.setOutlineThickness(std::max(2.f, grid_size * 0.06f));
        window.draw(marker);
        marker.setOutlineColor(sf::Color::White);
        marker.setOutlineThickness(std::max(1.f, grid_size * 0.025f));
        window.draw(marker);
    }
}

void BoardUI::drawMoveNumbers(sf::RenderWindow& window, const Board& current_board,
    const std::vector<std::vector<int>>& numbers, int selected_move) const {
    if (numbers.size() != current_board.size()) return;
    TextBox label(font);
    label.updateBoxColor(sf::Color::Transparent);
    label.updateBoxSize({ grid_size * 0.75f, grid_size * 0.55f });
    for (int x = 0; x < current_board.size(); ++x) {
        if (numbers[x].size() != current_board.size()) return;
        for (int y = 0; y < current_board.size(); ++y) {
            if (numbers[x][y] == 0 || current_board[x][y] == CellState::Empty) continue;
            const bool black = current_board[x][y] == CellState::Black;
            label.updateTextColor(numbers[x][y] == selected_move
                ? (black ? sf::Color(255, 210, 75) : sf::Color(25, 80, 200))
                : (black ? sf::Color::White : sf::Color::Black));
            label.updateStr(std::to_string(numbers[x][y]));
            label.updateBoxPos({ start_position.x + x * grid_size, start_position.y + y * grid_size });
            label.updateTextSize(label.getSize().y);
            label.updateTextSizeFitWithScale(0.95f);
            label.draw(window);
        }
    }
}

std::optional<sf::Vector2i> BoardUI::intersectionAt(const sf::Vector2i& mouse_pos) const {
    if (grid_size <= 0.f || board_cell_number <= 0) return std::nullopt;
    const float x = (mouse_pos.x - start_position.x) / grid_size;
    const float y = (mouse_pos.y - start_position.y) / grid_size;
    const int column = static_cast<int>(std::round(x));
    const int row = static_cast<int>(std::round(y));
    if (column < 0 || row < 0 || column >= board_cell_number || row >= board_cell_number ||
        std::abs(x - column) > inner_percent || std::abs(y - row) > inner_percent)
        return std::nullopt;
    return sf::Vector2i{ column, row };
}

std::string BoardUI::drawAssistance(sf::RenderWindow& window, const Board& current_board,
    const sf::Vector2i& mouse_pos) const {
    if (current_board.size() != board_cell_number) return "Hover stones for liberties";
    const sf::Color danger(230, 75, 35);
    const sf::Color selected(0, 170, 235);
    const sf::Color liberty(130, 245, 85);
    auto ring = [&](const Cell& cell, sf::Color color, float scale) {
        const float radius = grid_size * scale;
        sf::CircleShape marker(radius);
        marker.setOrigin({ radius, radius });
        marker.setPosition({ start_position.x + cell.x * grid_size,
            start_position.y + cell.y * grid_size });
        marker.setFillColor(sf::Color::Transparent);
        marker.setOutlineColor(sf::Color::Black);
        marker.setOutlineThickness(std::max(3.f, grid_size * 0.09f));
        window.draw(marker);
        marker.setOutlineColor(color);
        marker.setOutlineThickness(std::max(1.5f, grid_size * 0.045f));
        window.draw(marker);
    };

    // Recompute from the displayed position: captures, undo and history cannot leave stale rings.
    std::vector<std::vector<bool>> seen(current_board.size(),
        std::vector<bool>(current_board.size(), false));
    for (int x = 0; x < current_board.size(); ++x) {
        for (int y = 0; y < current_board.size(); ++y) {
            if (seen[x][y] || current_board[x][y] == CellState::Empty) continue;
            const GroupInfo group = current_board.inspectGroup(x, y);
            for (const Cell& cell : group.stones) {
                seen[cell.x][cell.y] = true;
                if (group.liberties.size() == 1) ring(cell, danger, 0.42f);
            }
        }
    }

    const auto point = intersectionAt(mouse_pos);
    if (!point) return "Hover stones | Red: atari";
    const GroupInfo group = current_board.inspectGroup(point->x, point->y);
    if (group.stones.empty()) return "Hover stones | Red: atari";
    for (const Cell& cell : group.stones)
        ring(cell, group.liberties.size() == 1 ? danger : selected, 0.35f);
    for (const Cell& cell : group.liberties) {
        const float radius = grid_size * 0.13f;
        sf::CircleShape dot(radius);
        dot.setOrigin({ radius, radius });
        dot.setPosition({ start_position.x + cell.x * grid_size,
            start_position.y + cell.y * grid_size });
        dot.setFillColor(liberty);
        dot.setOutlineColor(sf::Color::Black);
        dot.setOutlineThickness(std::max(1.f, grid_size * 0.035f));
        window.draw(dot);
    }
    return std::to_string(group.stones.size()) + " stones | " +
        std::to_string(group.liberties.size()) + " liberties" +
        (group.liberties.size() == 1 ? " | ATARI" : "");
}

void BoardUI::drawScoring(sf::RenderWindow& window, const AreaScore& score,
    const std::vector<std::vector<bool>>& dead_stones) const {
    if (score.ownership.size() != static_cast<std::size_t>(board_cell_number) ||
        dead_stones.size() != score.ownership.size()) return;
    for (int x = 0; x < board_cell_number; ++x) {
        for (int y = 0; y < board_cell_number; ++y) {
            const sf::Vector2f center{ start_position.x + x * grid_size, start_position.y + y * grid_size };
            if (dead_stones[x][y]) {
                const float radius = grid_size * 0.43f;
                sf::CircleShape veil(radius);
                veil.setOrigin({ radius, radius });
                veil.setPosition(center);
                veil.setFillColor(sf::Color(150, 150, 150, 160));
                window.draw(veil);
                sf::RectangleShape cross({ grid_size * 0.65f, grid_size * 0.075f });
                cross.setOrigin(cross.getSize() / 2.f);
                cross.setPosition(center);
                cross.setFillColor(sf::Color(245, 55, 45));
                cross.setOutlineColor(sf::Color::Black);
                cross.setOutlineThickness(std::max(1.f, grid_size * 0.025f));
                for (float angle : { 45.f, -45.f }) {
                    cross.setRotation(sf::degrees(angle));
                    window.draw(cross);
                }
            }
            const CellState owner = score.ownership[x][y];
            if (owner == CellState::Invalid) continue;
            // Offset markers on dead stones so their cross and identity stay readable.
            const sf::Vector2f marker_pos = center + (dead_stones[x][y]
                ? sf::Vector2f{ grid_size * 0.28f, grid_size * 0.28f } : sf::Vector2f{});
            if (owner == CellState::Empty) {
                const float radius = grid_size * 0.09f;
                sf::CircleShape neutral(radius);
                neutral.setOrigin({ radius, radius });
                neutral.setPosition(marker_pos);
                neutral.setFillColor(sf::Color(150, 150, 150));
                neutral.setOutlineColor(sf::Color::Black);
                neutral.setOutlineThickness(1.f);
                window.draw(neutral);
            }
            else {
                sf::RectangleShape territory({ grid_size * 0.24f, grid_size * 0.24f });
                territory.setOrigin(territory.getSize() / 2.f);
                territory.setPosition(marker_pos);
                territory.setFillColor(owner == CellState::Black ? sf::Color::Black : sf::Color::White);
                territory.setOutlineColor(owner == CellState::Black ? sf::Color::White : sf::Color::Black);
                territory.setOutlineThickness(std::max(1.f, grid_size * 0.025f));
                window.draw(territory);
            }
        }
    }
}

void BoardUI::scoringClick(const sf::Vector2i& mouse_pos, Game& game) const {
    const auto point = intersectionAt(mouse_pos);
    if (point) game.toggleDeadGroup(point->x, point->y);
}

void BoardUI::hoverStone(const sf::Vector2i& mouse_pos, const Game& game) {
    hoverOnStone = false;
    const auto point = intersectionAt(mouse_pos);
    if (!point || game.getBoardSize() != board_cell_number ||
        game.getCurrentBoard()[point->x][point->y] != CellState::Empty) return;
    pos = *point;
    if (game.getCurrentPlayer() == CellState::White) predict_stone.setTexture(white_texture, true);
    else predict_stone.setTexture(black_texture, true);
    updateStone();
    predict_stone.setPosition({ start_position.x + grid_size * pos.x, start_position.y + grid_size * pos.y });
    hoverOnStone = true;
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
