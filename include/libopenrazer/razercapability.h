/*
 * Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RAZERCAPABILITY_H
#define RAZERCAPABILITY_H

namespace libopenrazer {

/*!
 * \class libopenrazer::RazerCapability
 * \inmodule libopenrazer
 *
 * \brief The libopenrazer::RazerCapability class provides helper-class used by libopenrazer for providing lists of lighting effect capabilities that can be implemented e.g. in a combobox. It can contain the number of colors an effect needs, if the effect is the 'Wave' effect and a human readable string for displaying in the UI.
 */
class RazerCapability
{
public:
    RazerCapability();
    RazerCapability(razer_test::RazerEffect identifier, QString displayString, int numColors);
    RazerCapability(razer_test::RazerEffect, QString displayString, bool wave);
    RazerCapability(const RazerCapability &other);
    ~RazerCapability();

    /*!
     * \fn int libopenrazer::RazerCapability::getNumColors() const
     *
     * Returns the number of colors this capability can use.
     */
    int getNumColors() const;

    /*!
     * \fn razer_test::RazerEffect libopenrazer::RazerCapability::getIdentifier() const
     *
     * Returns a string identifying the capability
     *
     * e.g. \c "lighting_logo_spectrum"
     */
    razer_test::RazerEffect getIdentifier() const;

    /*!
     * \fn QString libopenrazer::RazerCapability::getDisplayString() const
     *
     * Returns a human-readable string describing the capability
     *
     * e.g. \c "Spectrum"
     */
    QString getDisplayString() const;

    /*!
     * \fn bool libopenrazer::RazerCapability::isWave() const
     *
     * Returns if the capability is the wave effect.
     */
    bool isWave() const;

private:
    int numColors;
    razer_test::RazerEffect identifier;
    QString displayString;
    bool wave;
};

}

Q_DECLARE_METATYPE(libopenrazer::RazerCapability)

#endif // RAZERCAPABILITY_H
