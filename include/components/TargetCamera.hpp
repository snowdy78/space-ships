#pragma once

#include "decl.hpp"
#include "game/Camera2d.hpp"

class TargetCamera : public Camera2d
{
	sf::RenderTarget *target = nullptr;
	std::function<void()> update_on_move;

public:
	TargetCamera(sf::RenderTarget &target, std::function<void()> update_on_move)
		: target(&target),
		  update_on_move(std::move(update_on_move))
	{
	}
	TargetCamera(TargetCamera &&camera) noexcept
		: update_on_move(std::move(camera.update_on_move))
	{
		target		  = camera.target;
		camera.target = nullptr;
	}
	TargetCamera &operator=(TargetCamera &&camera) noexcept
	{
		if (this != &camera)
		{
			update_on_move = std::move(camera.update_on_move);
			target		  = camera.target;
			camera.target = nullptr;
		}
		return *this;
	}

protected:
	void onCameraMove() override
	{
		target->setView(getView());
		update_on_move();
	}
};
