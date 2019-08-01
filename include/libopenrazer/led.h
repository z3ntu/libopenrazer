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

class Led : public QObject
{
    Q_OBJECT
public:
    Led(QDBusObjectPath objectPath, razer_test::RazerLedId ledId, QString lightingLocation);
    ~Led() override;

    QDBusObjectPath getObjectPath();
    razer_test::RazerEffect getCurrentEffect();
    QList<razer_test::RGB> getCurrentColors();
    razer_test::RazerLedId getLedId();

    /*!
     * \fn bool libopenrazer::Led::setOff()
     *
     * Sets the LED to none / off.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setOff();

    /*!
     * \fn bool libopenrazer::Led::setStatic(QColor color)
     *
     * Sets the lighting to static lighting in the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setStatic(QColor color);

    /*!
     * \fn bool libopenrazer::Led::setBreathSingle(QColor color)
     *
     * Sets the lighting to the single breath effect with the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBreathing(QColor color);

    /*!
     * \fn bool libopenrazer::Led::setBreathDual(QColor color, QColor color2)
     *
     * Sets the lighting to the dual breath effect with the specified \a color and \a color2.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBreathingDual(QColor color, QColor color2);

    /*!
     * \fn bool libopenrazer::Led::setBreathRandom()
     *
     * Sets the lighting wheel to the random breath effect.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBreathingRandom();

    /*!
     * \fn bool libopenrazer::Led::setBreathRandom()
     *
     * Sets the lighting wheel to the random breath effect.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBlinking(QColor color);

    /*!
     * \fn bool libopenrazer::Led::setSpectrum()
     *
     * Sets the lighting to spectrum mode.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setSpectrum();

    /*!
     * \fn bool libopenrazer::Led::setWave(WaveDirection direction)
     *
     * Sets the lighting effect to wave, in the direction \a direction.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setWave(razer_test::WaveDirection direction);

    /*!
     * \fn bool libopenrazer::Led::setReactive(QColor color, ReactiveSpeed speed)
     *
     * Sets the lighting to reactive mode with the specified \a color and \a speed.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setReactive(QColor color, razer_test::ReactiveSpeed speed);

    /*!
     * \fn bool libopenrazer::Led::setBrightness(double brightness)
     *
     * Sets the \a brightness (0-100).
     *
     * Returns if the D-Bus call was successful.
     */
    bool setBrightness(uchar brightness);

    /*!
     * \fn double libopenrazer::Led::getBrightness()
     *
     * Returns the current brightness (0-100).
     */
    uchar getBrightness();

private:
    LedPrivate *d;
};

}

#endif // LED_H
