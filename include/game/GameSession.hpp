#pragma once

#include "components/LocalDriveSession.hpp"
#include "SpaceField.hpp"
#include "components/TargetCamera.hpp"

class GameSession : protected LocalDriveSession
{
	constexpr static const char *s_file_path = "src/game_session_data.json";

	void createPlayer();
protected:

	void beforeSave() override;
	void afterLoad() override;
public:
	GameSession(TargetCamera &&camera);
	AbstractShip *player = nullptr;
	TargetCamera camera;
	struct GameSessionSpaceField : SpaceField
	{
		GameSessionSpaceField(GameSession *session, const Camera2d *camera = nullptr);
		void onObjectAppend(GameObject *object) const override;
	private:
		GameSession *m_session;
	} field;
private:
	rn::Json m_game_objects = rn::Json::array();
	std::hash<GameObject *> m_hash;
};
