#include<Game.h>
#include<vector>

class Rule {
public:
	Rule(Game& game);

	std::vector<Move> CheckCapture(const Move& move); // return the vector of "1" group of captured stones or empty if no stone captured

private:
	std::vector<Move> GetGroup(const Move &current) const;
	int CountGroupLibery(std::vector<Move>);
	Game& game;
};