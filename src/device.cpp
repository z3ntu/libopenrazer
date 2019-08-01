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

#include <QDBusReply>
#include <QVector>
#include <QColor>
#include <QDomDocument>
#include <QDebug>

#include "libopenrazer.h"
#include "libopenrazer_private.h"

namespace libopenrazer {

/*!
 * \class libopenrazer::Device
 * \inmodule libopenrazer
 *
 * \brief The libopenrazer::Device class provides an abstraction for the OpenRazer daemon D-Bus interface for easy interaction from C++ applications.
 */

/*!
 * \fn libopenrazer::Device::Device(QString serial)
 *
 * Constructs a new device object with the given \a serial.
 */
Device::Device(QDBusObjectPath objectPath)
{
    mObjectPath = objectPath;

    introspect();
    setupCapabilities();

    QMap<razer_test::RazerLedId, QString>::const_iterator i = supportedLeds.constBegin();
    while (i != supportedLeds.constEnd()) {
        libopenrazer::Led *led = new libopenrazer::Led(mObjectPath, i.key(), i.value());
        leds.append(led);
        ++i;
    }
}

/*
 * Destructor
 */
Device::~Device()
{
    foreach (libopenrazer::Led *led, leds) {
        delete led;
    }
}

void Device::introspect()
{
    QStringList intr;

    QDBusMessage m = QDBusMessage::createMethodCall("org.razer", mObjectPath.path(), "org.freedesktop.DBus.Introspectable", "Introspect");
    QDBusReply<QString> reply = RAZER_TEST_DBUS_BUS.call(m);
    if (!reply.isValid()) {
        throw DBusException(reply.error());
    }
    QDomDocument doc;
    doc.setContent(reply.value());

    QDomNodeList nodes = doc.documentElement().childNodes();
    for (int i = 0; i < nodes.count(); i++) {
        // Check if "interface" and also not org.freedesktop.DBus.Introspectable
        QDomElement element = nodes.at(i).toElement();
        QString interfacename = element.attributeNode("name").value();

        QDomNodeList methodnodes = element.childNodes();
        for (int ii = 0; ii < methodnodes.count(); ii++) {
            QDomElement methodelement = methodnodes.at(ii).toElement();
            intr.append(interfacename + ";" + methodelement.attributeNode("name").value());
        }
        intr.append(interfacename);
    }
    introspection = intr;
}

/**
 * Fill "capabilities" list with the capabilities of the device. Names are from the pylib, parsed with the script ./scripts/capabilities_to_cpp.sh in the root of this repo.
 */
void Device::setupCapabilities()
{
    if (hasCapabilityInternal("razer.device.misc", "getKeyboardLayout"))
        supportedFeatures.append("keyboard_layout");
    if (hasCapabilityInternal("razer.device.dpi", "setDPI"))
        supportedFeatures.append("dpi");
    if (hasCapabilityInternal("razer.device.misc", "setPollRate"))
        supportedFeatures.append("poll_rate");

    if (hasCapabilityInternal("razer.device.lighting.chroma", "setNone"))
        supportedLeds.insert(razer_test::RazerLedId::Unspecified, "Chroma");
    if (hasCapabilityInternal("razer.device.lighting.logo"))
        supportedLeds.insert(razer_test::RazerLedId::LogoLED, "Logo");
    if (hasCapabilityInternal("razer.device.lighting.scroll"))
        supportedLeds.insert(razer_test::RazerLedId::ScrollWheelLED, "Scroll");
    if (hasCapabilityInternal("razer.device.lighting.backlight"))
        supportedLeds.insert(razer_test::RazerLedId::BacklightLED, "Backlight");
    if (hasCapabilityInternal("razer.device.lighting.left"))
        supportedLeds.insert(razer_test::RazerLedId::LeftSideLED, "Left");
    if (hasCapabilityInternal("razer.device.lighting.right"))
        supportedLeds.insert(razer_test::RazerLedId::RightSideLED, "Right");

    QString lightingLocation = supportedLeds.first();
    QString lightingLocationMethod;
    // Leave lightingLocationMethod empty in case it's Chroma
    if (lightingLocation != "Chroma") {
        lightingLocationMethod = lightingLocation;
    }
    QString interface;
    // Handle the "Unspecified" led special case
    if (lightingLocation == "Chroma")
        interface = "razer.device.lighting.chroma";
    else
        interface = "razer.device.lighting." + lightingLocation.toLower();

    // FIXME loc is Chroma in that case
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "None"))
        supportedFx.append("off");
    // FIXME on??? this active stuff I think
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Static"))
        supportedFx.append("static");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Blinking"))
        supportedFx.append("blinking");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathSingle"))
        supportedFx.append("breathing");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathDual"))
        supportedFx.append("breathing_dual");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathRandom"))
        supportedFx.append("breathing_random");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Spectrum"))
        supportedFx.append("spectrum");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Wave"))
        supportedFx.append("wave");
    if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Reactive"))
        supportedFx.append("reactive");

    if (hasCapabilityInternal("razer.device.lighting.chroma", "setCustom"))
        supportedFx.append("custom_frame");

    if (lightingLocation == "Chroma") {
        if (hasCapabilityInternal("razer.device.lighting.brightness", "setBrightness"))
            supportedFx.append("brightness");
    } else {
        if (hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Brightness"))
            supportedFx.append("brightness");
    }
}

