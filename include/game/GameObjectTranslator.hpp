#pragma once

#include "coop/TransferableObject.hpp"

class GameObjectTranslator : public TransferObjectBase<GameObjectTranslator>
{
	std::unordered_map<size_t, TransferJson> data_to_update{};
	static constexpr size_t npos = -1;
	friend class GameObjectFactory;

public:
	enum class TranslateType
	{
		Append = 'a',
		Clear  = 'c',
	};
	GameObjectTranslator();

	TranslateType getTranslateType() const;
	void setTranslateType(TranslateType type);

	void assignUpdateData(GameObjectFactory::ConstIterator begin, const GameObjectFactory::ConstIterator &end);
	/**
	 * @brief returns true, if the object was inserted
	 *
	 * @param iterator
	 */
	bool insert(GameObjectFactory::ConstIterator iterator);
	size_t erase(size_t game_object_id);
	size_t updateCount() const;
	void clearUpdateData();

	void receiveJson(const rn::Json &json) override;
	rn::Json toJson() const override;

private:
	TranslateType translate_type = TranslateType::Append;
};
