#include<UI/Component.h>

TextBox::TextBox(sf::Vector2f position, sf::Vector2f size, const std::string& str,
	unsigned int text_size, const sf::Font& font) : box(size), text(font, str, text_size), size(size), position(position), text_size(text_size) {
	update();
}

void TextBox::draw(sf::RenderWindow& window) const {
	window.draw(box);
	window.draw(text);
}

void TextBox::update() {
	box.setSize(size);
	box.setOrigin(size / 2.f);
	box.setPosition(position);

	text.setCharacterSize(text_size);
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(position);
}