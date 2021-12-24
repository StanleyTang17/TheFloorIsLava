#include"engine/Game.h"

// CURRENT LINES OF CODE: 5305

int main()
{
	Game* game = new Game("OpenGL Project", 1270, 800, 4, 6, GL_FALSE);
	
	// MAIN LOOP
	while (!game->get_window_should_close())
	{
		game->update();
		game->render();
	}
    return 0;
}