#pragma once

#include "AbstractShip.hpp"
#include "components/FileLoader.hpp"
#include "decl.hpp"
#include "GameObjectBase.hpp"


class EnemyShip : public AbstractShip, public GameObjectBase<EnemyShip>
{
	inline static loading<sf::Texture> texture = FileLoader::Instance().addTextureToUpload("img/enemy_ship.png").get();
	static constexpr float min_distance_to_target = 100.f;
	sf::Clock clock;
	AbstractShip *target					 = nullptr;
	mutable std::optional<int> randomly_move = std::nullopt;
	bool ready_to_shoot						 = false;

public:
	EnemyShip();
	~EnemyShip() override = default;
	void setTarget(AbstractShip *ship);
	const AbstractShip *getTarget() const;
	void update() override;
	void movement() override;
	void rotation() override;
	void summonCopy(SpaceField &field) const override;
	rn::Vec2f countMove() const override;
};
