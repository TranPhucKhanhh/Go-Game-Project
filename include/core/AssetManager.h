#pragma once
#include<SFML/Graphics.hpp>

class AssetManager {
public:
	bool loadFont(const std::string& name, std::string path);
	bool loadTexture(const std::string& name, std::string path);

	const sf::Font& getFont(const std::string& name) const;
	const sf::Texture& getTexture(const std::string& name) const;
private:
	std::unordered_map<std::string, sf::Font> fonts;
	std::unordered_map<std::string, sf::Texture> textures;
};