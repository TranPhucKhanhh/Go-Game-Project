#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<algorithm>

BaseNotification::BaseNotification(const sf::Font& font) :
	title(font), notification(font) {
	title.updateBoxColor(sf::Color({ 0, 0, 0, 0 }));
	notification.updateBoxColor(sf::Color({ 0, 0, 0, 0 }));
}

void BaseNotification::draw(sf::RenderWindow& window) {
	window.draw(background);

	window.draw(container);
	title.draw(window);
	notification.draw(window);

	for (int i = 0; i < selection.size(); i++) {
		selection[i].draw(window);
	}
}

void BaseNotification::eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond) {
	for (int i = 0; i < selection.size(); i++) {
		selection[i].eventHandle(event, ui_cfg, respond);
	}
	if (respond == "CloseNoti") BaseNotification::updateOnScreen(false);
}

void BaseNotification::updateState() {

	float _margin = std::min(size.x / 7.f, size.y / 7.f);
	float _inner_margin = _margin / 2.f;

	container.setFillColor(container_color);
	background.setFillColor(background_color);
	title.updateTextColor(title_color);
	notification.updateTextColor(notification_color);

	container.setSize(size);
	container.setOrigin(container.getLocalBounds().getCenter());
	container.setPosition(position);

	background.setSize({30000.f,30000.f});
	background.setPosition({0.f,0.f});

	title.updateBoxSize({size.x, size.y / 5.f });
	title.updateBoxPos({position.x, position.y - size.y/2.f + title.getSize().y/2.f + _inner_margin / 2.f});
	title.updateTextSizeFit(1.f);
	
	float _tmp = (selection.size() == 0 ? 1 : selection.size());
	_selection_size = { std::min(size.x / 3.f, (size.x - 2 * _margin - (_tmp - 1) * _inner_margin) / _tmp), title.getSize().y };
	float _delta_pos_x = (size.x - _selection_size.x * _tmp - (_tmp-1) * _inner_margin) / 2.f + _selection_size.x / 2.f;
	for (int i = 0; i < selection.size(); i++) {
		selection[i].updateSize(_selection_size);
		selection[i].updatePos({position.x-size.x/2.f+ _delta_pos_x +i*(_selection_size.x+_inner_margin),position.y+size.y/2.f- selection[i].getSize().y/2.f - _inner_margin});
		selection[i].updateTextSizeFit(0.9f);
	}

	notification.updateBoxSize({size.x - _margin, (size.y - 2 * _margin - title.getSize().y - _selection_size.y) / 2.f });
	notification.updateBoxPos({position.x, title.getPos().y + title.getSize().y / 2.f + _inner_margin + notification.getSize().y/2.f});
	notification.updateTextSizeFitWithScale(0.9f);
}

TextInputNotification::TextInputNotification(const sf::Font& font) : BaseNotification(font), input_box(font) {
	input_box.updateOutlineColor(sf::Color::Black);
}

void TextInputNotification::draw(sf::RenderWindow& window) {
	BaseNotification::draw(window);
	input_box.draw(window);
}

void TextInputNotification::updateState() {
	BaseNotification::updateState();

	float _margin = std::min(size.x / 7.f, size.y / 7.f);
	float _inner_margin = _margin / 2.f;

	input_box.updateBoxColor(input_box_color);
	input_box.updateTextColor(input_text_color);
	input_box.updateBoxSize(notification.getSize());
	input_box.updateBoxPos({ position.x, notification.getPos().y + notification.getSize().y / 2.f + input_box.getSize().y / 2.f + _inner_margin});
}

void TextInputNotification::eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond) {
	BaseNotification::eventHandle(event, ui_cfg, respond);
	if (input_focus) {
		if (const auto *key = event.getIf<sf::Event::TextEntered>()) {
			char32_t unicode = key->unicode;

			// handle Backspace
			if (unicode == 8) { 
				if (!input_str.empty()) {
					input_str.pop_back();
				}
			}
			// handle printable characters
			else if (unicode >= 32 && unicode <= 126) {
				if (input_str.size() < max_input_length) {
					input_str += static_cast<char>(unicode);
				}
			}
			input_box.updateStr(input_str);
			input_box.updateTextSizeFitWithScale(0.9f);
		}
	}

	if (mouse_release) {

		if (event.is<sf::Event::MouseButtonPressed>()) {
			mouse_release = false;
			if (input_box.containPoint((sf::Vector2f)ui_cfg.mouse_pos)) {
				input_box.updateOutlineThickness(3.f);
				input_focus = true;
			}
			else {
				input_box.updateOutlineThickness(0);
				input_focus = false;
			}
		}
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		mouse_release = true;
	}
}