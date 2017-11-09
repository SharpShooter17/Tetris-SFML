#include "model.h"
#include <Windows.h>

Model::Model() 
{
	app_Width = 1024;
	app_Height = 768;
	size_tile = app_Height / size_board_y;

	sf::Texture *temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("orange.png")) { MessageBox(nullptr, "No file orange.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); }
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("red.png")) { MessageBox(nullptr, "No file red.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file);}
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("yellow.png")) { MessageBox(nullptr, "No file yellow.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file);}
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("blue.png")) { MessageBox(nullptr, "No file blue.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file);}
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("green.png")) { MessageBox(nullptr, "No file green.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); }
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("purple.png")) { MessageBox(nullptr, "No file purple.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); };
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("brown.png")) { MessageBox(nullptr, "No file brown.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); }
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("torquise.png")) { MessageBox(nullptr, "No file torquise.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); }
	textures.push_back(*temp);
	delete temp;

	temp = new sf::Texture;
	if (!temp->loadFromFile("pause.png")) { MessageBox(nullptr, "No file pause.png", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); }
	textures.push_back(*temp);
	delete temp;

	if (!theme.loadFromFile("theme.flac")) { MessageBox(nullptr, "No file theme.flac", "Error", MB_ICONERROR); throw Exceptions(Exceptions::except::no_file); }
}

Model::~Model(){}

const sf::Texture * Model::get_texture(Color color)
{
	switch (color)
	{
	case Color::Red:
		return &textures[1];
	case Color::Yellow:
		return &textures[2];
	case Color::Green:
		return &textures[4];
	case Color::Blue:
		return &textures[3];
	case Color::Orange:
		return &textures[0];
	case Color::Purple:
		return &textures[5];
	case Color::Brown:
		return &textures[6];
	case Color::Torquise:
		return &textures[7];
	default:
		return &textures[0];
	}
}

void Model::move_down_pack_of_blocks()
{
	for (int i = 0;i < pack_of_blocks.size();i++)
	{
		pack_of_blocks[i].move_down();
	}
}

void Model::move_left_pack_of_blocks()
{
	for (int i = 0;i < pack_of_blocks.size();i++)
	{
		pack_of_blocks[i].move_left();
	}
}

void Model::move_right_pack_of_blocks()
{
	for (int i = 0;i < pack_of_blocks.size();i++)
	{
		pack_of_blocks[i].move_right();
	}
}

void Model::rotate_pack_of_blocks()
{
	make_copy_pack_of_block();

	int max_x{ 0 }, max_y{ 0 }, min_x{ size_board_x }, min_y{ size_board_y }, len_x, len_y;

	for (int i = 0; i < pack_of_blocks.size(); i++) {
		max_x = max_x > pack_of_blocks[i].get_x() ? max_x : pack_of_blocks[i].get_x();
		max_y = max_y > pack_of_blocks[i].get_y() ? max_y : pack_of_blocks[i].get_y();

		min_x = min_x < pack_of_blocks[i].get_x() ? min_x : pack_of_blocks[i].get_x();
		min_y = min_y < pack_of_blocks[i].get_y() ? min_y : pack_of_blocks[i].get_y();
	}

	len_x = max_x - min_x + 1;
	len_y = max_y - min_y + 1;

	std::vector<std::vector<int>> matrix;
	matrix.resize(len_y);
	for (int i = 0; i < len_y; i++)
	{
		matrix[i].resize(len_x);
		for (int j = 0; j < len_x; j++)
		{
			matrix[i][j] = -1;
		}
	}

	for (int i = 0; i < pack_of_blocks.size(); i++) 
	{
		matrix[pack_of_blocks[i].get_y() - min_y][pack_of_blocks[i].get_x() - min_x] = i;
	}

	std::vector<std::vector<int>> rotated_matrix;
	rotated_matrix.resize(len_x);
	for (int i = 0; i < len_x; i++)
	{
		rotated_matrix[i].resize(len_y);
	}												
	
	for (int i = 0; i < len_y; i++)
	{
		for (int j = 0; j < len_x; j++)
		{
			rotated_matrix[j][i] = matrix[len_y - 1 -i][j];
		}
	}
	
	int index = 0;
	for (int i = 0; i < len_x; i++)
	{
		for (int j = 0; j < len_y; j++)
		{
			if (rotated_matrix[i][j] != -1)
			{
				pack_of_blocks[index].set_xy(j + min_x, i + min_y);
				index++;
			}
		}
	}

	///This code say: You can't go out behind edge of board 
	int minus_x{ 0 }, minus_y{ 0 };

	for (int i = 0; i < pack_of_blocks.size(); i++)
	{
		int x = pack_of_blocks[i].get_x();
		int y = pack_of_blocks[i].get_y();

		while ((x - minus_x) >= size_board_x) minus_x++;
		while ((y - minus_y) >= size_board_y) minus_y++;
	}
	for (int i = 0; i < pack_of_blocks.size(); i++)
	{
		int x = pack_of_blocks[i].get_x();
		int y = pack_of_blocks[i].get_y();
		pack_of_blocks[i].set_xy(x - minus_x, y - minus_y);
	}
	///End of protection
}

void Model::make_copy_pack_of_block()
{
	pack_of_blocks_before_operation.clear();
	pack_of_blocks_before_operation = pack_of_blocks;
}

void Model::restore_the_pack_of_block_before_operation()
{
	pack_of_blocks = pack_of_blocks_before_operation;
}

void Model::restart_game()
{
	pack_of_blocks.clear();
	blocks.clear();
	pack_of_blocks_before_operation.clear();
	holded_pack_of_blocks.clear();
	next_pack_of_blocks.clear();
}

bool Model::hold_package_of_blocks()
{
	bool b_holded_pack = holded_pack_of_blocks.size() == 0 ? false : true;

	int min_x{ size_board_x }, min_y{ size_board_y };

	if (b_holded_pack) 
	{
		for (int i = 0; i < pack_of_blocks.size(); i++) {
			min_x = min_x < pack_of_blocks[i].get_x() ? min_x : pack_of_blocks[i].get_x();
			min_y = min_y < pack_of_blocks[i].get_y() ? min_y : pack_of_blocks[i].get_y();
		}

		Swap< std::vector<Block> >(pack_of_blocks, holded_pack_of_blocks);

		int minus_x{ 0 }, minus_y{ 0 };

		for (int i = 0; i < pack_of_blocks.size(); i++)
		{
			int x = pack_of_blocks[i].get_x() + min_x;
			int y = pack_of_blocks[i].get_y() + min_y;

			while ((x - minus_x) >= size_board_x) minus_x++;
			while ((y - minus_y) >= size_board_y) minus_y++;
		}
		for (int i = 0; i < pack_of_blocks.size(); i++)
		{
			int x = pack_of_blocks[i].get_x() + min_x;
			int y = pack_of_blocks[i].get_y() + min_y;
			pack_of_blocks[i].set_xy(x - minus_x, y - minus_y);
		}
	}
	else
	{
		holded_pack_of_blocks = std::move(pack_of_blocks);
	}

	for (int i = 0; i < holded_pack_of_blocks.size(); i++) {
		min_x = min_x < holded_pack_of_blocks[i].get_x() ? min_x : holded_pack_of_blocks[i].get_x();
		min_y = min_y < holded_pack_of_blocks[i].get_y() ? min_y : holded_pack_of_blocks[i].get_y();
	}
	
	for (int i = 0; i < holded_pack_of_blocks.size(); i++)
	{
		holded_pack_of_blocks[i].set_xy(holded_pack_of_blocks[i].get_x() - min_x, holded_pack_of_blocks[i].get_y() - min_y);
	}

	return b_holded_pack;
}

bool Model::block_over_top()
{
	for (int i = 0;i < blocks.size(); ++i)
	{
		if (blocks[i].get_y() == -1)
			return true;
	}
	return false;
}

int Model::remove_full_line()
{
	int removed_lines{ 0 };

	int tab[size_board_y] { 0 };
	
	///checking lines
	for (int i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i].get_y() >= 0 && blocks[i].get_y() < size_board_y)
			tab[blocks[i].get_y()]++;
	}

	//removing full lines
	for (int i = 0; i < size_board_y; ++i)
	{
		if (tab[i] >= size_board_x)
		{
			removed_lines++;
			for (int j = 0; j < blocks.size(); ++j)
			{
				if (i == blocks[j].get_y())
				{
					blocks.erase(blocks.begin() + j);
					j--;
				}
			}

			///moving down blocks
			for (int j = 0; j < blocks.size(); j++)
			{
				if (blocks[j].get_y() < i)
				{
					blocks[j].set_y(blocks[j].get_y() + 1);
				}
			}

		}
	}
	
	return removed_lines;
}

void Model::new_pack(std::initializer_list<std::initializer_list<char>> block, Color color, std::vector<Block> &v)
{
	auto p = block.begin();

	for (int i = 0; (p + i) != block.end(); ++i)
	{
		auto q = (p+i)->begin();
		for (int j = 0; (q + j) != (p+i)->end(); ++j)
		{
			if (*(q + j) == '*')
			{
				v.push_back(Block(color, j + 16/2-4/2, i - 4));
			}
		}
	}
}

void Model::new_pack(std::initializer_list<std::initializer_list<char>> block, Color color)
{
	if (this->next_pack_of_blocks.size() == 0)
	{
		new_pack(block, color, this->next_pack_of_blocks);
	}
	else
	{
		for (int i = 0; i < this->pack_of_blocks.size(); i++)
		{
			blocks.push_back(this->pack_of_blocks[i]);
		}

		this->pack_of_blocks.clear();
	}
	
	this->pack_of_blocks = std::move(this->next_pack_of_blocks);

	new_pack(block, color, this->next_pack_of_blocks);
}

void Model::new_pack()
{
	int ran = std::rand() % 8;
	switch (ran)
	{
	case 0:
	{
		new_pack({
			{ '*','*','*' },
			{ '*','*','*' },
			{ '*','*','*' } }, Color::Red);

		break;
	}
	case 1:
	{
		new_pack({
			{ '*','*' },
			{ '*','#' },
			{ '*','#' } }, Color::Orange);

		break;
	}
	case 2:
	{
		new_pack({
			{ '*','*' },
			{ '#','*' },
			{ '#','*' } }, Color::Yellow);

		break;
	}
	case 3:
	{
		new_pack({
			{ '#','*','*' },
			{ '*','*' } }, Color::Green);

		break;
	}
	case 4:
	{
		new_pack({
			{ '*','*','*' },
			{ '#','*', } }, Color::Blue);

		break;
	}
	case 5:
		new_pack({
			{ '*' },
			{ '*' },
			{ '*' },
			{ '*' } }, Color::Purple);

		break;
	case 6:
		new_pack({
			{ '*','*'},
			{ '*','*'} }, Color::Torquise);
		break;
	case 7:
		new_pack({
			{ '#','*' },
			{ '*','*' },
			{ '*',} }, Color::Brown);
		break;
	default:
		new_pack({
			{ '#','*' },
			{ '*','*','*' } }, Color::Blue);
		break;
	}
}
