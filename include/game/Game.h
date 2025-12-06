#pragma once
#include<iostream>
#include<game/InGameState.h>
#include<game/History.h>
#include<model/Move.h>
#include<model/CellState.h>
#include<model/GameState.h>
#include<JSON/json.hpp>
#include<string.h>

using json = nlohmann::json;

enum class SoundTheme { Default, Minimal };
enum class BoardDesign { Default, Minimal };
enum class StoneDesign { Default, Minimal };

struct GameCfg {
	// Initial set up
	GameState prev_state = GameState::Menu;
	GameState state = GameState::Menu;
	GameMode game_mode = GameMode::PvP;
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

void to_json(json& j, const Move& move);
void to_json(json& j, const GameCfg& config);

void from_json(const json& j, Move& move);
void from_json(const json& j, GameCfg& config);

class Game {
	//Global JSON
	friend void to_json(json& j, const Move& move);
	friend void to_json(json& j, const GameCfg& config);

	friend void from_json(const json& j, Move& move);
	friend void from_json(const json& j, GameCfg& config);
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
	bool saveGame(const std::string& name);
	bool loadPreviewGame(const std::string& name);
	bool loadGame(const std::string& name);
	bool deleteSaveGame(const std::string& name);
	std::vector<std::string> loadAllSaveGameName();
	int getMoveListSize();
	Board getKthBoard(const int k);
	std::string getLastMove();
	void print(); // debug

	// Return value
	CellState getCurrentPlayer() const { return state.current_player; };
	GameCfg& getGameCfg() { return game_config; }
	Board getPreviewBoard() const { return previewBoard; };
	Board getCurrentBoard() const { return state.current_board; };
	int getBoardSize() const { return game_config.board_size; }
	std::vector<std::string> getMoveList() { return history.getFullMove(game_config.board_size); }

	bool isGameEnd() const { return game_end; }

	std::pair<float, float> getScore(); // First value is black score and second value is white score

private:
	//Game configuration
	GameCfg game_config;

	//Current game state
	InGameState state;

	//Current game history
	GameHistory history;

	Board previewBoard;

	bool game_end;
};