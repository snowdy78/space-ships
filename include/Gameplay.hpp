#pragma once

#include "components/GameInfo.hpp"
#include "game/Camera2d.hpp"
#include "game/GameManager.hpp"
#include "components/Background.hpp"

// audio distance playing

class Gameplay : public rn::MenuBranch
{
	Background background{};
	GameInfo info;
	std::unique_ptr<sf::Thread> th;
	GameSession *space = nullptr;
	rn::Stopwatch fps_clock;
	bool dev_mode = true;

public:
	explicit Gameplay(sf::RenderWindow &window);
	~Gameplay() override;
	void start() override;
	void updateObjectsState();
	void summonShip();
	void update() override;
	void onEvent(sf::Event &event) override;
};
