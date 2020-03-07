#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(glm::vec2 position) : _position(position)
{
  _collider = new SDL_Rect {position.x, position.y, 10, 10};
  _color = {255, 0, 0, 255};
}

GameObject::~GameObject()
{
  delete _collider;
}

void GameObject::Update()
{
}

void GameObject::Render()
{
  SDL_SetRenderDrawColor(Game::_renderer, _color.r, _color.g, _color.b, _color.a);
  SDL_RenderFillRect(Game::_renderer, _collider);
}

void GameObject::Select()
{
  _color = {0, 0, 255, 255};
}

void GameObject::Deselect()
{
  _color = {255, 0, 0, 255};
}

SDL_Rect* GameObject::GetCollider() const
{
  return _collider;
}