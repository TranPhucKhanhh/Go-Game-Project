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

void TextBox::updateTextSizeFit(const float _ratio) {
	sf::Vector2f _size = box.getLocalBounds().size;
	text.setCharacterSize(_size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = bound.size.x / bound.size.y;
	ratio = _size.x / ratio * _ratio;

	updateTextSize(ratio);
}

float TextBox::getTextSizeFit(const float _ratio) {
	sf::Vector2f _size = box.getLocalBounds().size;
	text.setCharacterSize(_size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = bound.size.x / bound.size.y;
	ratio = _size.x / ratio * _ratio;

	return ratio;
}