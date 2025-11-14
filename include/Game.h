#pragma once
#include<iostream>
#include<InGameState.h>
#include<History.h>
#include<Move.h>
#include<CellState.h>
#include<GameState.h>

enum class SoundTheme { Default, Minimal };
enum class BoardDesign { Default, Minimal };
enum class StoneDesign { Default, Minimal };

namespace {				// using namspace without name to avoid other files accessing this struct
	struct GameCfg {
		// Window size
		int height = -1;
		int weight = -1;

		// Initial set up
		GameState state = GameState::Menu;
		int board_size = 13;
		int komi = 7;

		// Design selection
		SoundTheme sound_theme = SoundTheme::Minimal;  // temporarily set to minimal
		BoardDesign board_design = BoardDesign::Minimal;
		StoneDesign stone_design = StoneDesign::Minimal;

		// On/Off state of the game audio
		bool background_music = 1;
		bool placing_stone = 1, capturing_stone = 1, end_game_sound = 1; // Sound effect

		GameCfg(int w, int h) : weight(w), height(h) {}
	};
}

class Game {
public:
	//Initial state for the game
	Game(int _weight, int _height);

	// Return value
	unsigned int windowHeight() const { return game_config.height; };
	unsigned int windowWeight() const { return game_config.weight; };
	GameState getGameState() const { return game_config.state; };

	CellState currPos(int x, int y) const {
		if (x >= 1 && x <= game_config.board_size && y >= 1 && y <= game_config.board_size) return state.current_board[x][y];
		std::cerr << "The current position is out of board x: " << x << " y: " << y << std::endl;
		return CellState::Invalid;
	};

	// Player's input
	void start();
	void pass();
	void undo();
	void redo();
	void reset();
	void placeStone(int x, int y);
	void end();
	void print();

	// Helper function
	CellState getCurrentPlayer() const { return state.current_player; };

private:
	//Game configuration
	GameCfg game_config;

	//Current game state
	InGameState state;

	//Current game history
	GameHistory history;
};