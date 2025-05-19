#pragma once

#include "GameObject.hpp"
#include "decl.hpp"

template<class T>
concept GameObjectConcept = std::is_base_of_v<GameObject, T> && !std::is_same_v<GameObject, T>;

class GameObjectFactory
{
	using create_fn = std::function<GameObject *()>;
	std::unordered_map<size_t, GameObject *> objects{};
	std::unordered_map<size_t, create_fn> m_factory{};
	size_t unique_object_counter = 0;
	size_t id_encounter			 = 0;

	void emplace(size_t id, GameObject *object);
	friend class GameObjectTranslator;
	GameObjectFactory();

public:
	using Iterator		= std::unordered_map<size_t, GameObject *>::iterator;
	using ConstIterator = std::unordered_map<size_t, GameObject *>::const_iterator;

	static GameObjectFactory &instance();
	static std::unique_ptr<GameObject> create(size_t id);
	Iterator begin();
	ConstIterator cbegin() const;
	ConstIterator begin() const;
	Iterator end();
	ConstIterator cend() const;
	ConstIterator end() const;
	using emplace_callback_fn = std::function<void(const std::unique_ptr<GameObject> &)>;
	using remove_callback_fn = std::function<void(GameObject *)>;
	void update(
		const GameObjectTranslator &translator, const emplace_callback_fn &emplace_outside
		= [](const std::unique_ptr<GameObject> &) {}
	);

	void remove(
		const GameObjectTranslator &translator,
		const remove_callback_fn &callback
		= [](GameObject *) {}
	);
	GameObject *const &get(size_t id);
	const GameObject *const &get(size_t id) const;
	size_t push(GameObject *);

	template<GameObjectConcept T>
	size_t push() noexcept;
	size_t find(const GameObject *object) const;
	void erase(size_t id);
	void clear();
	size_t size() const;
};

template<GameObjectConcept T>
size_t GameObjectFactory::push() noexcept
{
	m_factory.emplace(++unique_object_counter, []() -> GameObject * {
		return new T;
	});
	std::cout << "object registered [" << typeid(T).name() << "] with identifier: " << unique_object_counter << "\n";
	return unique_object_counter;
}
