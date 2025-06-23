#include "game/ships/PlayerShip.hpp"

#include "Helpers.hpp"
#include "game/SpaceField.hpp"
#include "game/actions/AccelerateShipAction.hpp"
#include "game/actions/MoveShipAction.hpp"
#include "game/actions/ShootAction.hpp"

struct PlayerDodgeRollAction : TransferActionBase<PlayerDodgeRollAction>
{
	inline static const std::string direction_key = "direction";
	PlayerDodgeRollAction(const ActionProps &props)
		: TransferActionBase(props)
	{
		if (props.authorExist())
			m_player = *props.castAuthor<PlayerShip>();
		m_direction = vector_from_json<float>(props.props[direction_key]);
	}
	void play() override
	{
		if (m_player.expired())
			return;
		m_player.lock()->dodge();
	}

private:
	SpaceField::StatePtr<PlayerShip> m_player;
	rn::Vec2f m_direction;
};

using controls = PlayerControls;

const controls PlayerShip::basic_controls = {
	{ PlayerControlsTypeSep::Movement,
	  {
		  controls::create<MoveShipAction>(sf::Keyboard::Key::W, "Move Forward", MovementProps({ 0, -1 })),
		  controls::create<MoveShipAction>(sf::Keyboard::Key::A, "Move Left", MovementProps{ { -1, 0 } }),
		  controls::create<MoveShipAction>(sf::Keyboard::Key::S, "Move Backward", MovementProps{ { 0, 1 } }),
		  controls::create<MoveShipAction>(sf::Keyboard::Key::D, "Move Right", MovementProps{ { 1, 0 } }),
	  }																									 },
	{ PlayerControlsTypeSep::WindowEvent,
	  { controls::create<ShootAction>(sf::Mouse::Button::Left, "Shoot", ShootProps({})),
		controls::create<AccelerateShipAction>(sf::Keyboard::Key::LShift, "Increase speed", AccelerationProps()),
		controls::create<PlayerDodgeRollAction>(sf::Mouse::Button::Right, "Dodge Roll", DodgeRollProps()) } }
};


PlayerShip::PlayerShip()
	: AbstractShip(*texture)
{
	dodge_roll_anim.setRepeating(false);
	dodge_roll_anim.setDuration(std::chrono::milliseconds{ *props::dodge_roll_time });
}

void PlayerShip::rotation()
{
	if (!GameManager::exist() && !isDodging())
		return;
	rn::Vec2f camera_pos = GameManager::session()->camera.getPosition();

	Direction dir{ rn::Vec2f(rn::mouse_position) - getPosition() + camera_pos };
	setDirection(dir.x, dir.y);
	setRotation(static_cast<float>(rn::math::rot(getDirection())));
}
void PlayerShip::update()
{
	AbstractShip::update();
	if (isDodging())
	{
		if (!dodge_roll_anim.played())
		{
			auto keyframe_before = dodge_roll_anim.current();
			dodge_roll_anim.update();
			auto keyframe_after = dodge_roll_anim.current();
			if (keyframe_after != dodge_roll_anim.cend() && keyframe_after != keyframe_before)
				getSprite().setTexture((*keyframe_after)->getTexture());
			move(getDirection() * dodge_speed);
		}
		else
		{
			getSprite().setTexture(*texture);
			is_dodging = false;
		}
	}
	if (!hit_rollback.is_stopped() && everyTime(hit_rollback, hit_rollback_time))
	{
		setInvincible(false);
		hit_rollback.stop();
		hit_rollback.reset();
	}
}

void PlayerShip::movement()
{
	if (!GameManager::exist())
		return;
	auto classification = basic_controls.find(PlayerControlsTypeSep::Movement);
	rn::Vec2f md;
	if (!isDodging())
	{
		classification.pushAsOneAction<MovementProps, sf::Keyboard::Key>(
			rn::isKeyhold,
			[this, &md](const MovementProps &props) {
				md		  = rn::math::norm(md + props.move_dir);
				is_moving = true;
				setMoveDirection(md);
			},
			{ self() }
		);
		if (!classification.oneOf<MovementProps, sf::Keyboard::Key>(rn::isKeyhold))
			is_moving = false;
	}
}

void PlayerShip::onMove()
{
	if (getPosition().x != getPosition().x || getPosition().y != getPosition().y)
	{
#ifdef SPACE_SHIP_DEBUG
		std::cout << "position is nan '" << to_json(getPosition()).dump() << "'\n";
#endif
		return;
	}
	if (GameManager::exist())
	{
		auto &camera = GameManager::session()->camera;
		camera.setPosition(getPosition() - camera.getViewSize() / 2.f);
	}
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
	if (gun
		&& (!gun->isAutomatic() && classification.oneOf<ShootProps, MButton>(rn::isKeydown)
			|| gun->isAutomatic() && classification.oneOf<ShootProps, MButton>(rn::isKeyhold)))
	{
		shoot();
	}
	classification.pushActionIfOneOf<AccelerationProps, Key>(
		rn::isKeydown,
		{ self(), std::nullopt, rn::Json{ { AccelerateShipAction::acceleration, *props::shift_acceleration } } }
	);
	classification.pushActionIfOneOf<AccelerationProps, Key>(
		rn::isKeyup, { self(), std::nullopt, rn::Json{ { AccelerateShipAction::acceleration, 1 } } }
	);
	if (is_moving && !isDodging())
	{
		classification.pushActionIfOneOf<DodgeRollProps, MButton>(
			rn::isKeydown,
			{ self(), std::nullopt, { { PlayerDodgeRollAction::direction_key, to_json(getDirection()) } } }
		);
	}
}

rn::Vec2f PlayerShip::countMove() const
{
	return getVelocity() * getMoveDirection();
}

void PlayerShip::onRotation()
{
	sf::Listener::setDirection({ getDirection().x, getDirection().y, 0.f });
	AbstractShip::onRotation();
	updateCollider();
}
bool PlayerShip::resolve(const Collidable *obstacle) const
{
	return AbstractShip::resolve(obstacle) && !isDodging() && !isInvincible();
}

void PlayerShip::summonCopy(SpaceField &field) const
{
	field.summonShip<PlayerShip>();
}

void PlayerShip::dodge()
{
	dodge_roll_anim.setCurrentKeyframe(0);
	is_dodging = true;
	dodge_roll_anim.start();
}

bool PlayerShip::isDodging() const
{
	return is_dodging;
}

void PlayerShip::onHit()
{
	AbstractShip::onHit();
	setInvincible(true);
	hit_rollback.start();
}

void PlayerShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (hit_rollback.is_stopped() || timeStep(hit_rollback, props::flicker_period))
		AbstractShip::draw(target, states);
}
