// Copyright (C) 2017-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer.h"

namespace libopenrazer {

Capability::Capability()
{
    this->identifier = ::openrazer::Effect::Off;
    this->displayString = "";
    this->numColors = 0;
}
Capability::Capability(::openrazer::Effect identifier, const char *displayString, int numColors)
{
    this->identifier = identifier;
    this->displayString = displayString;
    this->numColors = numColors;
}

Capability::Capability(const Capability &other)
{
    this->identifier = other.identifier;
    this->displayString = other.displayString;
    this->numColors = other.numColors;
}

Capability::~Capability() = default;

int Capability::getNumColors() const
{
    return numColors;
}

::openrazer::Effect Capability::getIdentifier() const
{
    return identifier;
}

const char *Capability::getDisplayString() const
{
    return displayString;
}

}
