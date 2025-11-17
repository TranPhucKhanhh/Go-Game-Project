#include<UI/Component.h>
#include<UI/UI.h>
#include<iostream>

SimpleButton::SimpleButton(const std::string& str, const sf::Font& font) : 
	button(size), text(font, str) {
	
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

void SimpleButton::updateState() {
	button.setSize(size);
	button.setOrigin(size / 2.f);
	button.setPosition(position);

	text.setCharacterSize(text_size);
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(position);
}

void SimpleButton::updateEffect() {
	if (mouse_hold && hovered) {
		onMouseHold();
	}
	else if (hovered) {
		onHover();
	}
	else {
		onIdle();
	}
}

void SimpleButton::eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond) {
	if (event.is<sf::Event::MouseMoved>()) {
		checkHover(ui_cfg.mouse_pos);
	}
	else if (const auto* button = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (button->button == sf::Mouse::Button::Left)
			mouse_hold = true;
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		if (hovered && mouse_hold) {
			respond = on_click_respond;
		}
		mouse_hold = false;
	}
}

void SimpleButton::updateTextSizeFit(const float _ratio) {
	text.setCharacterSize(size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = bound.size.x / bound.size.y;
	ratio = size.x / ratio * _ratio;

	updateTextSize(ratio);
}

float SimpleButton::getTextSizeFit(const float _ratio) {
	text.setCharacterSize(size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = bound.size.x / bound.size.y;
	ratio = size.x / ratio * _ratio;

	return ratio;
}