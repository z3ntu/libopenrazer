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

#include "device_p.h"
#include "libopenrazer.h"
#include "libopenrazer_private.h"

#include <QColor>
#include <QDBusReply>
#include <QDebug>
#include <QDomDocument>
#include <QVector>

namespace libopenrazer {

Device::Device(QDBusObjectPath objectPath)
{
    d = new DevicePrivate();
    d->mParent = this;
    d->mObjectPath = objectPath;

    d->introspect();
    d->setupCapabilities();

    QMap<razer_test::RazerLedId, QString>::const_iterator i = d->supportedLeds.constBegin();
    while (i != d->supportedLeds.constEnd()) {
        libopenrazer::Led *led = new libopenrazer::Led(d->mObjectPath, i.key(), i.value());
        d->leds.append(led);
        ++i;
    }
}

Device::~Device()
{
    foreach (libopenrazer::Led *led, d->leds) {
        delete led;
    }
}

void DevicePrivate::introspect()
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
void DevicePrivate::setupCapabilities()
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
bool DevicePrivate::hasCapabilityInternal(const QString &interface, const QString &method)
{
    if (method.isNull()) {
        return introspection.contains(interface);
    }
    return introspection.contains(interface + ";" + method);
}

QDBusObjectPath Device::objectPath()
{
    return d->mObjectPath;
}

bool Device::hasFx(const QString &fxStr)
{
    return d->supportedFx.contains(fxStr);
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
    return d->supportedFeatures.contains(featureStr);
}

QString Device::getPngFilename()
{
    return getRazerUrls().value("top_img").toString().split("/").takeLast();
}

QString Device::getPngUrl()
{
    return getRazerUrls().value("top_img").toString();
}

// ----- DBUS METHODS -----

QList<Led *> Device::getLeds()
{
    return d->leds;
}

QString Device::getDeviceMode()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getDeviceMode");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Device::setDeviceMode(uchar mode_id, uchar param)
{
    QDBusReply<void> reply = d->deviceMiscIface()->call("setDeviceMode");
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

QString Device::getSerial()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getSerial");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QString Device::getDeviceName()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getDeviceName");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QString Device::getDeviceType()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getDeviceType");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QString Device::getFirmwareVersion()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getFirmware");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QString Device::getKeyboardLayout()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getKeyboardLayout");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QVariantHash Device::getRazerUrls()
{
    return {}; // FIXME
}

ushort Device::getPollRate()
{
    QDBusReply<int> reply = d->deviceMiscIface()->call("getPollRate");
    if (reply.isValid()) {
        return reply.value();
    } else {
        printDBusError(reply.error(), Q_FUNC_INFO);
        throw DBusException(reply.error());
    }
}

bool Device::setPollRate(ushort pollrate)
{
    QDBusReply<void> reply = d->deviceMiscIface()->call("setPollRate", QVariant::fromValue(pollrate));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

bool Device::setDPI(razer_test::RazerDPI dpi)
{
    QDBusReply<void> reply = d->deviceDpiIface()->call("setDPI", QVariant::fromValue(dpi.dpi_x), QVariant::fromValue(dpi.dpi_y));
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

razer_test::RazerDPI Device::getDPI()
{
    QDBusReply<QList<int>> reply = d->deviceDpiIface()->call("getDPI");
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

ushort libopenrazer::Device::maxDPI()
{
    QDBusReply<int> reply = d->deviceDpiIface()->call("maxDPI");
    if (reply.isValid()) {
        return reply.value();
    } else {
        printDBusError(reply.error(), Q_FUNC_INFO);
        throw DBusException(reply.error());
    }
}

bool Device::displayCustomFrame()
{
    QDBusReply<void> reply = d->deviceLightingChromaIface()->call("setCustom");
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

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
    QDBusReply<void> reply = d->deviceLightingChromaIface()->call("setKeyRow", data);
    return handleVoidDBusReply(reply, Q_FUNC_INFO);
}

razer_test::MatrixDimensions Device::getMatrixDimensions()
{
    QDBusReply<QList<int>> reply = d->deviceMiscIface()->call("getMatrixDimensions");
    QList<int> dims = handleDBusReply(reply, Q_FUNC_INFO);
    if (dims.size() != 2)
        throw DBusException("Invalid return array from getMatrixDimensions", "The getMatrixDimensions return array has an invalid size.");
    return { static_cast<uchar>(dims[0]), static_cast<uchar>(dims[1]) };
}

QDBusInterface *DevicePrivate::deviceMiscIface()
{
    if (ifaceMisc == nullptr) {
        ifaceMisc = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.misc",
                                       RAZER_TEST_DBUS_BUS, mParent);
    }
    if (!ifaceMisc->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceMisc;
}

QDBusInterface *DevicePrivate::deviceDpiIface()
{
    if (ifaceDpi == nullptr) {
        ifaceDpi = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.dpi",
                                      RAZER_TEST_DBUS_BUS, mParent);
    }
    if (!ifaceDpi->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceDpi;
}

QDBusInterface *DevicePrivate::deviceLightingChromaIface()
{
    if (ifaceLightingChroma == nullptr) {
        ifaceLightingChroma = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.chroma",
                                                 RAZER_TEST_DBUS_BUS, mParent);
    }
    if (!ifaceLightingChroma->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceLightingChroma;
}

}
