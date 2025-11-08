#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class SimpleButton {
public:
    SimpleButton(const sf::Vector2f& position, const sf::Vector2f& size, const unsigned int font_size,
        const std::string& text, const sf::Font& font);

    void update(const sf::RenderWindow& window);
    bool isClicked(const sf::RenderWindow& window) const;
    void draw(sf::RenderWindow& window) const;

    void ChangeHoverColor(sf::Color color) { hover_color = color; }
    void ChangeNormalColor(sf::Color color) { normal_color = color; }

private:
    sf::RectangleShape shape;
    sf::Text label;
    bool hovered = false;

    sf::Color normal_color = sf::Color(100, 100, 200);
    sf::Color hover_color = sf::Color(150, 150, 250);
};
