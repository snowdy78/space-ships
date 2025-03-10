#include "game/GameObject.hpp"
#include "game/GameObjectFabric.hpp"

GameObject::GameObject()
    : id(GameObjectFabric::instance().push(this)) 
{   
}


