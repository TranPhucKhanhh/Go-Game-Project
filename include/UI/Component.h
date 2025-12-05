#pragma once
#include<SFML/Graphics.hpp>
#include<game/Board.h>
#include<game/Game.h>
#include<model/UICfg.h>
#include<functional>

class BaseButton {
public:

	// Color for button
	sf::Color normal_button_color{ 100, 100, 200 };
	sf::Color hover_button_color{ 90, 90, 150 };
	sf::Color press_button_color{ 62, 62, 89 };

	BaseButton() = default;

	// Return value
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };
	bool getHovered() const { return hovered; };
	bool isMouseHold() const { return mouse_hold; };

	// Update value
	void updateOutlineThickness(const float& thickness) { button.setOutlineThickness(thickness); };
	void updateOutlineColor(const sf::Color& color) { button.setOutlineColor(color); };
	void updatePos(const sf::Vector2f& _position) { position = _position; updateState(); };
	void updateSize(const sf::Vector2f& _size) { size = _size; updateState(); };
	void updateOpacity(const float& c) { normal_button_color.a = c, hover_button_color.a = c, press_button_color.a = c; };
	void updateBorder(const float& thickness, const sf::Color& color) { button.setOutlineColor(color), button.setOutlineThickness(thickness); };
	void updateRespondStr(const std::string& respond_str) { on_click_respond = respond_str; }

	// Event check and update
	void checkHover(const sf::Vector2i& mouse_pos);

	void eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond);
	virtual void updateState();
	virtual void updateEffect();

	virtual void draw(sf::RenderWindow& window);

	// Animation or effect function
	virtual void onIdle() { button.setFillColor(normal_button_color); };

	virtual void onHover() { button.setFillColor(hover_button_color); };

	virtual void onMouseHold() { button.setFillColor(press_button_color); };

protected:
	sf::RectangleShape button;

	sf::Vector2f position = { 0.f,0.f };
	sf::Vector2f size = { 0.f,0.f };

	std::string on_click_respond;

	bool hovered = false;
	bool mouse_hold = false;
};

class TextButton : public BaseButton {
public:
	// Color for text
	sf::Color normal_text_color{ 255, 255, 255 };
	sf::Color hover_text_color{ 255, 255, 255 };

	TextButton(const std::string& str, const sf::Font& font);

	// update value
	void updateTextSize(unsigned int _text_size) { text_size = _text_size; updateState(); };
	void updateStr(const std::string& _text) { text.setString(_text); updateState(); }
	void updateTextSizeFit(const float _ratio);
	void updateState() override;

	// Return value
	float getTextSizeFit(const float _ratio);

	// draw
	void draw(sf::RenderWindow& window) override;

private:
	sf::Text text;
	unsigned int text_size = 0;
};

class BoardUI {
public:
	void draw(sf::RenderWindow& window, const Board& current_board);

	// Return value
	sf::Vector2f getPos() const { return position; }
	sf::Vector2f getSize() const { return { canvas_size,canvas_size }; }
	
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
	sf::Vector2f getPos() const { return position; };
	sf::Vector2f getSize() const { return box.getSize(); };

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

	float pre_value = 0;

	bool hold = false;
	bool hover = false;
};

class Scroll {
public:
	Scroll() = default;

	// Return value
	const sf::Vector2f getPos() const { return position; };
	const sf::Vector2f getSize() const { return size; };

	// Update value and index	
	void updateSize(const sf::Vector2f& _size) { size = _size; updateState(); };
	void updatePos(const sf::Vector2f& _pos) { position = _pos; updateState(); };
	void udpateContent(const TextButton _content) { content.push_back(_content); updateState(); };
	void clearContent() { content.clear(); updateState(); };
	void updatePreviewSize(const int& _preview_size) { preview_size = _preview_size; updateState(); };
	void updateIndex(const int& _index) { index = _index; updateState(); };
	void updateState();

	// event handle & draw
	void eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond);
	void draw(sf::RenderWindow& window);

private:
	sf::Vector2f position, size;
	int preview_size = 0;
	int index = 0;
	float _thumb_pos;
	float last_mouse_pos_y = -1;

	int pre_index = 0;

	sf::RectangleShape container, placeholder;
	BaseButton thumb;
	std::vector<TextButton> content;
};

class BaseNotification {
public:
	// Accesible values
	sf::Color title_color = sf::Color({ 0, 0, 0 });
	sf::Color notification_color = sf::Color({ 0, 0, 0 });
	sf::Color container_color = sf::Color({ 255,204,0 });
	sf::Color background_color = sf::Color({ 0, 0, 0, 150 });

	BaseNotification(const sf::Font &_font);

	// Return value
	sf::Vector2f getPos() const { return position; };
	sf::Vector2f getSize() const { return size; };
	bool onScreen() const { return on_screen; };

	// Update value
	void updateSize(const sf::Vector2f& _size) { size = _size; updateState(); };
	void updatePos(const sf::Vector2f& _pos) { position = _pos; updateState(); };
	void updateTitleStr(const std::string& _str) { title.updateStr(_str); updateState();};
	void updateNotificationStr(const std::string& _str) { notification.updateStr(_str); updateState();};
	void updateOutlineThickness(const float& thickness) { container.setOutlineThickness(thickness); };
	void updateOutlineColor(const sf::Color& color) { container.setOutlineColor(color); };


	void clearSelection() { selection.clear(); };
	void addSelection(const TextButton& button) { selection.push_back(button); };
	
	// Update state, draw and event handle
	virtual void updateState();
	virtual void draw(sf::RenderWindow& window);
	virtual void eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond);
protected:
	sf::RectangleShape container, background;
	TextBox title, notification;
	std::vector<TextButton> selection;

	sf::Vector2f size, position;
	bool on_screen = false;
};