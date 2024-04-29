// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEVICE_H
#define DEVICE_H

#include "libopenrazer/openrazer.h"

#include <QDBusInterface>
#include <QObject>

namespace libopenrazer {

class Led;

/*!
 * \brief Abstraction for accessing Device objects via D-Bus.
 */
class Device : public QObject
{
    Q_OBJECT
public:
    /*!
     * Returns the DBus object path.
     */
    virtual QDBusObjectPath objectPath() = 0;

    /*!
     * Returns if the device has the specified \a featureStr
     */
    virtual bool hasFeature(const QString &featureStr) = 0;

    /*!
     * Returns the URL of an image that shows this device. Could return an empty string if no image was found.
     */
    virtual QString getDeviceImageUrl() = 0;

    /*!
     * Returns a list of Leds supported on the device.
     */
    virtual QList<Led *> getLeds() = 0;

    /*!
     * Returns the device mode of the device, like `0:0` or `3:0` for normal mode and driver mode respetively.
     */
    virtual QString getDeviceMode() = 0;

    /*!
     * Returns the serial number of the device which can be used to identify the device.
     */
    virtual QString getSerial() = 0;

    /*!
     * Returns a human readable device name like `Razer DeathAdder Chroma` or `Razer Kraken 7.1`.
     */
    virtual QString getDeviceName() = 0;

    /*!
     * Returns the type of the device. Could be one of `accessory`, `headset`, `keyboard`, `keypad`, `mouse`, `mousepad` or another type, if added to the daemon.
     */
    virtual QString getDeviceType() = 0;

    /*!
     * Returns the firmware version of the device (e.g. `v1.0`).
     */
    virtual QString getFirmwareVersion() = 0;

    /*!
     * Returns the physical layout of the keyboard (e.g. `de_DE`, `en_US`, `en_GB` or `unknown`)
     */
    virtual QString getKeyboardLayout() = 0;

    /*!
     * Returns the current poll rate, e.g. `125`, `500` or `1000`.
     *
     * \sa setPollRate(), getSupportedPollRates()
     */
    virtual ushort getPollRate() = 0;

    /*!
     * Sets the poll rate of the mouse to the specified \a pollrate, e.g. `125`, `500` or `1000`.
     *
     * \sa getPollRate()
     */
    virtual void setPollRate(ushort pollrate) = 0;

    /*!
     * Returns the poll rates that are supported by the device.
     *
     * \sa setPollRate()
     */
    virtual QVector<ushort> getSupportedPollRates() = 0;

    /*!
     * Sets the DPI of the mouse to the specified \a dpi_x for the x-Axis and \a dpi_y for the y-Axis. Maximum value is what is returned by maxDPI().
     *
     * \sa getDPI(), maxDPI(), getAllowedDPIValues()
     */
    virtual void setDPI(::openrazer::RazerDPI dpi) = 0;

    /*!
     * Returns the DPI of the mouse (e.g. `[800, 800]`).
     *
     * \sa setDPI()
     */
    virtual ::openrazer::RazerDPI getDPI() = 0;

    /*!
     * Sets the DPI stages of the mouse to the specified \a dpiStages and sets stage nr. \a activeStage active.
     * A maximum of 5 stages are possible.
     * The active stage is 1-indexed.
     * The DPI value of each must not exceed the maximum DPI of the device.
     *
     * \sa getDPIStages(), maxDPI()
     */
    virtual void setDPIStages(uchar activeStage, QVector<::openrazer::RazerDPI> dpiStages) = 0;

    /*!
     * Returns a pair of the active DPI stage and the configured DPI stages of the mouse.
     * A maximum of 5 stages are possible.
     * The active stage is 1-indexed.
     * Ex: 3, [(500, 500), (1500, 1500), (1800, 1800)]
     *
     * \sa setDPIStages(), maxDPI()
     */
    virtual QPair<uchar, QVector<::openrazer::RazerDPI>> getDPIStages() = 0;

    /*!
     * Returns the maximum DPI possible for the device.
     *
     * \sa getDPI(), setDPI()
     */
    virtual ushort maxDPI() = 0;

    /*!
     * Some devices only support a small number of DPI values instead of arbitrary values.
     *
     * Returns the allowed values you can pass to setDPI
     */
    virtual QVector<ushort> getAllowedDPI() = 0;

    /*!
     * Returns the percentage the battery of the device is charged.
     */
    virtual double getBatteryPercent() = 0;

    /*!
     * Returns whether the device is currently charging.
     */
    virtual bool isCharging() = 0;

    /*!
     * Returns the time (in seconds) after which the device will enter sleep mode.
     *
     * Maximum value: 900 seconds (15 minutes)
     *
     * \sa setIdleTime()
     */
    virtual ushort getIdleTime() = 0;

