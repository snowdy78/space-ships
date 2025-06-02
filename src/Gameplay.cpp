#include "Gameplay.hpp"

#include "Helpers.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "RuneEngine/SettingsFile.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "decl.hpp"
#include "game/EnemyShip.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"


Gameplay::Gameplay(sf::RenderWindow &window)
	: MenuBranch(window)
{
	th.reset(new sf::Thread([this]() {
		updateObjectsState();
	}));
	info.setVisible(dev_mode);
}

Gameplay::~Gameplay()
{
	window.setView(window.getDefaultView());
	GameManager::clear();
}

void Gameplay::start()
{
	GameManager::create(window, [&]() {
		background.setPosition(space->camera.getPosition());
		info.updateData("camera_pos");
		info.updateData("view_area");
		info.updateData("fps");
	});
	if (GameManager::exist())
		space = GameManager::session();
	info.addData("camera_pos", [&]() -> sf::String {
		rn::Vec2i p{ space->camera.getPosition() };
		return "{ " + std::to_string(p.x) + ", " + std::to_string(p.y) + " }";
	});
	info.addData("view_area", [&]() -> sf::String {
		const sf::View &view = space->field.getBulletMother().getViewArea();

		return "{ " + std::to_string(view.getCenter().x) + ", " + std::to_string(view.getCenter().y) + ", "
			   + std::to_string(view.getSize().x) + ", " + std::to_string(view.getSize().y) + " }";
	});
	info.addData("fps", [&]() -> sf::String {
		return std::to_string(rn::FPS);
	});
	if (space)
	{
		space->start();
		GameManager::instance().effect_manager.start();
		GameManager::instance().sound_manager.start();
		GameManager::session()->action_manager.start();
	}
	background.start();
	fps_clock.start();
}

void Gameplay::update()
{
	if (!space || !window.isOpen())
		return;
	th->launch();
	background.update();
	th->wait();
	sf::Transform bg_transform = space->camera.getTransform();
	window.clear();
	window.draw(background, bg_transform);
	window.draw(GameManager::instance().effect_manager);
	window.draw(space->field);

	window.draw(info, bg_transform);
	window.display();
}

void Gameplay::onEvent(sf::Event &event)
{
	background.onEvent(event);
	if (space)
	{
		GameManager::session()->action_manager.onEvent(event);
		if (window.hasFocus())
			space->onEvent(event);
	}
	if (event.type == sf::Event::Closed)
	{
		th = nullptr;
		window.close();
	}
	if (rn::isKeydown(sf::Keyboard::F3))
	{
		dev_mode = !dev_mode;
		info.setVisible(dev_mode);
	}
	if (dev_mode)
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
void Gameplay::updateObjectsState()
{
	if (!window.hasFocus() || !space)
		return;
	GameManager::session()->action_manager.update();
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
				if (!space->player)
					std::cerr << "cannot set nullptr as ship target\n";
#endif
				enemy->setTarget(space->player);
				rn::Vec2f randomPosition{ rn::random::real(0.f, 1.f) * space->camera.getViewSize().x,
										  rn::random::real(0.f, 1.f) * space->camera.getViewSize().x };
				enemy->setPosition(space->camera.getPosition() + randomPosition);
			}
			
		}
	);
}
