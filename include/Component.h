#pragma once
#include<SFML/Graphics.hpp>

class SimpleButton {
public:

	SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& text, unsigned int text_size, const sf::Font& font);

	// Return value
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };

	// Update value
	void updatePos(const sf::Vector2f& _position) { position = _position; };
	void updateSize(const sf::Vector2f& _size) { size = _size; };

	void setButtonColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_button_color = norm_c, hover_button_color = hover_c; }
	void setTextColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_text_color = norm_c, hover_text_color = hover_c; }

	// Event check and update

	void checkHover(const sf::Vector2i& mouse_pos);

	void handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	void update();

	void draw(sf::RenderWindow& window) const;

private:
	sf::RectangleShape button;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;

	bool hovered = false;

	// Color for button
	sf::Color normal_button_color{ 100, 100, 200 };
	sf::Color hover_button_color{ 150, 150, 250 };

	// Color for text
	sf::Color normal_text_color{ 255, 255, 255 };
	sf::Color hover_text_color{ 255, 255, 255 };
};