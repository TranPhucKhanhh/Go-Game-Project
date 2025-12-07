#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

class AssetManager {
public:
	bool loadFont(const std::string& name, std::string path);
	bool loadTexture(const std::string& name, std::string path);
	bool loadMusic(const std::string& name, std::string path);

	const sf::Font& getFont(const std::string& name) const;
	const sf::Texture& getTexture(const std::string& name) const;
	std::string getMusicPath(const std::string& name) const;
private:
	std::unordered_map<std::string, sf::Font> fonts;
	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, std::string> musics;
};