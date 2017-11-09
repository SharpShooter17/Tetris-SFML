#include <Windows.h>

#include "Game.h"
#include "model.h"

int main()
{
	try
	{
		Model model{};

		const sf::ContextSettings settings = sf::ContextSettings();

		Game game{ &model, settings };
		game.Initialize();
		game.run();
	}
	catch (Model::Exceptions &e)
	{
		MessageBox(nullptr, e.what(), "Error", MB_ICONERROR);
	}

	return 0;
};