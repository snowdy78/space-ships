#include "game/actions/ShootAction.hpp"
#include "game/Gun.hpp"

const size_t ShootAction::id = identify<ShootAction>();

ShootAction::ShootAction(GameObject *author, GameObject *contributor, const rn::Json &props)
	: TransferableAction(author, contributor, props)
{
	auto gun = dynamic_cast<Gun *>(author);
	auto d = "direction", x = "x", y = "y";
	if (!gun
		|| !(
			props.contains(d) && props[d].is_object() && props[d].contains(x) && props[d].contains(y)
			&& props[d][x].is_number() && props[d][y].is_number()
		))
	{
		throw std::runtime_error("ShootAction failed to initialize with instance: " + props.dump());
	}
	m_gun = gun;
	rn::Vec2f direction{ props[d][x].get<float>(), props[d][y].get<float>() };
	m_direction = rn::math::norm(direction);
}

void ShootAction::play()
{
	if (!m_gun)
		return;
	m_gun->fire();
}

TransferableAction::TransferJson ShootAction::toJson() const
{
	return { id, { { "direction", { { "x", m_direction.x }, { "y", m_direction.y } } } } };
}

AbstractAction *ShootAction::copy() const
{
	return new ShootAction();
}
