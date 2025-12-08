#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<algorithm>

void Scroll::draw(sf::RenderWindow& window) {
	window.draw(container);
	window.draw(placeholder);
	thumb.draw(window);

	for (int i  = index; i < index + preview_size && i < content.size(); i++) {
		content[i].draw(window);
	}
}

void Scroll::eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond) {
	std::string event_repond = "";
	for (int i = index; i < index + preview_size && i < content.size(); i++) {
		content[i].eventHandle(event, ui_cfg, respond);
	}
	thumb.eventHandle(event, ui_cfg, event_repond);

	if (container.getGlobalBounds().contains((sf::Vector2f)ui_cfg.mouse_pos)) {
		if (const auto* event_wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
			index += (event_wheel->delta > 0) ? -1 : 1;
			index = std::max(0, std::min((int)content.size() - preview_size, index));
			updateState();
		}
	}

	if (thumb.isMousePress()) {
		if (last_mouse_pos_y == -1) {
			last_mouse_pos_y = ui_cfg.mouse_pos.y;
			pre_index = index;
		}
	}

	if (event.is<sf::Event::MouseMoved>()) {
		if (last_mouse_pos_y != -1) {
			float delta_y = ui_cfg.mouse_pos.y - last_mouse_pos_y;
			float move_ratio = delta_y / _thumb_pos;
			index = pre_index + (int)move_ratio;
			index = std::max(0, std::min((int)content.size() - preview_size, index));
			updateState();
		}
	}

	if (event.is<sf::Event::MouseButtonReleased>()) {
		last_mouse_pos_y = -1;
	}
}

void Scroll::updateState() {
	container.setSize(size);
	container.setOrigin(size / 2.f);
	container.setPosition(position);

	float _ratio = ((int) content.size() - preview_size) * 1.f / preview_size;
	float _thumb_size_y = (preview_size >= content.size()) ? 1.f : size.y/std::min(7.f,(float)content.size()-preview_size+1);
	_thumb_pos = (preview_size >= content.size()) ? _thumb_size_y : (size.y - _thumb_size_y) / ((float)content.size() - preview_size);

	thumb.updateSize({ size.x / 7.f, _thumb_size_y });
	thumb.updatePos({ position.x + size.x / 2.f - thumb.getSize().x / 2.f, position.y - size.y/2.f + _thumb_size_y /2.f + index * _thumb_pos });

	placeholder.setSize({ thumb.getSize().x, size.y});
	placeholder.setOrigin(placeholder.getLocalBounds().getCenter());
	placeholder.setPosition({ position.x + size.x / 2.f - placeholder.getSize().x / 2.f, position.y});
	sf::Color placeholder_color = container.getFillColor();
	placeholder_color.a = 180;
	placeholder_color.r *= 0.7;
	placeholder_color.b *= 0.7;
	placeholder_color.g *= 0.7;
	placeholder.setFillColor(placeholder_color);

	// update content position
	for (int i = index; i < index + preview_size && i < content.size(); i++) {
		content[i].updateSize({ size.x - thumb.getSize().x, size.y / preview_size});
		content[i].updatePos({ position.x - size.x / 2.f + (content[i].getSize().x) / 2.f, position.y - size.y / 2 + (i - index) * content[i].getSize().y + content[i].getSize().y / 2.f });
		content[i].updateTextSizeFit(0.9f);
	}
}

void Scroll::deleteLastContent() {
	if (content.empty()) return;
	content.pop_back(); 
	if (index != 0 && index + preview_size > content.size()) {
		index--;
	}
	updateState();
}