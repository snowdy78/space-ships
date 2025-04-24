#include "game/actions/ActionManager.hpp"

void ActionManager::realizeActions()
{
	do
	{
		start_loop = true;
		for (auto iaction = actions.begin(); iaction != actions.end(); iaction = actions.erase(iaction))
		{
			(*iaction)->play();
		}
		start_loop = false;
		actions.swap(stack);
		stack.clear();
	}
	while (!actions.empty());
}

void ActionManager::addToTop(std::unique_ptr<AbstractAction> &&action) 
{
	if (!start_loop)
	{
		actions.push_back(std::move(action));
	}
	else
	{
		stack.push_back(std::move(action));
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
