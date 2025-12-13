#include<SFML/Graphics.hpp>
#include<UI/Menu.h>
#include<UI/Component.h>
#include<model/UICfg.h>
#include<core/AssetManager.h>
#include<algorithm>
#include<iostream>


Menu::Menu(const AssetManager& _asset_manager, UICfg& ui_cfg)
    : asset_manager(_asset_manager), ui_cfg(ui_cfg),
    start_button("START", _asset_manager.getFont("Metal-Glass"), ui_cfg), 
    setting_button("SETTING", _asset_manager.getFont("Metal-Glass"), ui_cfg),
    title(_asset_manager.getFont("Spicy-Sale"), "Go Game"),
    credit(_asset_manager.getFont("RobotoSlab-Bold"))
{

    title.setFillColor(sf::Color(0, 0, 0));
    credit.updateBoxColor(sf::Color::Transparent);
    credit.updateTextColor(sf::Color::Black);
    credit.updateStr("Created by Tran Phuc Khanh and Ly Tuan Kiet");

    start_button.updateRespondStr("Start");
    start_button.updateTextSize(50);
    start_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
    start_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
    start_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
    start_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));
    
    setting_button.updateRespondStr("Setting");
    setting_button.updateTextSize(50);
    setting_button.updateTextEffectColor(sf::Color::Black, sf::Color::White, sf::Color::White);
    setting_button.updateIdleTex(asset_manager.getTexture("button_rectangle_depth_border"));
    setting_button.updateHoverTex(asset_manager.getTexture("button_rectangle_depth_gradient"));
    setting_button.updateHoldTex(asset_manager.getTexture("button_rectangle_depth_gloss"));
    
    resize();
	
    std::cerr << "Load Menu UI successfully." << std::endl;
}

void Menu::enter() {
    updateEventHandle();
    resize();
}

void Menu::eventHandle(const sf::Event &event, std::string& event_respond) {
    if (event.is<sf::Event::Resized>()) {
        resize();
    }

    std::string respond = "";
    start_button.eventHandle(event, ui_cfg, respond);
    setting_button.eventHandle(event, ui_cfg, respond);

    if (respond == "Start") {
        event_respond = "GameNewOption";
    }
    else if (respond == "Setting") {
        event_respond = "OpenSetting";
    }
}

void Menu::draw() {
    ui_cfg.window.clear(sf::Color(240, 217, 181));
    
    start_button.draw(ui_cfg.window);
    
    setting_button.draw(ui_cfg.window);
    
    ui_cfg.window.draw(title);

    credit.draw(ui_cfg.window);

}

void Menu::resize() {
    float scale = std::min(ui_cfg.window_size.x / 5.f, ui_cfg.window_size.y / 7.f * 3);
    sf::Vector2f button_size;
    button_size.x = std::min(500.f, std::max(scale, 300.f));
    button_size.y = std::min(166.f, std::max(scale / 3.f, 100.f));

    margin = std::min(150.f, std::max(ui_cfg.window_size.y / 14.f, 50.f));

    start_button.updateSize(button_size);
    start_button.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f });
    start_button.updateTextSizeFit(0.9f);

    setting_button.updateSize(button_size);
    setting_button.updatePos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f + (button_size.y + margin + button_size.y / 2.f) });
    setting_button.updateTextSizeFit(0.9f);

    title.setCharacterSize(std::min(ui_cfg.window_size.x, ui_cfg.window_size.y) / 7);

    sf::FloatRect title_bound = title.getLocalBounds();
    title.setOrigin({ title_bound.position.x + title_bound.size.x / 2.f, title_bound.position.y + title_bound.size.y / 2.f });
    title.setPosition({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y / 2.f - (button_size.y + margin + title_bound.size.y) });


    credit.updateBoxSize({ ui_cfg.window_size.x * 0.8f, ui_cfg.window_size.y / 20.f});
    credit.updateBoxPos({ ui_cfg.window_size.x / 2.f, ui_cfg.window_size.y - credit.getSize().y });
    credit.updateTextSizeFit(0.9f);
}