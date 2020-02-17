#include <SDL2/SDL.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
		return false;
	}
	SDL_Log("SDL initiated successfully");

	window = SDL_CreateWindow("sdlrts", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);
	if(window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
		return false;
	}
	
	return true;
}

int main(int argc, char* argv[])
{
  bool isRunning = Init();

	while (isRunning)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
				if (e.type == SDL_QUIT) 
				{
						isRunning = false;
						break;
				}
				else if(e.type == SDL_KEYDOWN)
				{
					if(e.key.keysym.sym == SDLK_ESCAPE)
					{
						isRunning = false;
						break;
					}
				}
		}
		
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
  return 0;
}