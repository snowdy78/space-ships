#pragma once

#include "decl.hpp"
#include "components/EffectManager.hpp"
#include "SpaceField.hpp"
#include "actions/ActionManager.hpp"
#include "components/TargetCamera.hpp"
#include "game/Ship.hpp"

class GameGlobals 
{
    using func_update_on_move = const std::function<void()> &;
    GameGlobals(sf::RenderTarget &target, func_update_on_move update_on_move);
public:
    static void create(sf::RenderTarget &target, func_update_on_move update_on_move);
    static GameGlobals &instance();
    static bool exist();
    void reset(sf::RenderTarget &new_target, func_update_on_move new_update_on_move);
    void clear();
private:
    inline static std::unique_ptr<GameGlobals> instance_ptr = nullptr;
public:
    TargetCamera camera;
    ActionManager action_manager;
    SpaceField field;
    AbstractShip *player = nullptr;
    EffectManager effect_manager;
};

inline GameGlobals::GameGlobals(sf::RenderTarget &target, func_update_on_move update_on_move) 
    : camera(target, update_on_move), field(&camera)
{
    field.appendShip<Ship>(&camera);
    player = field[0];
}

inline void GameGlobals::create(sf::RenderTarget &target, func_update_on_move update_on_move)
{
    instance_ptr.reset(new GameGlobals(target, update_on_move));
}

inline GameGlobals &GameGlobals::instance() 
{
    return *instance_ptr.get();
}

inline bool GameGlobals::exist() 
{
    return instance_ptr.get();
}

inline void GameGlobals::reset(sf::RenderTarget &new_target, func_update_on_move new_update_on_move)
{
    if (!instance_ptr)
        return;
    instance_ptr.reset(new GameGlobals(new_target, new_update_on_move)); 
}

inline void GameGlobals::clear()
{
    if (!instance_ptr)
        return;
    instance_ptr = nullptr;
}
