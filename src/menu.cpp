#include<SFML/Graphics.hpp>
#include<UI.h>
#include<Component.h>

void UI::MenuProp::build(const sf::Vector2u& window_size, const FontManager& font_manager) {

    sf::Vector2f button_size = { 300,100 };

    std::cerr << "Get window size " << window_size.x << " " << window_size.y << std::endl;

    SimpleButton start_button({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f }, button_size, "Start", 20, font_manager.get("arial"));

    buttons.emplace_back(start_button);
}

void UI::MenuProp::checkHover(const sf::Vector2i& mouse_pos) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].checkHover(mouse_pos);
    }
}

void UI::MenuProp::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 217, 181));
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].draw(window);
    }
}

void UI::MenuProp::resize(const sf::Vector2u& window_size) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].updatePos({ window_size.x / 2.f - buttons[i].size.x / 2.f, window_size.y / 2.f }, buttons[i].size);
    }
}