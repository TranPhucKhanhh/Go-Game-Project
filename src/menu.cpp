#include<SFML/Graphics.hpp>
#include<Menu.h>
#include<UI.h>
#include<Component.h>

void Menu::build(const sf::Vector2u& window_size, const FontManager& font_manager) {

    sf::Vector2f button_size = { 300,100 };

    SimpleButton start_button({ window_size.x / 2.f - button_size.x / 2.f, window_size.y / 2.f }, button_size, "Start", 20, font_manager.get("arial"));

    start_button::onIdle() {

    }

    buttons.emplace("Start", std::move(start_button));
}

void Menu::eventHandle(const sf::Event &event, const sf::Vector2i &mouse_pos, const sf::Vector2u &window_size) {
    
    if (event.is<sf::Event::Resized>()) {
        resize(window_size);
    }

    for (auto& [label, button] : buttons) {
        button.eventHandle(*event,mouse_pos);
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
    SimpleButton &start_button = buttons.at("Start");

    start_button.updatePos({ window_size.x / 2.f - start_button.getSize().x / 2.f, window_size.y / 2.f});
}