#pragma once

#include<SFML/Graphics.hpp>
#include<core/AssetManager.h>
#include<model/UICfg.h>

class Loading {
public:
    Loading(const AssetManager& _asset_manager, UICfg& _ui_cfg);

    void run();

private:
    UICfg& ui_cfg;
    const AssetManager& asset_manager;
};