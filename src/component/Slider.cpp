#include<UI/Component.h>
#include<model/UICfg.h>
#include<algorithm>

static std::string slide_texture = "slide_horizontal_grey";
static std::string fill_texture = "slide_horizontal_color";
static std::string handle_texture = "slide_hangle";

Slider::Slider(const AssetManager& asset_manager) {
	
	handle.updateColor({255,255,255});
	handle.updateTexture(asset_manager.getTexture(handle_texture));
	slide.setTexture(&asset_manager.getTexture(slide_texture));
	fill.setTexture(&asset_manager.getTexture(fill_texture));

	sf::Texture tex(asset_manager.getTexture(fill_texture));
	tex_size = tex.getSize();
}

void Slider::draw(sf::RenderWindow& window) {

	if (hold) {
		value = pre_value + std::min(100.f - pre_value, std::max(-pre_value, (float)(sf::Mouse::getPosition(window).x - mouse_last_pos.x) / (size.x - handle.getSize().x) * 100.f));
		updateState();
	}
	else {
		pre_value = value;
	}

	window.draw(slide);
	window.draw(fill);
	handle.draw(window);
}

void Slider::updateState() {
	slide.setSize(size);
	fill.setSize({ (value / 100.f) * (size.x - handle.getSize().x) + handle.getSize().x, size.y });
	float _button_size = std::min(size.x, size.y) * 1.2f;
	handle.updateSize({_button_size, _button_size * 4/3});

	int _t = 39;
	sf::IntRect fullRect({ 0, 0 }, { (int)((value / 100.f) * ( tex_size.x - _t ) + _t), (int)tex_size.y });
	fill.setTextureRect(fullRect);

	slide.setOrigin(slide.getLocalBounds().getCenter());
	fill.setOrigin(fill.getLocalBounds().getCenter());

	slide.setPosition(position);
	fill.setPosition({ position.x - size.x / 2.f + fill.getSize().x / 2.f, position.y });
	handle.updatePos({position.x - size.x/2.f + (value/100.f) * ( size.x - handle.getSize().x ) + handle.getSize().x/2.f, position.y});
}

void Slider::eventHandle(const sf::Event& event, const UICfg& ui_cfg) {
	std::string tmp;
	handle.eventHandle(event, ui_cfg, tmp);
	if (!hold && handle.isMousePress()) {
		hold = true;
		mouse_last_pos = ui_cfg.mouse_pos;
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) hold = false;
}