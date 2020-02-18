#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game
{
private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  bool _isRunning;
  float ticksLastFrame;

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