#pragma once

enum class GameState { Menu, Setting, Playing, Paused, GameOver };
enum class SoundTheme { Default, Minimal };
enum class BoardDesign { Default, Minimal };
enum class StoneDesign { Default, Minimal };
enum class Player { White, Black, AI };


class GameCfg {
public:
	GameCfg(int _w, int _h) : weight(_w), height(_h) {};
	int WindowHeight() const { return height; };
	int WindowWeight() const { return weight; };
private:
	// Window size
	int height = -1;
	int weight = -1;

	// Initial set up
	GameState state = GameState::Menu;
	int borad_size = 13;

	// Design selection
	SoundTheme sound_theme = SoundTheme::Minimal;  // temporarily set to minimal
	BoardDesign board_design = BoardDesign::Minimal;
	StoneDesign stone_design = StoneDesign::Minimal;

	// On/Off state of the game audio
	bool background_music = 1;
	bool placing_stone = 1, capturing_stone = 1, end_game_sound = 1; // Sound effect
};

class Game {
public:
	Game(GameCfg &cfg);

	// Return value

	unsigned int WindowHeight() { return cfg.WindowHeight(); }
	unsigned int WindowWeight() { return cfg.WindowWeight(); }

	// Player's input
	void pass();
	void undo();
	void redo();
	void reset();

	// Helper function
	Player getCurrentPlayer() const { return cur_player; };

private:
	Player cur_player;
	GameCfg cfg;
};