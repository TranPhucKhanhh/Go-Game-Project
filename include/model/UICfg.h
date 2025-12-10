#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>

enum class SoundTheme { Default, Minimal };
const std::string BoardDesign[] = { "White color", "Cyan color", "Green color", "Black color", "Yellow color" };
const std::string StoneDesign[] = { "Default", "Fruit", "Animal1", "Animal2", "Animal3"};

struct UICfg {
    sf::Vector2u window_size;
    sf::Vector2u min_window_size;

    sf::Vector2i mouse_pos;
    std::string title;
    
    sf::RenderWindow& window;


	// Adio & Theme settings
    SoundTheme sound_theme = SoundTheme::Minimal;  // temporarily set to minimal
    std::string board_design = "White color";
    std::string stone_design = "Default";

    // Volumn state of the game audio
    float background_music_volume = 0, pre_background_music_volume = 0;
    float sound_effect_volume = 25, pre_sound_effect_volume = 0;

    std::string music_name = "The-Spunky-Princess";
    sf::Music music;

    UICfg(sf::Vector2u _window_size, sf::Vector2u _min_window_size, sf::RenderWindow& _window) : 
        window_size(_window_size), min_window_size(_min_window_size), window(_window) {
    }

    void updateMusic(const std::string &_path) {
        if (!music.openFromFile(_path)) {
            std::cerr << "Failed to load music: " << music_name << " from path " << _path << std::endl;
        }
        music.play();
        music.setLooping(true);
		music.setVolume(background_music_volume);
    }

    void updateSoundEffectVolume(float volume) {
		std::cout << "Sound effect volume updated to: " << volume << std::endl;
    }
};