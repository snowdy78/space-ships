#include "game/GameGlobals.hpp"
#include "game/PlayerShip.hpp"

GameGlobals::GameGlobals(sf::RenderTarget &target, func_update_on_move update_on_move, const OnlineTraits &traits) 
    : camera(target, update_on_move), field(&camera), traits(std::make_unique<OnlineTraits>(traits))
{
    field.appendShip<PlayerShip>(&camera);
    player = field[0];
}

void GameGlobals::create(sf::RenderTarget &target, func_update_on_move update_on_move)
{
    instance_ptr.reset(new GameGlobals(target, update_on_move));
}

GameGlobals &GameGlobals::instance() 
{
    return *instance_ptr.get();
}

bool GameGlobals::exist() 
{
    return instance_ptr.get();
}

void GameGlobals::reset(sf::RenderTarget &new_target, func_update_on_move new_update_on_move)
{
    if (!instance_ptr)
        return;
    instance_ptr.reset(new GameGlobals(new_target, new_update_on_move)); 
}

void GameGlobals::clear()
{
    if (!instance_ptr)
        return;
    instance_ptr = nullptr;
}
