/*
 * Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
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
 */

#ifndef MANAGER_H
#define MANAGER_H

#include "libopenrazer/misc.h"

#include <QDBusInterface>

namespace libopenrazer {

class Manager : public QObject
{
    Q_OBJECT
private:
    QDBusInterface *ifaceDaemon = nullptr;
    QDBusInterface *ifaceDevices = nullptr;
    QDBusInterface *managerDaemonIface();
    QDBusInterface *managerDevicesIface();

public:
    Manager();

    QList<QDBusObjectPath> getDevices();
    QString getDaemonVersion();
    bool isDaemonRunning();

    QVariantHash getSupportedDevices();

    // Sync
    bool syncEffects(bool yes);
    bool getSyncEffects();

    // Screensaver
    bool setTurnOffOnScreensaver(bool turnOffOnScreensaver);
    bool getTurnOffOnScreensaver();

    // Misc
    DaemonStatus getDaemonStatus();
    QString getDaemonStatusOutput();
    bool enableDaemon();
    // bool disableDaemon();

    // - Signal Connect Mehtods -
    bool connectDevicesChanged(QObject *receiver, const char *slot);
};

}

#endif // MANAGER_H