/**
 * Internal method to determine whether a device has a given capability based on interface and method names.
 */
bool Device::hasCapabilityInternal(const QString &interface, const QString &method)
{
    if (method.isNull()) {
        return introspection.contains(interface);
    }
    return introspection.contains(interface + ";" + method);
}

/*!
 * \fn QString libopenrazer::Device::serial()
 *
 * Returns the device serial.
 */
QDBusObjectPath Device::objectPath()
{
    return mObjectPath;
}

bool Device::hasFx(const QString &fxStr)
{
    return supportedFx.contains(fxStr);
}

bool Device::hasFx(razer_test::RazerEffect fx)
{
    QString fxStr;
    switch (fx) {
    case razer_test::RazerEffect::Off:
        fxStr = "off";
        break;
    case razer_test::RazerEffect::On:
        fxStr = "on";
        break;
    case razer_test::RazerEffect::Static:
        fxStr = "static";
        break;
    case razer_test::RazerEffect::Breathing:
        fxStr = "breathing";
        break;
    case razer_test::RazerEffect::BreathingDual:
        fxStr = "breathing_dual";
        break;
    case razer_test::RazerEffect::BreathingRandom:
        fxStr = "breathing_random";
        break;
    case razer_test::RazerEffect::Blinking:
        fxStr = "blinking";
        break;
    case razer_test::RazerEffect::Spectrum:
        fxStr = "spectrum";
        break;
    case razer_test::RazerEffect::Wave:
        fxStr = "wave";
        break;
    case razer_test::RazerEffect::Reactive:
        fxStr = "reactive";
        break;
    }
    return hasFx(fxStr);
}

bool Device::hasFeature(const QString &featureStr)
{
    return supportedFeatures.contains(featureStr);
}

/*!
 * \fn QString libopenrazer::Device::getPngFilename()
 *
 * Returns the filename for the \c top_img picture (e.g. \c razer-naga-hex-gallery-12.png).
 *
 * Could return an empty string (qstring.isEmpty()) if no picture was found.
 *
 * \sa getRazerUrls(), getPngUrl()
 */
QString Device::getPngFilename()
{
    return getRazerUrls().value("top_img").toString().split("/").takeLast();
}

/*!
 * \fn QString libopenrazer::Device::getPngUrl()
 *
 * Returns the URL for the \c top_img picture.
 *
 * Could return an empty string (\c qstring.isEmpty()) if no picture was found.
 *
 * \sa getRazerUrls(), getPngFilename()
 */
