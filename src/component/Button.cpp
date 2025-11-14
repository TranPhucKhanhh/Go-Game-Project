#include<UI/Component.h>
#include<UI/UI.h>
#include<iostream>

SimpleButton::SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& str,
	unsigned int text_size, const sf::Font& font) : button(size), text(font, str, text_size), size(size), position(position), text_size(text_size) {
	update();
}

void SimpleButton::checkHover(const sf::Vector2i& mouse_pos) {
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

void SimpleButton::update() {
	if (mouse_hold && hovered) {
		onMouseHold();
	}
	else if (hovered) {
		onHover();
	}
	else {
		onIdle();
	}

	button.setSize(size);
	button.setOrigin(size / 2.f);
	button.setPosition(position);

	text.setCharacterSize(text_size);
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(position);
}

void SimpleButton::eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos) {
	if (event.is<sf::Event::MouseMoved>()) {
		checkHover(mouse_pos);
	}
	else if (const auto* button = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (button->button == sf::Mouse::Button::Left)
			mouse_hold = true;
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		if (hovered && mouse_hold) {
			onClick();
		}
		mouse_hold = false;
	}
}