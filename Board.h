#include <SFML/Graphics.hpp>
#include "Square.h"
#include <iostream>
using namespace sf;
using namespace std;

class Board {
private:
	Square all_squares[8][8];
public:
	Board() {}
	Square& get_all_squares(int _i, int _j) {
		return this->all_squares[_i][_j];
	}

	void draw_board(RenderWindow& _window) {
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				all_squares[i - 1][j - 1].set_position(i * 50.0, j * 50);
				if ((i + j) % 2 == 1) all_squares[i - 1][j - 1].set_color(Color(255, 178, 115));
				else all_squares[i - 1][j - 1].set_color(Color(101, 61, 16));
				_window.draw(all_squares[i - 1][j - 1].get_square());
			}
		}
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				if (all_squares[i - 1][j - 1].get_backlight() == 1) {
					RectangleShape square_light(Vector2f(50, 50));
					square_light.setPosition(i * 50, j * 50);
					all_squares[i - 1][j - 1].set_color(Color(150, 120, 90));
				}
				_window.draw(all_squares[i - 1][j - 1].get_square());
			}
		}
		
	}
};