#pragma once

#include "AbstractShip.hpp"
#include "ConnectedPlayerShip.hpp"
#include "components/ControlsScheme/ControlsScheme.hpp"
#include "components/FileLoader.hpp"
#include "coop/TransferDataConverter.hpp"
#include "decl.hpp"

enum class PlayerControlsTypeSep
{
	Movement,
	WindowEvent
};

struct MovementProps
{
	rn::Vec2f move_dir{}; // movement_direction;
	explicit MovementProps(const rn::Vec2f &movement_direction = {})
		: move_dir(movement_direction)
	{
	}
};
struct AccelerationProps
{
	float acceleration{};
	explicit AccelerationProps(float acceleration = 0)
		: acceleration(acceleration)
	{
	}
};
struct ShootProps
{
	rn::Vec2f direction{};
	explicit ShootProps(const rn::Vec2f &direction = {})
		: direction(direction)
	{
	}
};
struct DodgeRollProps
{
	explicit DodgeRollProps() = default;
};

using PlayerControls = ControlsScheme<
	PlayerControlsTypeSep, size_t, type_list<sf::Keyboard::Key, sf::Mouse::Button>, MovementProps, AccelerationProps,
	ShootProps, DodgeRollProps>;

class PlayerShip final : public AbstractShip, public ConvertedTransfer<PlayerShip, ConnectedPlayerShip>
{
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload("img/ship.png").get();
	inline static loading<AnimatedSprite> dodge_animation = AnimationLoader::instance().addToUpload("img/animation/dodge-roll/.png").get();
	struct props
	{
		inline static auto &self_config = config::instance().get("PlayerShip");
		G_CONFIG_PROP_DEFINE(self_config, shift_acceleration);
		G_CONFIG_PROP_DEFINE(self_config, dodge_roll_time);
		G_CONFIG_PROP_DEFINE(self_config, dodge_roll_velocity);
	};
	static const PlayerControls basic_controls;
	AnimatedSprite dodge_roll_anim{ *dodge_animation };
	bool is_moving	  = false;
	bool is_dodging = false;
	float dodge_speed = *props::dodge_roll_velocity;

public:
	/**
	 * \brief Constructor for Ship
	 *
	 * Construct a Ship object with the image set to the contents of Ship::texture
	 */
	PlayerShip();
	~PlayerShip() override = default;

	void rotation() override;
	void update() override;
	void movement() override;
	void onMove() override;
	void onEvent(sf::Event &event) override;
	void onRotation() override;
	void onHit() override;
	bool resolve(const Collidable *obstacle) const override;
	void summonCopy(SpaceField &field) const override;
	void dodge();
	bool isDodging() const;
	rn::Vec2f countMove() const override;
};
