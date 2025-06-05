#include "game/GameSession.hpp"
#include "game/PlayerShip.hpp"


GameSession::GameSession(TargetCamera &&camera)
	: LocalDriveSession(s_file_path),
	  camera(std::move(camera)),
	  field(this, &this->camera)
{
}

void GameSession::start()
{
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	field.start();
	createPlayer();
	if (player.expired())
		return;
	auto player_val = player.lock();
	player_val->setPosition(res / 2.f);
}

void GameSession::update()
{
	action_manager.update();
	field.update();
}

void GameSession::onEvent(sf::Event &event)
{
	field.onEvent(event);
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

void GameSession::GameSessionSpaceField::onObjectSummon(GameObject *object) const
{
	if (!m_session)
		return;
	size_t hash	  = m_session->m_hash(object);
	auto &objects = m_session->m_game_objects;
	if (std::find(objects.begin(), objects.end(), hash) != objects.end())
		objects.push_back(hash);
}
