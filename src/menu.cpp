#include<SFML/Graphics.hpp>
#include<Menu.h>
#include<UI.h>
#include<Component.h>
#include<algorithm>

sf::Vector2f button_size = { 300,100 };
int margin = 50;

void Menu::build(const sf::Vector2u& window_size, const FontManager& font_manager) {
    SimpleButton start_button({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f }, button_size, "START", 50, font_manager.get("Roboto-Slab-Bold"));
    SimpleButton setting_button({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f + (button_size.y + margin)}, button_size, "SETTING", 50, font_manager.get("Roboto-Slab-Bold"));
    //sf::Text title(font_manager.get("Rubik"), "Go Game", 75);

    buttons.emplace("Start", std::move(start_button));
    buttons.emplace("Setting", std::move(setting_button));
}

void Menu::eventHandle(const sf::Event &event, const sf::Vector2i &mouse_pos, const sf::Vector2u &window_size) {
    
    if (event.is<sf::Event::Resized>()) {
        resize(window_size);
    }

    for (auto& [label, button] : buttons) {
        button.eventHandle(event,mouse_pos);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 217, 181));
    for (auto& [label, button] : buttons) {
        button.update();
        button.draw(window);
    }
}

void Menu::resize(const sf::Vector2u& window_size) {
    SimpleButton& start_button = buttons.at("Start");
    SimpleButton& setting_button = buttons.at("Setting");

    unsigned int scale = std::min(window_size.x / 5, window_size.y / 7 * 3);
    button_size.x = std::min(500u, std::max( scale, 300u));
    button_size.y = std::min(166u, std::max( scale / 3, 100u));
    margin = std::min(150u, std::max(window_size.y / 14, 50u));

    start_button.updateSize(button_size);
    setting_button.updateSize(button_size);
    start_button.updatePos({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f});
    setting_button.updatePos({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f + (button_size.y + margin) });
}