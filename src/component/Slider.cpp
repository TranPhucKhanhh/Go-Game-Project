#include<UI/Component.h>
#include<model/UICfg.h>

void Slider::draw(sf::RenderWindow& window) {

	if (hold) {
		if (direction == "Right") value = pre_value + std::min(100.f - pre_value, std::max(-pre_value, (float)(sf::Mouse::getPosition(window).x - mouse_last_pos.x) / size.x * 100.f));
		else if (direction == "Left") value = pre_value +  std::min(100.f - pre_value, std::max(-pre_value, (float)(mouse_last_pos.x - sf::Mouse::getPosition(window).x) / size.x * 100.f));
		else if (direction == "Down") value = pre_value + std::min(100.f - pre_value, std::max(-pre_value, (float)(sf::Mouse::getPosition(window).y - mouse_last_pos.y) / size.y * 100.f));
		else value = pre_value +  std::min(100.f - pre_value, std::max(-pre_value, (float)(mouse_last_pos.y - sf::Mouse::getPosition(window).y) / size.y * 100.f));

		updateState();
	}
	else {
		pre_value = value;
	}

	window.draw(slide);
	window.draw(fill);
	window.draw(handle);
}

void Slider::updateState() {
	slide.setFillColor(slide_color);
	fill.setFillColor(fill_color);
	handle.setFillColor(handle_color);

	slide.setSize(size);
	if (direction == "Right" || direction == "Left") fill.setSize({ size.x * (value / 100.f), size.y });
	else fill.setSize({ size.x, size.y * (value / 100.f) });
	handle.setRadius(std::min(size.x, size.y) * 1.2f);
	

	slide.setOrigin(slide.getLocalBounds().getCenter());
	handle.setOrigin(handle.getLocalBounds().getCenter());
	fill.setOrigin(fill.getLocalBounds().getCenter());

	slide.setPosition(position);

	if (direction == "Down") {
		fill.setPosition({ position.x, position.y - size.y / 2.f + fill.getSize().y / 2.f });
		handle.setPosition({ position.x, position.y - size.y / 2.f + size.y * (value / 100.f)});
	}
	else if (direction == "Up") {
		fill.setPosition({ position.x, position.y + size.y / 2.f - fill.getSize().y / 2.f });
		handle.setPosition({ position.x, position.y + size.y / 2.f - size.y * (value / 100.f) });
	}
	else if (direction == "Left") {
		fill.setPosition({ position.x + size.x / 2.f - fill.getSize().x / 2.f, position.y });
		handle.setPosition({ position.x + size.x / 2.f - size.x * (value / 100.f) , position.y});
	}
	else {
		fill.setPosition({ position.x - size.x / 2.f + fill.getSize().x / 2.f, position.y });
		handle.setPosition({ position.x - size.x / 2.f + size.x * (value / 100.f) , position.y });
	}
}

void Slider::eventHandle(const sf::Event& event, const UICfg& ui_cfg) {
	if (event.is<sf::Event::MouseButtonPressed>()) {
		if (hover) {
			hold = true;
			if (mouse_last_pos.x == -1) {
				mouse_last_pos = ui_cfg.mouse_pos;
			}
		}
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		hold = false;
		mouse_last_pos.x = -1;
	}
	else if (event.is<sf::Event::MouseMoved>()) {
		sf::FloatRect handle_bound = handle.getGlobalBounds();
		if (handle_bound.contains((sf::Vector2f) ui_cfg.mouse_pos)) hover = true;
		else if(!hold) hover = false;
	}
}