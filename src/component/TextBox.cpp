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
	float ratio = std::min(1.f,_size.x / bound.size.x) * bound.size.y * _ratio;

	updateTextSize(ratio);
}

float TextBox::getTextSizeFit(const float _ratio) {
	sf::Vector2f _size = box.getLocalBounds().size;
	text.setCharacterSize(_size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = std::min(1.f, _size.x / bound.size.x) * bound.size.y * _ratio;

	return ratio;
}

void TextBox::updateTextSizeFitWithScale(const float _ratio) {
    sf::FloatRect tb = text.getLocalBounds();

    if (tb.size.x <= 0.f || tb.size.y <= 0.f)
        return;

    sf::FloatRect boxBounds = box.getLocalBounds();
    float availableWidth = boxBounds.size.x * _ratio;
    float availableHeight = boxBounds.size.y * _ratio;

    if (availableWidth <= 0 || availableHeight <= 0)
        return;

    float scaleX = availableWidth / tb.size.x;
    float scaleY = availableHeight / tb.size.y;

    float scale = std::min(scaleX, scaleY);

    scale = std::min(scale, 1.f);

    text.setScale({ scale, scale });
}