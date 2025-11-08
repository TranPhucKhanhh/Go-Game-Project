#pragma once
#include<vector>
#include<iostream>

enum class GameState { Menu, Setting, Playing, Paused, GameOver };
enum class SoundTheme { Default, Minimal };
enum class BoardDesign { Default, Minimal };
enum class StoneDesign { Default, Minimal };
enum class Player { White, Black };
enum class Position { Invalid, Empty, Black, White };

struct Move {
	int x, y;
	Position player;

	Move() {};
	Move(int _x, int _y, Position _p) : x(_x), y(_y), player(_p) {}
};

namespace {				// using namspace without name to avoid other files accessing this struct
	struct GameCfg {
		// Window size
		int height = -1;
		int weight = -1;

		// Initial set up
		GameState state = GameState::Menu;
		int board_size = 13;

		// Design selection
		SoundTheme sound_theme = SoundTheme::Minimal;  // temporarily set to minimal
		BoardDesign board_design = BoardDesign::Minimal;
		StoneDesign stone_design = StoneDesign::Minimal;

		// On/Off state of the game audio
		bool background_music = 1;
		bool placing_stone = 1, capturing_stone = 1, end_game_sound = 1; // Sound effect

		GameCfg(int _w, int _h) : weight(_w), height(_h) {}
	};
}


class Game {
public:
	Game(int _weight, int _height);

	// Return value
	unsigned int windowHeight() const { return game_config.height; };
	unsigned int windowWeight() const { return game_config.weight; };
	GameState getGameState() const { return game_config.state; };

	Position currPos(int x, int y) const {
		if (x >= 1 && x <= game_config.board_size && y >= 1 && y <= game_config.board_size) return board[x][y];
		std::cerr << "The current position is out of board x: " << x << " y: " << y << std::endl;
		return Position::Invalid;
	};

	// Player's input
	void pass();
	void undo();
	void redo();
	void reset();

	// Helper function
	Player getCurrentPlayer() const { return cur_player; };

private:
	Player cur_player;
	GameCfg game_config;
	std::vector< std::vector<Position> > board;
};