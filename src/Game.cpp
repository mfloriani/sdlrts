#include "Game.h"
#include "Constants.h"
// #include <string>

Game::Game() : _isRunning(false), _startSelection(), _endSelection(), _selectionRect()
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
    switch (e.type)
    {
    case SDL_QUIT:
    {
      _isRunning = false;
      break;
    }
    case SDL_KEYDOWN:
    {
      _isRunning = false;
      break;
    }
    case SDL_MOUSEMOTION:
    {
      int x, y;
      Uint32 buttonState = SDL_GetMouseState(&x, &y);
      if(buttonState & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        SDL_Log("Left button pressed during motion");
        _endSelection = {x, y};
        UpdateSelectionRect();
      }
      if(buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT))
      {
        SDL_Log("Right button pressed during motion");
      }
      if(buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
      {
        SDL_Log("Middle button pressed during motion");
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
      Uint8 button = e.button.button;
      Uint8 pressed = e.button.state == SDL_PRESSED;
      Uint8 released = e.button.state == SDL_RELEASED;

      if(button == SDL_BUTTON_LEFT)
      {
        if(pressed)
        {
          SDL_Log("Left button pressed");
          _startSelection = {e.button.x, e.button.y};
          _endSelection = _startSelection;
          
          // SDL_Rect rect = {e.button.x, e.button.y, 10, 10};
          // _objects.push_back(rect);
          
        }
        if(released)
        {
          SDL_Log("Left button released");
          _startSelection = {};
          _endSelection = {};
          UpdateSelectionRect();
        }
      }
      if(button == SDL_BUTTON_RIGHT)
      {
        if(pressed)
        {
          SDL_Log("Right button pressed");
        }
        if(released)
        {
          SDL_Log("Right button released");
        }
      }
      if(button == SDL_BUTTON_MIDDLE)
      {
        if(pressed)
        {
          SDL_Log("Middle button pressed");
        }
        if(released)
        {
          SDL_Log("Middle button released");
        }
      }
      break;
    }
    default:
      break;
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

  // SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
  // for(auto rect : _objects)
  // {
  //   SDL_RenderDrawRect(_renderer, &rect);
  // }

  SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 0);
  SDL_RenderDrawRect(_renderer, &_selectionRect);

  SDL_RenderPresent(_renderer);
}

void Game::Quit()
{
  SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Game::UpdateSelectionRect()
{
  int width = _endSelection.x - _startSelection.x;
  int height = _endSelection.y - _startSelection.y;
  _selectionRect = {_startSelection.x, _startSelection.y, width, height};
}