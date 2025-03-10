#pragma once

#include "decl.hpp"

class AbstractAction
{
public:
    AbstractAction() = default;
    virtual ~AbstractAction() = 0;
    virtual void play() = 0;
    virtual AbstractAction *copy() const = 0;
};
