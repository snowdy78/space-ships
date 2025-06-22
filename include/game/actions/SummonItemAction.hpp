#pragma once

#include "Helpers.hpp"
#include "coop/TransferableAction.hpp"
#include "game/GameManager.hpp"
#include "game/SpaceItem.hpp"
// FIX THIS REALIZATION !!!!!
// TODO SUMMONING WORKING CORRECTLY ON MULTIPLAYER GAME

template<class T>
concept SummonItemActionSummonType = requires(SpaceField field, T *t) { field.pushItem(t); };

template<SummonItemActionSummonType SummonType, class Action>
class AbstractSummonItemAction : public TransferActionBase<Action>
{
public:
	constexpr static const char *id_key = "id";
	AbstractSummonItemAction(const TransferableActionProps &props)
		: TransferActionBase<Action>(props)
	{
		if (!props.props.contains(id_key))
			throw std::runtime_error("props does not have key 'ship_id' to summon");
		if (!GameObjectFactory::instance().can_create(props.props[id_key]))
			throw std::runtime_error("unable to summon ship with unknown id");
		object_id = props.props[id_key].is_null() ? std::nullopt : std::optional<size_t>{ props.props[id_key] };
	}
	~AbstractSummonItemAction() override;
	void play() override
	{
		if (!object_id.has_value() || !GameManager::exist())
			return;
		auto obj = GameObjectFactory::create(*object_id);
		if (auto summon_object = std_impl::dynamic_unique_cast<SummonType>(std::move(obj)))
		{
			this->handleCreate(summon_object.get());
			auto raw_object_ptr = summon_object.release();
			try
			{
				GameManager::session()->field.pushItem(raw_object_ptr);
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

template<SummonItemActionSummonType SummonT, class Action>
AbstractSummonItemAction<SummonT, Action>::~AbstractSummonItemAction() = default;

template<SummonItemActionSummonType ST, class A>
struct BasicSummonItemAction : AbstractSummonItemAction<ST, A>
{
private:
	using base = AbstractSummonItemAction<ST, A>;

public:
	using init_func = std::function<void(ST &)>;
	using base::base;
	BasicSummonItemAction(
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
		return new BasicSummonItemAction(this->object_id, m_init_f);
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
template<SummonItemActionSummonType T>
struct SummonItemAction : BasicSummonItemAction<T, SummonItemAction<T>>
{
	using BasicSummonItemAction<SpaceItem, SummonItemAction>::BasicSummonItemAction;
};
