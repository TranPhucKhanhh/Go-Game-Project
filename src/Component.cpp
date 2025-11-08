#include "Component.h"

SimpleButton::SimpleButton(const sf::Vector2f& position, const sf::Vector2f& size, const unsigned int font_size,
    const std::string& text, const sf::Font& font) : shape(size), label(font, text, font_size)
{
    shape.setPosition(position);
    shape.setFillColor(normal_color);

    label.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = label.getLocalBounds();

    label.setOrigin({   textBounds.position.x + textBounds.size.x / 2.f, 
                        textBounds.position.y + textBounds.size.y / 2.f });

    label.setPosition({ position.x + size.x / 2.f, position.y + size.y / 2.f - 5 });
}

void SimpleButton::update(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        shape.setFillColor(hover_color);
        hovered = true;
    }
    else {
        shape.setFillColor(normal_color);
        hovered = false;
    }
}

bool SimpleButton::isClicked(const sf::RenderWindow& window) const {
    return hovered && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

void SimpleButton::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}
