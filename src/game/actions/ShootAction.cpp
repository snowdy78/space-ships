#include "game/actions/ShootAction.hpp"
#include "game/Gun.hpp"

ShootAction::ShootAction(const TransferableActionProps &props)
	: BaseTransferableAction(props)
{
	auto gun = dynamic_cast<Gun *>(props.author);
	const char *x = "x", *y = "y";
	auto &d = direction;
	if (!gun
		|| !(
			props.data.contains(d) && props.data[d].is_object() && props.data[d].contains(x) && props.data[d].contains(y)
			&& props.data[d][x].is_number() && props.data[d][y].is_number()
		))
	{
		throw std::runtime_error("ShootAction failed to initialize with instance: " + props.data.dump());
	}
	m_gun = gun;
	rn::Vec2f _direction{ props.data[d][x].get<float>(), props.data[d][y].get<float>() };
	m_direction = rn::math::norm(_direction);
}

void ShootAction::play()
{
	if (!m_gun)
		return;
	m_gun->fire();
}

TransferableAction::TransferJson ShootAction::toJson() const
{
	return { id(), { { direction, { { "x", m_direction.x }, { "y", m_direction.y } } } } };
}

AbstractAction *ShootAction::copy() const
{
	return new ShootAction;
}
