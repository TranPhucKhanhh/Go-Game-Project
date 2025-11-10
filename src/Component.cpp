#include<Component.h>
#include<UI.h>
#include<iostream>

bool FontManager::load(const std::string& name, std::string path) {
	sf::Font font;
	path = std::string(ASSET_DIR) + path;
	if (!font.openFromFile(path)) {
		std::cerr << "ERROR: Can't load font from " << path << "\n";
		return false;
	}
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
	
	update();
}

void SimpleButton::checkHover(const sf::Vector2i &mouse_pos)  {
	sf::FloatRect shape_bound = button.getGlobalBounds();

	if (shape_bound.contains((sf::Vector2f)mouse_pos)) {
		hovered = true;
		onHover();
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
	button.setFillColor(normal_button_color);
	text.setFillColor(normal_text_color);

	button.setPosition(position);

	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f,
						textBounds.position.y + textBounds.size.y / 2.f });
	text.setPosition({ position.x + size.x / 2.f, position.y + size.y / 2.f });
}

void SimpleButton::eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos) {
	if (event.is<sf::Event::MouseMoved>()) {
		checkHover(mouse_pos);
	}
	else if (event.is<sf::Event::MouseButtonPressed>()) {
		mouseHold = true;
		if (hovered) {
			onMouseHold();	
		}
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		if (hovered && mouseHold) {
			onClick();
		}
		mouseHold = false;
	}
}