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

#ifndef LED_H
#define LED_H

#include <QDBusInterface>
#include <razer_test.h>

namespace libopenrazer {

class LedPrivate;

/*!
 * \brief Abstraction for accessing Led objects via D-Bus.
 */
class Led : public QObject
{
    Q_OBJECT
public:
    /// @cond
    Led(QDBusObjectPath objectPath); // used by razer_test
    Led(QDBusObjectPath objectPath, razer_test::RazerLedId ledId, QString lightingLocation); // used by openrazer
    ~Led() override;
    /// @endcond

    /*!
     * Returns the D-Bus object path of the Led
     */
    QDBusObjectPath getObjectPath();

    /*!
     * Returns the currently active effect
     */
    razer_test::RazerEffect getCurrentEffect();

    /*!
     * Returns the currently active colors (the list will have at least 3 elements)
     */
    QList<razer_test::RGB> getCurrentColors();

    /*!
     * Returns the Led ID of this Led
     */
    razer_test::RazerLedId getLedId();

    /*!
     * Sets the LED to none / off.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setOff();

    /*!
     * Sets the lighting to static lighting in the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setStatic(QColor color);

    /*!
     * Sets the lighting to the single breath effect with the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBreathing(QColor color);

    /*!
     * Sets the lighting to the dual breath effect with the specified \a color and \a color2.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBreathingDual(QColor color, QColor color2);

    /*!
     * Sets the lighting wheel to the random breath effect.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBreathingRandom();

    /*!
     * Sets the lighting wheel to the random breath effect.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBlinking(QColor color);

    /*!
     * Sets the lighting to spectrum mode.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setSpectrum();

    /*!
     * Sets the lighting effect to wave, in the direction \a direction.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setWave(razer_test::WaveDirection direction);

    /*!
     * Sets the lighting to reactive mode with the specified \a color and \a speed.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setReactive(QColor color, razer_test::ReactiveSpeed speed);

    /*!
     * Sets the \a brightness (`0` - `255`).
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBrightness(uchar brightness);

    /*!
     * Returns the current brightness (`0` - `255`).
     */
    uchar getBrightness();

private:
    LedPrivate *d;
};

}

#endif // LED_H
