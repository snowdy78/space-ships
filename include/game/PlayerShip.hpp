#pragma once

#include "AbstractShip.hpp"
#include "Camera2d.hpp"
#include "ConnectedPlayerShip.hpp"
#include "components/ControlsScheme/ControlsScheme.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "game/actions/ShootAction.hpp"
#include "coop/TransferDataConverter.hpp"
enum class PlayerControlsTypeSep
{
	Movement,
	WindowEvent
};

struct MovementProps
{
public:
	const rn::Vec2f move_dir{}; // movement_direction;
	explicit MovementProps(const rn::Vec2f &movement_direction = {})
		: move_dir(movement_direction)
	{
	}
};
struct AccelerationProps
{
public:
	const float acceleration{};
	explicit AccelerationProps(float acceleration = 0)
		: acceleration(acceleration)
	{
	}
};
struct ShootProps
{
public:
	const rn::Vec2f direction{};
	explicit ShootProps(const rn::Vec2f &direction = {})
		: direction(direction)
	{
	}
};

using PlayerControls = ControlsScheme<
	PlayerControlsTypeSep, size_t,
	type_list<sf::Keyboard::Key, sf::Mouse::Button>,
	MovementProps,
	AccelerationProps,
	ShootProps
	>;

class PlayerShip : public AbstractShip, public Converted<PlayerShip, ConnectedPlayerShip>
{
	inline static loading<sf::Texture> texture		= FileLoader::Instance().addTextureToUpload("img/ship.png").get();
	constexpr static float shift_acceleration = 1.5f;
	static const PlayerControls basic_controls;
	Camera2d *camera = nullptr;

public:
	/**
	 * \brief Constructor for Ship
	 *
	 * Construct a Ship object with the image set to the contents of Ship::texture
	 */
	PlayerShip(Camera2d * = nullptr);
	~PlayerShip() override = default;

	void rotation() override;
	void movement() override;
	void onMove() override;
	void onEvent(sf::Event &event) override;
	void onRotation() override;
	void onHit() override;
	void summonCopy(SpaceField &field) const override;
	rn::Vec2f countMove() const override;
	TransferJson toJson() const override;
};
