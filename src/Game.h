#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "GameObject.h"

class AssetManager;
class Tile;

class Game
{
private:
  SDL_Window* _window;
  
  bool _isRunning;
  float ticksLastFrame;

  std::vector<GameObject*> _gameobjects;
  //TODO: move Tile to a ECS component
  std::vector<Tile*> _tiles;

  
  SDL_Point _startSelection;    //screen point  
  SDL_Point _endSelection;      //screen point
  SDL_Rect _selectionRect;      //screen point
  SDL_Rect _selectionCollider;  //world point
  bool _rangeSelection;
  bool _singleSelection;

  void UpdateSelectionRect();
  void GameObjectSelection();

  std::vector<SDL_Rect> _targets;
  std::vector<GameObject*> _selectedUnits;
  void MoveSelectedUnits(glm::vec2 target);

  void LoadMap();

  // AssetManager* _assetManager;

public:
  static SDL_Renderer* renderer;
  static AssetManager* assetManager;
  static SDL_Rect* camera;
  
  Game();
  ~Game(){}
  bool Init(int width, int height);
  void Quit();
  void ProcessInput();
  void Update();
  void Render();
  bool IsRunning() const { return _isRunning; }
  void UpdateCamera(int x, int y);
};

#endif