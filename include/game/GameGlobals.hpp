#pragma once

#include "components/SoundManager.hpp"
#include "decl.hpp"
#include "components/EffectManager.hpp"
#include "SpaceField.hpp"
#include "actions/ActionManager.hpp"
#include "components/TargetCamera.hpp"

class GameGlobals 
{
public:
    struct OnlineTraits
    {
       OnlineTraits() noexcept {}
    private:
        bool is_host{false};
    };
private:
    using func_update_on_move = const std::function<void()> &;
    
    GameGlobals(sf::RenderTarget &target, func_update_on_move update_on_move, const OnlineTraits &traits = OnlineTraits());
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
    SoundManager sound_manager;
    std::unique_ptr<OnlineTraits> traits{nullptr};
};
