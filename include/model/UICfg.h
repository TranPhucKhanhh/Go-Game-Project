#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>

enum class SoundTheme { Default, Minimal };
enum class BoardDesign { Default, Minimal };
enum class StoneDesign { Default, Minimal };

struct UICfg {
    sf::Vector2u window_size;
    sf::Vector2u min_window_size;

    sf::Vector2i mouse_pos;
    std::string title;

    sf::RenderWindow& window;


	// Adio & Theme settings
    SoundTheme sound_theme = SoundTheme::Minimal;  // temporarily set to minimal
    BoardDesign board_design = BoardDesign::Minimal;
    StoneDesign stone_design = StoneDesign::Minimal;

    // Volumn state of the game audio
    float background_music_volume = 25;
    float sound_effect_volume = 25; // Sound effect

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