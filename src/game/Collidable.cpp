#include "game/Collidable.hpp"
#include <memory>

#include "Helpers.hpp"
#include "game/colliders/EllipseCollider.hpp"
#include "game/colliders/PolygonCollider.hpp"

void Collidable::CollisionTraits::reset()
{
	before	= false;
	current = false;
}
void Collidable::CollisionTraits::set(bool is_collide)
{
	before	= current;
	current = is_collide;

	if ((state == None || state == End) && current && !before)
		state = Enter;
	else if (state == Enter && current && before)
		state = Update;
	else if ((state == Update || state == Enter) && !current && before)
		state = End;
	else
		state = None;
}

bool Collidable::CollisionTraits::getCurrent() const
{
	return current;
}
bool Collidable::CollisionTraits::getBefore() const
{
	return before;
}

Collidable::CollisionState Collidable::CollisionTraits::getState() const
{
	return state;
}

Collidable::CollisionTraits::CollisionTraits(bool is_collide)
{
	set(is_collide);
}

Collidable::CollisionTraits::~CollisionTraits() = default;

Collidable::Collidable()
{
	collidables.push_back(this);
}

Collidable::~Collidable()
{
	if (const auto i = std::ranges::find(collidables, this); i != collidables.end())
	{
		collidables.erase(i);
		multithread_for_each<parralel_count>(
			collidables.begin(), collidables.end(), [this](CollidablesContainer::value_type const &collidable) {
				if (auto it = collidable->m_collision_states.find(this); it != collidable->m_collision_states.end())
					collidable->m_collision_states.erase(it);
			}
		);
	}
}


void Collidable::updateCollisionState()
{
	if (collidables.empty())
		return;
	// parallel colliding objects
	multithread_for_each<parralel_count>(
		collidables.cbegin(), collidables.cend(), [](CollidablesContainer::value_type const &collidable) {
			for (const auto &obstacle: collidables)
				if (obstacle != collidable)
					collidable->collideWith(obstacle);
		}
	);
	for (auto &collidable: collidables)
		for (auto &state: collidable->m_collision_states)
			collidable->updateState(state.first);
}
void Collidable::collideWith(const Collidable *obstacle)
{
	using rect = rn::math::rectangle;
	if (obstacle == this || !obstacle)
		return;
	if (!resolve(obstacle) || !rect(getCollider()->getBounds()).collide(rect(obstacle->getCollider()->getBounds())))
	{
		removeCollisionState(obstacle);
		return;
	}
	if (auto el = dynamic_cast<const EllipseCollider *>(obstacle->getCollider()))
		setCollisionState(obstacle, getCollider()->collide(*el));
	else if (auto pl = dynamic_cast<const PolygonCollider *>(obstacle->getCollider()))
		setCollisionState(obstacle, getCollider()->collide(*pl));
}

void Collidable::updateState(const Collidable *obstacle)
{
	auto state = getCollisionState(obstacle);
	if (state == Enter)
		onCollisionEnter(obstacle);
	else if (state == Update)
		onCollisionUpdate(obstacle);
	else if (state == End)
		onCollisionEnd(obstacle);
}
void Collidable::setCollisionState(const Collidable *obstacle, bool value)
{
	const auto it = m_collision_states.find(obstacle);
	if (it == m_collision_states.end())
	{
		if (value)
		{
			// append collision enter state
			m_collision_states.emplace(obstacle, value);
		}
		return;
	}
	auto &traits = it->second;
	// update state
	traits.set(value);

	// if no collision remove state
	if (!traits.getCurrent() && !traits.getBefore())
		m_collision_states.erase(obstacle);
}

void Collidable::removeCollisionState(const Collidable *obstacle)
{
	if (m_collision_states.contains(obstacle))
		m_collision_states.erase(obstacle);
}

const Collidable::states_map_type &Collidable::getCollisionStates() const
{
	return m_collision_states;
}

size_t Collidable::getCollisionCount() const
{
	return m_collision_states.size();
}

Collidable::CollisionState Collidable::getCollisionState(const Collidable *obstacle) const
{
	if (auto traits = getCollisionTraits(obstacle); traits != m_collision_states.end())
		return traits->second.getState();
	return None;
}
Collidable::states_map_type::const_iterator Collidable::getCollisionTraits(const Collidable *obstacle) const
{
	return m_collision_states.find(obstacle);
}
