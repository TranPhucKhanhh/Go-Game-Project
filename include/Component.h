#pragma once
#include<SFML/Graphics.hpp>

class SimpleButton {
public:
	sf::Vector2f position;
	sf::Vector2f size;

	SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& text, unsigned int text_size, const sf::Font& font);
	
	void checkHover(const sf::Vector2i &mouse_pos);

	void handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	void updatePos(sf::Vector2f position, sf::Vector2f size);

	void draw(sf::RenderWindow& window) const;

	// Change value
	void setButtonColor(const sf::Color& norm_c, const sf::Color &hover_c) { normal_button_color = norm_c, hover_button_color = hover_c; }
	void setTextColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_text_color = norm_c, hover_text_color = hover_c; }
private:
	sf::RectangleShape button;
	sf::Text text;

	bool hovered = false;

	// Color for button
	sf::Color normal_button_color{ 100, 100, 200 };
	sf::Color hover_button_color{ 150, 150, 250 };

	// Color for text
	sf::Color normal_text_color{ 255, 255, 255 };
	sf::Color hover_text_color{ 255, 255, 255 };
};