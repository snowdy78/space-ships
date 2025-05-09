#include "game/Collidable.hpp"
#include <memory>
#include "SFML/System/Thread.hpp"
#include "game/colliders/EllipseCollider.hpp"
#include "game/colliders/PolygonCollider.hpp"

void Collidable::collision_traits::reset()
{
	before	= false;
	current = false;
}
void Collidable::collision_traits::set(bool is_collide)
{
	before	= current;
	current = is_collide;

	if (current && !before)
		state = Enter;
	else if (current && before)
		state = Update;
	else if (!current && before)
		state = End;
	else
		state = None;
}

bool Collidable::collision_traits::getCurrent() const
{
	return current;
}
bool Collidable::collision_traits::getBefore() const
{
	return before;
}
Collidable::collision_traits::collision_traits(bool is_collide)
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
		{
			if (auto it = collidable->collision_states.find(this); it != collidable->collision_states.end())
				collidable->collision_states.erase(it);
		}
	}
}

Collidable::CollisionState Collidable::getCollisionState(Collidable *obstacle) const
{
	auto it = collision_states.find(obstacle);
	if (it == collision_states.end())
		return None;

	return it->second.state;
}

Collidable::thread_array Collidable::threads = ([]() {
	thread_array arr;
	for (size_t i = 1; i <= arr.max_size(); i++)
	{
		arr[i - 1] = std::make_shared<sf::Thread>([i]() {
			size_t ss	= collidables.size();
			size_t cap	= threads.size() + 1;
			size_t oddy = collidables.size() % cap;
			size_t size = static_cast<size_t>(std::floorf((static_cast<float>(ss) - static_cast<float>(oddy)) / static_cast<float>(cap)));
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
	{
		thread->launch();
	}
	size_t ss	= collidables.size();
	size_t cap	= threads.max_size() + 1;
	size_t size = static_cast<size_t>(std::floorf(static_cast<float>(ss) / static_cast<float>(cap))) + ss % cap;
	for (size_t i = 0; i < size; i++)
	{
		if (!collideChunk(i))
			break;
	}
	for (auto &thread: threads)
	{
		thread->wait();
	}
	for (auto &collidable: collidables)
	{
		for (auto &state: collidable->collision_states)
		{
			collidable->updateState(state.first);
		}
	}
}
void Collidable::collideObjects(Collidable *collidable, Collidable *obstacle)
{
	auto o1 = dynamic_cast<sf::Transformable *>(obstacle);
	auto o2 = dynamic_cast<sf::Transformable *>(collidable);
	if (o1 && o2 && rn::math::length(o1->getPosition() - o2->getPosition()) > min_collision_distance)
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
	auto &obstates	 = obstacle->collision_states;
	auto &collstates = collidable->collision_states;
	if (!is_collide && !obstates.contains(collidable) && !collstates.contains(obstacle))
		return;

	collidable->setCollisionState(obstacle, is_collide);
	if (obstacle->resolve(collidable))
		obstacle->setCollisionState(collidable, is_collide);
}
void Collidable::updateState(Collidable *obstacle)
{
	auto state = getCollisionState(obstacle);
	if (state == Enter)
		onCollisionEnter(obstacle);
	else if (state == Update)
		onCollisionUpdate(obstacle);
	else if (state == End)
		onCollisionEnd(obstacle);
}
void Collidable::setCollisionState(Collidable *obstacle, bool value)
{
	const auto it = collision_states.find(obstacle);
	if (it == collision_states.end())
	{
		// append collision enter state
		collision_states.emplace(obstacle, value);
		return;
	}
	auto &traits = it->second;
	// update state
	traits.set(value);

	// if no collision remove state
	if (!traits.current && !traits.before)
	{
		collision_states.erase(obstacle);
	}
}
bool Collidable::collideChunk(size_t start)
{
	if (start == collidables.size() - 1)
		return false;
	for (size_t i = start + 1; i < collidables.size(); i++)
	{
		collideObjects(collidables[start], collidables[i]);
	}
	return true;
}
Collidable *Collidable::getObstacle(size_t index)
{
	auto it = collision_states.begin();
	std::advance(it, index);
	return it->first;
}
const Collidable *Collidable::getObstacle(size_t index) const
{
	auto it = collision_states.begin();
	std::advance(it, index);
	return it->first;
}
size_t Collidable::getCollisionCount() const
{
	return collision_states.size();
}
