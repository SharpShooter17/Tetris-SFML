#pragma once
#include "model.h"

class Render
{
private:
	sf::RenderWindow *p_app;
	Model *p_model;
public:
	Render(sf::RenderWindow *app, Model *model);
	~Render();
	void draw();
};

