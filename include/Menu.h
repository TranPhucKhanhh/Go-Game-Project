#pragma once
#include<SFML/Graphics.hpp>
#include<Component.h>
#include<vector>
#include<string>

class Menu {
public:
    Menu() = default;

    void build(const sf::Vector2u& window_size, const FontManager& font_manager);

    void eventHandle(const sf::Event &event, const sf::Vector2i &mouse_pos, const sf::Vector2u &window_size);

    void draw(sf::RenderWindow& window);
private:
    std::unordered_map<std::string, SimpleButton> buttons;
    std::unordered_map<std::string, sf::Text> texts;

    void resize(const sf::Vector2u& window_size);  
};