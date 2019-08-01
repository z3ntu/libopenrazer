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

#include <QObject>
#include <QDBusInterface>

#include "libopenrazer/led.h"

namespace libopenrazer {

class Device : public QObject
{
    Q_OBJECT
private:
    QDBusInterface *ifaceMisc = nullptr;
    QDBusInterface *ifaceDpi = nullptr;
    QDBusInterface *ifaceLightingChroma = nullptr;
    QDBusInterface *deviceMiscIface();
    QDBusInterface *deviceDpiIface();
    QDBusInterface *deviceLightingChromaIface();

    QDBusObjectPath mObjectPath;

    QStringList supportedFx;
    QStringList supportedFeatures;

    QList<Led *> leds;

    void introspect();
    void setupCapabilities();
    bool hasCapabilityInternal(const QString &interface, const QString &method = QString());
    QStringList introspection;

    // Maps RazerLedId to "Chroma" or "Scroll" (the string put e.g. into setScrollSpectrum)
    QMap<razer_test::RazerLedId, QString> supportedLeds;

public:
    Device(QDBusObjectPath objectPath);
    ~Device() override;

    QDBusObjectPath objectPath();
    bool hasFx(const QString &fxStr);
    bool hasFx(razer_test::RazerEffect fx);
    bool hasFeature(const QString &featureStr);
    QString getPngFilename();
    QString getPngUrl();

    QList<Led *> getLeds();

    // --- MISC METHODS ---
    QString getDeviceMode();
    bool setDeviceMode(uchar mode_id, uchar param);
    QString getSerial();
    QString getDeviceName();
    QString getDeviceType();
    QString getFirmwareVersion();
    QString getKeyboardLayout();
    QVariantHash getRazerUrls();

    // --- POLL RATE ---
    ushort getPollRate();
    bool setPollRate(ushort pollrate);

    // --- DPI ---
    bool setDPI(razer_test::RazerDPI dpi);
    razer_test::RazerDPI getDPI();
    ushort maxDPI();

    // - Custom frame -
    bool displayCustomFrame();
    bool defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<QColor> colorData);
    razer_test::MatrixDimensions getMatrixDimensions();
};

}

#endif // DEVICE_H
