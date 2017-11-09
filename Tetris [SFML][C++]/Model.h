#pragma once
#include <vector>
#include "Block.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

template<typename T>
void Swap(T & swap1, T & swap2)
{
	T temp = std::move(swap1);
	swap1 = std::move(swap2);
	swap2 = std::move(temp);
	return;
}

class Model
{
	std::vector<Block> blocks;
	std::vector<Block> pack_of_blocks;
	std::vector<Block> next_pack_of_blocks;
	std::vector<Block> pack_of_blocks_before_operation;
	std::vector<Block> holded_pack_of_blocks;
	std::vector<sf::Texture> textures;
	sf::SoundBuffer theme;

	constexpr static short size_board_x{ 16 };
	constexpr static short size_board_y{ 24 };
	short size_tile;

	int app_Width;
	int app_Height;

	void new_pack(std::initializer_list<std::initializer_list<char>> block, Color color, std::vector<Block> &v);	//do not use this function!
	void new_pack(std::initializer_list<std::initializer_list<char>> block, Color color);

public:
	Model();
	~Model();

	class Exceptions
	{
	public:
		enum class except { no_file } exception;
		Exceptions(Exceptions::except e) { this->exception = e; }
		~Exceptions() {}
		char* what() { return "no connection with http://matrixxx.cba.pl"; }
	};

	inline constexpr static short get_size_board_x() { return size_board_x; }
	inline constexpr static short get_size_board_y() { return size_board_y; }
	inline short get_size_tile() { return size_tile; }

	inline int get_app_width() { return app_Width; }
	inline int get_app_height() { return app_Height; }
	inline void set_app_height(int height) { app_Height = height; }
	inline void set_app_width(int width) { app_Width = width; }

	inline sf::SoundBuffer * get_theme() { return &theme; }

	inline int size_of_blocks() { return static_cast<int>(blocks.size()); }
	inline Block & get_block(int i) { return blocks[i]; }
	const sf::Texture * get_texture(Color color);
	inline const sf::Texture * get_pause_texture() { return &textures[8]; }

	inline int size_of_package() { return static_cast<int>(pack_of_blocks.size()); }
	inline Block & get_block_from_package(int i) { return pack_of_blocks[i]; }

	inline int size_of_next_package() { return static_cast<int>(next_pack_of_blocks.size()); }
	inline Block & get_block_from_next_pack(int i) { return next_pack_of_blocks[i]; }

	inline int size_of_hold_package() { return static_cast<int>(holded_pack_of_blocks.size()); }
	inline Block & get_block_from_hold_pack(int i) { return holded_pack_of_blocks[i]; }
	
	void move_down_pack_of_blocks();
	void move_left_pack_of_blocks();
	void move_right_pack_of_blocks();
	void rotate_pack_of_blocks();
	void make_copy_pack_of_block();
	void restore_the_pack_of_block_before_operation();
	void restart_game();

	bool hold_package_of_blocks();

	int remove_full_line();
	bool block_over_top();

	void new_pack();
};

