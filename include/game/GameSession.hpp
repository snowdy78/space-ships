#pragma once

#include "SpaceField.hpp"
#include "actions/ActionManager.hpp"
#include "components/LocalDriveSession.hpp"
#include "components/TargetCamera.hpp"
#include "game/levels/AbstractLevel.hpp"

template<class T>
concept LevelConcept = std::is_base_of_v<AbstractLevel, T> && std::is_final_v<T>;

class GameSession : protected LocalDriveSession, public rn::LogicalObject, public sf::Drawable
{
	constexpr static const char *s_file_path = "src/game_session_data.enc";

	void createPlayer();

public:
	GameSession(TargetCamera &&camera);
	void start() override;
	void update() override;
	void onEvent(sf::Event &event) override;
	template<LevelConcept T>
	void up_level();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	void beforeSave() override;
	void afterLoad() override;

public:
	ActionManager action_manager;
	TargetCamera camera;
	struct GameSessionSpaceField : SpaceField
	{
		GameSessionSpaceField(GameSession *session, const Camera2d *camera = nullptr);
		void onObjectSummon(const StatePtrType &state_ptr) const override;
		void onObjectDestroy(const StatePtrType &state_ptr) const override;

	private:
		GameSession *m_session;
	} field;
	GameSessionSpaceField::StatePtr<AbstractShip> player;


private:
	friend struct GameSessionSpaceField;

	std::unique_ptr<AbstractLevel> m_level;
	rn::Json m_game_objects = rn::Json::array();
	std::hash<GameObject *> m_hash;
};

template<LevelConcept T>
void GameSession::up_level()
{
	m_level = std::make_unique<T>(field);
	m_level->start();
}