QString Device::getPngUrl()
{
    return getRazerUrls().value("top_img").toString();
}

// ----- DBUS METHODS -----

QList<Led *> Device::getLeds()
{
    return leds;
}

/*!
 * \fn QString libopenrazer::Device::getDeviceMode()
 *
 * Returns the device mode of the device, like '0:0' or '3:0' for normal mode and driver mode respetively.
 *
 * \sa setDeviceMode()
 */
QString Device::getDeviceMode()
{
    return "error";
    // QDBusReply<QString> reply = deviceMiscIface()->call("getDeviceMode");
    // return handleDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Device::setDeviceMode(uchar mode_id, uchar param)
 *
 * Sets the device mode to the specified \a mode_id and \a param.
 * Allowed values are 0x00 (normal mode) and 0x03 (driver mode) for \a mode_id and 0x00 for param.
 *
 * When the device is set to Driver Mode, tilting the mouse wheel stops working, and the 'Up DPI' and 'Down DPI' buttons don't actually do anything. Razer Synapse on Windows is supposed to change the DPI instead of the mouse.
 *
 * Returns if the D-Bus call was successful.
 *
 * \sa getDeviceMode()
 */
bool Device::setDeviceMode(uchar mode_id, uchar param)
{
    return false; // FIXME
}

QString Device::getSerial()
{
    QDBusReply<QString> reply = deviceMiscIface()->call("getSerial");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn QString libopenrazer::Device::getDeviceName()
 *
 * Returns a human readable device name like \c {"Razer DeathAdder Chroma"} or \c {"Razer Kraken 7.1"}.
 */
QString Device::getDeviceName()
{
    QDBusReply<QString> reply = deviceMiscIface()->call("getDeviceName");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn QString libopenrazer::Device::getDeviceType()
 *
 * Returns the type of the device. Could be one of \c 'keyboard', \c 'mouse', \c 'mousemat', \c 'core', \c 'keypad', \c 'headset', \c 'mug' or another type, if added to the daemon.
 */
QString Device::getDeviceType()
{
    QDBusReply<QString> reply = deviceMiscIface()->call("getDeviceType");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn QString libopenrazer::Device::getFirmwareVersion()
 *
 * Returns the firmware version of the device (e.g. \c 'v1.0').
 */
QString Device::getFirmwareVersion()
{
    QDBusReply<QString> reply = deviceMiscIface()->call("getFirmware");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn QString libopenrazer::Device::getKeyboardLayout()
 *
 * Returns the physical layout of the keyboard (e.g. \c 'de_DE', \c 'en_US', \c 'en_GB' or \c 'unknown')
 */
QString Device::getKeyboardLayout()
{
    QDBusReply<QString> reply = deviceMiscIface()->call("getKeyboardLayout");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn QVariantHash libopenrazer::Device::getRazerUrls()
 *
 * Returns a QVariantHash (\c {QHash<QString, QVariant>}).
 * Most likely contains keys \c "top_img", \c "side_img", \c "store" and \c "perspective_img".
 * Values are \c QVariant<QString> with a full URL as value.
 */
QVariantHash Device::getRazerUrls()
{
    return {}; // FIXME
}

/*!
 * \fn int libopenrazer::Device::getPollRate()
 *
 * Returns the current poll rate.
 */
ushort Device::getPollRate()
{
    QDBusReply<int> reply = deviceMiscIface()->call("getPollRate");
    if (reply.isValid()) {
        return reply.value();
    } else {
        printDBusError(reply.error(), Q_FUNC_INFO);
        throw DBusException(reply.error());
    }
}

/*!
 * \fn bool libopenrazer::Device::setPollRate(ushort pollrate)
 *
 * Sets the poll rate of the mouse to the specified \a pollrate.
 *
 * Returns if the D-Bus call was successful.
 */
bool Device::setPollRate(ushort pollrate)
{
    QDBusReply<void> reply = deviceMiscIface()->call("setPollRate", QVariant::fromValue(pollrate));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Device::setDPI(int dpi_x, int dpi_y)
 *
 * Sets the DPI of the mouse to the specified \a dpi_x for the x-Axis and \a dpi_y for the y-Axis. Maximum value is what is returned by maxDPI().
 *
 * Returns if the D-Bus call was successful.
 */
bool Device::setDPI(razer_test::RazerDPI dpi)
{
    QDBusReply<void> reply = deviceDpiIface()->call("setDPI", QVariant::fromValue(dpi.dpi_x), QVariant::fromValue(dpi.dpi_y));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn QList<int> libopenrazer::Device::getDPI()
 *
 * Returns the DPI of the mouse (e.g. \c [800, 800]).
 */
razer_test::RazerDPI Device::getDPI()
{
    QDBusReply<QList<int>> reply = deviceDpiIface()->call("getDPI");
    if (reply.isValid()) {
        QList<int> dpi = reply.value();
        if (dpi.size() == 1) {
            return { static_cast<ushort>(dpi[0]), 0 };
        } else if (dpi.size() == 2) {
            return { static_cast<ushort>(dpi[0]), static_cast<ushort>(dpi[1]) };
        } else {
            throw DBusException("Invalid return array from DPI", "The DPI return array has an invalid size.");
        }
    } else {
        printDBusError(reply.error(), Q_FUNC_INFO);
        throw DBusException(reply.error());
    }
}

/*!
 * \fn int libopenrazer::Device::maxDPI()
 *
 * Returns the maximum DPI possible for the device.
 */
ushort libopenrazer::Device::maxDPI()
{
    QDBusReply<int> reply = deviceDpiIface()->call("maxDPI");
    if (reply.isValid()) {
        return reply.value();
    } else {
        printDBusError(reply.error(), Q_FUNC_INFO);
        throw DBusException(reply.error());
    }
}

/*!
 * \fn bool libopenrazer::Device::setCustom()
 *
 * Sets the lighting to custom mode (applies effects set from setKeyRow()).
 *
 * Returns if the D-Bus call was successful.
 *
 * \sa setKeyRow()
 */
bool Device::displayCustomFrame()
{
    QDBusReply<void> reply = deviceLightingChromaIface()->call("setCustom");
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

/*!
 * \fn bool libopenrazer::Device::setKeyRow(uchar row, uchar startcol, uchar endcol, QVector<QColor> colors)
 *
 * Sets the lighting of a key row to the specified \a colors.
 * \a row is the row in the matrix, \a startcol the column the \a colors list starts and \a endcol where the list ends.
 * Note, that you have to call setCustom() after setting otherwise the effect won't be displayed (even if you have already called setCustom() before).
 * Currently the driver only accepts whole rows that are sent.
 *
 * Returns if the D-Bus call was successful.
 *
 * \sa setCustom()
 */
bool Device::defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<QColor> colorData)
{
    QByteArray data;
    data.append(row);
    data.append(startColumn);
    data.append(endColumn);
    foreach (const QColor &color, colorData) {
        data.append(color.red());
        data.append(color.green());
        data.append(color.blue());
    }
    QDBusReply<void> reply = deviceLightingChromaIface()->call("setKeyRow", data);
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

razer_test::MatrixDimensions Device::getMatrixDimensions()
{
    QDBusReply<QList<int>> reply = deviceMiscIface()->call("getMatrixDimensions");
    QList<int> dims = handleDBusReply(reply, Q_FUNC_INFO);
    if (dims.size() != 2)
        throw DBusException("Invalid return array from getMatrixDimensions", "The getMatrixDimensions return array has an invalid size.");
    return { static_cast<uchar>(dims[0]), static_cast<uchar>(dims[1]) };
}

}
