#pragma once
#include<SFML/Graphics.hpp>
#include<game/Board.h>
#include<game/Game.h>
#include<model/UICfg.h>
#include<functional>

class SimpleButton {
public:

	// Color for button
	sf::Color normal_button_color{ 100, 100, 200 };
	sf::Color hover_button_color{ 90, 90, 150 };
	sf::Color press_button_color{ 62, 62, 89 };

	// Color for text
	sf::Color normal_text_color{ 255, 255, 255 };
	sf::Color hover_text_color{ 255, 255, 255 };

	SimpleButton() = default;
	SimpleButton(const std::string& text, const sf::Font& font);

	// Return value
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };
	float getTextSizeFit(const float _ratio);

	// Update value
	void updatePos(const sf::Vector2f& _position) { position = _position; updateState(); };
	void updateSize(const sf::Vector2f& _size) { size = _size; updateState(); };
	void updateTextSize(unsigned int _text_size) { text_size = _text_size; updateState(); };
	void updateOpacity(const float& c) { normal_button_color.a = c, hover_button_color.a = c, press_button_color.a = c; };
	void updateBorder(const float& thickness, const sf::Color& color) { button.setOutlineColor(color), button.setOutlineThickness(thickness); };
	void updateTextSizeFit(const float _ratio);

	void updateRespondStr(const std::string& respond_str) { on_click_respond = respond_str; }
	// Event check and update

	void checkHover(const sf::Vector2i& mouse_pos);

	void eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond);

	void updateState();

	void updateEffect();

	void draw(sf::RenderWindow& window);

	// Function intended for modifying outside the class
	void onIdle() { button.setFillColor(normal_button_color); };

	void onHover() { button.setFillColor(hover_button_color); };

	void onMouseHold() { button.setFillColor(press_button_color); };

private:
	sf::RectangleShape button;
	sf::Text text;

	sf::Vector2f position = { 0.f,0.f };
	sf::Vector2f size = { 0.f,0.f };
	unsigned int text_size = 0;

	std::string on_click_respond;

	bool hovered = false;
	bool mouse_hold = false;
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
	void updateStoneTexture(const sf::Texture& w_texture, const sf::Texture& b_texture) { white_texture = w_texture, black_texture = b_texture; };

	// React to mouse event
	void hoverStone(const sf::Vector2i& mouse_pos, const Game& game);

	void placeStone(const sf::Vector2i& mouse_pos, Game& game) const;

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

	TextBox(const sf::Font& font) : text(font) {};

	// Return value
	float getTextSizeFit(const float _ratio);

	// Update value
	void updateBoxPos(const sf::Vector2f& _position) { position = _position; update(); };
	void updateBoxSize(const sf::Vector2f& _size) { box.setSize(_size); update(); };
	void updateBoxColor(const sf::Color& color) { box.setFillColor(color); }

	void updateTextColor(const sf::Color& color) { text.setFillColor(color); }
	void updateTextSize(const float& size) { text.setCharacterSize(size); update(); }
	void updateTextSizeFit(const float _ratio);
	void updateStr(const std::string& _text) { text.setString(_text); update(); }
	void update();

	void draw(sf::RenderWindow& window) const;

private:
	sf::RectangleShape box;
	sf::Text text;

	sf::Vector2f position;
};

class Slider {
public:
	float value = 0;
	float pre_value = 0;
	bool show = false;
	std::string direction = "Right";

	sf::Color handle_color = sf::Color({ 0, 0, 0 });
	sf::Color slide_color = sf::Color({ 255, 255, 255 });
	sf::Color fill_color = sf::Color({ 173, 240, 199 });

	Slider() = default;

	// update value
	void updateSize(const sf::Vector2f& _size) { size = _size; updateState(); };
	void updatePos(const sf::Vector2f& _pos) { position = _pos; updateState(); };

	void updateState();

	void updateEffect();

	void eventHandle(const sf::Event& event, const UICfg& ui_cfg);

	void draw(sf::RenderWindow& window);
private:
	sf::RectangleShape slide;
	sf::RectangleShape fill;
	sf::CircleShape handle;

	sf::Vector2i mouse_last_pos = { -1,-1 };
	sf::Vector2f position = { 0,0 }, size = { 0,0 };

	bool hold = false;
	bool hover = false;
};