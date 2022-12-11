// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RAZER_TEST_MANAGER_P_H
#define RAZER_TEST_MANAGER_P_H

#include "libopenrazer/manager.h"

#include <QDBusInterface>

#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
#define RAZER_TEST_DBUS_BUS QDBusConnection::systemBus()
#elif defined(Q_OS_DARWIN) || defined(Q_OS_WIN)
#define RAZER_TEST_DBUS_BUS QDBusConnection::sessionBus()
#else
#error "Please choose a RAZER_TEST_DBUS_BUS for this platform!"
#endif

namespace libopenrazer {

namespace razer_test {

const char *OPENRAZER_SERVICE_NAME = "io.github.openrazer1";
QDBusConnection OPENRAZER_DBUS_BUS = RAZER_TEST_DBUS_BUS;

class ManagerPrivate
{
public:
    Manager *mParent = nullptr;

    QDBusInterface *iface = nullptr;
    QDBusInterface *managerIface();
};

}

}

#endif // RAZER_TEST_MANAGER_P_H
