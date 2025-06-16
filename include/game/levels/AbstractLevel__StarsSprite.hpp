#pragma once

#include "AbstractLevel.hpp"

class AbstractLevel::StarsSprite final : public rn::Rect
{
	using rn::Rect::setSize;
	using rn::Rect::setTexture;
	using rn::Rect::setTextureRect;
	inline static loading<sf::Texture> star_texture = TextureLoader::instance().addToUpload("img/star.png").get();
	const Difficulty m_difficulty;
	constexpr static int default_size = 128;

public:
	StarsSprite(const Difficulty &difficulty);

	void setSize(int size);
};
