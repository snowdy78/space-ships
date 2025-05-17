#pragma once

#include "components/GameInfo.hpp"
#include "game/Camera2d.hpp"
#include "game/GameManager.hpp"
#include "components/Background.hpp"

// audio distance playing

class AudioMenu : public rn::MenuBranch
{
	class ShipCamera : public Camera2d
	{
		sf::RenderTarget &target;
		std::function<void()> update_on_move;

	public:
		ShipCamera(sf::RenderTarget &target, std::function<void()> update_on_move);
		virtual void onCameraMove() override;
	};
	Background background{};
	GameInfo info;
	std::unique_ptr<sf::Thread> th;
	GameManager *space = nullptr;
	rn::Stopwatch fps_clock;
	bool dev_mode = true;

public:
	explicit AudioMenu(sf::RenderWindow &window);
	~AudioMenu() override;
	void start() override;
	void updateObjectsState();
	void summonShip();
	void update() override;
	void onEvent(sf::Event &event) override;
};
