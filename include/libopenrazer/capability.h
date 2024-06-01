// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CAPABILITY_H
#define CAPABILITY_H

#include "libopenrazer/openrazer.h"

namespace libopenrazer {

/*!
 * \brief Helper-class for providing information what effect is what
 *
 * Helper-class used by libopenrazer for providing lists of lighting effect capabilities that can be implemented e.g. in a combobox. It can contain the number of colors an effect needs, if the effect is the 'Wave' effect and a human readable string for displaying in the UI.
 */
class Capability
{
public:
    /// @cond
    Capability();
    Capability(::openrazer::Effect identifier, const char *displayString, int numColors);
    Capability(::openrazer::Effect, const char *displayString);
    Capability(const Capability &other);
    ~Capability();
    /// @endcond

    /*!
     * Returns the number of colors this capability can use.
     */
    int getNumColors() const;

    /*!
     * Returns a string identifying the capability
     *
     * e.g. \c "lighting_logo_spectrum"
     */
    ::openrazer::Effect getIdentifier() const;

    /*!
     * Returns a human-readable string describing the capability
     *
     * This needs to be translated by the user!
     *   qApp->translate("libopenrazer", foo.getDisplayString())
     *
     * e.g. \c "Spectrum"
     */
    const char *getDisplayString() const;

private:
    int numColors;
    ::openrazer::Effect identifier;
    const char *displayString;
};

}

Q_DECLARE_METATYPE(libopenrazer::Capability)

#endif // CAPABILITY_H
