// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENRAZER_DEVICE_P_H
#define OPENRAZER_DEVICE_P_H

#include "libopenrazer/device.h"
#include "libopenrazer/led.h"

#include <QDBusInterface>

namespace libopenrazer {

namespace openrazer {

class DevicePrivate
{
public:
    Device *mParent = nullptr;

    QDBusInterface *ifaceMisc = nullptr;
    QDBusInterface *ifaceDpi = nullptr;
    QDBusInterface *ifaceLightingChroma = nullptr;
    QDBusInterface *deviceMiscIface();
    QDBusInterface *deviceDpiIface();
    QDBusInterface *deviceLightingChromaIface();

    QDBusObjectPath mObjectPath;

    QStringList supportedFeatures;

    QList<::libopenrazer::Led *> leds;

    void introspect();
    void setupCapabilities();
    bool hasCapabilityInternal(const QString &interface, const QString &method = QString());
    QStringList introspection;

    // Maps RazerLedId to "Chroma" or "Scroll" (the string put e.g. into setScrollSpectrum)
    QMap<::openrazer::RazerLedId, QString> supportedLeds;
};

}

}

#endif // OPENRAZER_DEVICE_P_H
