#include "Game.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
	Game game;
  game.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (game.IsRunning())
	{
		game.ProcessInput();
		game.Update();
		game.Render();
	}

	game.Quit();
  return 0;
}