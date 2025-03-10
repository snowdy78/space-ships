#pragma once

#include "AbstractAction.hpp"

class ActionManager : public rn::LogicalObject
{
    std::vector<std::unique_ptr<AbstractAction>> actions;
    // for appending actions while action is performed
    std::vector<std::unique_ptr<AbstractAction>> stack;
    bool start_loop = false;
    void realizeActions();
public:
    ActionManager() = default;
    template<class ActionT, class...Args>
    void addToTop(Args &&...args);
    void start() override;
    void update() override;
};