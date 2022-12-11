// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENRAZER_LED_P_H
#define OPENRAZER_LED_P_H

#include "libopenrazer/led.h"

#include <QDBusInterface>

namespace libopenrazer {

namespace openrazer {

class LedPrivate
{
public:
    Led *mParent = nullptr;

    QDBusInterface *iface = nullptr;
    QDBusInterface *ifaceBrightness = nullptr;
    QDBusInterface *ifaceBw2013 = nullptr;
    QDBusInterface *ifaceCustom = nullptr;
    QDBusInterface *ledIface();
    QDBusInterface *ledBrightnessIface();
    QDBusInterface *ledBw2013Iface();
    QDBusInterface *ledCustomIface();

    Device *device;
    QDBusObjectPath mObjectPath;

    QVector<::openrazer::RazerEffect> supportedFx;
    bool supportsBrightness;

    ::openrazer::RazerLedId ledId;
    QString lightingLocation;
    QString lightingLocationMethod;
    QString interface;

    bool hasFx();
    bool isProfileLed();
    void setupCapabilities();
};

}

}

#endif // OPENRAZER_LED_P_H
