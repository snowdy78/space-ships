#include "game/actions/ShootAction.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "game/GameManager.hpp"
#include "game/Gun.hpp"

ShootAction::ShootAction(const TransferableActionProps &props)
	: TransferActionBase(props)
{
	auto gun	  = dynamic_cast<Gun *>(getAuthor());
	const char *x = "x", *y = "y";
	auto &d = direction;
	if (!gun
		|| !(
			props.props.contains(d) && props.props[d].is_object() && props.props[d].contains(x)
			&& props.props[d].contains(y) && props.props[d][x].is_number() && props.props[d][y].is_number()
		))
	{
		throw std::runtime_error("ShootAction failed to initialize with instance: " + props.props.dump());
	}
	m_gun = gun;
	rn::Vec2f direction{ props.props[d][x].get<float>(), props.props[d][y].get<float>() };
	m_direction = rn::math::norm(direction);
}

void ShootAction::play()
{
	if (!m_gun)
		return;
	m_gun->fire();
}

rn::Json ShootAction::toJson() const
{
	return {
		{ "direction", { { "x", m_direction.x }, { "y", m_direction.y } } }
	};
}
