#pragma once

#include "components/SoundManager.hpp"
#include "decl.hpp"
#include "components/EffectManager.hpp"
#include "coop/TcpRouter.hpp"
#include "coop/UdpRouter.hpp"
#include "GameSession.hpp"
#include "components/Background.hpp"

class GameManager 
{
public:
    struct OnlineTraits
    {
		OnlineTraits(sf::IpAddress host_ip, uint16_t host_port = sf::Socket::AnyPort);
    	std::shared_ptr<TcpRouter> tcp;
		std::shared_ptr<UdpRouter> udp;
		sf::SocketSelector selector;
    private:
		friend class GameManager;
    	bool is_host{ false };
    };
private:
    using func_update_on_move = const std::function<void()> &;
    GameManager(sf::RenderTarget &target);
	void createHost(OnlineTraits &&traits);
	void createClient(OnlineTraits &&traits);

public:

    static void create(sf::RenderTarget &target);
	static GameSession *session();
	static void host(sf::RenderTarget &target, OnlineTraits &&traits);
	static void client(sf::RenderTarget &target, OnlineTraits &&traits);
    static GameManager &instance();
    static bool exist();
    static void reset(sf::RenderTarget &new_target);
    static void clear();
	void createOnline(OnlineTraits &&traits);
	bool existOnline() const;

private:
    inline static std::unique_ptr<GameManager> instance_ptr = nullptr;
	GameSession m_session;
public:
    EffectManager effect_manager;
    SoundManager sound_manager;
    std::unique_ptr<OnlineTraits> online{nullptr};
};
