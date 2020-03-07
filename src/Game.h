#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "GameObject.h"


class Game
{
private:
  SDL_Window* _window;
  
  bool _isRunning;
  float ticksLastFrame;

  std::vector<GameObject*> _gameobjects;

  SDL_Point _startSelection;
  SDL_Point _endSelection;
  SDL_Rect _selectionRect;
  SDL_Rect _selectionCollider;
  bool _rangeSelection;
  bool _singleSelection;

  void UpdateSelectionRect();
  void GameObjectSelection();

public:
  static SDL_Renderer* _renderer;

  Game();
  ~Game(){};
  bool Init(int width, int height);
  void Quit();
  void ProcessInput();
  void Update();
  void Render();
  bool IsRunning()const { return _isRunning; }
};

#endif