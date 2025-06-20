#pragma once

#include "SpaceFieldObject.hpp"
#include "SpaceFieldStatement.hpp"
#include "components/FileLoader.hpp"
#include "components/GameConfiguration.hpp"
#include "game/colliders/EllipseCollider.hpp"

class SpaceItem : virtual GameObject,
				  public SpaceFieldStatement,
				  public sf::Drawable,
				  public sf::Transformable,
				  public rn::LogicalObject
{
	struct props
	{
		inline static auto &self_config = config::instance().get("SpaceItem");
		G_CONFIG_PROP_DEFINE(self_config, top_layer_ms_frame_time);
		G_CONFIG_PROP_DEFINE(self_config, bottom_layer_ms_frame_time);
		G_CONFIG_PROP_DEFINE(self_config, collider_visible);
	};
	inline static loading<AnimatedSprite> s_item_cell_top_layer
		= AnimationLoader::instance().addToUpload("img/animation/itemcell/top layer/.png").get();
	inline static loading<AnimatedSprite> s_item_cell_bottom_layer
		= AnimationLoader::instance().addToUpload("img/animation/itemcell/bottom layer/.png").get();

	void updateCollider();

public:
	using OwnerPtrType = std::weak_ptr<SpaceFieldStatement>;
	void assignOwner(OwnerPtrType owner);

	template<class T = SpaceFieldStatement>
	std::shared_ptr<T> getOwner() const;

	template<class T>
	bool hasSameOwner(const T *possible_owner) const noexcept;
	bool hasOwner() const;
	void start() override;

	void update() override;
	virtual const sf::Drawable &getItemSprite() const = 0;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void collideWithShip(const std::shared_ptr<AbstractShip> &);
	virtual void onOwnerAssign();

	virtual void onSummon()
	{
	}

private:
	AnimatedSprite top_layer_anim{ *s_item_cell_top_layer };
	AnimatedSprite bottom_layer_anim{ *s_item_cell_bottom_layer };
	EllipseCollider collider;
	sf::CircleShape m_collider_widget;

	OwnerPtrType owner_ptr;
};

template<class T>
std::shared_ptr<T> SpaceItem::getOwner() const
{
	return std::dynamic_pointer_cast<T>(owner_ptr.lock());
}

template<class T>
bool SpaceItem::hasSameOwner(const T *possible_owner) const noexcept
{
	if (!hasOwner())
		return false;
	return owner_ptr.lock().get() == dynamic_cast<const SpaceFieldObject *>(possible_owner);
}
