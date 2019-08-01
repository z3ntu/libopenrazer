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

class ManagerPrivate;

/*!
 * \brief Abstraction for accessing Manager objects via D-Bus.
 */
class Manager : public QObject
{
    Q_OBJECT
public:
    Manager();

    /*!
     * Returns a list of connected devices in form of their DBus object paths.
     *
     * Can be used to create a libopenrazer::Device object and get further information about the device.
     */
    QList<QDBusObjectPath> getDevices();

    /*!
     * Returns the daemon version currently running (e.g. \c '2.3.0').
     */
    QString getDaemonVersion();

    /*!
     * Returns if the daemon is running (and responding to the version call).
     */
    bool isDaemonRunning();

    /*!
     * Returns a list of supported devices in the format of \c {QHash<QString(DeviceName), QList<double(VID), double(PID)>>}.
     *
     * \sa Device::getVid(), Device::getPid()
     */
    QVariantHash getSupportedDevices();

    /*!
     * If devices should sync effects, as specified by \a yes.
     *
     * Example: Set it to \c 'on', set the lighting on one device to something, other devices connected will automatically get set to the same effect.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa getSyncEffects()
     */
    bool syncEffects(bool yes);

    /*!
     * Returns if devices should sync effect.
     *
     * \sa syncEffects()
     */
    bool getSyncEffects();

    /*!
     * Sets if the LEDs should turn off if the screensaver is turned on, as specified by \a turnOffOnScreensaver.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa getTurnOffOnScreensaver()
     */
    bool setTurnOffOnScreensaver(bool turnOffOnScreensaver);

    /*!
     * Returns if the LEDs should turn off if the screensaver is turned on.
     *
     * \sa setTurnOffOnScreensaver()
     */
    bool getTurnOffOnScreensaver();

    /*!
     * Returns status of the daemon, see DaemonStatus.
     */
    DaemonStatus getDaemonStatus();

    /*!
     * Returns the multiline output of \c {"systemctl status razer_test.service"}.
     */
    QString getDaemonStatusOutput();

    /*!
     * Enables the systemd unit for the OpenRazer daemon to auto-start when the user logs in. Runs \c {"systemctl enable razer_test.service"}
     *
     * Returns if the call was successful.
     */
    bool enableDaemon();
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
    bool connectDevicesChanged(QObject *receiver, const char *slot);

private:
    ManagerPrivate *d;
};

}

#endif // MANAGER_H
