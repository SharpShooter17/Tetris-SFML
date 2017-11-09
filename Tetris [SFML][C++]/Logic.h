#pragma once
#include "model.h"
#include "Level.h"

class Logic
{
	enum class State { no_pack, is_pack } state;

	bool Collisions_with_the_bottom();
	bool Collision_with_the_left_side();
	bool Collision_with_the_right_side();
	bool Collision_block_in_block();

	void move_pack_down();

	Model *p_model;	
	Level level;

public:
	Logic(Model *model);
	~Logic();

	void Control(sf::Event & event);
	void update();
	void pack_to_down();
	inline bool game_over() const { return p_model->block_over_top(); }
	inline double get_speed() const { return level.get_speed(); }
	inline int get_scores() const { return level.get_scores(); }
	inline int get_lines() const { return level.get_lines(); }
};

