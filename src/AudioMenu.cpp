#include "AudioMenu.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "RuneEngine/SettingsFile.hpp"
#include "decl.hpp"
#include "game/EnemyShip.hpp"


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
	if (GameGlobals::exist())
	{
		GameGlobals::instance().clear();
	}
}

void AudioMenu::start()
{
	GameGlobals::create(window, [&]() {
		background.setPosition(space->camera.getPosition());
		info.updateData("camera_pos");
		info.updateData("view_area");
		info.updateData("fps");
	});
	if (GameGlobals::exist())
		space = &GameGlobals::instance();
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
	summonShip();
	if (space)
	{
		space->player->setPosition(res / 2.f);
		space->field.start();
		space->effect_manager.start();
		space->sound_manager.start();
		space->action_manager.start();
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
	space->sound_manager.update();
	space->effect_manager.update();
	th->wait();
	sf::Transform bg_transform;
	bg_transform = space->camera.getTransform();
	window.clear();
	window.draw(background, bg_transform);
	window.draw(space->effect_manager);
	window.draw(space->field);

	window.draw(info, bg_transform);
	window.display();
}

void AudioMenu::onEvent(sf::Event &event)
{
	background.onEvent(event);
	if (space)
	{
		space->action_manager.onEvent(event);
		if (window.hasFocus())
		{
			space->field.onEvent(event);
		}
	}
	if (event.type == sf::Event::Closed)
	{
		delete th.release();
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
		{
			summonShip();
		}
	}
}
void AudioMenu::updateObjectsState()
{
	if (!window.hasFocus() || !space)
		return;
	space->field.update();
	space->action_manager.update();
	Collidable::updateCollisionState();
}

void AudioMenu::summonShip()
{
	if (!space)
		return;
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	space->field.appendShip<EnemyShip>();
	auto ship = dynamic_cast<EnemyShip *>(space->field[space->field.size() - 1]);
	if (ship)
	{
		ship->start();
		ship->setTarget(space->player);
		rn::Vec2f randomPosition{ rn::random::real(0.f, 1.f) * space->camera.getViewSize().x,
								  rn::random::real(0.f, 1.f) * space->camera.getViewSize().x };
		ship->setPosition(space->camera.getPosition() + randomPosition);
	}
}

AudioMenu::ShipCamera::ShipCamera(sf::RenderTarget &target, std::function<void()> update_on_move)
	: target(target),
	  update_on_move(update_on_move)
{}

void AudioMenu::ShipCamera::onCameraMove()
{
	target.setView(getView());
	update_on_move();
}
