#pragma once

#include <memory>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "decl.hpp"

class Collidable : public virtual GameObject
{
public:
	enum CollisionState
	{
		None,
		Enter,
		Update,
		End
	};
	struct CollisionTraits
	{
		CollisionTraits(bool is_collide);
		~CollisionTraits();
		void reset();
		void set(bool is_collide);
		bool getCurrent() const;
		CollisionState getState() const;
		bool getBefore() const;
	private:
		bool before			 = false;
		bool current		 = false;
		CollisionState state = None;
	};

private:
	constexpr static size_t parralel_count = 6;
	using states_map_type = std::unordered_map<const Collidable *, CollisionTraits>;
	using CollidablesContainer = std::vector<Collidable *>;
	inline static CollidablesContainer collidables{};
	static constexpr float min_collision_distance = 25.f;

	states_map_type m_collision_states{};
	void setCollisionState(const Collidable *obstacle, bool value);
	void removeCollisionState(const Collidable *obstacle);
	void collideWith(const Collidable *obstacle);
	void updateState(const Collidable *obstacle);

public:
	Collidable();
	~Collidable() override = 0;
	virtual const Collider *getCollider() const = 0;
	static void updateCollisionState();
	virtual void onCollisionEnter(const Collidable* collidable) {}
	virtual void onCollisionUpdate(const Collidable* collidable) {}
	virtual void onCollisionEnd(const Collidable* collidable) {}
	const states_map_type &getCollisionStates() const;
	size_t getCollisionCount() const;
	states_map_type::const_iterator getCollisionTraits(const Collidable *obstacle) const;
	CollisionState getCollisionState(const Collidable *obstacle) const;
	/**
	 * \brief Resolves a collision between this Bullet and the given Collidable.
	 *
	 * The collision is resolved if the Collidable is not a Bullet.
	 *
	 * \param collidable The collidable to check against.
	 * \return True if the collision is resolved, false otherwise.
	 */
	virtual bool resolve(const Collidable *collidable) const = 0;
};
