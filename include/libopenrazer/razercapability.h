// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RAZERCAPABILITY_H
#define RAZERCAPABILITY_H

namespace libopenrazer {

/*!
 * \brief Helper-class for providing information what effect is what
 *
 * Helper-class used by libopenrazer for providing lists of lighting effect capabilities that can be implemented e.g. in a combobox. It can contain the number of colors an effect needs, if the effect is the 'Wave' effect and a human readable string for displaying in the UI.
 */
class RazerCapability
{
public:
    /// @cond
    RazerCapability();
    RazerCapability(::openrazer::RazerEffect identifier, QString displayString, int numColors);
    RazerCapability(::openrazer::RazerEffect, QString displayString);
    RazerCapability(const RazerCapability &other);
    ~RazerCapability();
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
    ::openrazer::RazerEffect getIdentifier() const;

    /*!
     * Returns a human-readable string describing the capability
     *
     * e.g. \c "Spectrum"
     */
    QString getDisplayString() const;

private:
    int numColors;
    ::openrazer::RazerEffect identifier;
    QString displayString;
};

}

Q_DECLARE_METATYPE(libopenrazer::RazerCapability)

#endif // RAZERCAPABILITY_H
