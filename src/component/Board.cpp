#include<SFML/Graphics.hpp>
#include<UI/Component.h>

void Board::resize(float size) {
    canvas.setSize({ size, size });
}

void Board::draw(sf::RenderWindow& window) {
    for (auto& stone : stone_placement) {
        stone.draw(window);
    }
    window.draw(canvas);
}