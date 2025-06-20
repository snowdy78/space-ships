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
	const rn::Vec2f move_dir{}; // movement_direction;
	explicit MovementProps(const rn::Vec2f &movement_direction = {})
		: move_dir(movement_direction)
	{
	}
};
struct AccelerationProps
{
	const float acceleration{};
	explicit AccelerationProps(float acceleration = 0)
		: acceleration(acceleration)
	{
	}
};
struct ShootProps
{
	const rn::Vec2f direction{};
	explicit ShootProps(const rn::Vec2f &direction = {})
		: direction(direction)
	{
	}
};

using PlayerControls = ControlsScheme<
	PlayerControlsTypeSep, size_t, type_list<sf::Keyboard::Key, sf::Mouse::Button>, MovementProps, AccelerationProps,
	ShootProps>;

class PlayerShip final : public AbstractShip, public ConvertedTransfer<PlayerShip, ConnectedPlayerShip>
{
	inline static loading<sf::Texture> texture = TextureLoader::instance().addToUpload("img/ship.png").get();

	inline static auto &self_config = config::instance().get("PlayerShip");
	G_CONFIG_PROP_DEFINE(self_config, shift_acceleration);
	static const PlayerControls basic_controls;

public:
	/**
	 * \brief Constructor for Ship
	 *
	 * Construct a Ship object with the image set to the contents of Ship::texture
	 */
	PlayerShip();
	~PlayerShip() override = default;

	void rotation() override;
	void movement() override;
	void onMove() override;
	void onEvent(sf::Event &event) override;
	void onRotation() override;
	void onHit() override;
	void summonCopy(SpaceField &field) const override;
	rn::Vec2f countMove() const override;
};
