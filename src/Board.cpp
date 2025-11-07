#include<Board.h> 
#include<Game.h>
#include<vector>

static int x_jump[4] = { 0, 0, 1, -1 };
static int y_jump[4] = { 1, -1, 0, 0 };

Rule::Rule(Game& game) : game(game) {

}

std::vector<Move> Rule::CheckCapture(const Move &current) {
	 Position ally = current.player;
	 Position enemy = (current.player == Position::White) ? Position::Black : Position::White;

	 // Idea: check around the current stone 
	 // If the near stones is enemy -> count number of liberties of the whole enemy group
	 for (int i = 0; i < 4; i++) {
		 int x = current.x + x_jump[i];
		 int y = current.y + y_jump[i];

		 if (game.CurrPos(x, y) == enemy) {
			 std::vector<Move> enemy_group = GetGroup(Move(x, y, enemy));
			 if (CountGroupLibery(enemy_group) == 0) {
				 return enemy_group;
			 }
		 }
	 }
	 
}

static void DfsGetGroup(const Move &current, std::vector<Move> &vec, const Game& game) {
	for (int i = 0; i < 4; i++) {
		int x = current.x  + x_jump[i];
		int y = current.y  + y_jump[i];

		if (game.CurrPos(x, y) == Position::Empty) {
			vec.push_back(Move(x, y, current.player));
		}
		else if (game.CurrPos(x, y) == current.player) {
			DfsGetGroup(Move(x, y, current.player), vec, game);
		}
	}
}

std::vector<Move> Rule::GetGroup(const Move &current) const {
	std::vector<Move> _tmp;
	DfsGetGroup(current, _tmp, game);
	return _tmp;
}