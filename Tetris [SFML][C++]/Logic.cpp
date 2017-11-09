#include "Logic.h"
#include "Block.h"

#include <cstdlib>

bool Logic::Collisions_with_the_bottom()
{
	//Checking Collisions with the bottom of screen
	for (int i = 0; i < p_model->size_of_package(); ++i)
	{
		Block & b = p_model->get_block_from_package(i);
		if (b.get_y() == p_model->get_size_board_y()-1)
		{
			return true;
		}
	}
	//Checking Collisions with other blocks
	for (int i = 0; i < p_model->size_of_package(); ++i)
	{
		Block & pack = p_model->get_block_from_package(i);
		for (int j = 0; j < p_model->size_of_blocks(); j++)
		{
			Block & block = p_model->get_block(j);
			if ((pack.get_x() == block.get_x()) && (pack.get_y() == block.get_y() - 1))
			{
				return true;
			}
		}
	}
	return false;
}

bool Logic::Collision_with_the_left_side()
{
	//checking collisions with left border
	for (int i = 0; i < p_model->size_of_package(); ++i)
	{
		Block & b = p_model->get_block_from_package(i);
		if (b.get_x() < 0)
		{
			return true;
		}
	}

	return Collision_block_in_block();
}

bool Logic::Collision_with_the_right_side()
{
	//checking collisions with right side border
	for (int i = 0; i < p_model->size_of_package(); ++i)
	{
		Block & b = p_model->get_block_from_package(i);
		if (b.get_x() > p_model->get_size_board_x() - 1)
		{
			return true;
		}
	}
	
	return Collision_block_in_block();
}

bool Logic::Collision_block_in_block()
{
	for (int i = 0; i < p_model->size_of_package(); ++i)
	{
		Block & pack = p_model->get_block_from_package(i);
		for (int j = 0; j < p_model->size_of_blocks(); j++)
		{
			Block & block = p_model->get_block(j);
			if ((pack.get_y() == block.get_y()) && (pack.get_x() == block.get_x()))
			{
				return true;
			}
		}
	}
	return false;
}

void Logic::move_pack_down()
{
	p_model->move_down_pack_of_blocks();
}

void Logic::Control(sf::Event & event)
{
	if (state != State::is_pack)
		return;

	if (event.key.code == sf::Keyboard::Space)
	{
		p_model->make_copy_pack_of_block();

		if (!p_model->hold_package_of_blocks())
		{
			state = State::no_pack;
		}

		if (Collision_block_in_block())
		{
			p_model->hold_package_of_blocks();
		}
	}

	if ((event.key.code == sf::Keyboard::Left))
	{
		p_model->make_copy_pack_of_block();
		p_model->move_left_pack_of_blocks();
		if (Collision_with_the_left_side())
			p_model->restore_the_pack_of_block_before_operation();
	}

	if ((event.key.code == sf::Keyboard::Right))
	{
		p_model->make_copy_pack_of_block();
		p_model->move_right_pack_of_blocks();
		if (Collision_with_the_right_side())
			p_model->restore_the_pack_of_block_before_operation();
	}

	if ((event.key.code == sf::Keyboard::Up))
	{
		p_model->rotate_pack_of_blocks();

		if (Collision_with_the_right_side() || Collision_with_the_left_side() || Collisions_with_the_bottom())
		{
			p_model->restore_the_pack_of_block_before_operation();
		}
	}

	if (event.key.code == sf::Keyboard::Down)
	{
		if (!Collisions_with_the_bottom())
		{
			move_pack_down();
		}
		else
		{
			state = State::no_pack;
		}
	}

}

Logic::Logic(Model *model) : p_model{model}, state{State::no_pack}
{
}

Logic::~Logic()
{
}

void Logic::update()
{
	if (state == State::no_pack)
	{
		p_model->new_pack();
		state = State::is_pack;
		int lines = p_model->remove_full_line();
		level.add_scores(lines*lines*100+10);
		level.add_lines(lines);
	}
}

void Logic::pack_to_down()
{
	if (!Collisions_with_the_bottom())
	{
		move_pack_down();
	}
	else
	{
		state = State::no_pack;
	}
}
