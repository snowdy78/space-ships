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
	window.draw(GameManager::instance().effect_manager);
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
			if (space->mode() == GameSession::Mode::Developer)
			{
				if (rn::isKeydown(sf::Keyboard::P))
					summonShip();
				if (rn::isKeydown(sf::Keyboard::O))
				{
					randomlySummonAsteroidOutsideArea<SimpleAsteroid>(
						space->camera.getTransform().transformRect({ {}, space->camera.getViewSize() }), 10.f
					);
				}
			}
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

void Gameplay::summonShip()
{
	if (!space || !GameManager::exist())
		return;
	GameManager::session()->action_manager.emplaceToTop<SummonShipAction>(
		EnemyShip::identifier, [this](AbstractShip &ship) {
			if (auto enemy = dynamic_cast<EnemyShip *>(&ship))
			{
#ifdef SPACE_SHIP_DEBUG
				if (space->player.expired())
					std::cerr << "cannot set nullptr as ship target\n";
#endif
				else enemy->setTarget(space->player);
				rn::Vec2f randomPosition{ rn::random::real(0.f, 1.f) * space->camera.getViewSize().x,
										  rn::random::real(0.f, 1.f) * space->camera.getViewSize().y };
				enemy->setPosition(space->camera.getPosition() + randomPosition);
			}
			
		}
	);
}
