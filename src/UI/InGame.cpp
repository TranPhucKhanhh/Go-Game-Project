#include<SFML/Graphics.hpp>
#include<UI/InGame.h>
#include<UI/Component.h>
#include<game.h>
#include<algorithm>

InGame::InGame(const AssetManager& _asset_manager, const GameCfg& _game_config) : asset_manager(_asset_manager), game_config(_game_config) {

}

void InGame::build(const sf::Vector2u& window_size) {
    resize(window_size);
}

void InGame::eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos, const sf::Vector2u& window_size) {

}

void InGame::draw(sf::RenderWindow& window) {
    board.draw(window);
}

void InGame::resize(const sf::Vector2u& window_size) {
    unsigned int min_scale = std::min(window_size.x, window_size.y);
    
    margin = std::min(min_scale / 10, 50u);
    canva_size = min_scale - margin;

    board.resize(canva_size);
}