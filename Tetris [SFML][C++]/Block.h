#pragma once

enum class Color { Red, Yellow, Green, Blue, Orange, Purple, Torquise, Brown };

class Block
{
	Color color;
	int x, y;
public:
	Block() = delete;
	Block(Color color, int x, int y);
	~Block();

	inline int get_x() { return x; }
	inline int get_y() { return y; }
	inline void set_x(int x) { this->x = x; }
	inline void set_y(int y) { this->y = y; }
	inline void set_xy(int x, int y) { this->x = x; this->y = y; }
	inline Color get_color() { return color; }
	inline void move_down() { y++; }
	inline void move_right() { x++; }
	inline void move_left() { x--; }
};