    /*!
     * Sets the time (in seconds) after which the device will enter sleep mode.
     *
     * Maximum value: 900 seconds (15 minutes)
     *
     * \sa getIdleTime()
     */
    virtual void setIdleTime(ushort idleTime) = 0;

    /*!
     * Returns the battery percentage before the device will enter low power mode.
     *
     * \sa setLowBatteryThreshold()
     */
    virtual double getLowBatteryThreshold() = 0;

    /*!
     * Sets the battery percentage before the device will enter low power mode.
     *
     * \sa getLowBatteryThreshold()
     */
    virtual void setLowBatteryThreshold(double threshold) = 0;

    /*!
     * Sets the lighting to custom mode (applies effects set from defineCustomFrame()).
     *
     * \sa defineCustomFrame()
     */
    virtual void displayCustomFrame() = 0;

    /*!
     * Sets the lighting of a key row to the specified \a colorData.
     * \a row is the row in the matrix, \a startColumn the column the \a colorData list starts and \a endColumn where the list ends.
     * Note, that you have to call displayCustomFrame() after setting otherwise the effect won't be displayed (even if you have already called displayCustomFrame() before).
     * Currently the driver only accepts whole rows that are sent.
     *
     * \sa displayCustomFrame()
     */
    virtual void defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<::openrazer::RGB> colorData) = 0;

    /*!
     * Returns the dimension of the matrix supported on the device.
     *
     * \sa defineCustomFrame()
     */
    virtual ::openrazer::MatrixDimensions getMatrixDimensions() = 0;
};

namespace openrazer {

class DevicePrivate;
class Led;
class LedPrivate;

class Device : public ::libopenrazer::Device
{
public:
    Device(QDBusObjectPath objectPath);
    ~Device() override;

    QDBusObjectPath objectPath() override;
    bool hasFeature(const QString &featureStr) override;
    QString getDeviceImageUrl() override;
    QList<::libopenrazer::Led *> getLeds() override;
    QString getDeviceMode() override;
    QString getSerial() override;
    QString getDeviceName() override;
    QString getDeviceType() override;
    QString getFirmwareVersion() override;
    QString getKeyboardLayout() override;
    ushort getPollRate() override;
    void setPollRate(ushort pollrate) override;
    QVector<ushort> getSupportedPollRates() override;
    void setDPI(::openrazer::RazerDPI dpi) override;
    ::openrazer::RazerDPI getDPI() override;
    void setDPIStages(uchar activeStage, QVector<::openrazer::RazerDPI> dpiStages) override;
    QPair<uchar, QVector<::openrazer::RazerDPI>> getDPIStages() override;
    ushort maxDPI() override;
    QVector<ushort> getAllowedDPI() override;
    double getBatteryPercent() override;
    bool isCharging() override;
    ushort getIdleTime() override;
    void setIdleTime(ushort idleTime) override;
    double getLowBatteryThreshold() override;
    void setLowBatteryThreshold(double threshold) override;
    void displayCustomFrame() override;
    void defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<::openrazer::RGB> colorData) override;
    ::openrazer::MatrixDimensions getMatrixDimensions() override;

private:
    DevicePrivate *d;

    friend class Led;
    friend class LedPrivate;
};

}

namespace razer_test {

class DevicePrivate;
class Led;
class LedPrivate;

class Device : public ::libopenrazer::Device
{
public:
    Device(QDBusObjectPath objectPath);
    ~Device() override;

    QDBusObjectPath objectPath() override;
    bool hasFeature(const QString &featureStr) override;
    QString getDeviceImageUrl() override;
    QList<::libopenrazer::Led *> getLeds() override;
    QString getDeviceMode() override;
    QString getSerial() override;
    QString getDeviceName() override;
    QString getDeviceType() override;
    QString getFirmwareVersion() override;
    QString getKeyboardLayout() override;
    ushort getPollRate() override;
    void setPollRate(ushort pollrate) override;
    QVector<ushort> getSupportedPollRates() override;
    void setDPI(::openrazer::RazerDPI dpi) override;
    ::openrazer::RazerDPI getDPI() override;
    void setDPIStages(uchar activeStage, QVector<::openrazer::RazerDPI> dpiStages) override;
    QPair<uchar, QVector<::openrazer::RazerDPI>> getDPIStages() override;
    ushort maxDPI() override;
    QVector<ushort> getAllowedDPI() override;
    double getBatteryPercent() override;
    bool isCharging() override;
    ushort getIdleTime() override;
    void setIdleTime(ushort idleTime) override;
    double getLowBatteryThreshold() override;
    void setLowBatteryThreshold(double threshold) override;
    void displayCustomFrame() override;
    void defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<::openrazer::RGB> colorData) override;
    ::openrazer::MatrixDimensions getMatrixDimensions() override;

private:
    DevicePrivate *d;

    friend class Led;
    friend class LedPrivate;
};

}

}

#endif // DEVICE_H
