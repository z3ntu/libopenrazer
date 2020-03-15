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

#ifndef RAZER_TEST_MANAGER_P_H
#define RAZER_TEST_MANAGER_P_H

#include "libopenrazer/manager.h"

#include <QDBusInterface>

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
