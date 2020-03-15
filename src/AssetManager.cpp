#include "AssetManager.h"
#include "TextureManager.h"

// AssetManager::AssetManager()
// {
  
// }

// AssetManager::~AssetManager()
// {

// }

void AssetManager::AddTexture(std::string id, const char* filePath)
{
  //TODO: check whether texture was loaded
  _textures.emplace(id, TextureManager::Load(filePath));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
  //TODO: check whether texture exists
  return _textures[id];
}

void AssetManager::Release()
{
  for(auto const& [id, texture] : _textures)
  {
    SDL_DestroyTexture(texture);
  }
  _textures.clear();
}