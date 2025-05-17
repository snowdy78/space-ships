#pragma once

#include "GameObject.hpp"
#include "coop/TransferableObject.hpp"
#include "decl.hpp"

template<class T>
concept GameObjectConcept = std::is_base_of_v<GameObject, T> && !std::is_same_v<GameObject, T>;

class GameObjectFabric
{
	std::unordered_map<size_t, GameObject *> objects{};
	size_t id_encounter = 0;

	void emplace(size_t id, GameObject *object);
	friend class GameObjectFabricTranslator;
	GameObjectFabric();

public:
	using Iterator		= std::unordered_map<size_t, GameObject *>::iterator;
	using ConstIterator = std::unordered_map<size_t, GameObject *>::const_iterator;

	static GameObjectFabric &instance();

	Iterator begin();
	ConstIterator cbegin() const;
	ConstIterator begin() const;
	Iterator end();
	ConstIterator cend() const;
	ConstIterator end() const;

	std::vector<std::unique_ptr<TransferableObject>> assign(const GameObjectFabricTranslator &translator);
	std::vector<std::unique_ptr<TransferableObject>> update(const GameObjectFabricTranslator &translator);
    void remove(const GameObjectFabricTranslator &translator, const std::function<void(GameObject *)> &on_remove = [](GameObject *) {});
	GameObject *const &get(size_t id);
	const GameObject *const &get(size_t id) const;
	size_t push(GameObject *);
	size_t find(const GameObject *object) const;
	void erase(size_t id);
	void clear();
	size_t size() const;
};

class GameObjectFabricTranslator : public TransferObjectBase<GameObjectFabricTranslator>
{
	std::unordered_map<size_t, TransferJson> data_to_update{};
	static constexpr size_t npos = -1;
    friend class GameObjectFabric;
public:
	enum class TranslateType
	{
		Append = 'a',
		Clear  = 'c',
	};
	GameObjectFabricTranslator();

	TranslateType getTranslateType() const;
	void setTranslateType(TranslateType type);

	void assignUpdateData(GameObjectFabric::ConstIterator begin, const GameObjectFabric::ConstIterator &end);
	/**
	 * @brief returns true, if the object was inserted
	 * 
	 * @param iterator 
	 */
    bool insert(GameObjectFabric::ConstIterator iterator);
	size_t erase(size_t game_object_id);
	size_t updateCount() const;
    void clearUpdateData();

	void receiveJson(const rn::Json &json) override;
	TransferableObject::TransferJson toJson() const override;

private:
	TranslateType translate_type = TranslateType::Append;
};
