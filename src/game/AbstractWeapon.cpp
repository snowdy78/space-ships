#include "game/AbstractWeapon.hpp"
#include "Helpers.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "game/GameManager.hpp"
#include "game/actions/ShootAction.hpp"
#include "game/AbstractBullet.hpp"
// DON'T REMOVE
#include "game/ships/tools/AssignWeapon.hpp" // DO NOT REMOVE THIS !!!!
// DON'T

class ShipGrabWeaponAction : public TransferActionBase<ShipGrabWeaponAction>
{
public:
	ShipGrabWeaponAction(const ActionProps &props)
		: TransferActionBase(props)
	{
		if (props.authorExist())
			ship = *props.castAuthor<AbstractShip>();
		if (props.contributorExist())
			item = *props.castContributor<SpaceItem>();
	}
	void play() override
	{
		if (item.expired() || item.lock()->hasOwner() || ship.expired())
			return;
		shipTakeWeapon(*ship.lock(), item);
	}

private:
	SpaceField::StatePtr<AbstractShip> ship;
	SpaceField::StatePtr<SpaceItem> item;
};

AbstractWeapon::~AbstractWeapon() = default;

void AbstractWeapon::startRollback()
{
	has_rollback = true;
	clock.start();
}
AbstractWeapon::AbstractWeapon() = default;

void AbstractWeapon::shoot()
{
	if (GameManager::exist() && !has_rollback)
	{
		auto direction{ getTrajectory() };
		GameManager::session()->action_manager.emplaceToTop<ShootAction>(TransferableActionProps{
			self(),
			std::nullopt,
			rn::Json{ { "direction", { { "x", direction.x }, { "y", direction.y } } } } });
	}
}

void AbstractWeapon::fire()
{
	if (GameManager::exist())
	{
		GameManager::session()->action_manager.emplaceToTop<SummonBulletAction>(
			bullet(), [this](AbstractBullet &bullet) {
				bullet.author_ptr = dynamic_state_ptr_cast<AbstractWeapon>(self());
				bullet.setPosition(getPosition());
				bullet.setDirection(getTrajectory());
			}
		);
		onShoot();
		startRollback();
	}
}

bool AbstractWeapon::hasRollback() const
{
	return has_rollback;
}

bool AbstractWeapon::isAutomatic() const
{
	return false;
}

void AbstractWeapon::update()
{
	SpaceItem::update();
	if (has_rollback && everyTime(clock, getRollback()))
	{
		clock.stop();
		clock.reset();
		has_rollback = false;
	}
}

std::chrono::milliseconds AbstractWeapon::getRollback() const
{
	return std::chrono::milliseconds{ 300 };
}


void AbstractWeapon::onPlayerTake(const std::shared_ptr<AbstractShip> &player)
{
	if (GameManager::exist())
		GameManager::session()->action_manager.emplaceToTop<ShipGrabWeaponAction>(ActionProps{ player, self() });
}
