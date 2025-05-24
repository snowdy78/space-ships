#include "AudioMenu.hpp"

#include "Helpers.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "RuneEngine/SettingsFile.hpp"
#include "coop/TransferableAction.hpp"
#include "decl.hpp"
#include "game/EnemyShip.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"

AudioMenu::AudioMenu(sf::RenderWindow &window)
	: MenuBranch(window)
{
	th.reset(new sf::Thread([this]() {
		updateObjectsState();
	}));
	info.setVisible(dev_mode);
}

AudioMenu::~AudioMenu()
{
	window.setView(window.getDefaultView());
	GameManager::clear();
}

void AudioMenu::start()
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
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	if (space)
	{
		space->start();
		summonShip();
		GameManager::instance().effect_manager.start();
		GameManager::instance().sound_manager.start();
		GameManager::instance().action_manager.start();
	}
	background.start();
	fps_clock.start();
}

void AudioMenu::update()
{
	if (!space || !window.isOpen())
		return;
	th->launch();
	background.update();
	GameManager::instance().sound_manager.update();
	GameManager::instance().effect_manager.update();
	th->wait();
	sf::Transform bg_transform = space->camera.getTransform();
	window.clear();
	window.draw(background, bg_transform);
	window.draw(GameManager::instance().effect_manager);
	window.draw(space->field);

	window.draw(info, bg_transform);
	window.display();
}

void AudioMenu::onEvent(sf::Event &event)
{
	background.onEvent(event);
	if (space)
	{
		GameManager::instance().action_manager.onEvent(event);
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
void AudioMenu::updateObjectsState()
{
	if (!window.hasFocus() || !space)
		return;
	space->update();
	GameManager::instance().action_manager.update();
	Collidable::updateCollisionState();
}

void AudioMenu::summonShip()
{
	if (!space)
		return;
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	space->field.summonShip<EnemyShip>();
	auto ship = dynamic_cast<EnemyShip *>(space->field[space->field.size() - 1]);
	if (ship)
	{
		ship->start();
		if (!space->player)
			std::cerr << "cannot set nullptr as ship target\n";
		ship->setTarget(space->player);
		rn::Vec2f randomPosition{ rn::random::real(0.f, 1.f) * space->camera.getViewSize().x,
								  rn::random::real(0.f, 1.f) * space->camera.getViewSize().x };
		ship->setPosition(space->camera.getPosition() + randomPosition);
	}
}
