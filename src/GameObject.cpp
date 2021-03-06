#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(glm::vec2 position) : _position(position), _targetLocation(glm::vec2(0,0)), _hasTarget(false), _direction(glm::vec2(0,0)), _color({255, 0, 0, 255})
{
  _collider = new SDL_Rect {position.x, position.y, 10, 10};
  
  SDL_Log("new go x=%f y=%f", position.x, position.y);
  SDL_Log("collider x=%d y=%d w=%d h=%d", _collider->x, _collider->y, _collider->w, _collider->h);
}

GameObject::~GameObject()
{
  delete _collider;
}

void GameObject::Update()
{
  if(_hasTarget)
  {
    float distance = glm::distance(_position, _targetLocation);
    //SDL_Log("Distance %f", distance);
    //TODO: handle fix distance value
    if(distance < 1)
    {
      _hasTarget = false;
      _direction = glm::vec2(0,0);
    }
    _position += _direction;
    
    _collider->x = _position.x;
    _collider->y = _position.y;
  }
}

void GameObject::Render()
{
  SDL_Rect sprite { _position.x - Game::camera->x, _position.y - Game::camera->y, _collider->w, _collider->h };
  SDL_SetRenderDrawColor(Game::renderer, _color.r, _color.g, _color.b, _color.a);
  SDL_RenderFillRect(Game::renderer, &sprite);
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

void GameObject::Move(glm::vec2 location)
{
  _hasTarget = true;
  _targetLocation = location;
  _direction = glm::normalize(_targetLocation - _position);
  SDL_Log("target %f %f", _targetLocation.x, _targetLocation.y);
  SDL_Log("position %f %f", _position.x, _position.y);
  SDL_Log("direction %f %f", _direction.x, _direction.y);
}