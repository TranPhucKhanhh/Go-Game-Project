#include<UI/Loading.h>
#include<UI/Component.h>
#include<core/AssetManager.h>

Loading::Loading(const AssetManager& _asset_manager, UICfg& _ui_cfg) :
	asset_manager(_asset_manager), ui_cfg(_ui_cfg) {
	Loading::run();
}

void Loading::run() {
	ui_cfg.window.clear(sf::Color(240,228,145));
	TextBox loading_text(asset_manager.getFont("Momo"));
	loading_text.updateStr("Loading...");
	loading_text.updateBoxSize({ ui_cfg.window_size.x / 3.f, ui_cfg.window_size.y / 3.f });
	loading_text.updateBoxPos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });
	loading_text.updateBoxColor(sf::Color(240, 228, 145));
	loading_text.updateTextColor(sf::Color::Black);
	loading_text.updateTextSizeFit(1.f);
	loading_text.draw(ui_cfg.window);
	ui_cfg.window.display();
	std::cerr << "Loading UI displayed." << std::endl;
}