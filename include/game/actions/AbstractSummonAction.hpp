#pragma once

#include "coop/TransferableAction.hpp"
#include "game/GameManager.hpp"
#include "game/ships/AbstractShip.hpp"
#include "game/AbstractAsteroid.hpp"
#include "game/AbstractBullet.hpp"
#include "dynamic_unique_cast.hpp"

// FIX THIS REALIZATION !!!!!
// TODO SUMMONING NOT WORKING CORRECTLY ON MULTIPLAYER GAME

template<class T>
concept SummonActionSummonType = requires(SpaceField field, T *t) { field.push_back(t); };

template<SummonActionSummonType SummonType, class Action>
class AbstractSummonAction : public TransferActionBase<Action>
{
public:
	constexpr static const char *id_key = "id";
	AbstractSummonAction(const TransferableActionProps &props)
		: TransferActionBase<Action>(props)
	{
		if (!props.props.contains(id_key))
			throw std::runtime_error("props does not have key 'ship_id' to summon");
		if (!GameObjectFactory::instance().can_create(props.props[id_key]))
			throw std::runtime_error("unable to summon ship with unknown id");
		object_id = props.props[id_key].is_null() ? std::nullopt : std::optional<size_t>{ props.props[id_key] };
	}
	~AbstractSummonAction() override;
	void play() override
	{
		if (!object_id.has_value() || !GameManager::exist())
			return;
		auto obj = GameObjectFactory::create(*object_id);
		if (auto summon_object = std_impl::dynamic_unique_cast<SummonType>(std::move(obj)))
		{
			auto raw_object_ptr = summon_object.release();
			this->handleCreate(raw_object_ptr);
			try
			{
				GameManager::session()->field.push_back(raw_object_ptr);
			}
			catch (std::runtime_error &err)
			{
				delete raw_object_ptr;
#ifdef SPACE_SHIP_DEBUG
				std::cerr << err.what() << " (unable to summon null)" << '\n';
#endif
			}
		}
	}

	rn::Json toJson() const override
	{
		if (object_id.has_value())
			return {
				{ id_key, *object_id }
			};
		return {
			{ id_key, nullptr }
		};
	}

protected:
	std::optional<size_t> object_id = std::nullopt;

	virtual void handleCreate(SummonType *) = 0;
};

template<SummonActionSummonType SummonT, class Action>
AbstractSummonAction<SummonT, Action>::~AbstractSummonAction() = default;

template<SummonActionSummonType ST, class A>
struct SummonAction : AbstractSummonAction<ST, A>
{
private:
	using base = AbstractSummonAction<ST, A>;

public:
	using init_func = std::function<void(ST &)>;
	using base::base;
	SummonAction(
		std::optional<size_t> id, init_func initializing_function = [](ST &) {}
	)
		: base(
			  TransferableActionProps{ std::nullopt,
									   std::nullopt,
									   { { base::id_key, id.has_value() ? rn::Json(*id) : rn::Json(nullptr) } } }
		  ),
		  m_init_f(std::move(initializing_function))
	{
	}
	AbstractAction *copy() const override
	{
		return new SummonAction(this->object_id, m_init_f);
	}

protected:
	void handleCreate(ST *obj) override
	{
		if (!obj)
			return;
		m_init_f(*obj);
	}

private:
	init_func m_init_f = [](ST &) {};
};
struct SummonShipAction : SummonAction<AbstractShip, SummonShipAction>
{
	using SummonAction::SummonAction;
};

struct SummonBulletAction : SummonAction<AbstractBullet, SummonBulletAction>
{
	using SummonAction::SummonAction;
};

struct SummonAsteroidAction : SummonAction<AbstractAsteroid, SummonAsteroidAction>
{
	using SummonAction::SummonAction;
};
