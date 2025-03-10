#pragma once

#include "decl.hpp"
#include "GameObject.hpp"
#include "coop/Transferable.hpp"

class GameObjectFabric
{
    std::unordered_map<size_t, GameObject *> objects{};
    size_t id_encounter = 0;
    GameObjectFabric();

    void assignReceived(const rn::Json &ids_of_objects);
    friend class ClientSocket;
    friend class GameObjectFabricTranslator;
public:
    static GameObjectFabric &instance();

    GameObject *get(size_t id);
    size_t push(GameObject *object);
    size_t getByValue(GameObject *object);
    void erase(size_t id);
    void clear();
};

class GameObjectFabricTranslator : public Transferable
{
    inline static size_t id = identify<Transferable>();
public:
    GameObjectFabricTranslator()
        : Transferable(Transferable::object)
    {

    }
    rn::Json toJson() const override;
};
