#include <SFML/Graphics.hpp>
#include <vector>
#include "Shashka.h"
#include "Board.h"
using namespace sf;

class ShashkyOnTable
{
private:
	vector <Shashka> checkers;
	Board board;
public:
	Board& get_board() {
		return this->board;
	}

	Shashka& get_checker(int _i) {
		return this->checkers[_i];
	}

	int get_size() {
		return this->checkers.size();
	}

	void start_game() {
		for (int i = 0; i < 24; i++) {
			if (i < 4) {
				checkers.push_back(Shashka((float)i * 100 + 107, (float)57, 1));
				board.get_all_squares(i * 2 + 1, 0).square_employment(1);
			}
			else if (i >= 4 && i < 8) {
				checkers.push_back(Shashka((float)(i - 4) * 100 + 57, (float)107, 1));
				board.get_all_squares((i - 4) * 2, 1).square_employment(1);
			}
			else if (i >= 8 && i < 12) {
				checkers.push_back(Shashka((float)(i - 8) * 100 + 107, (float)157, 1));
				board.get_all_squares((i - 8) * 2 + 1, 2).square_employment(1);
			}

			else if (i >= 12 && i < 16) {
				checkers.push_back(Shashka((float)(i - 12) * 100 + 57, (float)307, 0));
				board.get_all_squares((i - 12) * 2, 5).square_employment(0);
			}
			else if (i >= 16 && i < 20) {
				checkers.push_back(Shashka((float)(i - 16) * 100 + 107, (float)357, 0));
				board.get_all_squares((i - 16) * 2 + 1, 6).square_employment(0);
			}
			else if (i >= 20 && i < 24) {
				checkers.push_back(Shashka((float)(i - 20) * 100 + 57, (float)407, 0));
				board.get_all_squares((i - 20) * 2, 7).square_employment(0);
			}
			
		}
	}

	void delete_checker() {
		checkers.pop_back();
	}

	ShashkyOnTable() {
		start_game();
	};

	void draw_checkers(RenderWindow& _window) {
		board.draw_board(_window);
		for (int i = 0; i < checkers.size(); i++) {
			checkers[i].draw_checker(_window);
		}
	}

	void delete_backlight() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				this->board.get_all_squares(i, j).off_backlight();
			}
		}
	}
};

