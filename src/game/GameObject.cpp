#include "game/GameObject.hpp"
#include "game/GameObjectFactory.hpp"

GameObject::GameObject()
    : id(GameObjectFactory::instance().push(this)) 
{   
}

size_t GameObject::hash() const
{
	return id;
}

GameObject::~GameObject() = default;
