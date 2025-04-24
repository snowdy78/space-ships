#include "game/GameObject.hpp"
#include "game/GameObjectFabric.hpp"

GameObject::GameObject()
    : id(GameObjectFabric::instance().push(this)) 
{   
}

size_t GameObject::getGameObjectId() const
{
	return id;
}

GameObject::~GameObject() = default;
