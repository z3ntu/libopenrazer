// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MANAGER_H
#define MANAGER_H

#include "libopenrazer/misc.h"

#include <QDBusInterface>
#include <QDBusServiceWatcher>

namespace libopenrazer {

class Device;

/*!
 * \brief Abstraction for accessing Manager objects via D-Bus.
 */
class Manager : public QObject
{
    Q_OBJECT
public:
    /*!
     * Returns a list of connected devices in form of their DBus object paths.
     *
     * Can be used to call getDevice and get further information about the device.
     */
    virtual QList<QDBusObjectPath> getDevices() = 0;

    /*!
     * Returns a Device object with the given DBus object path.
     */
    virtual Device *getDevice(QDBusObjectPath objectPath) = 0;

    /*!
     * Returns the daemon version currently running (e.g. `2.3.0`).
     */
    virtual QString getDaemonVersion() = 0;

    /*!
     * Returns if the daemon is running (and responding to the version call).
     */
    virtual bool isDaemonRunning() = 0;

    /*!
     * Returns a list of supported devices in the format of `QHash<QString(DeviceName), QList<double(VID), double(PID)>>`.
     *
     * \sa Device::getVid(), Device::getPid()
     */
    virtual QVariantHash getSupportedDevices() = 0;

    /*!
     * If devices should sync effects, as specified by \a yes.
     *
     * Example: Set it to \c 'on', set the lighting on one device to something, other devices connected will automatically get set to the same effect.
     *
     * \sa getSyncEffects()
     */
    virtual void syncEffects(bool yes) = 0;

    /*!
     * Returns if devices should sync effect.
     *
     * \sa syncEffects()
     */
    virtual bool getSyncEffects() = 0;

    /*!
     * Sets if the LEDs should turn off if the screensaver is turned on, as specified by \a turnOffOnScreensaver.
     *
     * \sa getTurnOffOnScreensaver()
     */
    virtual void setTurnOffOnScreensaver(bool turnOffOnScreensaver) = 0;

    /*!
     * Returns if the LEDs should turn off if the screensaver is turned on.
     *
     * \sa setTurnOffOnScreensaver()
     */
    virtual bool getTurnOffOnScreensaver() = 0;

    /*!
     * Returns status of the daemon, see DaemonStatus.
     */
    virtual DaemonStatus getDaemonStatus() = 0;

    /*!
     * Returns the multiline output of systemctl status.
     */
    virtual QString getDaemonStatusOutput() = 0;

    /*!
     * Enables the systemd unit for the OpenRazer daemon to auto-start when the user logs in.
     *
     * Returns if the call was successful.
     */
    virtual bool enableDaemon() = 0;
    // bool disableDaemon();

    /*!
     * Connects the \c device_added signal of the daemon to the specified method using the \a receiver and \a slot.
     *
     * Can be used in the Qt4-style Signal&Slot syntax, e.g.:
     * \code
     * libopenrazer::connectDeviceAdded(this, SLOT(deviceAdded()));
     * \endcode
     *
     * Returns if the connection was successful.
     *
     * \sa connectDeviceRemoved()
     */
    virtual bool connectDevicesChanged(QObject *receiver, const char *slot) = 0;

    /*!
     * Returns a \c QDBusServiceWatcher that can be used to watch the daemon status.
     *
     * The \c serviceRegistered and \c serviceUnregistered signals are probably the most interesting ones.
     */
    virtual QDBusServiceWatcher *getServiceWatcher() = 0;
};

namespace openrazer {

class ManagerPrivate;
class Manager : public ::libopenrazer::Manager
{
public:
    Manager();
    QList<QDBusObjectPath> getDevices() override;
    Device *getDevice(QDBusObjectPath objectPath) override;
    QString getDaemonVersion() override;
    bool isDaemonRunning() override;
    QVariantHash getSupportedDevices() override;
    void syncEffects(bool yes) override;
    bool getSyncEffects() override;
    void setTurnOffOnScreensaver(bool turnOffOnScreensaver) override;
    bool getTurnOffOnScreensaver() override;
    DaemonStatus getDaemonStatus() override;
    QString getDaemonStatusOutput() override;
    bool enableDaemon() override;
    bool connectDevicesChanged(QObject *receiver, const char *slot) override;
    QDBusServiceWatcher *getServiceWatcher() override;

private:
    ManagerPrivate *d;
};

}

namespace razer_test {

class ManagerPrivate;
class Manager : public ::libopenrazer::Manager
{
public:
    Manager();
    QList<QDBusObjectPath> getDevices() override;
    Device *getDevice(QDBusObjectPath objectPath) override;
    QString getDaemonVersion() override;
    bool isDaemonRunning() override;
    QVariantHash getSupportedDevices() override;
    void syncEffects(bool yes) override;
    bool getSyncEffects() override;
    void setTurnOffOnScreensaver(bool turnOffOnScreensaver) override;
    bool getTurnOffOnScreensaver() override;
    DaemonStatus getDaemonStatus() override;
    QString getDaemonStatusOutput() override;
    bool enableDaemon() override;
    bool connectDevicesChanged(QObject *receiver, const char *slot) override;
    QDBusServiceWatcher *getServiceWatcher() override;

private:
    ManagerPrivate *d;
};

}

}

#endif // MANAGER_H
