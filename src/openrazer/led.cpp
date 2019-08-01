/*
 * Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
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
 *
 */

#include "libopenrazer.h"
#include "libopenrazer_private.h"

#include <QColor>
#include <QDBusReply>

#define QCOLOR_TO_QVARIANT(c)               \
    QVariant::fromValue(c.red()),           \
            QVariant::fromValue(c.green()), \
            QVariant::fromValue(c.blue())

namespace libopenrazer {

Led::Led(QDBusObjectPath objectPath, razer_test::RazerLedId ledId, QString lightingLocation)
    : mObjectPath(objectPath), ledId(ledId), lightingLocation(lightingLocation)
{
    // Leave lightingLocationMethod empty in case it's Chroma
    if (lightingLocation != "Chroma") {
        lightingLocationMethod = lightingLocation;
    }
}

/*
 * Destructor
 */
Led::~Led() = default;

QDBusObjectPath Led::getObjectPath()
{
    return mObjectPath;
}

razer_test::RazerEffect Led::getCurrentEffect()
{
    // FIXME
    return razer_test::RazerEffect::Spectrum;
}

QList<razer_test::RGB> Led::getCurrentColors()
{
    // FIXME
    return { { 0, 255, 0 }, { 255, 0, 0 }, { 0, 0, 255 } };
}

razer_test::RazerLedId Led::getLedId()
{
    return ledId;
}

/*!
 * \fn bool libopenrazer::Led::setOff()
 *
 * Sets the LED to none / off.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setOff()
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "None");
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setStatic(QColor color)
 *
 * Sets the lighting to static lighting in the specified \a color.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setStatic(QColor color)
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "Static", QCOLOR_TO_QVARIANT(color));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setBreathSingle(QColor color)
 *
 * Sets the lighting to the single breath effect with the specified \a color.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setBreathing(QColor color)
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "BreathSingle", QCOLOR_TO_QVARIANT(color));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setBreathDual(QColor color, QColor color2)
 *
 * Sets the lighting to the dual breath effect with the specified \a color and \a color2.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setBreathingDual(QColor color, QColor color2)
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "BreathDual", QCOLOR_TO_QVARIANT(color), QCOLOR_TO_QVARIANT(color2));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setBreathRandom()
 *
 * Sets the lighting wheel to the random breath effect.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setBreathingRandom()
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "BreathRandom");
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setBreathRandom()
 *
 * Sets the lighting wheel to the random breath effect.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setBlinking(QColor color)
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "Blinking", QCOLOR_TO_QVARIANT(color));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setSpectrum()
 *
 * Sets the lighting to spectrum mode.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setSpectrum()
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "Spectrum");
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setWave(WaveDirection direction)
 *
 * Sets the lighting effect to wave, in the direction \a direction.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setWave(razer_test::WaveDirection direction)
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "Wave", QVariant::fromValue(direction));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setReactive(QColor color, ReactiveSpeed speed)
 *
 * Sets the lighting to reactive mode with the specified \a color and \a speed.
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setReactive(QColor color, razer_test::ReactiveSpeed speed)
{
    QDBusReply<void> reply = ledIface()->call("set" + lightingLocationMethod + "Reactive", static_cast<uchar>(speed), QCOLOR_TO_QVARIANT(color));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Led::setBrightness(double brightness)
 *
 * Sets the \a brightness (0-100).
 *
 * Returns if the D-Bus call was successful.
 */
bool Led::setBrightness(uchar brightness)
{
    double dbusBrightness = brightness / 255 * 100;
    QDBusReply<void> reply;
    if (lightingLocation == "Chroma")
        reply = ledBrightnessIface()->call("setBrightness", QVariant::fromValue(dbusBrightness));
    else
        reply = ledIface()->call("set" + lightingLocationMethod + "Brightness", QVariant::fromValue(dbusBrightness));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn double libopenrazer::Led::getBrightness()
 *
 * Returns the current brightness (0-100).
 */
uchar Led::getBrightness()
{
    QDBusReply<double> reply;
    if (lightingLocation == "Chroma")
        reply = ledBrightnessIface()->call("getBrightness");
    else
        reply = ledIface()->call("get" + lightingLocationMethod + "Brightness");
    if (reply.isValid()) {
        return reply.value() / 100 * 255;
    } else {
        printDBusError(reply.error(), Q_FUNC_INFO);
        throw DBusException(reply.error());
    }
}

QDBusInterface *Led::ledIface()
{
    if (iface == nullptr) {
        iface = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting." + lightingLocation.toLower(),
                                   RAZER_TEST_DBUS_BUS, this);
    }
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return iface;
}

QDBusInterface *Led::ledBrightnessIface()
{
    if (ifaceBrightness == nullptr) {
        ifaceBrightness = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.brightness",
                                             RAZER_TEST_DBUS_BUS, this);
    }
    if (!ifaceBrightness->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceBrightness;
}

}
