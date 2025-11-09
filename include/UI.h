#pragma once
#include<SFML/Graphics.hpp>
#include<Game.h>
#include<Component.h>
#include<string>

class FontManager {
public:
    bool load(const std::string& name, std::string path) {
        sf::Font font;
        path = std::string(ASSET_DIR) + path;
        if (!font.openFromFile(path)) {
            std::cerr << "ERROR: Can't load font from " << path << "\n";
            return false;
        }
        fonts[name] = std::move(font); 
        return true;
    }

    const sf::Font& get(const std::string& name) const {
        return fonts.at(name); 
    }

    bool has(const std::string& name) const {
        return fonts.find(name) != fonts.end();
    }

private:
    std::unordered_map<std::string, sf::Font> fonts;
};



class UI {
public:
    UI(Game& game, std::string title, sf::Vector2u window_size);

    // UI initialize
    void run();
    void setupUI();

    // Define struct for each game state
    struct MenuProp {
        std::vector<SimpleButton> buttons;
        
        void build(const sf::Vector2u& window_size, const FontManager& font_manager);

        void checkHover(const sf::Vector2i& mouse_pos);

        void draw(sf::RenderWindow& window);

        void resize(const sf::Vector2u& window_size);
    } menu;

private:
    Game& game;
    Player cur_player;

    sf::RenderWindow window;
    FontManager font_manager;
    sf::Vector2i mouse_pos;
    sf::Vector2u window_size;
};