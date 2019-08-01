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

#ifndef DEVICE_P_H
#define DEVICE_P_H

#include "libopenrazer/device.h"
#include "libopenrazer/led.h"

#include <QDBusInterface>

namespace libopenrazer {

class DevicePrivate
{
public:
    Device *mParent = nullptr;

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
};

}

#endif // DEVICE_P_H
