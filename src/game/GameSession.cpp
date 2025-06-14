#include "game/GameSession.hpp"
#include "game/PlayerShip.hpp"

GameSession::GameSession(TargetCamera &&camera)
	: LocalDriveSession(s_file_path),
	  camera(std::move(camera)),
	  field(this, &this->camera),
	  m_level(nullptr)
{
}

void GameSession::start()
{
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	action_manager.start();
	field.start();
	if (m_level)
		m_level->start();
	createPlayer();
	if (player.expired())
		return;
	auto player_val = player.lock();
	player_val->setPosition(res / 2.f);
}

void GameSession::update()
{
	if (!player.expired())
		const auto p = player.lock();
	action_manager.update();
	field.update();
	if (m_level)
		m_level->update();
}

void GameSession::onEvent(sf::Event &event)
{
	field.onEvent(event);
	if (m_level)
		m_level->onEvent(event);
}

void GameSession::createPlayer()
{
	player = field.summonShip<PlayerShip>();
}

void GameSession::afterLoad()
{
	LocalDriveSession::afterLoad();
	m_game_objects = json()["game_objects"];
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

void GameSession::GameSessionSpaceField::onObjectSummon(const StatePtrType &state_ptr) const
{
	if (!m_session || state_ptr.expired())
		return;
	auto state	  = state_ptr.lock();
	size_t hash	  = m_session->m_hash(state.get());
	auto &objects = m_session->m_game_objects;
	if (std::find(objects.begin(), objects.end(), hash) != objects.end())
		objects.push_back(hash);
}

void GameSession::GameSessionSpaceField::onObjectDestroy(const StatePtrType &state_ptr) const
{
	if (!m_session || !m_session->m_level)
		return;
	auto &level	   = m_session->m_level;
	auto it_entity = std::ranges::find_if(*level, [&state_ptr](const StatePtrType &right) {
		return !state_ptr.expired() && !right.expired() && state_ptr.lock() == right.lock();
	});
	if (it_entity != level->end())
		level->erase(it_entity);
}

void GameSession::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_level)
	{
		auto rs = states;
		rs		= states.transform * camera.getTransform();
		target.draw(*m_level, rs);
	}
	target.draw(field, states);
}
