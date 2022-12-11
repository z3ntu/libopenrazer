// Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer.h"
#include "libopenrazer_private.h"

#include <QDBusReply>
#include <QRegularExpression>

namespace libopenrazer {

void printDBusError(QDBusError error, const char *functionname)
{
    qWarning("libopenrazer: There was an error in %s", functionname);
    qWarning("libopenrazer: %s", qUtf8Printable(error.name()));
    qWarning("libopenrazer: %s", qUtf8Printable(error.message()));
}

// Specialization for QDBusReply<void>
template<>
void handleDBusReply(QDBusReply<void> reply, const char *functionname)
{
    if (reply.isValid()) {
        return;
    }
    printDBusError(reply.error(), functionname);
    throw DBusException(reply.error());
}

void handleVoidDBusReply(QDBusReply<bool> reply, const char *functionname)
{
    if (!reply.isValid()) {
        printDBusError(reply.error(), functionname);
        throw DBusException(reply.error());
    }
    if (!reply.value()) {
        qWarning("libopenrazer: %s: The function has returned false", functionname);
        throw DBusException("Call failed", QString(functionname) + " has returned false");
    }
}

// Convert CamelCase string to snake_case
QString fromCamelCase(const QString &s)
{
    static QRegularExpression regExp1 { "(.)([A-Z][a-z]+)" };
    static QRegularExpression regExp2 { "([a-z0-9])([A-Z])" };
    QString result = s;
    result.replace(regExp1, "\\1_\\2");
    result.replace(regExp2, "\\1_\\2");
    return result.toLower();
}

}
