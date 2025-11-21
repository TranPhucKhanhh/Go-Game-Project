#pragma once
#include<iostream>
#include<game/InGameState.h>
#include<game/History.h>
#include<model/Move.h>
#include<model/CellState.h>
#include<model/GameState.h>

enum class SoundTheme { Default, Minimal };
enum class BoardDesign { Default, Minimal };
enum class StoneDesign { Default, Minimal };

struct GameCfg {
	// Initial set up
	GameState prev_state = GameState::Menu;
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
};

class Game {
public:
	// Return value
	GameState getGameState() const { return game_config.state; };

	// Player's input
	void start();
	void pass();
	void undo();
	void redo();
	void reset();
	void placeStone(int x, int y);
	void print(); // debug

	// Return value
	CellState getCurrentPlayer() const { return state.current_player; };
	GameCfg& getGameCfg() { return game_config; }
	Board getCurrentBoard() const { return state.current_board; };
	int getBoardSize() const { return game_config.board_size; }

	bool isGameEnd() const { return game_end; }

	std::pair<float, float> getScore(); // First value is black score and second value is white score

private:
	//Game configuration
	GameCfg game_config;

	//Current game state
	InGameState state;

	//Current game history
	GameHistory history;

	bool game_end;
};