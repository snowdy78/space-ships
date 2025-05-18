#include "game/GameManager.hpp"
#include "game/PlayerShip.hpp"

GameManager::OnlineTraits::OnlineTraits(sf::IpAddress host_ip, uint16_t host_port)
	: tcp(std::make_shared<TcpRouter>(host_ip, host_port)),
	  udp(std::make_shared<UdpRouter>())
{
	udp->bind(host_port, host_ip);
}

GameManager::GameManager(sf::RenderTarget &target, func_update_on_move update_on_move) 
    : camera(target, update_on_move), field(&camera)
{
	createPlayer();
}

void GameManager::createPlayer()
{
	player = field.summonShip<PlayerShip>(&camera);
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

void GameManager::create(sf::RenderTarget &target, func_update_on_move update_on_move)
{
    instance_ptr.reset(new GameManager(target, update_on_move));
}

void GameManager::host(sf::RenderTarget &target, func_update_on_move update_on_move, OnlineTraits &&traits)
{
	instance_ptr.reset(new GameManager(target, update_on_move));
	instance_ptr->createHost(std::move(traits));
}

void GameManager::client(sf::RenderTarget &target, func_update_on_move update_on_move, OnlineTraits &&traits)
{
	instance_ptr.reset(new GameManager(target, update_on_move));
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

void GameManager::reset(sf::RenderTarget &new_target, func_update_on_move new_update_on_move)
{
    if (!instance_ptr)
        return;
    instance_ptr.reset(new GameManager(new_target, new_update_on_move)); 
}

void GameManager::clear()
{
    instance_ptr = nullptr;
}

void GameManager::createOnline(OnlineTraits &&traits)
{
	online.reset(new OnlineTraits(traits));
}

bool GameManager::existOnline() const
{
	return !!online;
}
