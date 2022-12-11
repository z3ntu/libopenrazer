// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RAZER_TEST_DEVICE_P_H
#define RAZER_TEST_DEVICE_P_H

#include "libopenrazer/device.h"
#include "libopenrazer/led.h"

#include <QDBusInterface>

namespace libopenrazer {

namespace razer_test {

class DevicePrivate
{
public:
    Device *mParent = nullptr;

    QDBusInterface *iface = nullptr;
    QDBusInterface *deviceIface();

    QDBusObjectPath mObjectPath;

    QStringList supportedFx;
    QStringList supportedFeatures;

    QList<::libopenrazer::Led *> leds;
    QList<QDBusObjectPath> getLedObjectPaths();

    QStringList getSupportedFx();
    QStringList getSupportedFeatures();
};

}

}

#endif // RAZER_TEST_DEVICE_P_H
