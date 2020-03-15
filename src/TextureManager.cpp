#include "TextureManager.h"

SDL_Texture* TextureManager::Load(const char* fileName)
{
  SDL_Surface* surface = IMG_Load(fileName);
  if(!surface)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture %s: %s", fileName, SDL_GetError());
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void TextureManager::Render(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip)
{
  SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
}