#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL2/SDL.h>
#include "../lib/glm/glm.hpp"

class GameObject
{
public:
  GameObject(glm::vec2 position);
  ~GameObject();
  void Update();
  void Render();

  void Select();
  void Deselect();

  SDL_Rect* GetCollider() const;

private:
  glm::vec2 _position;
  SDL_Rect* _collider;
  SDL_Color _color;
};

#endif