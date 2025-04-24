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
    void emplaceToTop(const Args &...args);
    void addToTop(std::unique_ptr<AbstractAction> &&action);
    void clear();
    void start() override;
    void update() override;
};



template<class ActionT, class... Args>
void ActionManager::emplaceToTop(const Args &...args)
{
	if (!start_loop)
	{
		actions.emplace_back(std::make_unique<ActionT>(args...));
	}
	else
	{
		stack.emplace_back(std::make_unique<ActionT>(args...));
	}
}
