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

bool AssetManager::loadSound(const std::string& name, std::string path) {
	path = std::string(ASSET_DIR) + path;
	sf::SoundBuffer sound;
	if (!sound.loadFromFile(path)) {
		std::cerr << "Can not find Sound " << name << " from path " << path << std::endl;
		assert(0);
	}
	sounds[name] = std::move(sound);;
	return true;
}

const sf::Font& AssetManager::getFont(const std::string& name) const {
	return fonts.at(name);
}

const sf::Texture& AssetManager::getTexture(const std::string& name) const {
	if (textures.find(name) == textures.end()) {
		std::cerr << "CANT FIND " << name << std::endl;
		return textures.at("dummy");
	}
	return textures.at(name);
}

std::string AssetManager::getMusicPath(const std::string& name) const {
	return musics.at(name);
}

const sf::SoundBuffer& AssetManager::getSound(const std::string& name) const {
	return sounds.at(name);
}

void AssetManager::loadFontFromFolder(const std::string& _folder) {
	std::string path = std::string(ASSET_DIR) + "/" + _folder;
	if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
		std::cerr << "Error: Folder '" << path << "' does not exist or is not a directory." << std::endl;
		return;
	}	

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string file_path = "/" + _folder + "/" + entry.path().filename().string();
			AssetManager::loadFont(entry.path().filename().stem().string(), file_path);
		}
	}
}

void AssetManager::loadTextureFromFolder(const std::string &_folder) {
	std::string path = std::string(ASSET_DIR) + "/" + _folder;
	if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
		std::cerr << "Error: Folder '" << path << "' does not exist or is not a directory." << std::endl;
		return;
	}

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string file_path = "/" + _folder + "/" + entry.path().filename().string();
			AssetManager::loadTexture(entry.path().filename().stem().string(), file_path);
		}
	}
}

void AssetManager::loadMusicFromFolder(const std::string& _folder) {
	std::string path = std::string(ASSET_DIR) + "/" + _folder;
	if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
		std::cerr << "Error: Folder '" << path << "' does not exist or is not a directory." << std::endl;
		return;
	}

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string file_path = "/" + _folder + "/"  + entry.path().filename().string();
			AssetManager::loadMusic(entry.path().filename().stem().string(), file_path);
		}
	}
}

void AssetManager::loadSoundFromFolder(const std::string& _folder) {
	std::string path = std::string(ASSET_DIR) + "/" + _folder;
	if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
		std::cerr << "Error: Folder '" << path << "' does not exist or is not a directory." << std::endl;
		return;
	}

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string file_path = "/" + _folder + "/" + entry.path().filename().string();
			AssetManager::loadSound(entry.path().filename().stem().string(), file_path);
		}
	}
}