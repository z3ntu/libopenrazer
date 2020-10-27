// Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "device_p.h"
#include "libopenrazer.h"
#include "libopenrazer_private.h"

#include <QDBusReply>
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>

namespace libopenrazer {

namespace openrazer {

Device::Device(QDBusObjectPath objectPath)
{
    d = new DevicePrivate();
    d->mParent = this;
    d->mObjectPath = objectPath;

    d->introspect();
    d->setupCapabilities();

    QMap<::openrazer::RazerLedId, QString>::const_iterator i = d->supportedLeds.constBegin();
    while (i != d->supportedLeds.constEnd()) {
        Led *led = new Led(this, d->mObjectPath, i.key(), i.value());
        d->leds.append(led);
        ++i;
    }
}

Device::~Device()
{
    for (libopenrazer::Led *led : d->leds) {
        delete led;
    }
}

void DevicePrivate::introspect()
{
    QStringList intr;

    QDBusMessage m = QDBusMessage::createMethodCall(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "org.freedesktop.DBus.Introspectable", "Introspect");
    QDBusReply<QString> reply = OPENRAZER_DBUS_BUS.call(m);
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
    if (hasCapabilityInternal("razer.device.dpi", "availableDPI"))
        supportedFeatures.append("restricted_dpi");
    if (hasCapabilityInternal("razer.device.dpi", "setDPIStages"))
        supportedFeatures.append("dpi_stages");
    if (hasCapabilityInternal("razer.device.misc", "setPollRate"))
        supportedFeatures.append("poll_rate");
    if (hasCapabilityInternal("razer.device.lighting.chroma", "setCustom"))
        supportedFeatures.append("custom_frame");

    // razer.device.lighting.chroma more than only the normal fx, so check for methods directly
    if (hasCapabilityInternal("razer.device.lighting.chroma", "setNone")
        || hasCapabilityInternal("razer.device.lighting.chroma", "setStatic")
        || hasCapabilityInternal("razer.device.lighting.bw2013")
        || hasCapabilityInternal("razer.device.lighting.brightness"))
        supportedLeds.insert(::openrazer::RazerLedId::Unspecified, "Chroma");
    if (hasCapabilityInternal("razer.device.lighting.logo"))
        supportedLeds.insert(::openrazer::RazerLedId::LogoLED, "Logo");
    if (hasCapabilityInternal("razer.device.lighting.scroll"))
        supportedLeds.insert(::openrazer::RazerLedId::ScrollWheelLED, "Scroll");
    if (hasCapabilityInternal("razer.device.lighting.backlight"))
        supportedLeds.insert(::openrazer::RazerLedId::BacklightLED, "Backlight");
    if (hasCapabilityInternal("razer.device.lighting.left"))
        supportedLeds.insert(::openrazer::RazerLedId::LeftSideLED, "Left");
    if (hasCapabilityInternal("razer.device.lighting.right"))
        supportedLeds.insert(::openrazer::RazerLedId::RightSideLED, "Right");
    if (hasCapabilityInternal("razer.device.lighting.profile_led", "setRedLED"))
        supportedLeds.insert(::openrazer::RazerLedId::KeymapRedLED, "RedLED");
    if (hasCapabilityInternal("razer.device.lighting.profile_led", "setGreenLED"))
        supportedLeds.insert(::openrazer::RazerLedId::KeymapGreenLED, "GreenLED");
    if (hasCapabilityInternal("razer.device.lighting.profile_led", "setBlueLED"))
        supportedLeds.insert(::openrazer::RazerLedId::KeymapBlueLED, "BlueLED");
    if (hasCapabilityInternal("razer.device.lighting.charging"))
        supportedLeds.insert(::openrazer::RazerLedId::ChargingLED, "Charging");
    if (hasCapabilityInternal("razer.device.lighting.fast_charging"))
        supportedLeds.insert(::openrazer::RazerLedId::FastChargingLED, "FastCharging");
    if (hasCapabilityInternal("razer.device.lighting.fully_charged"))
        supportedLeds.insert(::openrazer::RazerLedId::FullyChargedLED, "FullyCharged");
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

bool Device::hasFeature(const QString &featureStr)
{
    return d->supportedFeatures.contains(featureStr);
}

QString Device::getDeviceImageUrl()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getRazerUrls");
    QString json = handleDBusReply(reply, Q_FUNC_INFO);
    return QJsonDocument::fromJson(json.toUtf8()).object().value("top_img").toString();
}

// ----- DBUS METHODS -----

QList<::libopenrazer::Led *> Device::getLeds()
{
    return d->leds;
}

QString Device::getDeviceMode()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getDeviceMode");
    return handleDBusReply(reply, Q_FUNC_INFO);
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
    QString type = handleDBusReply(reply, Q_FUNC_INFO);
    const QHash<QString, QString> translationTable = {
        { "core", "accessory" },
        { "mousemat", "mousepad" },
        { "mug", "accessory" },
    };
    if (translationTable.contains(type))
        return translationTable.value(type);
    return type;
}

QString Device::getFirmwareVersion()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getFirmware");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QString Device::getKeyboardLayout()
{
    QDBusReply<QString> reply = d->deviceMiscIface()->call("getKeyboardLayout");
    QString layout = handleDBusReply(reply, Q_FUNC_INFO);
    const QHash<QString, QString> translationTable = {
        { "de_DE", "German" },
        { "el_GR", "Greek" },
        { "en_GB", "UK" },
        { "en_US", "US" },
        { "en_US_mac", "US-mac" },
        { "es_ES", "Spanish" },
        { "fr_FR", "French" },
        { "it_IT", "Italian" },
        { "ja_JP", "Japanese" },
        { "pt_PT", "Portuguese" },
    };
    if (translationTable.contains(layout))
        return translationTable.value(layout);
    return layout;
}

ushort Device::getPollRate()
{
    QDBusReply<int> reply = d->deviceMiscIface()->call("getPollRate");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

void Device::setPollRate(ushort pollrate)
{
    QDBusReply<void> reply = d->deviceMiscIface()->call("setPollRate", QVariant::fromValue(pollrate));
    handleDBusReply(reply, Q_FUNC_INFO);
}

QVector<ushort> Device::getSupportedPollRates()
{
    // Not every device has getSupportedPollRates yet, return defaults in that case.
    if (!d->hasCapabilityInternal("razer.device.misc", "getSupportedPollRates")) {
        return { 125, 500, 1000 };
    }
    QDBusReply<QVector<ushort>> reply = d->deviceMiscIface()->call("getSupportedPollRates");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

void Device::setDPI(::openrazer::RazerDPI dpi)
{
    QDBusReply<void> reply = d->deviceDpiIface()->call("setDPI", QVariant::fromValue(dpi.dpi_x), QVariant::fromValue(dpi.dpi_y));
    handleDBusReply(reply, Q_FUNC_INFO);
}

::openrazer::RazerDPI Device::getDPI()
{
    QDBusReply<QList<int>> reply = d->deviceDpiIface()->call("getDPI");
    QList<int> dpi = handleDBusReply(reply, Q_FUNC_INFO);
    if (dpi.size() == 1) {
        return { static_cast<ushort>(dpi[0]), 0 };
    } else if (dpi.size() == 2) {
        return { static_cast<ushort>(dpi[0]), static_cast<ushort>(dpi[1]) };
    } else {
        throw DBusException("Invalid return array from DPI", "The DPI return array has an invalid size.");
    }
}

void Device::setDPIStages(uchar activeStage, QVector<::openrazer::RazerDPI> dpiStages)
{
    QDBusReply<void> reply = d->deviceDpiIface()->call("setDPIStages", QVariant::fromValue(activeStage), QVariant::fromValue(dpiStages));
    handleDBusReply(reply, Q_FUNC_INFO);
}

QPair<uchar, QVector<::openrazer::RazerDPI>> Device::getDPIStages()
{
    QDBusReply<QPair<uchar, QVector<::openrazer::RazerDPI>>> reply = d->deviceDpiIface()->call("getDPIStages");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

ushort Device::maxDPI()
{
    QDBusReply<int> reply = d->deviceDpiIface()->call("maxDPI");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QVector<ushort> Device::getAllowedDPI()
{
    QDBusReply<QVector<int>> reply = d->deviceDpiIface()->call("availableDPI");
    QVector<int> values = handleDBusReply(reply, Q_FUNC_INFO);
    if (values.isEmpty())
        throw DBusException("Invalid return array from availableDPI", "The availableDPI return array is empty.");
    // Convert QVector<int> to QVector<ushort>
    QVector<ushort> out;
    out.reserve(values.size());
    std::transform(values.cbegin(), values.cend(), std::back_inserter(out),
                   [](int c) { return static_cast<ushort>(c); });
    return out;
}

void Device::displayCustomFrame()
{
    QDBusReply<void> reply = d->deviceLightingChromaIface()->call("setCustom");
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Device::defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<::openrazer::RGB> colorData)
{
    QByteArray data;
    data.append(row);
    data.append(startColumn);
    data.append(endColumn);
    for (const ::openrazer::RGB &color : colorData) {
        data.append(color.r);
        data.append(color.g);
        data.append(color.b);
    }
    QDBusReply<void> reply = d->deviceLightingChromaIface()->call("setKeyRow", data);
    handleDBusReply(reply, Q_FUNC_INFO);
}

::openrazer::MatrixDimensions Device::getMatrixDimensions()
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
                                       OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceMisc->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceMisc;
}

QDBusInterface *DevicePrivate::deviceDpiIface()
{
    if (ifaceDpi == nullptr) {
        ifaceDpi = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.dpi",
                                      OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceDpi->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceDpi;
}

QDBusInterface *DevicePrivate::deviceLightingChromaIface()
{
    if (ifaceLightingChroma == nullptr) {
        ifaceLightingChroma = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.chroma",
                                                 OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceLightingChroma->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceLightingChroma;
}

}

}
