#include<SFML/Graphics.hpp>
#include<UI/Menu.h>
#include<UI/Component.h>
#include<core/AssetManager.h>
#include<algorithm>
#include<iostream>

sf::Vector2f button_size = { 300,100 };
int margin = 50;

static void testClickFunction() {
    std::cerr << "Click button" << std::endl;
}


void Menu::build(const sf::Vector2u& window_size, const AssetManager& asset_manager) {
    SimpleButton start_button({ window_size.x / 2.f, window_size.y / 2.f }, button_size, "START", 50, asset_manager.getFont("Roboto-Slab-Bold"));
    SimpleButton setting_button({ window_size.x / 2.f, window_size.y / 2.f + (button_size.y + margin + button_size.y / 2.f)}, button_size, "SETTING", 50, asset_manager.getFont("Roboto-Slab-Bold"));
   

    auto startButtonClick = [this]() {
        this -> game_config.state = GameState::Playing;
        std::cerr << "clciked" << std::endl;
    };

    start_button.updateOnClick(startButtonClick);
    setting_button.updateOnClick(testClickFunction);

    buttons.emplace("Start", std::move(start_button));
    buttons.emplace("Setting", std::move(setting_button));

    sf::Text title(asset_manager.getFont("Momo"), "Go Game", 100);
    title.setFillColor(sf::Color(0,0,0));
    sf::FloatRect title_bound = title.getLocalBounds();
    title.setOrigin({title_bound.position.x + title_bound.size.x / 2.f, title_bound.position.y + title_bound.size.y / 2.f });
    title.setPosition({ window_size.x / 2.f, window_size.y / 2.f - (button_size.y + margin + title_bound.size.y) });

    texts.emplace("Title", std::move(title));
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
    for (auto& [label, text] : texts) {
        window.draw(text);
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
    start_button.updatePos({ window_size.x / 2.f, window_size.y / 2.f});
    setting_button.updatePos({ window_size.x / 2.f, window_size.y / 2.f + (button_size.y + margin + button_size.y / 2.f) });

    sf::Text& title = texts.at("Title");
    title.setCharacterSize(std::min(window_size.x, window_size.y) / 7);
    
    sf::FloatRect title_bound = title.getLocalBounds();
    title.setOrigin({ title_bound.position.x + title_bound.size.x / 2.f, title_bound.position.y + title_bound.size.y / 2.f });
    title.setPosition({ window_size.x / 2.f, window_size.y / 2.f - (button_size.y + margin + title_bound.size.y) });
}