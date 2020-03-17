#include <iostream>
#include <string>
#include "../lib/glm/glm.hpp"
#include "../lib/lua/sol.hpp"
#include "Game.h"
#include "Constants.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Tile.h"

//TODO: I might move these public static members to behind a const method
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager();
SDL_Rect* Game::camera = new SDL_Rect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT});

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

  _window = SDL_CreateWindow("sdlrts", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
  if (_window == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
    return false;
  }

  renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL_Error: %s", SDL_GetError());
    return false;
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could not initialize! SDL_Image Error: %s", IMG_GetError());
		return false;
	}

  assetManager->AddTexture("tiles-spritesheet", "./assets/images/spritesheet_32x32.png");
  
  LoadMap();

  _isRunning = true;
  return true;
}

void Game::ProcessInput()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    //TODO: create commands from the inputs, store into an array and process it inside update()

    const Uint8* state = SDL_GetKeyboardState(NULL);

    switch (e.type)
    {
    case SDL_QUIT:
    {
      _isRunning = false;
      break;
    }
    case SDL_KEYDOWN:
    {

      switch (e.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        {
          _isRunning = false;
        }
        break;
      
      case SDLK_UP:
      case SDLK_DOWN:
      case SDLK_LEFT:
      case SDLK_RIGHT:
        {
          if(state[SDL_SCANCODE_UP])
          // if(e.key.keysym.sym == SDLK_UP)
          {
            SDL_Log("UP");
            UpdateCamera(0, -CAMERA_VELOCITY);
          }
          if(state[SDL_SCANCODE_DOWN])
          // if(e.key.keysym.sym == SDLK_DOWN)
          {
            SDL_Log("DOWN");
            UpdateCamera(0, CAMERA_VELOCITY);
          }
          if(state[SDL_SCANCODE_LEFT])
          // if(e.key.keysym.sym == SDLK_LEFT)
          {
            SDL_Log("LEFT");
            UpdateCamera(-CAMERA_VELOCITY, 0);
          }
          if(state[SDL_SCANCODE_RIGHT])
          // if(e.key.keysym.sym == SDLK_RIGHT)
          {
            SDL_Log("RIGHT");
            UpdateCamera(CAMERA_VELOCITY, 0);
          }
        }
        break;

      default:
        break;
      }
      
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
          _endSelection = {_startSelection.x + 1, _startSelection.y + 1};
          UpdateSelectionRect();
        }
        if (released)
        {
          SDL_Log("Left button released");

          if (_singleSelection)
          {
            //BUG: difficult to select object while is moving. Maybe create a bigger rect that has the click point as the center
            _endSelection = {_startSelection.x + 1, _startSelection.y + 1};
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
          SDL_Point worldPoint = ScreenToWorldPoint(e.button.x, e.button.y);
          glm::vec2 target {worldPoint.x, worldPoint.y};
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
          SDL_Point worldPoint = ScreenToWorldPoint(e.button.x, e.button.y);
          glm::vec2 position {worldPoint.x, worldPoint.y};
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

  for(auto tile : _tiles)
  {
    tile->Update();
  }
}

void Game::Render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for(auto tile : _tiles)
  {
    if(SDL_HasIntersection(tile->Collider(), camera))
    {
      tile->Render();
    }
  }

  for (auto go : _gameobjects)
  {
    go->Render();
  }

  if (_rangeSelection)
  {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_RenderFillRect(renderer, &_selectionRect);
  }

  for (auto target : _targets)
  {
    SDL_SetRenderDrawColor(renderer, 252, 244, 3, 255);
    target.x -= camera->x;
    target.y -= camera->y;
    SDL_RenderDrawRect(renderer, &target);
  }

  SDL_RenderPresent(renderer);
}

void Game::Quit()
{
  assetManager->Release();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(_window);
  IMG_Quit();
  SDL_Quit();
}

void Game::UpdateSelectionRect()
{
  int width = _endSelection.x - _startSelection.x;
  int height = _endSelection.y - _startSelection.y;
  _selectionRect = {_startSelection.x, _startSelection.y, width, height};
  // SDL_Log("_selectionRect %d %d %d %d", _selectionRect.x, _selectionRect.y, _selectionRect.w, _selectionRect.h);

  SDL_Point startSelectionWorldPoint = ScreenToWorldPoint(_startSelection.x, _startSelection.y);
  SDL_Point endSelectionWorldPoint = ScreenToWorldPoint(_endSelection.x, _endSelection.y);
  SDL_Rect worldPoint { startSelectionWorldPoint.x, startSelectionWorldPoint.y, width, height };
  // SDL_Log("worldPoint %d %d %d %d", worldPoint.x, worldPoint.y, worldPoint.w, worldPoint.h);

  _selectionCollider = worldPoint;
  if (_selectionRect.w < 0)
  {
    _selectionCollider.x = endSelectionWorldPoint.x;
    _selectionCollider.w = glm::abs(_selectionCollider.w);
  }
  if (_selectionRect.h < 0)
  {
    _selectionCollider.y = endSelectionWorldPoint.y;
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

void Game::LoadMap()
{
  sol::state lua;
  lua.open_libraries();
  lua.script_file("./assets/maps/map.lua");

  sol::table map = lua["map"];
  int width = map["width"];
  int height = map["height"];
  int tileCount = map["tilecount"];
  sol::table tiles = map["tiles"];
  // SDL_Log("w=%i h=%i", width, height);

  int x = 0;
  int y = 0;
  int countX = 0;
  for(int i = 1; i <= tiles.size(); ++i)
  {
    int type = tiles.get<int>(i);
    // SDL_Log("i=%i type=%i x=%i  y=%i", i, type, x, y);
    
    SDL_Rect source {(type-1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
    
    _tiles.push_back(new Tile(x, y, source, assetManager->GetTexture("tiles-spritesheet")));
    
    x += TILE_SIZE;
    if((i % width) == 0)
    {
      // SDL_Log("<br> %i", i);
      x = 0;
      y += TILE_SIZE;
    }
  }

}

void Game::UpdateCamera(int x, int y)
{
  camera->x += x;
  camera->y += y;

  camera->x = camera->x < 0 ? 0 : camera->x;
  camera->y = camera->y < 0 ? 0 : camera->y;
  
  camera->x = camera->x > (MAP_WIDTH - camera->w) ? (MAP_WIDTH - camera->w) : camera->x;
  camera->y = camera->y > (MAP_HEIGHT - camera->h) ? (MAP_HEIGHT - camera->h) : camera->y;

  SDL_Log("camx=%i camy=%i", camera->x, camera->y);
}

SDL_Point Game::ScreenToWorldPoint(int x, int y)
{
  return SDL_Point({x + camera->x, y + camera->y});
}