#pragma once

#include "SpaceFieldObject.hpp"
#include "SpaceFieldStatement.hpp"
#include "components/FileLoader.hpp"

class SpaceItem : virtual GameObject,
				  public SpaceFieldStatement,
				  public sf::Drawable,
				  public sf::Transformable,
				  public rn::LogicalObject
{

	inline static loading<AnimatedSprite> item_cell_top_layer
		= AnimationLoader::instance().addToUpload("img/animation/itemcell/top layer/.png").get();
	inline static loading<AnimatedSprite> item_cell_bottom_layer
		= AnimationLoader::instance().addToUpload("img/animation/itemcell/bottom layer/.png").get();

public:
	using OwnerPtrType = std::weak_ptr<SpaceFieldStatement>;
	void assignOwner(OwnerPtrType owner)
	{
		owner_ptr = std::move(owner);
	}
	template<class T = SpaceFieldStatement>
	std::shared_ptr<T> getOwner() const
	{
		return std::dynamic_pointer_cast<T>(owner_ptr.lock());
	}
	template<class T>
	bool hasSameOwner(const T *possible_owner) const noexcept
	{
		if (!hasOwner())
			return false;
		return owner_ptr.lock().get() == dynamic_cast<const SpaceFieldObject *>(possible_owner);
	}
	bool hasOwner() const
	{
		return !owner_ptr.expired();
	}
	virtual const sf::Drawable &getItemSprite() const = 0;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		target.draw(*item_cell_bottom_layer, states);
		target.draw(getItemSprite(), states);
		target.draw(*item_cell_top_layer, states);
	}

private:
	OwnerPtrType owner_ptr;
};
