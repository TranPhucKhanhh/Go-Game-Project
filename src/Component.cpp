#include<Component.h>
#include<UI.h>
#include<iostream>

bool FontManager::load(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::Font font;
	font.openFromFile(path);
	fonts[name] = std::move(font);
	return true;
}

const sf::Font& FontManager::get(const std::string& name) const {
	return fonts.at(name);
}

bool FontManager::has(const std::string& name) const {
	return fonts.find(name) != fonts.end();
}

SimpleButton::SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& str,
	unsigned int text_size, const sf::Font& font) : button(size), text(font, str, text_size), size(size), position(position), text_size(text_size) {
	
	sf::FloatRect text_bound = text.getLocalBounds();
	text.setOrigin({ text_bound.position.x + text_bound.size.x / 2.f,
						text_bound.position.y + text_bound.size.y / 2.f });
	button.setOrigin(size / 2.f);
	update();
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

	button.setPosition(position);
	button.setSize(size);

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