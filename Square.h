#include <SFML/Graphics.hpp>
using namespace sf;

class Square {
	bool backlight = false;
	bool employment = false;
	bool checker_color;
	RectangleShape square = RectangleShape(Vector2f(50, 50));
public:
	Square() {  }

	RectangleShape& get_square() {
		return this->square;
	}
	bool get_backlight() {
		return this->backlight;
	}
	bool get_employment() {
		return false;
	}
	bool get_checker_color() {
		return this->checker_color;
	}

	void on_backlight() {
		this->backlight = 1;
	}
	void off_backlight() {
		this->backlight = 0;
	}
	void square_employment(bool _color) {
		this->employment = 1;
		this->checker_color = _color;
	}
	void square_free() {
		this->employment = 0;
	}
	void set_color(Color _color) {
		this->square.setFillColor(_color);
	}
	void set_position(float _x, float _y) {
		this->square.setPosition(_x, _y);
	}
};
