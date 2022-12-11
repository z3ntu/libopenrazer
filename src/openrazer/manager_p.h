// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENRAZER_MANAGER_P_H
#define OPENRAZER_MANAGER_P_H

#include "libopenrazer/manager.h"

#include <QDBusInterface>

namespace libopenrazer {

namespace openrazer {

const char *OPENRAZER_SERVICE_NAME = "org.razer";
QDBusConnection OPENRAZER_DBUS_BUS = QDBusConnection::sessionBus();

class ManagerPrivate
{
public:
    Manager *mParent = nullptr;

    QDBusInterface *ifaceDaemon = nullptr;
    QDBusInterface *ifaceDevices = nullptr;
    QDBusInterface *managerDaemonIface();
    QDBusInterface *managerDevicesIface();
};

}

}

#endif // OPENRAZER_MANAGER_P_H
