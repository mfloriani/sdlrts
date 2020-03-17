#include "Tile.h"
#include "TextureManager.h"
#include "Constants.h"

Tile::Tile(int x, int y, SDL_Rect source, SDL_Texture* texture) : 
  _position(glm::vec2(x,y)), 
  _source(source), 
  _texture(texture), 
  _destination({x, y, TILE_SIZE, TILE_SIZE}),
  _collider(new SDL_Rect({x, y, TILE_SIZE, TILE_SIZE}))
{
  
}

void Tile::Update()
{
  _destination.x = _position.x - Game::camera->x;
  _destination.y = _position.y - Game::camera->y;
}

void Tile::Render()
{
  TextureManager::Render(_texture, _source, _destination, SDL_FLIP_NONE);
}