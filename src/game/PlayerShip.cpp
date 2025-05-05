#include "game/PlayerShip.hpp"
#include <variant>
#include "RuneEngine/EngineDecl.hpp"
#include "RuneEngine/variables.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "game/AbstractShip.hpp"
#include "game/Camera2d.hpp"
#include "game/GameGlobals.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/AccelerateShipAction.hpp"
#include "game/actions/MoveShipAction.hpp"
#include "game/actions/ShootAction.hpp"


const PlayerControls PlayerShip::basic_controls = {
	{ PlayerControlsTypeSep::Movement,
	  {
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::W, "Move Forward", MovementProps({ 0, -1 })),
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::S, "Move Backward", MovementProps{ { 0, 1 } }),
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::A, "Move Left", MovementProps{ { -1, 0 } }),
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::D, "Move Right", MovementProps{ { 1, 0 } }),
	  }																											},
	{ PlayerControlsTypeSep::WindowEvent,
	  { PlayerControls::create<ShootAction>(sf::Mouse::Button::Left, "Shoot", ShootProps({})),
		PlayerControls::create<AccelerateShipAction>(sf::Keyboard::Key::LShift, "NitroFly", AccelerationProps()) } }
};


PlayerShip::PlayerShip(Camera2d *camera)
	: AbstractShip(*texture),
	  camera(camera)
{
}

void PlayerShip::rotation()
{
	using namespace rn::math;
	rn::Vec2f camera_pos = camera->getPosition();
	Direction dir{ rn::Vec2f(rn::mouse_position) - getPosition() + camera_pos };
	setDirection(dir.x, dir.y);
	setRotation(rot(getDirection()));
}

void PlayerShip::movement()
{
	if (!GameGlobals::exist())
		return;
	for (auto &i: basic_controls.classified(PlayerControlsTypeSep::Movement))
	{
		i->pushIfReleased<sf::Keyboard::Key, MovementProps>(
			[](auto k) {
				return rn::isKeyhold(k);
			},
			[](auto &props) {
				return rn::Json{
					{ MoveShipAction::md, to_json(props.move_dir) }
				};
			},
			this, nullptr
		);
	}
}

void PlayerShip::onMove()
{
	if (camera)
		camera->setPosition(getPosition() - camera->getViewSize() / 2.f);
	sf::Listener::setPosition(getPosition().x, getPosition().y, 0);
	rn::Vec2f perp = rn::math::nor(getPosition());
	sf::Listener::setUpVector(perp.x, perp.y, 0.f);
	AbstractShip::onMove();
	updateCollider();
}
void PlayerShip::onEvent(sf::Event &event)
{
	using MButton = sf::Mouse::Button;
	using Key	  = sf::Keyboard::Key;
	AbstractShip::onEvent(event);
	for (auto &i: basic_controls.classified(PlayerControlsTypeSep::WindowEvent))
	{

		if (gun && i->released<MButton, ShootProps>([](auto b) {
				return rn::isKeydown(MButton::Left);
			}))
		{
			shoot();
		}
		bool down		   = false;
		float acceleration = 1;
		i->pushIfReleased<Key, AccelerationProps>(
			[&down, &acceleration](auto k) {
				down		 = rn::isKeydown(k);
				acceleration = down ? shift_acceleration : 1;
				return down || rn::isKeyup(k);
		},
			this, nullptr, rn::Json{ { AccelerateShipAction::acceleration, acceleration } }
		);
	}
}
void PlayerShip::onRotation()
{
	sf::Listener::setDirection({ getDirection().x, getDirection().y, 0.f });
	AbstractShip::onRotation();
	updateCollider();
}
void PlayerShip::onHit()
{
	AbstractShip::onHit();
}
void PlayerShip::summonCopy(SpaceField *field) const
{
	field->appendShip<PlayerShip>();
}
AbstractShip *PlayerShip::copy() const
{
	return new PlayerShip(camera);
}
