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

#ifndef DEVICE_H
#define DEVICE_H

#include "libopenrazer/led.h"

#include <QDBusInterface>
#include <QObject>

namespace libopenrazer {

class DevicePrivate;

/*!
 * \brief Abstraction for accessing Device objects via D-Bus.
 */
class Device : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a new device object with the given \a objectPath.
     */
    Device(QDBusObjectPath objectPath);
    ~Device() override;

    /*!
     * Returns the DBus object path.
     */
    QDBusObjectPath objectPath();

    /*!
     * Returns if the device has the specified \a featureStr
     */
    bool hasFeature(const QString &featureStr);

    /*!
     * Returns the URL of an image that shows this device. Could return an empty string if no image was found.
     */
    QString getDeviceImageUrl();

    /*!
     * Returns a list of Leds supported on the device.
     */
    QList<Led *> getLeds();

    /*!
     * Returns the device mode of the device, like `0:0` or `3:0` for normal mode and driver mode respetively.
     *
     * \sa setDeviceMode()
     */
    QString getDeviceMode();

    /*!
     * Sets the device mode to the specified \a mode_id and \a param.
     * Allowed values are `0x00` (normal mode) and `0x03` (driver mode) for \a mode_id and `0x00` for param.
     *
     * When the device is set to Driver Mode, tilting the mouse wheel stops working, and the 'Up DPI' and 'Down DPI' buttons don't actually do anything. Razer Synapse on Windows is supposed to change the DPI instead of the mouse.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa getDeviceMode()
     */
    bool setDeviceMode(uchar mode_id, uchar param);

    /*!
     * Returns the serial number of the device which can be used to identify the device.
     */
    QString getSerial();

    /*!
     * Returns a human readable device name like `Razer DeathAdder Chroma` or `Razer Kraken 7.1`.
     */
    QString getDeviceName();

    /*!
     * Returns the type of the device. Could be one of `accessory`, `headset`, `keyboard`, `keypad`, `mouse`, `mousepad` or another type, if added to the daemon.
     */
    QString getDeviceType();

    /*!
     * Returns the firmware version of the device (e.g. `v1.0`).
     */
    QString getFirmwareVersion();

    /*!
     * Returns the physical layout of the keyboard (e.g. `de_DE`, `en_US`, `en_GB` or `unknown`)
     */
    QString getKeyboardLayout();

    /*!
     * Returns the current poll rate, e.g. `125`, `500` or `1000`.
     *
     * \sa setPollRate()
     */
    ushort getPollRate();

    /*!
     * Sets the poll rate of the mouse to the specified \a pollrate, e.g. `125`, `500` or `1000`.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa getPollRate()
     */
    bool setPollRate(ushort pollrate);

    /*!
     * Sets the DPI of the mouse to the specified \a dpi_x for the x-Axis and \a dpi_y for the y-Axis. Maximum value is what is returned by maxDPI().
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa getDPI(), maxDPI()
     */
    bool setDPI(razer_test::RazerDPI dpi);

    /*!
     * Returns the DPI of the mouse (e.g. `[800, 800]`).
     *
     * \sa setDPI()
     */
    razer_test::RazerDPI getDPI();

    /*!
     * Returns the maximum DPI possible for the device.
     *
     * \sa getDPI(), setDPI()
     */
    ushort maxDPI();

    /*!
     * Sets the lighting to custom mode (applies effects set from defineCustomFrame()).
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa defineCustomFrame()
     */
    bool displayCustomFrame();

    /*!
     * Sets the lighting of a key row to the specified \a colorData.
     * \a row is the row in the matrix, \a startColumn the column the \a colorData list starts and \a endColumn where the list ends.
     * Note, that you have to call displayCustomFrame() after setting otherwise the effect won't be displayed (even if you have already called displayCustomFrame() before).
     * Currently the driver only accepts whole rows that are sent.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa displayCustomFrame()
     */
    bool defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<QColor> colorData);

    /*!
     * Returns the dimension of the matrix supported on the device.
     *
     * \sa defineCustomFrame()
     */
    razer_test::MatrixDimensions getMatrixDimensions();

private:
    DevicePrivate *d;
    friend class LedPrivate;
    friend class Led;
};

}

#endif // DEVICE_H
