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
 *
 */

#ifndef LIBOPENRAZER_PRIVATE_H
#define LIBOPENRAZER_PRIVATE_H

#include <QDBusReply>

namespace libopenrazer {

void printDBusError(QDBusError error, const char *functionname);
void handleVoidDBusReply(QDBusReply<bool> reply, const char *functionname);
QString fromCamelCase(const QString &s);

template<typename T>
T handleDBusReply(QDBusReply<T> reply, const char *functionname)
{
    if (reply.isValid()) {
        return reply.value();
    }
    printDBusError(reply.error(), functionname);
    throw DBusException(reply.error());
}

// Specialization for QDBusReply<void>
template<>
void handleDBusReply(QDBusReply<void> reply, const char *functionname);

template<typename T>
T handleDBusVariant(QVariant variant, QDBusError error, const char *functionname)
{
    if (variant.isValid()) {
        return variant.value<T>();
    }
    printDBusError(error, functionname);
    throw DBusException(error);
}

namespace openrazer {
extern const char *OPENRAZER_SERVICE_NAME;
extern QDBusConnection OPENRAZER_DBUS_BUS;
}
namespace razer_test {
extern const char *OPENRAZER_SERVICE_NAME;
extern QDBusConnection OPENRAZER_DBUS_BUS;
}

}

#endif // LIBOPENRAZER_PRIVATE_H
