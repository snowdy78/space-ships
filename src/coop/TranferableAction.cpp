#include "coop/TransferableAction.hpp"
#include "game/GameObject.hpp"

TransferableActionProps::TransferableActionProps(GameObject *author, GameObject *contributor, rn::Json props)
	: author(author),
	  contributor(contributor),
	  props(std::move(props))
{
}

TransferableAction::TransferableAction(const TransferableActionProps &props)
	: m_author_ptr(props.author),
	  m_contributor_ptr(props.contributor),
	  m_author_id(props.author ? std::optional{ props.author->id() } : std::nullopt),
	  m_contributor_id(props.contributor ? std::optional{ props.contributor->id() } : std::nullopt)
{
}

GameObject *TransferableAction::getAuthor() const
{
	return m_author_ptr;
}

GameObject *TransferableAction::getContributor() const
{
	return m_contributor_ptr;
}
