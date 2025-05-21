#include "game/GameSession.hpp"
#include "game/PlayerShip.hpp"


GameSession::GameSession(TargetCamera &&camera)
	: LocalDriveSession(s_file_path),
	  camera(std::move(camera)),
	  field(this, &this->camera)
{
	createPlayer();
}

void GameSession::createPlayer()
{
	player = field.summonShip<PlayerShip>(&camera);
}

void GameSession::afterLoad()
{
	LocalDriveSession::afterLoad();
	m_game_objects = toJson()["game_objects"];
}

void GameSession::beforeSave()
{
	LocalDriveSession::beforeSave();
	this->operator[]("game_objects") = m_game_objects;
}

GameSession::GameSessionSpaceField::GameSessionSpaceField(GameSession *session, const Camera2d *camera)
	: SpaceField(camera),
	  m_session(session)
{
}

void GameSession::GameSessionSpaceField::onObjectAppend(GameObject *object) const
{
	if (!m_session)
		return;
	size_t hash	  = m_session->m_hash(object);
	auto &objects = m_session->m_game_objects;
	if (std::find(objects.begin(), objects.end(), hash) != objects.end())
		objects.push_back(hash);
}
