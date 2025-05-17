#include "game/actions/ActionManager.hpp"

#include "coop/TransferableAction.hpp"
#include "game/GameManager.hpp"

void ActionManager::realizeActions()
{
	if (isTransfering())
		m_transfering->switch_off();
	do
	{
		m_start_loop = true;
		for (auto iaction = m_actions.begin(); iaction != m_actions.end(); iaction = m_actions.erase(iaction))
			(*iaction)->play();
		m_start_loop = false;
		m_actions.swap(m_stack);
		m_stack.clear();
	}
	while (!m_actions.empty());
	if (m_transfering && !m_transfering->swiched_on())
		m_transfering->switch_on();
}

void ActionManager::transferAction(AbstractAction *action) const
{
	if (isTransfering())
	{
		if (!GameManager::exist() || !GameManager::instance().existOnline())
			std::cerr << "ERROR: cannot use action transfer\n";
		if (auto transfer_action = dynamic_cast<TransferableAction *>(action))
		{
			if (m_transfering->transfer_type() == TransferType::Tcp)
				GameManager::instance().online->tcp->send(transfer_action);
			else if (m_transfering->transfer_type() == TransferType::Udp)
				GameManager::instance().online->udp->send(transfer_action);
		}
	}
}

void ActionManager::addToTop(std::unique_ptr<AbstractAction> &&action)
{
	transferAction(action.get());
	appendInto(std::move(action));
}

void ActionManager::receiveToTop(std::unique_ptr<TransferableAction> &&action)
{
	std::unique_ptr<AbstractAction> a{std::move(action)};
	appendInto(std::move(a));
}

void ActionManager::setTransfering(TransferType type)
{
	m_transfering = type;
}

bool ActionManager::isTransfering() const
{
	return m_transfering && m_transfering->swiched_on();
}

void ActionManager::clear()
{
	m_actions.clear();
	m_stack.clear();
}

void ActionManager::start()
{
	realizeActions();
}

void ActionManager::update()
{
	realizeActions();
}

ActionManager::Transfering::Transfering(TransferType type)
	: m_transfer_type(type)
{
}

void ActionManager::Transfering::switch_off()
{
	m_turn = false;
}

void ActionManager::Transfering::switch_on()
{
	m_turn = true;
}

void ActionManager::Transfering::transfer_type(TransferType type)
{
	m_transfer_type = type;
}

bool ActionManager::Transfering::swiched_on() const
{
	return m_turn;
}

TransferType ActionManager::Transfering::transfer_type() const
{
	return m_transfer_type;
}
