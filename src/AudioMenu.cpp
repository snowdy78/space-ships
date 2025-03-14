#include "AudioMenu.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "RuneEngine/SettingsFile.hpp"
#include "decl.hpp"
#include "game/EnemyShip.hpp"
#include "game/Ship.hpp"


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
}

void AudioMenu::start()
{
	camera.reset(new ShipCamera(window, [&]() {
		background.setPosition(camera->getPosition());
		info.updateData("camera_pos");
		info.updateData("view_area");
		info.updateData("fps");
	}));
	field.setCamera(camera.get());
	info.addData("camera_pos", [&]() -> sf::String {
		if (!camera)
			return "null";
		rn::Vec2i p{ camera->getPosition() };
		return "{ " + std::to_string(p.x) + ", " + std::to_string(p.y) + " }";
	});
	info.addData("view_area", [&]() -> sf::String {
		const sf::View &view = field.getBulletMother().getViewArea();

		return "{ " + std::to_string(view.getCenter().x) + ", " + std::to_string(view.getCenter().y) + ", "
			   + std::to_string(view.getSize().x) + ", " + std::to_string(view.getSize().y) + " }";
	});
	info.addData("fps", [&]() -> sf::String {
		return std::to_string(rn::FPS);
	});
	field.appendShip<Ship>(camera.get());
	player = field[0];
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	player->setPosition(res / 2.f);
	summonShip();
	background.start();
	field.start();
	fps_clock.start();
}

void AudioMenu::update()
{
	window.clear();

	th->launch();
	background.update();
	th->wait();
	sf::Transform bg_transform;
	if (camera)
		bg_transform = camera->getTransform();
	window.draw(background, bg_transform);
	window.draw(field);
	window.draw(info, bg_transform);
	window.display();
}

void AudioMenu::onEvent(sf::Event &event)
{
	background.onEvent(event);

	if (window.hasFocus())
	{
		field.onEvent(event);
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
	if (!window.hasFocus())
	{
		return;
	}
	field.update();
	Collidable::updateCollisionState();
}

void AudioMenu::summonShip()
{
	auto res = rn::Vec2f(rn::VideoSettings::getResolution());
	field.appendShip<EnemyShip>();
	auto ship = dynamic_cast<EnemyShip *>(field[field.size() - 1]);
	if (ship)
	{
		ship->start();
		ship->setTarget(player);
		if (camera)
		{
			rn::Vec2f randomPosition{ rn::random::real(0.f, 1.f) * camera->getViewSize().x,
									  rn::random::real(0.f, 1.f) * camera->getViewSize().x };
			ship->setPosition(camera->getPosition() + randomPosition);
		}
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
