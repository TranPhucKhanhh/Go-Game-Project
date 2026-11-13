#include<UI/Component.h>
#include<UI/TextSizing.h>

void TextBox::draw(sf::RenderWindow& window) const {
	window.draw(box);
	window.draw(text);
}

void TextBox::update() {
	box.setOrigin(box.getSize() / 2.f);
	box.setPosition(position);
	text.setScale({ 1.f, 1.f });
	if (auto_fit_text && scale_text_to_fit) {
		text.setCharacterSize(64);
		const float scale = ui_detail::textScaleToFit(text, box.getSize(), text_fit_ratio);
		text.setScale({ scale, scale });
	}
	else {
		const unsigned int fitted = ui_detail::fittedCharacterSize(text, box.getSize(), text_fit_ratio);
		text.setCharacterSize(auto_fit_text ? fitted : std::min(manual_text_size, fitted));
	}

	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(position);
}

void TextBox::updateTextSizeFit(const float _ratio) {
	text_fit_ratio = _ratio;
	auto_fit_text = true;
	scale_text_to_fit = false;
	update();
}

float TextBox::getTextSizeFit(const float _ratio) {
	return static_cast<float>(ui_detail::fittedCharacterSize(text, box.getSize(), _ratio));
}

void TextBox::updateTextSizeFitWithScale(const float _ratio) {
    text_fit_ratio = _ratio;
    auto_fit_text = true;
    scale_text_to_fit = true;
    update();
}
