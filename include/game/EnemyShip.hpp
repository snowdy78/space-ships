#pragma once

#include "AbstractShip.hpp"
#include "GameObjectBase.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"


class EnemyShip : public AbstractShip, public GameObjectBase<EnemyShip>
{
	using time_digit_type = std::chrono::milliseconds;
	inline static loading<sf::Texture> texture = FileLoader::Instance().addTextureToUpload("img/enemy_ship.png").get();
	static constexpr float min_distance_to_target = 100.f;
	constexpr static time_digit_type time_vertical_movement{ 1500 };
	constexpr static time_digit_type time_horizontal_movement{ 1500 };
	constexpr static time_digit_type shoot_timing{ 2000 };
	rn::Stopwatch vertical_movement_clock;
	rn::Stopwatch horizontal_movement_clock;
	rn::Stopwatch shoot_clock;
	AbstractShip *target					 = nullptr;
	mutable std::optional<int> randomly_move = std::nullopt;

public:
	using destroy_target_callback_type = std::function<void()>;
	EnemyShip();
	~EnemyShip() override = default;
	void setTarget(AbstractShip *ship);
	const AbstractShip *getTarget() const;
	void start() override;
	void update() override;
	void movement() override;
	void rotation() override;
	void summonCopy(SpaceField &field) const override;
	rn::Vec2f countMove() const override;
};
