#pragma once
class Level
{
private:
	int scores;
	double speed;
	int lines;
	int blocks;

public:
	Level();
	~Level();

	void add_scores(int scores) { this->scores += scores; speed += static_cast<double>(scores) / 600.0; blocks++; }
	inline double get_speed() const { return speed; }
	inline int get_scores() const { return scores; }
	inline void add_lines(int n) { lines += n; }
	inline int get_lines() const { return lines; }
};