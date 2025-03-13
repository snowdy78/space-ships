#pragma once

#include "decl.hpp"
#include "GameObject.hpp"
#include "coop/TransferableObject.hpp"

template<class T>
concept GameObjectConcept = std::is_base_of<GameObject, T>::value && !std::is_same_v<GameObject, T>;

class GameObjectFabric
{
    std::unordered_map<size_t, GameObject *> objects{};
    size_t id_encounter = 0;

    GameObjectFabric();
    std::vector<std::unique_ptr<TransferableObject>> assign(const GameObjectFabricTranslator &translator);

    friend class ClientSocket;
    friend class GameObjectFabricTranslator;
public:
    static GameObjectFabric &instance();
    
    GameObject * const &get(size_t id);
    size_t push(GameObject *);
    size_t getByValue(GameObject *object);
    void erase(size_t id);
    void clear();
};

class GameObjectFabricTranslator : public TransferableObject
{
    static size_t id;
public:
    GameObjectFabricTranslator();
    void receiveJson(const rn::Json &json) override;
    TransferableObject::TransferJson toJson() const override;
};

