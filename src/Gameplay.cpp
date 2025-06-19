#include "Gameplay.hpp"

#include "Helpers.hpp"
#include "MainMenu.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "decl.hpp"
#include "game/EnemyShip.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"

Gameplay::Gameplay(sf::RenderWindow &window)
	: MenuBranch(window)
{
}

Gameplay::~Gameplay()
{
	window.setView(window.getDefaultView());
	GameManager::clear();
}

void Gameplay::start()
{
	GameManager::create(window);
	if (GameManager::exist())
		space = GameManager::session();
	
	if (space)
	{
		space->start();
		GameManager::instance().effect_manager.start();
		GameManager::instance().sound_manager.start();
	}
}

void Gameplay::update()
{
	if (!space || !window.isOpen())
		return;
	updateObjectsState();
	window.clear();
	window.draw(*space);
	window.display();
}

void Gameplay::onEvent(sf::Event &event)
{
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}
	if (rn::isKeydown(sf::Keyboard::Escape))
	{
		next_branch<MainMenu>(window);
	}
	if (space)
	{
		if (window.hasFocus())
		{
			space->onEvent(event);
			
		}
	}
}
void Gameplay::updateObjectsState()
{
	if (!window.hasFocus() || !space)
		return;
	GameManager::instance().sound_manager.update();
	GameManager::instance().effect_manager.update();
	Collidable::updateCollisionState();
	space->update();
}
