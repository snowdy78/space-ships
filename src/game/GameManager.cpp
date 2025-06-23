#include "game/GameManager.hpp"
#include "game/ships/PlayerShip.hpp"

GameManager::OnlineTraits::OnlineTraits(sf::IpAddress host_ip, uint16_t host_port)
	: tcp(std::make_shared<TcpRouter>(host_ip, host_port)),
	  udp(std::make_shared<UdpRouter>())
{
	udp->bind(host_port, host_ip);
}

GameManager::GameManager(sf::RenderTarget &target)
	: m_session(target)
{
}

void GameManager::createHost(OnlineTraits &&traits)
{
	traits.is_host = true;
	createOnline(std::move(traits));
}

void GameManager::createClient(OnlineTraits &&traits)
{
	traits.is_host = false;
	createOnline(std::move(traits));
}

void GameManager::create(sf::RenderTarget &target)
{
	instance_ptr.reset(new GameManager(target));
}

GameSession *GameManager::session()
{
	if (exist())
		return &instance().m_session;
#ifdef SPACE_SHIP_DEBUG
	std::cout << "session does not exist.\n";
#endif
	return nullptr;
}

void GameManager::host(sf::RenderTarget &target, OnlineTraits &&traits)
{
	instance_ptr.reset(new GameManager(target));
	instance_ptr->createHost(std::move(traits));
}

void GameManager::client(sf::RenderTarget &target, OnlineTraits &&traits)
{
	instance_ptr.reset(new GameManager(target));
	instance_ptr->createClient(std::move(traits));
}

GameManager &GameManager::instance()
{
	return *instance_ptr;
}

bool GameManager::exist()
{
	return instance_ptr.get();
}

void GameManager::reset(sf::RenderTarget &new_target)
{
	if (!instance_ptr)
		return;
	instance_ptr.reset(new GameManager(new_target));
}

void GameManager::clear()
{
	instance_ptr = nullptr;
}

void GameManager::createOnline(OnlineTraits &&traits)
{
	online.reset(new OnlineTraits(std::move(traits)));
}

bool GameManager::existOnline() const
{
	return !!online;
}
