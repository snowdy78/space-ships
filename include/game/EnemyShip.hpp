#pragma once

#include "AbstractShip.hpp"
#include "GameObjectBase.hpp"
#include "SpaceField.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"

class EnemyShip final : public AbstractShip, public GameObjectBase<EnemyShip>
{
	using time_digit_type					   = std::chrono::milliseconds;
	inline static loading<sf::Texture> texture = FileLoader::Instance().addTextureToUpload("img/enemy_ship.png").get();
	constexpr static float enemy_velocity{ 5.f };
	constexpr static float min_distance_to_target = 100.f;
	constexpr static time_digit_type time_vertical_movement{ 660 };
	constexpr static time_digit_type time_horizontal_movement{ 1600 };
	constexpr static time_digit_type shoot_timing{ 2000 };
	rn::Stopwatch movement_clock;
	rn::Stopwatch shoot_clock;
	SpaceField::StatePtr<AbstractShip> m_target;
	class random_move
	{
	public:
		random_move() = default;
		void moving(const rn::Vec2f &direction)
		{
			m_direction = direction * (rn::random::chance(0.5) ? -1.f : 1.f);
		}
		void reset()
		{
			m_direction = std::nullopt;
		}
		rn::Vec2f moving() const
		{
			return *m_direction;
		}
		operator bool() const
		{
			return m_direction.has_value();
		}

	private:
		std::optional<rn::Vec2f> m_direction = std::nullopt;
	};
	random_move vertical;
	random_move horizontal;

public:
	using destroy_target_callback_type = std::function<void()>;
	EnemyShip();
	~EnemyShip() override = default;
	void setTarget(const SpaceField::StatePtr<AbstractShip> &target);
	SpaceField::StatePtr<AbstractShip> getTarget() const;
	void start() override;
	void update() override;
	void movement() override;
	void rotation() override;
	void summonCopy(SpaceField &field) const override;
	rn::Vec2f countMove() const override;
};
