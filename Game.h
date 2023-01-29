#include <SFML/Graphics.hpp>
#include "ShashkyOnTable.h"
using namespace sf;

class Game {
private:
	int how_many = 0;
	bool who_can_move = 0;
	bool select_is_made = 0;
	int choise_chacker;
	float x;
	float y;
	Vector2i mause_position;
	ShashkyOnTable checkers_on_board;
	vector <int> who_must_eat;
public:
	ShashkyOnTable& get_checkers_on_board() {
		return this->checkers_on_board;
	}
	void set_mause_position(RenderWindow& _window) {
		this->mause_position = Mouse::getPosition(_window);
	}
	Vector2i centre_on_square() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (mause_position.x < i * 50 + 100 && mause_position.x < (i + 1) * 50 + 100 &&
					mause_position.y < j * 50 + 100 && mause_position.y < (j + 1) * 50 + 100) {
					return Vector2i(i * 50 + 57, j * 50 + 57);
				}
			}
		}
	}

	void choise_of_chacker(RenderWindow& _window, Event _event) {
		if (_event.type == sf::Event::MouseButtonPressed) {
			if (_event.key.code == Mouse::Left) {
				set_mause_position(_window);
				if (_event.type == sf::Event::MouseButtonPressed) {//для отмены взятия фигуры
					if (_event.key.code == Mouse::Left) {
						if (checkers_on_board.get_checker(choise_chacker).get_select() == 1 && select_is_made == 1) {
							checkers_on_board.get_checker(choise_chacker).leave_checker();
							select_is_made = 0;
							checkers_on_board.delete_backlight();
							return;
						}
					}
				}
				for (int i = 0; i < checkers_on_board.get_size(); i++) {
					if (this->mause_position.x >= checkers_on_board.get_checker(i).get_position().x - 7 &&
						this->mause_position.x <= checkers_on_board.get_checker(i).get_position().x + 43 &&
						this->mause_position.y >= checkers_on_board.get_checker(i).get_position().y - 7 &&
						this->mause_position.y <= checkers_on_board.get_checker(i).get_position().y + 43) {
						if (checkers_on_board.get_checker(i).get_select() == 0 && select_is_made == 0) {
							if (checkers_on_board.get_checker(i).get_color() == who_can_move) {
								if (if_you_can_eat_you_must_eat(checkers_on_board.get_checker(i).get_color())) {
									int s = 0;
									for (int f : who_must_eat) {
										if (f == i) {
											s++;
										}
									}
									if (s == 0) {
										checkers_on_board.delete_backlight();
										return;
									}
								}
								choise_chacker = i;
								select_is_made = 1;
								checkers_on_board.get_checker(choise_chacker).select_checker();
								x = checkers_on_board.get_checker(choise_chacker).get_x();
								y = checkers_on_board.get_checker(choise_chacker).get_y();
								if (checkers_on_board.get_checker(choise_chacker).get_queen()) {
									queen_square_for_move(checkers_on_board.get_checker(choise_chacker).get_color(), 1, 1, 1, 1);
								}
								else {
									square_for_move();
								}
								return;
							}
						}
					}
				}
			}
		}
	}
	void change_position(RenderWindow& _window, Event _event) {//ход шашки
		if (_event.type == sf::Event::MouseButtonPressed) {
			if (_event.key.code == Mouse::Right) {
				if (checkers_on_board.get_checker(choise_chacker).get_select() == 1 && select_is_made == 1) {
					set_mause_position(_window);
					if (checkers_on_board.get_board().get_all_squares((centre_on_square().x - 57) / 50, (centre_on_square().y - 57) / 50).get_backlight()) {
						checkers_on_board.get_board().get_all_squares(x, y).square_free();
						if (checkers_on_board.get_checker(choise_chacker).get_queen() == 1) {
							checkers_on_board.get_checker(choise_chacker).set_position(centre_on_square().x, centre_on_square().y);
							if (queen_eat_checker()) {
								x = (centre_on_square().x - 57) / 50;
								y = (centre_on_square().y - 57) / 50;
								checkers_on_board.delete_backlight();
								if (queen_square_for_move(checkers_on_board.get_checker(choise_chacker).get_color(), 1, 1, 1, 1)) {
									return;
								}
							}
						}
						else {
							checkers_on_board.get_checker(choise_chacker).set_position(centre_on_square().x, centre_on_square().y);
							make_queen();
							if (eat_checker()) {
								checkers_on_board.delete_backlight();
								x = (centre_on_square().x - 57) / 50;
								y = (centre_on_square().y - 57) / 50;
								if (!chance_eat_checker(checkers_on_board.get_checker(choise_chacker).get_color())) {
									return;
								}
							}
						}
						checkers_on_board.get_board().get_all_squares((centre_on_square().x - 57) / 50, (centre_on_square().y - 57) / 50).square_employment(checkers_on_board.get_checker(choise_chacker).get_color());
						who_can_move = !who_can_move;
						checkers_on_board.get_checker(choise_chacker).leave_checker();
						select_is_made = 0;
						checkers_on_board.delete_backlight();
						return;
					}
				}
			}
		}
	}
	void make_move(RenderWindow& _window, Event _event) {
		choise_of_chacker(_window, _event);
		change_position(_window, _event);
	}

	void square_for_move() {//варианты хода
		if (checkers_on_board.get_checker(choise_chacker).get_color()) {
			if (checkers_on_board.get_board().get_all_squares(x + 1, y + 1).get_employment() == 0) {
				if (end_board(x + 1, y + 1)) {
					checkers_on_board.get_board().get_all_squares(x + 1, y + 1).on_backlight();
				}
			}
			if (checkers_on_board.get_board().get_all_squares(x - 1, y + 1).get_employment() == 0) {
				if (end_board(x - 1, y + 1)) {
					checkers_on_board.get_board().get_all_squares(x - 1, y + 1).on_backlight();
				}
			}
		}
		else {
			if (checkers_on_board.get_board().get_all_squares(x + 1, y - 1).get_employment() == 0) {
				if (end_board(x + 1, y - 1)) {
					checkers_on_board.get_board().get_all_squares(x + 1, y - 1).on_backlight();
				}
			}
			if (checkers_on_board.get_board().get_all_squares(x - 1, y - 1).get_employment() == 0) {
				if (end_board(x - 1, y - 1)) {
					checkers_on_board.get_board().get_all_squares(x - 1, y - 1).on_backlight();
				}
			}
		}
		if (!chance_eat_checker(checkers_on_board.get_checker(choise_chacker).get_color())) {
			checkers_on_board.delete_backlight();
			chance_eat_checker(checkers_on_board.get_checker(choise_chacker).get_color());
		}
	}
	bool chance_eat_checker(bool _color) {//варианты съесть шашку
		bool more = 1;
		if (checkers_on_board.get_board().get_all_squares(x + 1, y + 1).get_employment()) {
			if (checkers_on_board.get_board().get_all_squares(x + 1, y + 1).get_checker_color() != _color) {
				if (checkers_on_board.get_board().get_all_squares(x + 2, y + 2).get_employment() == 0) {
					if (end_board(x + 2, y + 2)) {
						checkers_on_board.get_board().get_all_squares(x + 2, y + 2).on_backlight();
						more = 0;
					}
				}
			}
		}
		if (checkers_on_board.get_board().get_all_squares(x - 1, y + 1).get_employment()) {
			if (checkers_on_board.get_board().get_all_squares(x - 1, y + 1).get_checker_color() != _color) {
				if (checkers_on_board.get_board().get_all_squares(x - 2, y + 2).get_employment() == 0) {
					if (end_board(x - 2, y + 2)) {
						checkers_on_board.get_board().get_all_squares(x - 2, y + 2).on_backlight();
						more = 0;
					}
				}
			}
		}
		if (checkers_on_board.get_board().get_all_squares(x + 1, y - 1).get_employment()) {
			if (checkers_on_board.get_board().get_all_squares(x + 1, y - 1).get_checker_color() != _color) {
				if (checkers_on_board.get_board().get_all_squares(x + 2, y - 2).get_employment() == 0) {
					if (end_board(x + 2, y - 2)) {
						checkers_on_board.get_board().get_all_squares(x + 2, y - 2).on_backlight();
						more = 0;
					}
				}
			}
		}
		if (checkers_on_board.get_board().get_all_squares(x - 1, y - 1).get_employment()) {
			if (checkers_on_board.get_board().get_all_squares(x - 1, y - 1).get_checker_color() != _color) {
				if (checkers_on_board.get_board().get_all_squares(x - 2, y - 2).get_employment() == 0) {
					if (end_board(x - 2, y - 2)) {
						checkers_on_board.get_board().get_all_squares(x - 2, y - 2).on_backlight();
						more = 0;
					}
				}
			}
		}
		return more;
	}
	bool eat_checker() {//ем шашку
		if ((centre_on_square().x - 57) / 50 - x == 2 || (centre_on_square().x - 57) / 50 - x == -2 &&
			(centre_on_square().y - 57) / 50 - y == 2 || (centre_on_square().y - 57) / 50 - y == -2) {
			for (int i = 0; i < checkers_on_board.get_size(); i++) {
				if (checkers_on_board.get_checker(i).get_x() == x + (checkers_on_board.get_checker(choise_chacker).get_x() - x) / 2 &&
					checkers_on_board.get_checker(i).get_y() == y + (checkers_on_board.get_checker(choise_chacker).get_y() - y) / 2) {
					checkers_on_board.get_board().get_all_squares(checkers_on_board.get_checker(i).get_x(), checkers_on_board.get_checker(i).get_y()).square_free();
					for (int j = i; j < checkers_on_board.get_size() - 1; j++) {
						checkers_on_board.get_checker(j) = checkers_on_board.get_checker(j + 1);
					}
					checkers_on_board.delete_checker();
					if (i < choise_chacker) {
						choise_chacker = choise_chacker - 1;
					}
					return 1;
				}
			}
		}
		return 0;
	}

	void make_queen() {
		if (checkers_on_board.get_checker(choise_chacker).get_color() == 0) {
			if (checkers_on_board.get_checker(choise_chacker).get_y() == 0) {
				checkers_on_board.get_checker(choise_chacker).make_queen();
			}
		}
		if (checkers_on_board.get_checker(choise_chacker).get_color() == 1) {
			if (checkers_on_board.get_checker(choise_chacker).get_y() == 7) {
				checkers_on_board.get_checker(choise_chacker).make_queen();
			}
		}
	}
	bool queen_square_for_move(bool _color, int _i1, int _i2, int _i3, int _i4) {//варианты хода дамки
		int must_eat = 0;
		bool can_eat[4] = { _i1, _i2, _i3, _i4 };
		checkers_on_board.delete_backlight();
		int x_queen = x;
		int y_queen = y;
		if (can_eat[0]) {
			can_eat[0] = 0;
			while (1) {
				if (end_board(x_queen + 1, y_queen + 1)) {
					if (checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen + 1).get_employment() == 1) {
						if (checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen + 1).get_checker_color() == _color) {
							break;
						}
						else if (checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen + 1).get_checker_color() != _color) {
							if (end_board(x_queen + 2, y_queen + 2)) {
								if (checkers_on_board.get_board().get_all_squares(x_queen + 2, y_queen + 2).get_employment() == 0) {
									checkers_on_board.get_board().get_all_squares(x_queen + 2, y_queen + 2).on_backlight();
									x_queen = x_queen + 2;
									y_queen = y_queen + 2;
									must_eat++;
									can_eat[0] = 1;
								}
								else break;
							}
							else break;
						}
					}
					else {
						checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen + 1).on_backlight();
						x_queen = x_queen + 1;
						y_queen = y_queen + 1;
					}
				}
				else break;
			}
		}
		if (can_eat[1]) {
			can_eat[1] = 0;
			x_queen = x;
			y_queen = y;
			while (1) {
				if (end_board(x_queen - 1, y_queen + 1)) {
					if (checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen + 1).get_employment() == 1) {
						if (checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen + 1).get_checker_color() == _color) {
							break;
						}
						else if (checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen + 1).get_checker_color() != _color) {
							if (end_board(x_queen - 2, y_queen + 2)) {
								if (checkers_on_board.get_board().get_all_squares(x_queen - 2, y_queen + 2).get_employment() == 0) {
									checkers_on_board.get_board().get_all_squares(x_queen - 2, y_queen + 2).on_backlight();
									x_queen = x_queen - 2;
									y_queen = y_queen + 2;
									must_eat++;
									can_eat[1] = 1;
								}
								else break;
							}
							else break;
						}
					}
					else {
						checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen + 1).on_backlight();
						x_queen = x_queen - 1;
						y_queen = y_queen + 1;
					}
				}
				else break;
			}
		}
		if (can_eat[2]) {
			can_eat[2] = 0;
			x_queen = x;
			y_queen = y;
			while (1) {
				if (end_board(x_queen + 1, y_queen - 1)) {
					if (checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen - 1).get_employment() == 1) {
						if (checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen - 1).get_checker_color() == _color) {
							break;
						}
						else if (checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen - 1).get_checker_color() != _color) {
							if (end_board(x_queen + 2, y_queen - 2)) {
								if (checkers_on_board.get_board().get_all_squares(x_queen + 2, y_queen - 2).get_employment() == 0) {
									checkers_on_board.get_board().get_all_squares(x_queen + 2, y_queen - 2).on_backlight();
									x_queen = x_queen + 2;
									y_queen = y_queen - 2;
									must_eat++;
									can_eat[2] = 1;
								}
								else break;
							}
							else break;
						}
					}
					else {
						checkers_on_board.get_board().get_all_squares(x_queen + 1, y_queen - 1).on_backlight();
						x_queen = x_queen + 1;
						y_queen = y_queen - 1;
					}
				}
				else break;
			}
		}
		if (can_eat[3]) {
			can_eat[3] = 0;
			x_queen = x;
			y_queen = y;
			while (1) {
				if (end_board(x_queen - 1, y_queen - 1)) {
					if (checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen - 1).get_employment() == 1) {
						if (checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen - 1).get_checker_color() == _color) {
							break;
						}
						else if (checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen - 1).get_checker_color() != _color) {
							if (end_board(x_queen - 2, y_queen - 2)) {
								if (checkers_on_board.get_board().get_all_squares(x_queen - 2, y_queen - 2).get_employment() == 0) {
									checkers_on_board.get_board().get_all_squares(x_queen - 2, y_queen - 2).on_backlight();
									x_queen = x_queen - 2;
									y_queen = y_queen - 2;
									must_eat++;
									can_eat[3] = 1;
								}
								else break;
							}
							else break;
						}
					}
					else {
						checkers_on_board.get_board().get_all_squares(x_queen - 1, y_queen - 1).on_backlight();
						x_queen = x_queen - 1;
						y_queen = y_queen - 1;
					}
				}
				else break;
			}
		}
		if (must_eat == 0) {
			return 0;
		}
		else {
			if (_i1 == 1 && _i2 == 1 && _i3 == 1 && _i4 == 1) {
				queen_square_for_move(_color, can_eat[0], can_eat[1], can_eat[2], can_eat[3]);
			}
			else return 1;
		}
	}
	bool queen_eat_checker() {//ем шашку
		int x_eat = 0;
		int y_eat = 0;
		if ((centre_on_square().x - 57) / 50 - x >= 2 && (centre_on_square().y - 57) / 50 - y >= 2) {
			int i = x;
			int j = y;
			while (1) {
				if (i < (centre_on_square().x - 57) / 50 - 1 && j < (centre_on_square().y - 57) / 50 - 1) {
					if (checkers_on_board.get_board().get_all_squares(i + 1, j + 1).get_employment() == 1) {
						x_eat = i + 1;
						y_eat = j + 1;
						break;
					}
					else {
						i++;
						j++;
					}
				}
				else break;
			}
		}
		if ((centre_on_square().x - 57) / 50 - x >= 2 && y - (centre_on_square().y - 57) / 50 >= 2) {
			int i = x;
			int j = y;
			while (1) {
				if (i < (centre_on_square().x - 57) / 50 - 1 && j >(centre_on_square().y - 57) / 50 + 1) {
					if (checkers_on_board.get_board().get_all_squares(i + 1, j - 1).get_employment() == 1) {
						x_eat = i + 1;
						y_eat = j - 1;
						break;
					}
					else {
						i++;
						j--;
					}
				}
				else break;
			}
		}
		if (x - (centre_on_square().x - 57) / 50 >= 2 && (centre_on_square().y - 57) - y / 50 >= 2) {
			int i = x;
			int j = y;
			while (1) {
				if (i > (centre_on_square().x - 57) / 50 + 1 && j < (centre_on_square().y - 57) / 50 - 1) {
					if (checkers_on_board.get_board().get_all_squares(i - 1, j + 1).get_employment() == 1) {
						x_eat = i - 1;
						y_eat = j + 1;
						break;
					}
					else {
						i--;
						j++;
					}
				}
				else break;
			}
		}
		if (x - (centre_on_square().x - 57) / 50 >= 2 && y - (centre_on_square().y - 57) / 50 >= 2) {
			int i = x;
			int j = y;
			while (1) {
				if (i > (centre_on_square().x - 57) / 50 + 1 && j > (centre_on_square().y - 57) / 50 + 1) {
					if (checkers_on_board.get_board().get_all_squares(i - 1, j - 1).get_employment() == 1) {
						x_eat = i - 1;
						y_eat = j - 1;
						break;
					}
					else {
						i--;
						j--;
					}
				}
				else break;
			}
		}

		for (int f = 0; f < checkers_on_board.get_size(); f++) {
			if (checkers_on_board.get_checker(f).get_x() == x_eat && checkers_on_board.get_checker(f).get_y() == y_eat) {
				checkers_on_board.get_board().get_all_squares(checkers_on_board.get_checker(f).get_x(), checkers_on_board.get_checker(f).get_y()).square_free();

				for (int c = f; c < checkers_on_board.get_size() - 1; c++) {
					checkers_on_board.get_checker(c) = checkers_on_board.get_checker(c + 1);
				}
				checkers_on_board.delete_checker();
				if (f < choise_chacker) {
					choise_chacker = choise_chacker - 1;
				}
				return 1;
			}
		}
		return 0;
	}

	bool end_board(float _x, float _y) {//проверка выходы за пределы поля
		if (_x >= 0 && _x < 8 && _y >= 0 && _y < 8) {
			return 1;
		}
		return 0;
	}

	bool if_you_can_eat_you_must_eat(bool _color) {
		while (who_must_eat.size() != 0) {
			who_must_eat.pop_back();
		}
		for (int i = 0; i < checkers_on_board.get_size(); i++) {
			if (checkers_on_board.get_checker(i).get_color() == _color) {
				if (checkers_on_board.get_checker(i).get_queen() == 0) {
					x = checkers_on_board.get_checker(i).get_x();
					y = checkers_on_board.get_checker(i).get_y();
					if (!chance_eat_checker(_color)) {
						who_must_eat.push_back(i);
					}
				}
				if (checkers_on_board.get_checker(i).get_queen() == 1) {
					x = checkers_on_board.get_checker(i).get_x();
					y = checkers_on_board.get_checker(i).get_y();
					if (queen_square_for_move(_color, 1, 1, 1, 1)) {
						who_must_eat.push_back(i);
					}
				}
			}
		}
		checkers_on_board.delete_backlight();
		if (who_must_eat.size() != 0) return 1;
		else return 0;
	}
	void start_game(RenderWindow& _window, Event _event, bool& _start) {
		
		while (1) {
			_window.clear(Color(0,0,0));
			_window.display();
			while (_window.pollEvent(_event)) {
				if (_event.type == sf::Event::MouseButtonPressed || _event.type == sf::Event::KeyPressed) {
					_start = 0;
					return;
				}
			}
		}
	}

	bool end_game(RenderWindow& _window, Event _event) {
		int black = 0;
		int white = 0;
		for (int i = 0; i < checkers_on_board.get_size(); i++) {
			if (checkers_on_board.get_checker(i).get_color() == 0) {
				white++;
			}
			if (checkers_on_board.get_checker(i).get_color() == 1) {
				black++;
			}
		}
		if (white == 0) {
			RectangleShape back = RectangleShape(Vector2f(350, 150));
			back.setFillColor(Color(245, 210, 175));
			back.setPosition(75, 175);
			Font font;
			font.loadFromFile("Font//bahnschrift.ttf");
			Text t = Text("", font, 81);
			t.setString(L"Black win");
			t.setFillColor(Color::Black);
			t.setPosition(78, 190);
			_window.draw(back);
			_window.draw(t);
			if (_event.type == Event::Closed)
				_window.close();
			return 0;
		}
		if (black == 0) {
			RectangleShape back = RectangleShape(Vector2f(350, 150));
			back.setFillColor(Color(245, 210, 175));
			back.setPosition(75, 175);
			Font font;
			font.loadFromFile("Font//bahnschrift.ttf");
			Text t = Text("", font, 81);
			t.setString(L"White win");
			t.setFillColor(Color::Black);
			t.setPosition(75, 190);
			_window.draw(back);
			_window.draw(t);
			if (_event.type == Event::Closed)
				_window.close();
			return 0;
		}
		return 1;
	}
};