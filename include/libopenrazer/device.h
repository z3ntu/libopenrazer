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
 * \class libopenrazer::Device
 * \inmodule libopenrazer
 *
 * \brief The libopenrazer::Device class provides an abstraction for the OpenRazer daemon D-Bus interface for easy interaction from C++ applications.
 */
class Device : public QObject
{
    Q_OBJECT
public:
    /*!
     * \fn libopenrazer::Device::Device(QDBusObjectPath objectPath)
     *
     * Constructs a new device object with the given \a objectPath.
     */
    Device(QDBusObjectPath objectPath);
    ~Device() override;

    /*!
     * \fn QString libopenrazer::Device::objectPath()
     *
     * Returns the DBus object path.
     */
    QDBusObjectPath objectPath();
    bool hasFx(const QString &fxStr);
    bool hasFx(razer_test::RazerEffect fx);
    bool hasFeature(const QString &featureStr);

    /*!
     * \fn QString libopenrazer::Device::getPngFilename()
     *
     * Returns the filename for the \c top_img picture (e.g. \c razer-naga-hex-gallery-12.png).
     *
     * Could return an empty string (qstring.isEmpty()) if no picture was found.
     *
     * \sa getRazerUrls(), getPngUrl()
     */
    QString getPngFilename();

    /*!
     * \fn QString libopenrazer::Device::getPngUrl()
     *
     * Returns the URL for the \c top_img picture.
     *
     * Could return an empty string (\c qstring.isEmpty()) if no picture was found.
     *
     * \sa getRazerUrls(), getPngFilename()
     */
    QString getPngUrl();

    QList<Led *> getLeds();

    /*!
     * \fn QString libopenrazer::Device::getDeviceMode()
     *
     * Returns the device mode of the device, like '0:0' or '3:0' for normal mode and driver mode respetively.
     *
     * \sa setDeviceMode()
     */
    QString getDeviceMode();

    /*!
     * \fn bool libopenrazer::Device::setDeviceMode(uchar mode_id, uchar param)
     *
     * Sets the device mode to the specified \a mode_id and \a param.
     * Allowed values are 0x00 (normal mode) and 0x03 (driver mode) for \a mode_id and 0x00 for param.
     *
     * When the device is set to Driver Mode, tilting the mouse wheel stops working, and the 'Up DPI' and 'Down DPI' buttons don't actually do anything. Razer Synapse on Windows is supposed to change the DPI instead of the mouse.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa getDeviceMode()
     */
    bool setDeviceMode(uchar mode_id, uchar param);
    QString getSerial();

    /*!
     * \fn QString libopenrazer::Device::getDeviceName()
     *
     * Returns a human readable device name like \c {"Razer DeathAdder Chroma"} or \c {"Razer Kraken 7.1"}.
     */
    QString getDeviceName();

    /*!
     * \fn QString libopenrazer::Device::getDeviceType()
     *
     * Returns the type of the device. Could be one of \c 'keyboard', \c 'mouse', \c 'mousemat', \c 'core', \c 'keypad', \c 'headset', \c 'mug' or another type, if added to the daemon.
     */
    QString getDeviceType();

    /*!
     * \fn QString libopenrazer::Device::getFirmwareVersion()
     *
     * Returns the firmware version of the device (e.g. \c 'v1.0').
     */
    QString getFirmwareVersion();

    /*!
     * \fn QString libopenrazer::Device::getKeyboardLayout()
     *
     * Returns the physical layout of the keyboard (e.g. \c 'de_DE', \c 'en_US', \c 'en_GB' or \c 'unknown')
     */
    QString getKeyboardLayout();

    /*!
     * \fn QVariantHash libopenrazer::Device::getRazerUrls()
     *
     * Returns a QVariantHash (\c {QHash<QString, QVariant>}).
     * Most likely contains keys \c "top_img", \c "side_img", \c "store" and \c "perspective_img".
     * Values are \c QVariant<QString> with a full URL as value.
     */
    QVariantHash getRazerUrls();

    /*!
     * \fn int libopenrazer::Device::getPollRate()
     *
     * Returns the current poll rate.
     */
    ushort getPollRate();

    /*!
     * \fn bool libopenrazer::Device::setPollRate(ushort pollrate)
     *
     * Sets the poll rate of the mouse to the specified \a pollrate.
     *
     * Returns if the D-Bus call was successful.
     */
    bool setPollRate(ushort pollrate);

    /*!
     * \fn bool libopenrazer::Device::setDPI(int dpi_x, int dpi_y)
     *
     * Sets the DPI of the mouse to the specified \a dpi_x for the x-Axis and \a dpi_y for the y-Axis. Maximum value is what is returned by maxDPI().
     *
     * Returns if the D-Bus call was successful.
     */
    bool setDPI(razer_test::RazerDPI dpi);

    /*!
     * \fn QList<int> libopenrazer::Device::getDPI()
     *
     * Returns the DPI of the mouse (e.g. \c [800, 800]).
     */
    razer_test::RazerDPI getDPI();

    /*!
     * \fn int libopenrazer::Device::maxDPI()
     *
     * Returns the maximum DPI possible for the device.
     */
    ushort maxDPI();

    /*!
     * \fn bool libopenrazer::Device::setCustom()
     *
     * Sets the lighting to custom mode (applies effects set from setKeyRow()).
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa setKeyRow()
     */
    bool displayCustomFrame();

    /*!
     * \fn bool libopenrazer::Device::setKeyRow(uchar row, uchar startcol, uchar endcol, QVector<QColor> colors)
     *
     * Sets the lighting of a key row to the specified \a colors.
     * \a row is the row in the matrix, \a startcol the column the \a colors list starts and \a endcol where the list ends.
     * Note, that you have to call setCustom() after setting otherwise the effect won't be displayed (even if you have already called setCustom() before).
     * Currently the driver only accepts whole rows that are sent.
     *
     * Returns if the D-Bus call was successful.
     *
     * \sa setCustom()
     */
    bool defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<QColor> colorData);
    razer_test::MatrixDimensions getMatrixDimensions();

private:
    DevicePrivate *d;
};

}

#endif // DEVICE_H
