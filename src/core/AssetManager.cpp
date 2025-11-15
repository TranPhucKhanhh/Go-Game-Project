#include<core/AssetManager.h>

bool AssetManager::loadFont(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::Font font;
	font.openFromFile(path);
	fonts[name] = std::move(font);
	return true;
}

bool AssetManager::loadTexture(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::Texture texture;
	texture.loadFromFile(path);
	textures[name] = std::move(texture);
	return true;
}

const sf::Font& AssetManager::getFont(const std::string& name) const {
	return fonts.at(name);
}

const sf::Texture& AssetManager::getTexture(const std::string& name) const {
	return textures.at(name);
}