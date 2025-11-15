#pragma once
#include<SFML/Graphics.hpp>
#include<game/Board.h>
#include<game/Game.h>
#include<functional>

class SimpleButton {
public:

	SimpleButton(sf::Vector2f position, sf::Vector2f size, const std::string& text, unsigned int text_size, const sf::Font& font);

	// Return value
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };

	// Update value
	void updatePos(const sf::Vector2f& _position) { position = _position; };
	void updateSize(const sf::Vector2f& _size) { size = _size; };
	void updateTextSize(unsigned int text_size) { text_size = text_size; };

	void updateButtonColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_button_color = norm_c, hover_button_color = hover_c; }
	void updateTextColor(const sf::Color& norm_c, const sf::Color& hover_c) { normal_text_color = norm_c, hover_text_color = hover_c; }
	void updateOnClick(std::function<void()> func) { onClick = func; }
	// Event check and update

	void checkHover(const sf::Vector2i& mouse_pos);

	void eventHandle(const sf::Event& event, const sf::Vector2i& mouse_pos);

	void update();

	void draw(sf::RenderWindow& window) const;

	// Function for modifying outside the class
	void onIdle() { button.setFillColor(normal_button_color); };

	void onHover() { button.setFillColor(hover_button_color); };

	void onMouseHold() { button.setFillColor(press_button_color); };

private:
	sf::RectangleShape button;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;
	unsigned int text_size;

	std::function<void()> onClick;

	bool hovered = false;
	bool mouse_hold = false;

	// Color for button
	sf::Color normal_button_color{ 100, 100, 200 };
	sf::Color hover_button_color{ 90, 90, 150 };
	sf::Color press_button_color{ 62, 62, 89 };

	// Color for text
	sf::Color normal_text_color{ 255, 255, 255 };
	sf::Color hover_text_color{ 255, 255, 255 };

};

class BoardUI {
public:
	void draw(sf::RenderWindow& window, const Board& current_board);

	// Return value
	sf::Vector2f getPos() const { return position; }
	
	// Update the board UI 
	void updateCellNumber(const int& size) { board_cell_number = size; }
	void updatePos(const sf::Vector2f& _position) { position = _position; }
	void updateSize(const float& _size);
	void updateTexture(const sf::Texture& texure) { board_background = texure; }

	// update stone
	void updateStoneTexture(const sf::Texture& b_texture, const sf::Texture& w_texture) { white_texture = w_texture, black_texture = b_texture; };

	// React to mouse event
	void hoverStone(const sf::Vector2i& mouse_pos, const Game& game);

	void placeStone(const sf::Vector2i& mouse_pos, Game& game);

	// Apply all changes to the board before draw
	void update();
private:
	sf::RectangleShape canvas;
	sf::RectangleShape board;
	sf::Texture board_background;

	sf::Vector2f position;
	sf::Vector2f start_position;
	float canvas_size;
	float board_size;
	float canvas_padding;
	float board_padding;
	float grid_size;
	float inner_percent = 0.45;

	int board_cell_number = 0;

	sf::CircleShape stone;
	sf::CircleShape predict_stone;
	sf::Vector2i pos = { -1,-1 };

	float stone_size;
	bool hoverOnStone = 0;

	sf::Texture white_texture;
	sf::Texture black_texture;
};

class TextBox {
public:

	TextBox(sf::Vector2f position, sf::Vector2f size, const std::string& text, unsigned int text_size, const sf::Font& font);

	// Return valuef::Font& f
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };

	// Update value
	void updatePos(const sf::Vector2f& _position) { position = _position; };
	void updateSize(const sf::Vector2f& _size) { size = _size;  };
	void updateTextSize(unsigned int _text_size) { text_size = _text_size; };

	void updateButtonColor(const sf::Color& color) { box.setFillColor(color); }
	void updateTextColor(const sf::Color& color) { text.setFillColor(color); }
	void update();

	void draw(sf::RenderWindow& window) const;

private:
	sf::RectangleShape box;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;
	unsigned int text_size;
};