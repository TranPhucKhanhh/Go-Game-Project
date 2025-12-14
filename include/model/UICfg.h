#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<core/AssetManager.h>

enum class SoundTheme { Default, Minimal };
const std::string BoardDesign[] = { "White color", "Cyan color", "Green color", "Black color", "Yellow color", "Anime", "Wooden", "Forest"};
const std::string StoneDesign[] = { "Default", "Fruit", "Animal1", "Animal2", "Animal3"};
const std::string SoundEffect[] = {"123", "123"};
const std::string Music[] = { "Puzzle-Dreams", "The-Spunky-Princess"};

struct UICfg {
    sf::Vector2u window_size;
    sf::Vector2u min_window_size;

    sf::Vector2i mouse_pos;
    std::string title;
    
    sf::RenderWindow& window;
    bool AI_in_turn = false;

	// Adio & Theme settings
    SoundTheme sound_theme = SoundTheme::Minimal;  // temporarily set to minimal
    std::string board_design = "White color";
    std::string stone_design = "Default";

    // Volumn state of the game audio
    float background_music_volume = 0, pre_background_music_volume = 0;
    float sound_effect_volume = 25, pre_sound_effect_volume = 0;

    std::string music_name = "The-Spunky-Princess";
    std::string sound_effect_name = "";
    sf::Music music;
    sf::Sound button_click_sound, button_hover_sound;
    sf::Sound stone_place_sound, stone_error_sound, stone_capture_sound;

    UICfg(sf::Vector2u _window_size, sf::Vector2u _min_window_size, sf::RenderWindow& _window, const AssetManager &asset_manager) : 
        window_size(_window_size), min_window_size(_min_window_size), window(_window),
        button_click_sound(asset_manager.getSound("click-a")),
        button_hover_sound(asset_manager.getSound("tap-a")),
        stone_place_sound(asset_manager.getSound("Default-stone-place")),
        stone_error_sound(asset_manager.getSound("Default-stone-place-no")),
        stone_capture_sound(asset_manager.getSound("Default-stone-capture")) {
        music.setVolume(background_music_volume);
        updateSoundEffectVolume(sound_effect_volume);
    }

    void updateMusic(const AssetManager& asset_manager) {
        std::string _path = asset_manager.getMusicPath(music_name);
        if (!music.openFromFile(_path)) {
            std::cerr << "Failed to load music: " << music_name << " from path " << _path << std::endl;
        }
        music.play();
        music.setLooping(true);
		music.setVolume(background_music_volume);
    }

    void updateSoundEffectVolume(float volume) {
        button_click_sound.setVolume(volume);
        button_hover_sound.setVolume(volume);
        stone_place_sound.setVolume(volume);
        stone_error_sound.setVolume(volume);
        stone_capture_sound.setVolume(volume);
    }

    void updateSoundEffect(const AssetManager& asset_manager) {
        std::cerr << "Set sound effect to " << sound_effect_name << std::endl;
    }
};