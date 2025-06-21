#pragma once

#include "components/Effect.hpp"
#include "components/FileLoader.hpp"

class HitSparkle : public AnimatedSprite
{
	inline static loading<AnimatedSprite> sparkle_anim = AnimationLoader::instance().addToUpload("img/animation/hit-sparkle/.png").get();
	constexpr static std::chrono::milliseconds duration{225};
	inline static rn::Vec2f size{30.f, 30.f};

public:
	HitSparkle()
		: AnimatedSprite(*sparkle_anim)
	{
		setDuration(duration);
		rn::Vec2f texsize{(*current())->getTexture().getSize()};
		auto ratio = size;
		ratio.x /= texsize.x;
		ratio.y /= texsize.y;
		setScale(ratio);
		setOrigin(texsize);
	}
};

