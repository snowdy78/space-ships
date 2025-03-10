#pragma once

#include "decl.hpp"

class TransferableFabric 
{
    using create_func = std::function<Transferable *()>;
    std::unordered_map<size_t, create_func> transfer_objects{};
    size_t id_encounter = 0;
    TransferableFabric() = default;
public:
    static TransferableFabric &instance();

    const create_func &get(size_t id);
    template<class T>
    size_t push();
    void erase(size_t id);
    void clear();
};

template<class T>
size_t TransferableFabric::push() 
{
    transfer_objects.emplace(id_encounter, []() { return new T(); });
    return id_encounter++;
}
