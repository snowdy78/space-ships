#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

TransferableActionProps::TransferableActionProps(OptionalObjectPtr author, OptionalObjectPtr contributor, JsonType props)
	: author(std::move(author)),
	  contributor(std::move(contributor)),
	  props(std::move(props))
{
}

bool TransferableActionProps::authorExist() const
{
	return author.has_value() && !author->expired();
}

bool TransferableActionProps::contributorExist() const
{
	return contributor.has_value() && !contributor->expired();
}

TransferableAction::TransferableAction(const TransferableActionProps &props)
	: m_author_ptr(props.author),
	  m_contributor_ptr(props.contributor)
{
	
	
}

TransferableAction::OptionalObjectPtr TransferableAction::getAuthor() const
{
	return m_author_ptr;
}

TransferableAction::OptionalObjectPtr TransferableAction::getContributor() const
{
	return m_contributor_ptr;
}

TransferableAction::OptionalId TransferableAction::getAuthorId() const
{
	if (m_author_ptr.has_value() && !m_author_ptr->expired())
	{
		const auto object = m_author_ptr->lock();
		return object->id();
	}
	return std::nullopt;
}

TransferableAction::OptionalId TransferableAction::getContributorId() const
{
	if (m_contributor_ptr.has_value() && !m_contributor_ptr->expired())
	{
		const auto object		 = m_contributor_ptr->lock();
		return object->id();
	}
	return std::nullopt;
}

bool TransferableAction::playable() const
{
	auto ahv = m_author_ptr.has_value();
	auto chv = m_contributor_ptr.has_value();
	return (!ahv || ahv && !m_author_ptr->expired())
	       && (!chv || chv && !m_contributor_ptr->expired());
}
