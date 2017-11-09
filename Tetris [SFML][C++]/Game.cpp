#include <Windows.h>

#include <ctime>
#include <cstdlib>
#include <string>

#include <SFML\System.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Network.hpp>

#include "Game.h"
#include "Logic.h"
#include "Render.h"

std::string int_to_time(int n);

void Game::game_loop()
{
	p_model->restart_game();
	sf::Clock clock;
	Logic logic{p_model};
	Render render(&app, p_model);

	clock.restart();

	sf::Time time;
	sf::Time game_time;
	sf::Time Control;
	int Move = 4;
	bool Frame = true;
	bool pack_down = false;
	sf::Sprite s_pause(*p_model->get_pause_texture());
	s_pause.setPosition(128, 416);

	while ((state == State::in_game) || (state == State::pause) )
	{
		time += clock.getElapsedTime();
		Control += clock.getElapsedTime();
		if (state == State::in_game)
			game_time += clock.getElapsedTime();

		clock.restart();

		if (time.asSeconds() >= 1.0 / logic.get_speed()) 
		{
			time = sf::Time::Zero;
			Move = 4;
			Frame = true;
			pack_down = true;
		}


		sf::Event event;
		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (Move && (state == State::in_game) && Control.asSeconds() > 1.0 / logic.get_speed() / 4.0) {
					Move--;
					Frame = true;
					logic.Control(event);
					Control = sf::Time::Zero;
				}

				if (event.key.code == sf::Keyboard::P)
				{
					if (state == State::in_game)
						state = State::pause;
					else if (state == State::pause)
						state = State::in_game;
				}
			}

			if (event.type == sf::Event::Closed) {
				app.close();
				state = State::exit;
			}

			tgui_game.handleEvent(event);
		}

		if (pack_down && (state == State::in_game))
		{
			logic.pack_to_down();
			pack_down = false;
		}

		if (Frame && (state == State::in_game))
		{
			logic.update();

			if (logic.game_over())
			{
				state = State::game_over;
				try
				{
					Send_Scores(static_cast<int>(game_time.asSeconds()), logic.get_scores(), logic.get_lines(), static_cast<int>(logic.get_speed()));
				}
				catch (Exceptions & e)
				{
					MessageBox(nullptr, e.what(), "Error", MB_ICONERROR);
				}
			}

			tgui_game.get<tgui::Label>("label_scores")->setText("Scores: " + std::to_string(logic.get_scores()));

			std::string s_game_time = "Time: " + std::to_string(static_cast<int>(game_time.asSeconds())) + "s";

			tgui_game.get<tgui::Label>("label_time")->setText(s_game_time);
		}

		if (Frame && ((state == State::in_game) || (state == State::pause)))
		{
			app.clear();
			render.draw();

			if (state == State::pause)
				app.draw(s_pause);

			tgui_game.draw();
			app.display();
			Frame = false;
		}
	}
}

