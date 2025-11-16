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

    void eventHandle(const sf::Event &event, std::string& repond);

    void resize();

    void draw();
private:
	SimpleButton start_button;
	SimpleButton setting_button;
    sf::Text title;

    UICfg& ui_cfg;
    const AssetManager& asset_manager;

    float margin;

    void resize(const sf::Vector2u& window_size);  
};