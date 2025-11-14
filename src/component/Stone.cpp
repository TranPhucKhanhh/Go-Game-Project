#include<SFML/Graphics.hpp>
#include<UI/Component.h>

Stone::Stone(float size, unsigned int edge) : stone(size,edge) {
}

void Stone::draw(sf::RenderWindow& window) const {
	window.draw(stone);
}