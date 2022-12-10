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

#include "libopenrazer/openrazer.h"

#include <QDBusInterface>

namespace libopenrazer {

/*!
 * \brief Abstraction for accessing Led objects via D-Bus.
 */
class Led : public QObject
{
    Q_OBJECT
public:
    /*!
     * Returns the D-Bus object path of the Led
     */
    virtual QDBusObjectPath getObjectPath() = 0;

    /*!
     * Returns if the device has brightness functionality
     */
    virtual bool hasBrightness() = 0;

    /*!
     * Returns if the device has the specified \a fx
     */
    virtual bool hasFx(::openrazer::RazerEffect fx) = 0;

    /*!
     * Returns the currently active effect
     */
    virtual ::openrazer::RazerEffect getCurrentEffect() = 0;

    /*!
     * Returns the currently active colors (the list will have at least 3 elements)
     */
    virtual QVector<::openrazer::RGB> getCurrentColors() = 0;

    /*!
     * Returns the Led ID of this Led
     */
    virtual ::openrazer::RazerLedId getLedId() = 0;

    /*!
     * Sets the LED to none / off.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setOff() = 0;

    /*!
     * Sets the LED to on.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setOn() = 0;

    /*!
     * Sets the lighting to static lighting in the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setStatic(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting to the single breath effect with the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setBreathing(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting to the dual breath effect with the specified \a color and \a color2.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2) = 0;

    /*!
     * Sets the lighting wheel to the random breath effect.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setBreathingRandom() = 0;

    /*!
     * Sets the lighting wheel to the random breath effect.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setBlinking(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting to spectrum mode.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setSpectrum() = 0;

    /*!
     * Sets the lighting effect to wave, in the direction \a direction.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setWave(::openrazer::WaveDirection direction) = 0;

    /*!
     * Sets the lighting to reactive mode with the specified \a color and \a speed.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed) = 0;

    /*!
     * Sets the lighting effect to ripple with the specified \a color.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setRipple(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting effect to ripple with random colors.
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setRippleRandom() = 0;

    /*!
     * Sets the \a brightness (`0` - `255`).
     *
     * Returns if the D-Bus call was successful.
     */
    virtual bool setBrightness(uchar brightness) = 0;

    /*!
     * Returns the current brightness (`0` - `255`).
     */
    virtual uchar getBrightness() = 0;
};

namespace openrazer {

class Device;
class LedPrivate;
class Led : public ::libopenrazer::Led
{
public:
    Led(Device *device, QDBusObjectPath objectPath, ::openrazer::RazerLedId ledId, QString lightingLocation);
    ~Led() override;

    QDBusObjectPath getObjectPath() override;
    bool hasBrightness() override;
    bool hasFx(::openrazer::RazerEffect fx) override;
    ::openrazer::RazerEffect getCurrentEffect() override;
    QVector<::openrazer::RGB> getCurrentColors() override;
    ::openrazer::RazerLedId getLedId() override;
    bool setOff() override;
    bool setOn() override;
    bool setStatic(::openrazer::RGB color) override;
    bool setBreathing(::openrazer::RGB color) override;
    bool setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2) override;
    bool setBreathingRandom() override;
    bool setBlinking(::openrazer::RGB color) override;
    bool setSpectrum() override;
    bool setWave(::openrazer::WaveDirection direction) override;
    bool setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed) override;
    bool setRipple(::openrazer::RGB color) override;
    bool setRippleRandom() override;
    bool setBrightness(uchar brightness) override;
    uchar getBrightness() override;

private:
    LedPrivate *d;
};

}

namespace razer_test {

class Device;
class LedPrivate;
class Led : public ::libopenrazer::Led
{
public:
    Led(Device *device, QDBusObjectPath objectPath);
    ~Led() override;

    QDBusObjectPath getObjectPath() override;
    bool hasBrightness() override;
    bool hasFx(::openrazer::RazerEffect fx) override;
    ::openrazer::RazerEffect getCurrentEffect() override;
    QVector<::openrazer::RGB> getCurrentColors() override;
    ::openrazer::RazerLedId getLedId() override;
    bool setOff() override;
    bool setOn() override;
    bool setStatic(::openrazer::RGB color) override;
    bool setBreathing(::openrazer::RGB color) override;
    bool setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2) override;
    bool setBreathingRandom() override;
    bool setBlinking(::openrazer::RGB color) override;
    bool setSpectrum() override;
    bool setWave(::openrazer::WaveDirection direction) override;
    bool setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed) override;
    bool setRipple(::openrazer::RGB color) override;
    bool setRippleRandom() override;
    bool setBrightness(uchar brightness) override;
    uchar getBrightness() override;

private:
    LedPrivate *d;
};

}

}

#endif // LED_H
