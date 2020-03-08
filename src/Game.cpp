#include "Game.h"
#include "Constants.h"
// #include <string>
#include "../lib/glm/glm.hpp"

SDL_Renderer *Game::_renderer;

Game::Game() : _isRunning(false), _startSelection(), _endSelection(), _selectionRect(), _rangeSelection(false), _singleSelection(false)
{
}

bool Game::Init(int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
    return false;
  }

  //TODO: uncomment SDL_WINDOW_BORDERLESS
  _window = SDL_CreateWindow("sdlrts", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, NULL /*SDL_WINDOW_BORDERLESS*/);
  if (_window == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
    return false;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (_renderer == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
    return false;
  }

  _isRunning = true;
  return true;
}

void Game::ProcessInput()
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
      if (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        SDL_Log("Left button pressed during motion");
        _rangeSelection = true;
        _singleSelection = false;
        _endSelection = {x, y};
        UpdateSelectionRect();
      }
      if (buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT))
      {
        SDL_Log("Right button pressed during motion");
      }
      if (buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
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

      if (button == SDL_BUTTON_LEFT)
      {
        if (pressed)
        {
          SDL_Log("Left button pressed");
          _singleSelection = true;
          _rangeSelection = false;
          _startSelection = {e.button.x, e.button.y};
          _endSelection = {_startSelection.x + 10, _startSelection.y + 10};
          UpdateSelectionRect();
        }
        if (released)
        {
          SDL_Log("Left button released");

          if (_singleSelection)
          {
            _endSelection = {_startSelection.x + 10, _startSelection.y + 10};
          }
          UpdateSelectionRect();

          _rangeSelection = false;
          _singleSelection = false;
        }
      }
      if (button == SDL_BUTTON_RIGHT)
      {
        if (pressed)
        {
          SDL_Log("Right button pressed");
        }
        if (released)
        {
          SDL_Log("Right button released");
          glm::vec2 target = glm::vec2(e.button.x, e.button.y);
          SDL_Rect rect = {target.x, target.y, 10, 10};
          _targets.push_back(rect);

          MoveSelectedUnits(target);
        }
      }
      if (button == SDL_BUTTON_MIDDLE)
      {
        if (pressed)
        {
          SDL_Log("Middle button pressed");
          glm::vec2 position;
          position.x = e.button.x;
          position.y = e.button.y;
          GameObject *go = new GameObject(position);
          _gameobjects.push_back(go);
        }
        if (released)
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
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_LENGTH));
  float deltatime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
  ticksLastFrame = SDL_GetTicks();
  // std::string dt;
  // dt = std::to_string(deltatime);
  // SDL_Log(dt.c_str());

  for (auto go : _gameobjects)
  {
    go->Update();
  }

  if (_rangeSelection || _singleSelection)
  {
    GameObjectSelection();
  }
}

void Game::Render()
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);

  for (auto go : _gameobjects)
  {
    go->Render();
  }

  if (_rangeSelection)
  {
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(_renderer, &_selectionRect);
  }

  for (auto target : _targets)
  {
    SDL_SetRenderDrawColor(_renderer, 252, 244, 3, 255);
    SDL_RenderDrawRect(_renderer, &target);
  }

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
  // SDL_Log("_selectionRect %d %d %d %d", _selectionRect.x, _selectionRect.y, _selectionRect.w, _selectionRect.h);

  _selectionCollider = _selectionRect;
  if (_selectionRect.w < 0)
  {
    _selectionCollider.x = _endSelection.x;
    _selectionCollider.w = glm::abs(_selectionCollider.w);
  }
  if (_selectionRect.h < 0)
  {
    _selectionCollider.y = _endSelection.y;
    _selectionCollider.h = glm::abs(_selectionCollider.h);
  }
  // SDL_Log("_selectionCollider %d %d %d %d", _selectionCollider.x, _selectionCollider.y, _selectionCollider.w, _selectionCollider.h);
}

void Game::GameObjectSelection()
{
  _selectedUnits.clear();
  for (auto go : _gameobjects)
  {
    // SDL_Log("SDL_HasIntersection collider(%d %d %d %d)", _selectionCollider.x, _selectionCollider.y, _selectionCollider.w, _selectionCollider.h);
    // SDL_Log("SDL_HasIntersection go(%d %d %d %d)", go->GetCollider()->x, go->GetCollider()->y, go->GetCollider()->w, go->GetCollider()->h);
    if (SDL_HasIntersection(&_selectionCollider, go->GetCollider()))
    {
      // SDL_Log("Select");
      go->Select();
      _selectedUnits.push_back(go);
    }
    else
    {
      // SDL_Log("Deselect");
      go->Deselect();
    }
  }
}

void Game::MoveSelectedUnits(glm::vec2 target)
{
  for (auto go : _selectedUnits)
  {
    go->Move(target);
  }
}