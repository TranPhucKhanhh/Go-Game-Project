#include<UI/Component.h>

void TextBox::draw(sf::RenderWindow& window) const {
	window.draw(box);
	window.draw(text);
}

void TextBox::update() {
	box.setOrigin(box.getSize() / 2.f);
	box.setPosition(position);

	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(position);
}