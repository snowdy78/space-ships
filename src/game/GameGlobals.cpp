#include "game/GameGlobals.hpp"
#include "game/PlayerShip.hpp"

GameGlobals::OnlineTraits::OnlineTraits(sf::IpAddress host_ip, uint16_t host_port)
	: tcp(std::make_shared<TcpRouter>(host_ip, host_port)),
	  udp(std::make_shared<UdpRouter>())
{
	udp->bind(host_port, host_ip);
}

GameGlobals::GameGlobals(sf::RenderTarget &target, func_update_on_move update_on_move) 
    : camera(target, update_on_move), field(&camera)
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
    return *instance_ptr;
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
    instance_ptr = nullptr;
}

void GameGlobals::createOnline(OnlineTraits &&traits)
{
	online.reset(new OnlineTraits(traits));
}

bool GameGlobals::existOnline() const
{
	return !!online;
}
