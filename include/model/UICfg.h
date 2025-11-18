#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>

struct UICfg {
    sf::Vector2u window_size;
    sf::Vector2u min_window_size;

    sf::Vector2i mouse_pos;
    std::string title;

    sf::RenderWindow& window;

    UICfg(sf::Vector2u _window_size, sf::Vector2u _min_window_size, sf::RenderWindow& _window) : 
        window_size(_window_size), min_window_size(_min_window_size), window(_window) {

    }
};