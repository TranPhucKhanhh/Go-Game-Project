#pragma once
#include<SFML/Graphics.hpp>
#include<UI/Component.h>
#include<game/Game.h>
#include<core/AssetManager.h>
#include<model/UICfg.h>
#include<vector>
#include<string>

class Menu {
public:
    Menu(const AssetManager& asset_manager, UICfg& ui_cfg);

    void enter();

    void eventHandle(const sf::Event &event, std::string& repond);

    void resize();

    void draw();
private:
	TextureEffectButton start_button, setting_button;
    sf::Text title;

    UICfg& ui_cfg;
    const AssetManager& asset_manager;

    float margin;

    // Special function to prevent bug in event handling
    void updateEventHandle() {
        sf::Event _d = sf::Event::Closed{};
        std::string _dt = "Test";
        eventHandle(_d, _dt);
    }
};