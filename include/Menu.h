#pragma once
#include<SFML/Graphics.hpp>
#include<Component.h>
#include<FontManager.h>
#include<vector>
#include<string>

class Menu {
public:
    void build(const sf::Vector2u& window_size, const FontManager& font_manager);

    void checkHover(const sf::Vector2i& mouse_pos);

    void resize(const sf::Vector2u& window_size);
    
    void draw(sf::RenderWindow& window);
private:
    std::unordered_map<std::string, SimpleButton> buttons;
};