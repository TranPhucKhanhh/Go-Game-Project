#include<Game.h>
#include<vector>

class Rule {
public:
	Rule(Game& game);

	std::vector<Move> checkCapture(const Move& move); // return the vector of "1" group of captured stones or empty if no stone captured

private:
	std::vector<Move> getGroup(const Move &current) const;
	int countStoneLiberty(const Move &move) const;
	int countGroupLibery(const std::vector<Move> &group) const;
	Game& game;
};