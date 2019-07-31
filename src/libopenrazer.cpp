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

#include <QDBusArgument>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusMetaType>
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QVariantHash>
#include <QDBusReply>
#include <QtGui/qcolor.h>

#include <iostream>

#include "libopenrazer.h"

/*!
    \namespace libopenrazer
    \inmodule libopenrazer

    \brief Contains methods, enums and classes to interface with the OpenRazer daemon via the D-Bus interface.
*/
namespace libopenrazer {

void printDBusError(QDBusError error, const char *functionname)
{
    qWarning("libopenrazer: There was an error in %s", functionname);
    qWarning("libopenrazer: %s", qUtf8Printable(error.name()));
    qWarning("libopenrazer: %s", qUtf8Printable(error.message()));
}

// TODO Could generics help here?

void handleVoidReply(QDBusReply<void> reply, const char *functionname)
{
    if (reply.isValid()) {
        return;
    }
    printDBusError(reply.error(), functionname);
    throw DBusException(reply.error());
}

bool handleBoolReply(QDBusReply<bool> reply, const char *functionname)
{
    if (reply.isValid()) {
        return true;
    }
    printDBusError(reply.error(), functionname);
    throw DBusException(reply.error());
}

QString handleStringReply(QDBusReply<QString> reply, const char *functionname)
{
    if (reply.isValid()) {
        return reply.value();
    }
    printDBusError(reply.error(), functionname);
    throw DBusException(reply.error());
}

QStringList handleStringListReply(QDBusReply<QStringList> reply, const char *functionname)
{
    if (reply.isValid()) {
        return reply.value();
    }
    printDBusError(reply.error(), functionname);
    throw DBusException(reply.error());
}

QDBusInterface *Device::deviceMiscIface()
{
    if (ifaceMisc == nullptr) {
        ifaceMisc = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.misc",
                                       RAZER_TEST_DBUS_BUS, this);
    }
    if (!ifaceMisc->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceMisc;
}

QDBusInterface *Device::deviceDpiIface()
{
    if (ifaceDpi == nullptr) {
        ifaceDpi = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.dpi",
                                      RAZER_TEST_DBUS_BUS, this);
    }
    if (!ifaceDpi->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceDpi;
}

QDBusInterface *Manager::managerDaemonIface()
{
    if (ifaceDaemon == nullptr) {
        ifaceDaemon = new QDBusInterface(OPENRAZER_SERVICE_NAME, "/org/razer", "razer.daemon",
                                         RAZER_TEST_DBUS_BUS, this);
    }
    if (!ifaceDaemon->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceDaemon;
}

QDBusInterface *Manager::managerDevicesIface()
{
    if (ifaceDevices == nullptr) {
        ifaceDevices = new QDBusInterface(OPENRAZER_SERVICE_NAME, "/org/razer", "razer.devices",
                                          RAZER_TEST_DBUS_BUS, this);
    }
    if (!ifaceDevices->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceDevices;
}

QDBusInterface *Led::ledIface()
{
    if (iface == nullptr) {
        iface = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting." + someStr.toLower(),
                                   RAZER_TEST_DBUS_BUS, this);
    }
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return iface;
}

QDBusInterface *Led::ledBrightnessIface()
{
    if (ifaceBrightness == nullptr) {
        ifaceBrightness = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.brightness",
                                             RAZER_TEST_DBUS_BUS, this);
    }
    if (!ifaceBrightness->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return ifaceBrightness;
}

DBusException::DBusException(const QDBusError &error)
    : name(error.name()), message(error.message()) {}
DBusException::DBusException(const QString &name, const QString &message)
    : name(name), message(message) {}
void DBusException::raise() const
{
    throw *this;
}
DBusException *DBusException::clone() const
{
    return new DBusException(*this);
}
QString DBusException::getName()
{
    return name;
}
QString DBusException::getMessage()
{
    return message;
}

}
