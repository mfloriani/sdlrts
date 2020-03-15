#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetManager
{
public:
  AssetManager(){};
  ~AssetManager(){};
  void Release();
  void AddTexture(std::string id, const char* filePath);
  SDL_Texture* GetTexture(std::string id);

private:
  std::map<std::string, SDL_Texture*> _textures;
};

#endif