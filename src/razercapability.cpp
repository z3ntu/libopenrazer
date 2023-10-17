// Copyright (C) 2017-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer.h"

namespace libopenrazer {

RazerCapability::RazerCapability()
{
    this->identifier = ::openrazer::RazerEffect::Off;
    this->displayString = "";
    this->numColors = 0;
}
RazerCapability::RazerCapability(::openrazer::RazerEffect identifier, QString displayString, int numColors)
{
    this->identifier = identifier;
    this->displayString = displayString;
    this->numColors = numColors;
}

RazerCapability::RazerCapability(const RazerCapability &other)
{
    this->identifier = other.identifier;
    this->displayString = other.displayString;
    this->numColors = other.numColors;
}

RazerCapability::~RazerCapability() = default;

int RazerCapability::getNumColors() const
{
    return numColors;
}

::openrazer::RazerEffect RazerCapability::getIdentifier() const
{
    return identifier;
}

QString RazerCapability::getDisplayString() const
{
    return displayString;
}

}