void Game::menu_loop()
{
	Render render(&app, p_model);

	while (state == State::in_menu)
	{
		sf::Event event;
		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				app.close();
				state = State::exit;
			}

			if (event.type == sf::Event::Resized)
			{
				p_model->set_app_height(event.size.height);
				p_model->set_app_width(event.size.width);
				app.setView(sf::View(sf::FloatRect(0.0f, 0.0f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				tgui_menu.setView(app.getView());
			}

			tgui_menu.handleEvent(event);
		}
		
		if ( theme.getVolume() != tgui_menu.get<tgui::Slider>("slider_volume")->getValue())
			theme.setVolume(static_cast<float>(tgui_menu.get<tgui::Slider>("slider_volume")->getValue()));

		app.clear();
		tgui_menu.draw();
		//render.draw();
		app.display();
	}
}

void Game::Send_Scores(int time, int scores, int lines, int level)
{
	sf::Http http("matrixxx.cba.pl/");
	sf::Http::Request request("index.php/rankings/add_record_tetris", sf::Http::Request::Post);

	std::ostringstream stream;
	stream << "name=" << tgui_menu.get<tgui::TextBox>("nickname")->getText().toAnsiString() << "&scores=" << scores <<"&time="<< int_to_time(time) << "&level=" << level <<"&lines="<<lines;

	request.setBody(stream.str());

	sf::Http::Response response = http.sendRequest(request);
	
	if (response.getStatus() != sf::Http::Response::Status::Ok)
	{
		throw Exceptions(Exceptions::except::no_connection);
	}
}

Game::Game(Model * model, const sf::ContextSettings &settings)
	: state{ State::in_menu }, p_model{model},
		app{ sf::VideoMode(model->get_app_width(), model->get_app_height()), 
				std::string("Tetris - Game created by Bartosz Ujazdowski - b.ujazdowski@gmail.com - http://matrixxx.cba.pl/index.php/rankings/tetris"), 
				sf::Style::Titlebar, settings},
		tgui_game{app}, tgui_menu{app}, theme{}
{	
	theme.setBuffer(*p_model->get_theme());
}

Game::~Game() {}

void Game::Initialize()
{
	std::srand(static_cast<unsigned int>(std::time(0)));

	try
	{
		///gui menu
		auto windowWidth = tgui::bindWidth(tgui_menu);
		auto windowHeight = tgui::bindHeight(tgui_menu);
		
		tgui_menu.setFont(tgui::Font("fonts\\DejaVuSans.ttf"));
		tgui_game.setFont(tgui::Font("fonts\\DejaVuSans.ttf"));

		tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("widgets\\Black.txt");

		tgui::Picture::Ptr picture = std::make_shared<tgui::Picture>("tlo_inkspace.png");
		picture->setSize(windowWidth, windowHeight);
		tgui_menu.add(picture);
		tgui_game.add(picture);

		tgui::Button::Ptr button_start_game = theme->load("Button");
		button_start_game->setSize(256, 64);
		button_start_game->setPosition( p_model->get_app_width()*3/4 - button_start_game->getSize().x/2, 196);
		button_start_game->setText("Start Game!");
		button_start_game->connect("pressed", [&]() {state = State::in_game;});
		tgui_menu.add(button_start_game);		

		tgui::TextBox::Ptr textBox_nickname = theme->load("TextBox");
		textBox_nickname->setText("your nick name");
		textBox_nickname->setSize(256, 40);
		textBox_nickname->setPosition(p_model->get_app_width() * 3 / 4 - textBox_nickname->getSize().x / 2, 276);
		textBox_nickname->setTextSize(24);
		textBox_nickname->setMaximumCharacters(20);
		tgui_menu.add(textBox_nickname, "nickname");

		tgui::Label::Ptr label_volume = std::make_shared<tgui::Label>();
		label_volume = theme->load("Label");
		label_volume->setSize(128, 32);
		label_volume->setPosition(p_model->get_app_width() * 3 / 4 - label_volume->getSize().x / 2, 324);
		label_volume->setTextColor(sf::Color(200, 200, 200));
		label_volume->setTextSize(24);
		label_volume->setText("Volume:");
		tgui_menu.add(label_volume);

		tgui::Slider::Ptr slider = theme->load("Slider");
		slider->setValue(2);
		slider->setMaximum(100);
		slider->setMinimum(0);
		slider->setSize(256, slider->getSize().x);
		slider->setPosition(p_model->get_app_width() * 3 / 4 - slider->getSize().x / 2, 372);
		tgui_menu.add(slider, "slider_volume");

		tgui::Button::Ptr button_rank = theme->load("Button");
		button_rank->setSize(tgui::bindSize(button_start_game));
		button_rank->setPosition(p_model->get_app_width() * 3 / 4 - button_rank->getSize().x / 2, 420);
		button_rank->setText("Go to ranking");
		button_rank->connect("pressed", [&]() {ShellExecute(NULL, "open", "http://matrixxx.cba.pl/index.php/rankings/tetris", "", NULL, SW_SHOWNORMAL);});
		tgui_menu.add(button_rank);

		tgui::Button::Ptr button_exit = theme->load("Button");
		button_exit->setSize(tgui::bindSize(button_start_game));
		button_exit->setPosition(p_model->get_app_width() * 3 / 4 - button_exit->getSize().x/2, 500);
		button_exit->setText("Exit game");
		button_exit->connect("pressed", [&]() {state = State::exit;});
		tgui_menu.add(button_exit);

		//game gui
		tgui::Label::Ptr label_scores = theme->load("Label");
		label_scores->setSize(128, 64);
		label_scores->setPosition(p_model->get_app_width() * 3 / 4 - label_scores->getSize().x / 2, 196);
		label_scores->setTextColor(sf::Color(200, 200, 200));
		label_scores->setTextSize(24);

		tgui_game.add(label_scores, "label_scores");

		tgui::Label::Ptr label_time = theme->load("Label");
		label_time->setSize(128, 64);
		label_time->setPosition(p_model->get_app_width() * 3 / 4 - label_time->getSize().x / 2, 260);
		label_time->setTextColor(sf::Color(200, 200, 200));
		label_time->setTextSize(24);

		tgui_game.add(label_time, "label_time");

		tgui::Label::Ptr label_hold = theme->load("Label");
		label_hold->setSize(128, 64);
		label_hold->setText("Hold");
		label_hold->setPosition(p_model->get_app_width() * 5 / 4 / 2 - label_hold->getSize().x / 2, 340);
		label_hold->setTextColor(sf::Color(200, 200, 200));
		label_hold->setTextSize(24);

		tgui_game.add(label_hold, "label_hold");

		tgui::Label::Ptr label_next = theme->load("Label");
		label_next->setSize(128, 64);
		label_next->setText("Next");
		label_next->setPosition(p_model->get_app_width() * 7 / 4 / 2 - label_hold->getSize().x / 2, 340);
		label_next->setTextColor(sf::Color(200, 200, 200));
		label_next->setTextSize(24);
		
		tgui_game.add(label_next, "label_next");
	}
	catch (const tgui::Exception& e)
	{
		MessageBox(nullptr, "Failed to load: " + *e.what(), "Error", MB_OK | MB_ICONERROR);
	}
	catch (std::bad_alloc & e)
	{
		MessageBox(nullptr, "Failed" + *e.what(), "Error", MB_OK | MB_ICONERROR);
	}
}

void Game::run()
{
	theme.play();
	theme.setLoop(true);

	while (state != State::exit)
	{
		switch (state)
		{
		case Game::State::in_game:
			game_loop();
			break;
		case Game::State::in_menu:
			menu_loop();
			break;
		case Game::State::exit:
			return;
		case Game::State::game_over:
			state = State::in_menu;
			break;
		default:
			break;
		}
	}
}

std::string int_to_time(int n)
{
	int minutes, hours;

	hours = n / 3600;
	n = n % 3600;
	minutes = n / 60;
	n %= 60;

	std::ostringstream res;
	res << hours << ":" << minutes << ":" << n;
	
	return res.str();
}