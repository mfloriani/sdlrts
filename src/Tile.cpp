#include "Tile.h"
#include "TextureManager.h"
#include "Constants.h"


Tile::Tile(int x, int y, SDL_Rect source, std::string textureId) 
  : _position(glm::vec2(x,y)), _source(source)
{
  //_texture = Game::AssetManager()->GetTexture(textureId);
  _destination = {x, y, TILE_SIZE, TILE_SIZE};
}

void Tile::Render()
{
  TextureManager::Render(_texture, _source, _destination, SDL_FLIP_NONE);
}