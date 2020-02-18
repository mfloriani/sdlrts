#include "Game.h"
#include "Constants.h"
// #include <string>

Game::Game() : _isRunning(false)
{

}

bool Game::Init(int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
		return false;
	}
	
	_window = SDL_CreateWindow("sdlrts", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
	if(_window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
		return false;
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if(_renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
		return false;
	}
	
  _isRunning = true;
	return true;
}

void Game::Input()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
      if (e.type == SDL_QUIT) 
      {
          _isRunning = false;
          break;
      }
      else if(e.type == SDL_KEYDOWN)
      {
        if(e.key.keysym.sym == SDLK_ESCAPE)
        {
          _isRunning = false;
          break;
        }
      }
  }
}

void Game::Update()
{
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_LENGTH));
  float deltatime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
  ticksLastFrame = SDL_GetTicks();
  // std::string dt;
  // dt = std::to_string(deltatime);
  // SDL_Log(dt.c_str());
}

void Game::Render()
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);
  SDL_RenderPresent(_renderer);
}

void Game::Quit()
{
  SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}