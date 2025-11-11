#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>


class FontManager {
public:
	bool load(const std::string& name, std::string path);

	const sf::Font& get(const std::string& name) const;

	bool has(const std::string& name) const;

private:
	std::unordered_map<std::string, sf::Font> fonts;
};

class SimpleButton {
public:

	SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& text, unsigned int text_size, const sf::Font& font);

	// Return valuef::Font& f
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };

	// Update value
	void updatePos(const sf::Vector2f& _position) { position = _position; };
	void updateSize(const sf::Vector2f& _size) { size = _size; };
	void updateTextSize(unsigned int text_size) { text_size = text_size; };

	void updateButtonColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_button_color = norm_c, hover_button_color = hover_c; }
	void updateTextColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_text_color = norm_c, hover_text_color = hover_c; }
	// Event check and update

	void checkHover(const sf::Vector2i& mouse_pos);

	void eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos);

	void update();

	void draw(sf::RenderWindow& window) const;

	// Function for modifying outside the class
	void onIdle() { button.setFillColor(normal_button_color); };

	void onHover() { button.setFillColor(hover_button_color); };

	void onClick() { 
		std::cerr << "Button Click!" << std::endl; 
		sf::FloatRect b = text.getLocalBounds();
		std::cerr << b.position.x << " " << b.position.y << " " << b.size.x << " " << b.size.y << std::endl;
		b = text.getGlobalBounds();
		std::cerr << b.position.x << " " << b.position.y << " " << b.size.x << " " << b.size.y << std::endl;
	}

	void onMouseHold() { button.setFillColor(press_button_color); };

private:
	sf::RectangleShape button;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;

	bool hovered = false;
	bool mouse_hold = false;
	unsigned int text_size;

	// Color for button
	sf::Color normal_button_color{ 100, 100, 200 };
	sf::Color hover_button_color{ 90, 90, 150 };
	sf::Color press_button_color{ 62, 62, 89 };

	// Color for text
	sf::Color normal_text_color{ 255, 255, 255 };
	sf::Color hover_text_color{ 255, 255, 255 };
};