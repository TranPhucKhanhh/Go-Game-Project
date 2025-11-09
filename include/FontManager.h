#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>

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