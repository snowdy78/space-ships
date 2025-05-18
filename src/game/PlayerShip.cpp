#include "game/PlayerShip.hpp"
#include "RuneEngine/EngineDecl.hpp"
#include "RuneEngine/variables.hpp"
#include "game/AbstractShip.hpp"
#include "game/Camera2d.hpp"
#include "game/GameManager.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/AccelerateShipAction.hpp"
#include "game/actions/MoveShipAction.hpp"
#include "game/actions/ShootAction.hpp"


const PlayerControls PlayerShip::basic_controls = {
	{ PlayerControlsTypeSep::Movement,
	  {
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::W, "Move Forward", MovementProps({ 0, -1 })),
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::A, "Move Left", MovementProps{ { -1, 0 } }),
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::S, "Move Backward", MovementProps{ { 0, 1 } }),
		  PlayerControls::create<MoveShipAction>(sf::Keyboard::Key::D, "Move Right", MovementProps{ { 1, 0 } }),
	  }		},
	{ PlayerControlsTypeSep::WindowEvent,
	  { PlayerControls::create<ShootAction>(sf::Mouse::Button::Left, "Shoot", ShootProps({})),
		PlayerControls::create<AccelerateShipAction>(
			sf::Keyboard::Key::LShift, "Increase speed", AccelerationProps()
		) } }
};


PlayerShip::PlayerShip(Camera2d *camera)
	: AbstractShip(*texture),
	  camera(camera)
{
}

void PlayerShip::rotation()
{
	rn::Vec2f camera_pos = camera->getPosition();
	Direction dir{ rn::Vec2f(rn::mouse_position) - getPosition() + camera_pos };
	setDirection(dir.x, dir.y);
	setRotation(rn::math::rot(getDirection()));
}

void PlayerShip::movement()
{
	if (!GameManager::exist())
		return;
	auto classification = basic_controls.find(PlayerControlsTypeSep::Movement);
	rn::Vec2f md;
	classification.pushAsOneAction<MovementProps, sf::Keyboard::Key>(
		rn::isKeyhold,
		[this, &md](const MovementProps &props) {
			md = rn::math::norm(md + props.move_dir);
			setMoveDirection(md);
		},
		{ this, nullptr }
	);
}

void PlayerShip::onMove()
{
	if (getPosition().x != getPosition().x || getPosition().y != getPosition().y)
	{
		std::cout << "position is nan '" << to_json(getPosition()).dump() << "'\n";
		return;
	}
	if (camera)
		camera->setPosition(getPosition() - camera->getViewSize() / 2.f);
	sf::Listener::setPosition(getPosition().x, getPosition().y, 0);
	rn::Vec2f perp = rn::math::nor(getPosition());
	sf::Listener::setUpVector(perp.x, perp.y, 0.f);
	AbstractShip::onMove();
}
void PlayerShip::onEvent(sf::Event &event)
{
	using MButton = sf::Mouse::Button;
	using Key	  = sf::Keyboard::Key;
	AbstractShip::onEvent(event);
	auto classification = basic_controls.find(PlayerControlsTypeSep::WindowEvent);
	if (classification.oneOf<ShootProps, MButton>(rn::isKeydown))
		shoot();
	classification.pushActionIfOneOf<AccelerationProps, Key>(
		rn::isKeydown, { this, nullptr, rn::Json{ { AccelerateShipAction::acceleration, shift_acceleration } } }
	);
	classification.pushActionIfOneOf<AccelerationProps, Key>(
		rn::isKeyup, { this, nullptr, rn::Json{ { AccelerateShipAction::acceleration, 1 } } }
	);
}

rn::Vec2f PlayerShip::countMove() const
{
	return getAcceleration() * getVelocity() * getMoveDirection();
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
void PlayerShip::summonCopy(SpaceField &field) const
{
	field.summonShip<PlayerShip>();
}
