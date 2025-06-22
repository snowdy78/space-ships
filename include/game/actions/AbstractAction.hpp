#pragma once

#include "decl.hpp"

class AbstractAction
{
#ifdef SPACE_SHIP_DEBUG
	friend class TransferableActionFabric;
	inline static std::string name{ "UnknownAction" };
#endif
public:
	AbstractAction()					 = default;
	virtual ~AbstractAction()			 = 0;
	virtual void play()					 = 0;
	virtual size_t static_id() const	 = 0;
	virtual AbstractAction *copy() const = 0;
	virtual bool playable() const;
#ifdef SPACE_SHIP_DEBUG
	static const std::string &getName();
#endif
};
