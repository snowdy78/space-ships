#pragma once

#include "components/SoundManager.hpp"
#include "decl.hpp"
#include "components/EffectManager.hpp"
#include "SpaceField.hpp"
#include "actions/ActionManager.hpp"
#include "components/TargetCamera.hpp"
#include "coop/TcpRouter.hpp"
#include "coop/UdpRouter.hpp"

class GameGlobals 
{
public:
    struct OnlineTraits
    {
		OnlineTraits(sf::IpAddress host_ip, uint16_t host_port = sf::Socket::AnyPort);
    	bool is_host{false};
		std::shared_ptr<TcpRouter> tcp;
		std::shared_ptr<UdpRouter> udp;
		sf::SocketSelector selector;
    };
private:
    using func_update_on_move = const std::function<void()> &;
    
    GameGlobals(sf::RenderTarget &target, func_update_on_move update_on_move);
public:
    static void create(sf::RenderTarget &target, func_update_on_move update_on_move);
    static GameGlobals &instance();
    static bool exist();
    static void reset(sf::RenderTarget &new_target, func_update_on_move new_update_on_move);
    static void clear();
	void createOnline(OnlineTraits &&traits);

private:
    inline static std::unique_ptr<GameGlobals> instance_ptr = nullptr;
public:
    TargetCamera camera;
    ActionManager action_manager;
    SpaceField field;
    AbstractShip *player = nullptr;
    EffectManager effect_manager;
    SoundManager sound_manager;
    std::unique_ptr<OnlineTraits> online{nullptr};
};
