#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

class Game
{
private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  bool _isRunning;
  float ticksLastFrame;

  std::vector<SDL_Rect> _objects;
  SDL_Point _startSelection;
  SDL_Point _endSelection;
  SDL_Rect _selectionRect;

  void UpdateSelectionRect();

public:
  Game();
  ~Game(){};
  bool Init(int width, int height);
  void Quit();
  void Input();
  void Update();
  void Render();
  bool IsRunning()const { return _isRunning; }
};

#endif