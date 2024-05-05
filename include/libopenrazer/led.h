// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    virtual bool hasFx(::openrazer::Effect fx) = 0;

    /*!
     * Returns the currently active effect
     */
    virtual ::openrazer::Effect getCurrentEffect() = 0;

    /*!
     * Returns the currently active colors (the list will have at least 3 elements)
     */
    virtual QVector<::openrazer::RGB> getCurrentColors() = 0;

    /*!
     * Returns the wave direction of this Led
     */
    virtual ::openrazer::WaveDirection getWaveDirection() = 0;

    /*!
     * Returns the Led ID of this Led
     */
    virtual ::openrazer::LedId getLedId() = 0;

    /*!
     * Sets the LED to none / off.
     */
    virtual void setOff() = 0;

    /*!
     * Sets the LED to on.
     */
    virtual void setOn() = 0;

    /*!
     * Sets the lighting to static lighting in the specified \a color.
     */
    virtual void setStatic(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting to the single breath effect with the specified \a color.
     */
    virtual void setBreathing(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting to the dual breath effect with the specified \a color and \a color2.
     */
    virtual void setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2) = 0;

    /*!
     * Sets the lighting to the random breath effect.
     */
    virtual void setBreathingRandom() = 0;

    /*!
     * Sets the lighting to the mono-color breath effect.
     */
    virtual void setBreathingMono() = 0;

    /*!
     * Sets the lighting wheel to the random breath effect.
     */
    virtual void setBlinking(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting to spectrum mode.
     */
    virtual void setSpectrum() = 0;

    /*!
     * Sets the lighting effect to wave, in the direction \a direction.
     */
    virtual void setWave(::openrazer::WaveDirection direction) = 0;

    /*!
     * Sets the lighting effect to wheel, in the direction \a direction.
     */
    virtual void setWheel(::openrazer::WheelDirection direction) = 0;

    /*!
     * Sets the lighting to reactive mode with the specified \a color and \a speed.
     */
    virtual void setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed) = 0;

    /*!
     * Sets the lighting effect to ripple with the specified \a color.
     */
    virtual void setRipple(::openrazer::RGB color) = 0;

    /*!
     * Sets the lighting effect to ripple with random colors.
     */
    virtual void setRippleRandom() = 0;

    /*!
     * Sets the \a brightness (`0` - `255`).
     */
    virtual void setBrightness(uchar brightness) = 0;

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
    Led(Device *device, QDBusObjectPath objectPath, ::openrazer::LedId ledId, QString lightingLocation);
    ~Led() override;

    QDBusObjectPath getObjectPath() override;
    bool hasBrightness() override;
    bool hasFx(::openrazer::Effect fx) override;
    ::openrazer::Effect getCurrentEffect() override;
    QVector<::openrazer::RGB> getCurrentColors() override;
    ::openrazer::WaveDirection getWaveDirection() override;
    ::openrazer::LedId getLedId() override;
    void setOff() override;
    void setOn() override;
    void setStatic(::openrazer::RGB color) override;
    void setBreathing(::openrazer::RGB color) override;
    void setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2) override;
    void setBreathingRandom() override;
    void setBreathingMono() override;
    void setBlinking(::openrazer::RGB color) override;
    void setSpectrum() override;
    void setWave(::openrazer::WaveDirection direction) override;
    void setWheel(::openrazer::WheelDirection direction) override;
    void setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed) override;
    void setRipple(::openrazer::RGB color) override;
    void setRippleRandom() override;
    void setBrightness(uchar brightness) override;
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
    bool hasFx(::openrazer::Effect fx) override;
    ::openrazer::Effect getCurrentEffect() override;
    QVector<::openrazer::RGB> getCurrentColors() override;
    ::openrazer::WaveDirection getWaveDirection() override;
    ::openrazer::LedId getLedId() override;
    void setOff() override;
    void setOn() override;
    void setStatic(::openrazer::RGB color) override;
    void setBreathing(::openrazer::RGB color) override;
    void setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2) override;
    void setBreathingRandom() override;
    void setBreathingMono() override;
    void setBlinking(::openrazer::RGB color) override;
    void setSpectrum() override;
    void setWave(::openrazer::WaveDirection direction) override;
    void setWheel(::openrazer::WheelDirection direction) override;
    void setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed) override;
    void setRipple(::openrazer::RGB color) override;
    void setRippleRandom() override;
    void setBrightness(uchar brightness) override;
    uchar getBrightness() override;

private:
    LedPrivate *d;
};

}

}

#endif // LED_H
