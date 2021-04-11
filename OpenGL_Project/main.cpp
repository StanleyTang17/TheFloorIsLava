#include"Game.h"

int main()
{
	Game* game = new Game("OpenGL Project", 740, 740, 4, 4, GL_FALSE);
	
	// MAIN LOOP
	while (!game->get_window_should_close())
	{
		game->update();
		game->render();
	}

	delete game;
	game = nullptr;

	return 0;
}