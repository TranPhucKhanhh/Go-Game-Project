#include<Component.h>
#include<UI.h>
#include<iostream>

SimpleButton::SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& str,
	unsigned int text_size, const sf::Font& font) : button(size), text(font, str, text_size), size(size), position(position) {
	
	button.setPosition(position);
	button.setFillColor(normal_button_color);

	text.setFillColor(normal_text_color);

	sf::FloatRect textBounds = text.getLocalBounds();

	std::cerr << "Create a button on " << position.x << " " << position.y << std::endl;

	text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f,
						textBounds.position.y + textBounds.size.y / 2.f });

	text.setPosition({ position.x + size.x / 2.f, position.y + size.y / 2.f});
}

void SimpleButton::checkHover(const sf::Vector2i &mouse_pos)  {
	sf::FloatRect shape_bound = button.getGlobalBounds();

	if (shape_bound.contains((sf::Vector2f)mouse_pos)) {
		hovered = true;
	}
	else {
		hovered = false;
	}
}

void SimpleButton::draw(sf::RenderWindow& window) const {
	window.draw(button);
	window.draw(text);
}

void SimpleButton::updatePos(sf::Vector2f position, sf::Vector2f size) {
	button.setPosition(position);

	sf::FloatRect textBounds = text.getLocalBounds();

	text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f,
						textBounds.position.y + textBounds.size.y / 2.f });

	text.setPosition({ position.x + size.x / 2.f, position.y + size.y / 2.f });
}
