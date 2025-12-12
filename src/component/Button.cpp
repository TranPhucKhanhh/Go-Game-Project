#include<UI/Component.h>
#include<UI/UI.h>
#include<iostream>

// BaseButton function definition

void BaseButton::checkHover(const sf::Vector2i& mouse_pos) {
	sf::FloatRect shape_bound = button.getGlobalBounds();

	if (shape_bound.contains((sf::Vector2f)mouse_pos)) {
		if (hovered == false) {
			ui_cfg.button_hover_sound.play();
		}
		hovered = true;
	}
	else {
		hovered = false;
	}
}

void BaseButton::updateEffect() {
	if (mouse_hold && hovered) {
		onMouseHold();
	}
	else if (hovered) {
		onHover();
	}
	else {
		onIdle();
	}
}

void BaseButton::onIdle() {
	button.setFillColor(button_color);
}

void BaseButton::onHover() {
	sf::Color _tmp = button_color;
	_tmp.r *= 0.7;
	_tmp.b *= 0.7;
	_tmp.g *= 0.7;
	button.setFillColor(_tmp);
}

void BaseButton::onMouseHold() {
	sf::Color _tmp = button_color;
	_tmp.r *= 0.5;
	_tmp.b *= 0.5;
	_tmp.g *= 0.5;
	button.setFillColor(_tmp);
}

void BaseButton::draw(sf::RenderWindow& window) {
	updateEffect();
	window.draw(button);
}

void BaseButton::updateState() {
	button.setSize(size);
	button.setOrigin(size / 2.f);
	button.setPosition(position);
}

void BaseButton::eventHandle(const sf::Event& event, const UICfg& _ui_cfg, std::string& respond) {
	checkHover(_ui_cfg.mouse_pos);
	if (const auto* button = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (button->button == sf::Mouse::Button::Left)
			mouse_hold = true;
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		if (hovered && mouse_hold) {
			respond = on_click_respond;
			ui_cfg.button_click_sound.play();
		}
		mouse_hold = false;
	}
}

// Thumbutton function definition
void ThumbButton::eventHandle(const sf::Event& event, const UICfg& ui_cfg, std::string& respond) {
	checkHover(ui_cfg.mouse_pos);
	if (const auto* button = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (button->button == sf::Mouse::Button::Left ) 
			if (hovered) on_hold = mouse_hold = true;
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		on_hold = false;
		mouse_hold = false;
	}
}

void ThumbButton::updateEffect() {
	if (on_hold) {
		onMouseHold();
	}
	else if (hovered) {
		onHover();
	}
	else {
		onIdle();
	}
}

// TextureEffectButton function definition

void TextureEffectButton::onIdle() {
	button.setTexture(&idle_tex);
	text.setFillColor(text_idle_color);
}

void TextureEffectButton::onHover() {
	button.setTexture(&hover_tex);
	text.setFillColor(text_hover_color);
}

void TextureEffectButton::onMouseHold() {
	button.setTexture(&hold_tex);
	text.setFillColor(text_hold_color);
}

void TextureEffectButton::updateEffect() {
	if ((mouse_hold && hovered) || force_tex == 2) {
		onMouseHold();
	}

	else if (hovered || force_tex == 1) {
		onHover();
	}
	else {
		onIdle();
	}
}

// TextButton function definition

void TextButton::updateTextSizeFit(const float _ratio) {
	sf::Vector2f _size = button.getLocalBounds().size;
	text.setCharacterSize(_size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = std::min(1.f, _size.x / bound.size.x) * bound.size.y * _ratio;

	updateTextSize(ratio);
}

float TextButton::getTextSizeFit(const float _ratio) {
	sf::Vector2f _size = button.getLocalBounds().size;
	text.setCharacterSize(_size.y);
	sf::FloatRect bound = text.getLocalBounds();
	float ratio = std::min(1.f, _size.x / bound.size.x) * bound.size.y * _ratio;

	return ratio;
}

TextButton::TextButton(const std::string& str, const sf::Font& font, UICfg& _ui_cfg)
	: BaseButton(_ui_cfg)
	, text(font, str) {

}

void TextButton::draw(sf::RenderWindow& window) {
	updateEffect();
	window.draw(button);
	window.draw(text);
}

void TextButton::updateState() {
	button.setSize(size);
	button.setOrigin(size / 2.f);
	button.setPosition(position);

	text.setCharacterSize(text_size);
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(position);
}