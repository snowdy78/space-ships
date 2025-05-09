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
	struct collision_traits
	{
		bool before			 = false;
		bool current		 = false;
		CollisionState state = None;
		friend class Collidable;

	public:
		collision_traits(bool is_collide);
		void reset();
		void set(bool is_collide);
		bool getCurrent() const;
		bool getBefore() const;
	};

private:
	using thread_array = std::array<std::shared_ptr<sf::Thread>, 6>;
	inline static std::vector<Collidable *> collidables{};
	inline static constexpr float min_collision_distance = 25.f;
	static thread_array threads;

	std::map<Collidable *, collision_traits> collision_states{};
	void setCollisionState(Collidable *obstacle, bool value);
	static bool collideChunk(size_t start);
	static void collideObjects(Collidable *collidable, Collidable *obstacle);
	void updateState(Collidable *obstacle);

public:
	Collidable();
	~Collidable() override = 0;
	virtual const Collider *getCollider() const = 0;
	static void updateCollisionState();
	virtual void onCollisionEnter(Collidable *collidable) {}
	virtual void onCollisionUpdate(Collidable *collidable) {}
	virtual void onCollisionEnd(Collidable *collidable) {}
	Collidable *getObstacle(size_t index);
	const Collidable *getObstacle(size_t index) const;
	size_t getCollisionCount() const;
	CollisionState getCollisionState(Collidable *obstacle) const;
	virtual bool resolve(const Collidable *collidable) const = 0;
};
