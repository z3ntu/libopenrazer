/*
 * Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
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
