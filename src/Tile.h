#ifndef TILE_H
#define TILE_H

#include <string>
#include "SDL2/SDL.h"
#include "../lib/glm/glm.hpp"

class Tile
{
public:
  Tile(int x, int y, SDL_Rect source, std::string textureId);
  ~Tile();
  void Render();

private:
  glm::vec2 _position;
  SDL_Texture* _texture;
  SDL_Rect _source;
  SDL_Rect _destination;

};

#endif