// Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer.h"
#include "libopenrazer_private.h"
#include "manager_p.h"

#include <QDBusMetaType>
#include <QFileInfo>
#include <QProcess>

namespace libopenrazer {

namespace razer_test {

Manager::Manager()
{
    d = new ManagerPrivate();
    d->mParent = this;

    // Register the enums with the Qt system
    ::openrazer::registerMetaTypes();
}

bool Manager::isDaemonRunning()
{
    QVariant reply = d->managerIface()->property("Version");
    return reply.isValid();
}

QVariantHash Manager::getSupportedDevices()
{
    return QVariantHash(); // TODO Needs implementation
}

QList<QDBusObjectPath> Manager::getDevices()
{
    QVariant reply = d->managerIface()->property("Devices");
    return handleDBusVariant<QList<QDBusObjectPath>>(reply, d->managerIface()->lastError(), Q_FUNC_INFO);
}

Device *Manager::getDevice(QDBusObjectPath objectPath)
{
    return new Device(objectPath);
}

void Manager::syncEffects(bool yes)
{
    // TODO Needs implementation
}

bool Manager::getSyncEffects()
{
    return false; // TODO Needs implementation
}

QString Manager::getDaemonVersion()
{
    QVariant reply = d->managerIface()->property("Version");
    return handleDBusVariant<QString>(reply, d->managerIface()->lastError(), Q_FUNC_INFO);
}

void Manager::setTurnOffOnScreensaver(bool turnOffOnScreensaver)
{
    // TODO Needs implementation
}

bool Manager::getTurnOffOnScreensaver()
{
    return false; // TODO Needs implementation
}

DaemonStatus Manager::getDaemonStatus()
{
    // Scenarios to handle:
    // - Command systemctl doesn't exist (e.g. Alpine or Gentoo) - exit code 255
    // - Unit wasn't found (i.e. daemon is not installed - or only an old version) - exit code 1
    // Daemon can be not installed but enabled -.-
    QProcess process;
    process.start("systemctl", QStringList() << "is-enabled"
                                             << "razer_test.service");
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
    QString error(process.readAllStandardError());
    if (output == "enabled\n")
        return DaemonStatus::Enabled;
    else if (output == "disabled\n")
        return DaemonStatus::Disabled;
    else if (error == "Failed to get unit file state for razer_test.service: No such file or directory\n")
        return DaemonStatus::NotInstalled;
    else if (process.error() == QProcess::FailedToStart) { // check if systemctl could be started - fails on non-systemd distros and flatpak
        QFileInfo daemonFile("/usr/bin/razer_test");
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
    process.start("systemctl", QStringList() << "status"
                                             << "razer_test.service");
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
    process.start("systemctl", QStringList() << "enable"
                                             << "razer_test.service");
    process.waitForFinished();
    return process.exitCode() == 0;
}

// TODO New Qt5 connect style syntax - maybe https://stackoverflow.com/a/35501065/3527128
bool Manager::connectDevicesChanged(QObject *receiver, const char *slot)
{
    return OPENRAZER_DBUS_BUS.connect(OPENRAZER_SERVICE_NAME, "/io/github/openrazer1", "io.github.openrazer1.Manager", "devicesChanged", receiver, slot);
}

QDBusServiceWatcher *Manager::getServiceWatcher()
{
    return new QDBusServiceWatcher(OPENRAZER_SERVICE_NAME, OPENRAZER_DBUS_BUS);
}

QDBusInterface *ManagerPrivate::managerIface()
{
    if (iface == nullptr) {
        iface = new QDBusInterface(OPENRAZER_SERVICE_NAME, "/io/github/openrazer1", "io.github.openrazer1.Manager",
                                   OPENRAZER_DBUS_BUS, mParent);
    }
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return iface;
}

}

}
