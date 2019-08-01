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

#ifndef LED_P_H
#define LED_P_H

#include "libopenrazer/led.h"

#include <QDBusInterface>

namespace libopenrazer {

class LedPrivate
{
public:
    Led *mParent = nullptr;

    QDBusInterface *iface = nullptr;
    QDBusInterface *ifaceBrightness = nullptr;
    QDBusInterface *ledIface();
    QDBusInterface *ledBrightnessIface();

    QDBusObjectPath mObjectPath;

    razer_test::RazerLedId ledId;
    QString lightingLocation;
    QString lightingLocationMethod;
};

}

#endif // LED_P_H
