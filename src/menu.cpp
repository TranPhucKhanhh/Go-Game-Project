#include<SFML/Graphics.hpp>
#include<Menu.h>
#include<UI.h>
#include<Component.h>

void Menu::build(const sf::Vector2u& window_size, const FontManager& font_manager) {

    sf::Vector2f button_size = { 300,100 };

    SimpleButton start_button({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f }, button_size, "Start", 20, font_manager.get("arial"));

    buttons.emplace_back(start_button);
}

void Menu::checkHover(const sf::Vector2i& mouse_pos) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].checkHover(mouse_pos);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 217, 181));
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].update();
        buttons[i].draw(window);
    }
}

void Menu::resize(const sf::Vector2u& window_size) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].updatePos({ window_size.x / 2.f - buttons[i].getSize().x / 2.f, window_size.y / 2.f});
    }
}