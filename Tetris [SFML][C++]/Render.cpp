#include "Render.h"
Render::Render(sf::RenderWindow *app, Model *model) : p_app{app}, p_model{model} {}
Render::~Render() {}

void Render::draw()
{
	static const float size_texture = static_cast<float>(p_model->get_texture(Color::Blue)->getSize().x);
	const float scale = static_cast<float>(p_model->get_size_tile()/size_texture);

	for (int i = 0; i < p_model->size_of_next_package(); i++)
	{
		Block & block = p_model->get_block_from_next_pack(i);
		sf::Sprite sprite;
		sprite.setTexture(*p_model->get_texture(block.get_color()));
		sprite.setPosition(static_cast<float>(block.get_x()* p_model->get_size_tile()+ p_model->get_app_width() * 5 / 2 / 4), 380.0f + (block.get_y()+4) * p_model->get_size_tile());
		sprite.setScale(scale, scale);
		p_app->draw(sprite);
	}

	for (int i = 0; i < p_model->size_of_hold_package(); i++)
	{
		Block & block = p_model->get_block_from_hold_pack(i);
		float x = static_cast<float>(block.get_x()* p_model->get_size_tile() + p_model->get_app_width() * 9 / 16);
		float y = 380.0f + (block.get_y()) * p_model->get_size_tile();
		sf::Sprite sprite;
		sprite.setTexture(*p_model->get_texture(block.get_color()));
		sprite.setPosition(x, y);
		sprite.setScale(scale, scale);
		p_app->draw(sprite);
	}

	for (int i = 0; i < p_model->size_of_blocks(); ++i)
	{
		Block & block = p_model->get_block(i);
		sf::Sprite sprite;
		sprite.setTexture(*p_model->get_texture(block.get_color()));
		sprite.setPosition(static_cast<float>(block.get_x() * p_model->get_size_tile()), static_cast<float>(block.get_y() *  p_model->get_size_tile()));
		sprite.setScale(scale, scale);
		p_app->draw(sprite);
	}

	for (int i = 0; i < p_model->size_of_package(); ++i)
	{
		Block & block = p_model->get_block_from_package(i);
		sf::Sprite sprite;
		sprite.setTexture(*p_model->get_texture(block.get_color()));
		sprite.setScale(scale,scale);
		sprite.setPosition(static_cast<float>(block.get_x() * p_model->get_size_tile()), static_cast<float>(block.get_y() *  p_model->get_size_tile()));
		p_app->draw(sprite);
	}
}
