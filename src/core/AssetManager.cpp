#include<core/AssetManager.h>
#include<iostream>

bool AssetManager::loadFont(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::Font font;
	if (!font.openFromFile(path)) {
		std::cerr << "Can not find Font " << name << " from path " << path << std::endl;
		assert(0);
	}
	fonts[name] = std::move(font);
	return true;
}

bool AssetManager::loadTexture(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::Texture texture;
	if (!texture.loadFromFile(path)) {
		std::cerr << "Can not find Texture " << name << " from path " << path << std::endl;
		assert(0);
	}
	texture.setSmooth(true);
	textures[name] = std::move(texture);
	return true;
}

bool AssetManager::loadMusic(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::Music music;
	if (!music.openFromFile(path)) {
		std::cerr << "Can not find Music " << name << " from path " << path << std::endl;
		assert(0);
	}
	musics[name] = path;
	return true;
}

const sf::Font& AssetManager::getFont(const std::string& name) const {
	return fonts.at(name);
}

const sf::Texture& AssetManager::getTexture(const std::string& name) const {
	return textures.at(name);
}

std::string AssetManager::getMusicPath(const std::string& name) const {
	return musics.at(name);
}