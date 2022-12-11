// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
