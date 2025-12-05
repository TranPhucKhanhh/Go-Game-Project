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
}

void BaseNotification::updateState() {

	float _margin = std::min(size.x / 10.f, size.y / 10.f);
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
	title.updateBoxPos({position.x, position.y - size.y/2.f + title.getSize().y/2.f});
	title.updateTextSizeFit(1.f);
	
	float _selecttion_size_x = std::min(size.x / 3.f, (size.x - 2 * _margin - selection.size() * _inner_margin ) / (selection.size() == 0 ? 1 : selection.size()));
	for (int i = 0; i < selection.size(); i++) {
		selection[i].updateSize({ _selecttion_size_x, size.y / 5.f});
		selection[i].updatePos({position.x-size.x/2.f+_margin+i*(_selecttion_size_x+_inner_margin),position.y+size.y/2.f- size.y / 5.f - _margin});
		selection[i].updateTextSizeFit(0.9f);
	}

	notification.updateBoxPos(position + sf::Vector2f(10.f, size.y * 0.2f + 10.f));
	notification.updateBoxSize(sf::Vector2f(size.x - 20.f, size.y * 0.5f - 10.f));
	notification.updateTextSizeFit(0.7f);
}