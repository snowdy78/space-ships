#include "game/GameSession.hpp"

#include "Helpers.hpp"
#include "game/EnemyShip.hpp"
#include "game/PlayerShip.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"
#include "game/actions/AbstractSummonAction.hpp"
#include "game/levels/Level1.hpp"

struct SummonShipAction;

GameSession::GameSession(sf::RenderTarget &target)
	: LocalDriveSession(s_file_path),
	  camera(
		  target,
		  [this](const rn::Vec2f &before_pos) {
			  m_background.setPosition(camera.getPosition());
			  m_gameinfo.updateData("camera_pos");
			  m_gameinfo.updateData("view_area");
			  m_gameinfo.updateData("fps");
		  }
	  ),
	  field(this, &this->camera),
	  m_level(nullptr)
{
}

void GameSession::start()
{
	initVisibleInfo(); // init before camera move
	m_background.start();
	action_manager.start();
	field.start();
	if (m_level)
		m_level->start();
	createPlayer();
	if (player.expired())
		return;
	auto player_val = player.lock();
	rn::Vec2f res{ rn::VideoSettings::getResolution() };
	player_val->setPosition(res / 2.f);
	up_level<Level1>();
	m_gameinfo.setVisible(static_cast<bool>(m_mode));
}

void GameSession::update()
{
	m_background.update();
	if (!player.expired())
		const auto p = player.lock();
	action_manager.update();
	field.update();
	if (m_level->complete())
		up_level();
	if (m_level)
		m_level->update();
}

void GameSession::onEvent(sf::Event &event)
{
	m_background.onEvent(event);
	action_manager.onEvent(event);
	field.onEvent(event);
	if (m_level)
		m_level->onEvent(event);
	
	if (rn::isKeydown(sf::Keyboard::F3))
	{
		m_mode = m_mode == Mode::Developer ? Mode::User : Mode::Developer;
		m_gameinfo.setVisible(static_cast<bool>(m_mode));
	}
	if (mode() == Mode::Developer)
	{
		if (rn::isKeydown(sf::Keyboard::P))
		{
			action_manager.emplaceToTop<SummonShipAction>(EnemyShip::identifier, [this](AbstractShip &ship) {
				if (auto enemy = dynamic_cast<EnemyShip *>(&ship))
				{
#ifdef SPACE_SHIP_DEBUG
					if (player.expired())
						std::cerr << "cannot set nullptr as ship target\n";
#endif
					else
						enemy->setTarget(player);
					rn::Vec2f randomPosition{ rn::random::real(0.f, 1.f) * camera.getViewSize().x,
											  rn::random::real(0.f, 1.f) * camera.getViewSize().y };
					enemy->setPosition(camera.getPosition() + randomPosition);
				}
			});
		}
		if (rn::isKeydown(sf::Keyboard::O))
		{
			randomlySummonAsteroidOutsideArea<SimpleAsteroid>(
				camera.getTransform().transformRect({ {}, camera.getViewSize() }), 10.f
			);
		}
		if (rn::isKeydown(sf::Keyboard::I))
		{
			if (!player.expired())
			{
				player.lock()->setInvincible(true);
			}
#ifdef SPACE_SHIP_DEBUG
			else
			{
				std::cerr << "unable to do player invincible\n";
			}
#endif
		}
	}
}

void GameSession::createPlayer()
{
	player = field.summonShip<PlayerShip>();
}

void GameSession::initVisibleInfo()
{
	m_gameinfo.addData("camera_pos", [this]() -> sf::String {
		const auto p{ camera.getPosition() };
		return "{ " + std::to_string(p.x) + ", " + std::to_string(p.y) + " }";
	});
	m_gameinfo.addData("view_area", [this]() -> sf::String {
		const auto view = camera.getView();
		return "{ " + std::to_string(view.getCenter().x) + ", " + std::to_string(view.getCenter().y) + ", "
			   + std::to_string(view.getSize().x) + ", " + std::to_string(view.getSize().y) + " }";
	});
	m_gameinfo.addData("fps", [this]() -> sf::String {
		return std::to_string(rn::FPS);
	});
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

void GameSession::up_level()
{
	auto factory = m_level->next();
	m_level		 = factory->create(field);
	m_level->start();
}

GameSession::Mode GameSession::mode() const
{
	return m_mode;
}

void GameSession::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_background, states);
	target.draw(field, states);
	auto relative_camera = states;
	relative_camera.transform		*= camera.getTransform();
	if (m_level)
	{
		target.draw(*m_level, relative_camera);
		target.draw(m_gameinfo, relative_camera);
	}
}
