// Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer.h"
#include "libopenrazer_private.h"
#include "manager_p.h"

#include <QDBusMetaType>
#include <QDBusReply>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

namespace libopenrazer {

namespace openrazer {

Manager::Manager()
{
    d = new ManagerPrivate();
    d->mParent = this;

    // Register the enums with the Qt system
    ::openrazer::registerMetaTypes();
}

bool Manager::isDaemonRunning()
{
    QDBusReply<QString> reply = d->managerDaemonIface()->call("version");
    return reply.isValid();
}

QVariantHash Manager::getSupportedDevices()
{
    QDBusReply<QString> reply = d->managerDevicesIface()->call("supportedDevices");
    QString content = handleDBusReply(reply, Q_FUNC_INFO);
    return QJsonDocument::fromJson(content.toUtf8()).object().toVariantHash();
}

QList<QDBusObjectPath> Manager::getDevices()
{
    QDBusReply<QStringList> reply = d->managerDevicesIface()->call("getDevices");
    QStringList serialList = handleDBusReply(reply, Q_FUNC_INFO);
    QList<QDBusObjectPath> ret;
    for (const QString &serial : serialList) {
        ret.append(QDBusObjectPath("/org/razer/device/" + serial));
    }
    return ret;
}

Device *Manager::getDevice(QDBusObjectPath objectPath)
{
    return new Device(objectPath);
}

void Manager::syncEffects(bool yes)
{
    QDBusReply<void> reply = d->managerDevicesIface()->call("syncEffects", QVariant::fromValue(yes));
    handleDBusReply(reply, Q_FUNC_INFO);
}

bool Manager::getSyncEffects()
{
    QDBusReply<bool> reply = d->managerDevicesIface()->call("getSyncEffects");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

QString Manager::getDaemonVersion()
{
    QDBusReply<QString> reply = d->managerDaemonIface()->call("version");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

void Manager::setTurnOffOnScreensaver(bool turnOffOnScreensaver)
{
    QDBusReply<void> reply = d->managerDevicesIface()->call("enableTurnOffOnScreensaver", QVariant::fromValue(turnOffOnScreensaver));
    handleDBusReply(reply, Q_FUNC_INFO);
}

bool Manager::getTurnOffOnScreensaver()
{
    QDBusReply<bool> reply = d->managerDevicesIface()->call("getOffOnScreensaver");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

DaemonStatus Manager::getDaemonStatus()
{
    // Scenarios to handle:
    // - Command systemctl doesn't exist (e.g. Alpine or Gentoo) - exit code 255
    // - Unit wasn't found (i.e. daemon is not installed - or only an old version) - exit code 1
    // Daemon can be not installed but enabled -.-
    QProcess process;
    process.start("systemctl", QStringList() << "--user"
                                             << "is-enabled"
                                             << "openrazer-daemon.service");
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
    QString error(process.readAllStandardError());
    if (output == "enabled\n")
        return DaemonStatus::Enabled;
    else if (output == "disabled\n")
        return DaemonStatus::Disabled;
    else if (error == "Failed to get unit file state for openrazer-daemon.service: No such file or directory\n")
        return DaemonStatus::NotInstalled;
    else if (process.error() == QProcess::FailedToStart) { // check if systemctl could be started - fails on non-systemd distros and flatpak
        QFileInfo daemonFile("/usr/bin/openrazer-daemon");
        // if the daemon executable does not exist, show the not_installed message - probably flatpak
        if (!daemonFile.exists())
            return DaemonStatus::NotInstalled;
        // otherwise show the NoSystemd message - probably a non-systemd distro
        return DaemonStatus::NoSystemd;
    } else {
        qWarning("libopenrazer: There was an error checking if the daemon is enabled. Unit state is: %s. Error message: %s", qUtf8Printable(output), qUtf8Printable(error));
        return DaemonStatus::Unknown;
    }
}

QString Manager::getDaemonStatusOutput()
{
    QProcess process;
    process.start("systemctl", QStringList() << "--user"
                                             << "status"
                                             << "openrazer-daemon.service");
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
    QString error(process.readAllStandardError());
    // TODO Handle systemctl not found
    // TODO Check if output and error and only display what's filled (to get rid of stray newline)
    return output + "\n" + error;
}

bool Manager::enableDaemon()
{
    QProcess process;
    process.start("systemctl", QStringList() << "--user"
                                             << "enable"
                                             << "openrazer-daemon.service");
    process.waitForFinished();
    return process.exitCode() == 0;
}

// TODO New Qt5 connect style syntax - maybe https://stackoverflow.com/a/35501065/3527128
bool Manager::connectDevicesChanged(QObject *receiver, const char *slot)
{
    bool ret = OPENRAZER_DBUS_BUS.connect(OPENRAZER_SERVICE_NAME, "/org/razer", "razer.devices", "device_added", receiver, slot);
    ret &= OPENRAZER_DBUS_BUS.connect(OPENRAZER_SERVICE_NAME, "/org/razer", "razer.devices", "device_removed", receiver, slot);
    return ret;
}

QDBusServiceWatcher *Manager::getServiceWatcher()
{
    return new QDBusServiceWatcher(OPENRAZER_SERVICE_NAME, OPENRAZER_DBUS_BUS);
}

QDBusInterface *ManagerPrivate::managerDaemonIface()
{
    if (ifaceDaemon == nullptr) {
        ifaceDaemon = new QDBusInterface(OPENRAZER_SERVICE_NAME, "/org/razer", "razer.daemon",
                                         OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceDaemon->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceDaemon;
}

QDBusInterface *ManagerPrivate::managerDevicesIface()
{
    if (ifaceDevices == nullptr) {
        ifaceDevices = new QDBusInterface(OPENRAZER_SERVICE_NAME, "/org/razer", "razer.devices",
                                          OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceDevices->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceDevices;
}

}

}
