#include "game/actions/ActionManager.hpp"

void ActionManager::realizeActions() 
{
    do
	{
		start_loop = true;
		for (auto iaction = actions.begin(); iaction != actions.end(); iaction = actions.erase(iaction))
		{
			(*iaction)->play();
			actions.erase(iaction);
		}
		start_loop = false;
		actions.swap(stack);
		stack.clear();
	}
	while (!actions.empty());
}

template<class ActionT, class... Args>
void ActionManager::addToTop(Args &&...args)
{
	if (!start_loop)
	{
		actions.push_back(new ActionT(args...));
	}
	else
	{
		stack.push_back(new ActionT(args...));
	}
}

void ActionManager::clear() 
{
    actions.clear();
	stack.clear();
}

void ActionManager::start()
{
	realizeActions();
}

void ActionManager::update()
{
	realizeActions();
}
