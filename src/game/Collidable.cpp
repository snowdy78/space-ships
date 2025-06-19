#include "game/Collidable.hpp"
#include <memory>
#include "SFML/System/Thread.hpp"
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
	else if (state != End && state != None && current && before)
		state = Update;
	else if (!current && before)
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

Collidable::Collidable()
{
	collidables.push_back(this);
}

Collidable::~Collidable()
{
	if (const auto i = std::ranges::find(collidables, this); i != collidables.end())
	{
		collidables.erase(i);
		for (const auto &collidable: collidables)
			if (auto it = collidable->m_collision_states.find(this); it != collidable->m_collision_states.end())
				collidable->m_collision_states.erase(it);
	}
}

Collidable::CollisionState Collidable::getCollisionState(const Collidable *obstacle) const
{
	auto it = m_collision_states.find(obstacle);
	if (it == m_collision_states.end())
		return None;

	return it->second.getState();
}

Collidable::thread_array Collidable::threads = ([]() {
	thread_array arr;
	for (size_t i = 1; i <= arr.max_size(); i++)
	{
		arr[i - 1] = std::make_shared<sf::Thread>([i]() {
			size_t ss	= collidables.size();
			size_t cap	= threads.size() + 1;
			size_t oddy = collidables.size() % cap;
			size_t size = static_cast<size_t>(
				std::floorf((static_cast<float>(ss) - static_cast<float>(oddy)) / static_cast<float>(cap))
			);
			for (size_t m = i * size; m < (i + 1) * size; m++)
			{
				mutex.lock();
				if (!collideChunk(m))
				{
					mutex.unlock();
					break;
				}
				mutex.unlock();
			}
		});
	}
	return arr;
})();

void Collidable::updateCollisionState()
{
	if (collidables.empty())
		return;
	for (auto &thread: threads)
		thread->launch();
	size_t ss	= collidables.size();
	size_t cap	= threads.max_size() + 1;
	size_t size = static_cast<size_t>(std::floorf(static_cast<float>(ss) / static_cast<float>(cap))) + ss % cap;
	for (size_t i = 0; i < size; i++)
		if (!collideChunk(i))
			break;
	for (auto &thread: threads)
		thread->wait();
	for (auto &collidable: collidables)
		for (auto &state: collidable->m_collision_states)
			collidable->updateState(state.first);
}
void Collidable::collideObjects(Collidable *collidable, Collidable *obstacle)
{
	if (collidable && obstacle
		&& !rn::math::rectangle(obstacle->getCollider()->getBounds())
			   .collide(rn::math::rectangle(collidable->getCollider()->getBounds())))
		return;

	auto el = dynamic_cast<const EllipseCollider *>(obstacle->getCollider());
	auto pl = dynamic_cast<const PolygonCollider *>(obstacle->getCollider());
	if (obstacle == collidable || !collidable->resolve(obstacle) || !el && !pl)
		return;
	bool is_collide;
	if (el)
		is_collide = collidable->getCollider()->collide(*el);
	else
		is_collide = collidable->getCollider()->collide(*pl);
	collidable->setCollisionState(obstacle, is_collide);
	if (obstacle->resolve(collidable))
		obstacle->setCollisionState(collidable, is_collide);
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
	{
		m_collision_states.erase(obstacle);
	}
}
bool Collidable::collideChunk(size_t start)
{
	if (start == collidables.size() - 1)
		return false;
	for (size_t i = start + 1; i < collidables.size(); i++)
		collideObjects(collidables[start], collidables[i]);
	return true;
}
const Collidable::states_map_type & Collidable::getCollisionStates() const
{
	return m_collision_states;
}

size_t Collidable::getCollisionCount() const
{
	return m_collision_states.size();
}

std::optional<Collidable::CollisionTraits> Collidable::getCollisionTraits(const Collidable *obstacle) const
{
	auto it = m_collision_states.find(obstacle);
	if (it != m_collision_states.end())
		return it->second;
	return std::nullopt;
}
