// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RAZER_TEST_LED_P_H
#define RAZER_TEST_LED_P_H

#include "libopenrazer/led.h"

#include <QDBusInterface>

namespace libopenrazer {

namespace razer_test {

class LedPrivate
{
public:
    Led *mParent = nullptr;

    bool hasFx(const QString &fxStr);

    QDBusInterface *iface = nullptr;
    QDBusInterface *ledIface();

    Device *device;
    QDBusObjectPath mObjectPath;
};

}

}

#endif // RAZER_TEST_LED_P_H
