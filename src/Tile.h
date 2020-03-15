#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include <string>
#include "../lib/glm/glm.hpp"

class Tile
{
public:
  Tile(int x, int y, SDL_Rect source, SDL_Texture* texture);
  ~Tile();
  void Render();

private:
  glm::vec2 _position;
  SDL_Texture* _texture;
  SDL_Rect _source;
  SDL_Rect _destination;

};

#endif