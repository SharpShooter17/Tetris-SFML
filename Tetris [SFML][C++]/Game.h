#pragma once
#include <TGUI\TGUI.hpp>
#include "model.h"
#include <string>

class Game
{
private:
	enum class State { in_game, pause, in_menu, exit, game_over } state;

	class Exceptions
	{
	public:
		enum class except { no_connection } exception;
		Exceptions(Exceptions::except e) { this->exception = e; }
		~Exceptions() {}
		char* what() { return "no connection with matrixxx.cba.pl"; }
	};

	void game_loop();
	void menu_loop();

	sf::RenderWindow app;
	Model * p_model;

	///gui
	tgui::Gui tgui_game;
	tgui::Gui tgui_menu;

	sf::Sound theme;

	void Send_Scores(int time, int scores, int lines, int level);
	
public:
	Game(Model * model, const sf::ContextSettings &settings);
	~Game();

	void Initialize();
	void run();
};

