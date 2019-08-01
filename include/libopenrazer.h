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
 *
 */

#ifndef LIBRAZER_H
#define LIBRAZER_H

#include <razer_test.h>

#include "libopenrazer/dbusexception.h"
#include "libopenrazer/device.h"
#include "libopenrazer/led.h"
#include "libopenrazer/manager.h"
#include "libopenrazer/misc.h"
#include "libopenrazer/razercapability.h"

// NOTE: DBus types -> Qt/C++ types: http://doc.qt.io/qt-5/qdbustypesystem.html#primitive-types

namespace libopenrazer {

const QList<RazerCapability> ledFxList {
    RazerCapability(razer_test::RazerEffect::Off, Led::tr("Off"), 0),
    RazerCapability(razer_test::RazerEffect::Static, Led::tr("Static"), 1),
    RazerCapability(razer_test::RazerEffect::Breathing, Led::tr("Breathing"), 1),
    RazerCapability(razer_test::RazerEffect::BreathingDual, Led::tr("Breathing Dual"), 2),
    RazerCapability(razer_test::RazerEffect::BreathingRandom, Led::tr("Breathing Random"), 0),
    RazerCapability(razer_test::RazerEffect::Blinking, Led::tr("Blinking"), 1),
    RazerCapability(razer_test::RazerEffect::Spectrum, Led::tr("Spectrum"), 0),
    RazerCapability(razer_test::RazerEffect::Wave, Led::tr("Wave"), true),
    RazerCapability(razer_test::RazerEffect::Reactive, Led::tr("Reactive"), 1),
};

const QHash<razer_test::RazerLedId, QString> ledIdToStringTable {
    { razer_test::RazerLedId::Unspecified, Led::tr("Unspecified") },
    { razer_test::RazerLedId::ScrollWheelLED, Led::tr("Scroll Wheel") },
    { razer_test::RazerLedId::BatteryLED, Led::tr("Battery") },
    { razer_test::RazerLedId::LogoLED, Led::tr("Logo") },
    { razer_test::RazerLedId::BacklightLED, Led::tr("Backlight") },
    { razer_test::RazerLedId::MacroRecordingLED, Led::tr("Macro Recording") },
    { razer_test::RazerLedId::GameModeLED, Led::tr("Game Mode") },
    { razer_test::RazerLedId::KeymapRedLED, Led::tr("Keymap Red") },
    { razer_test::RazerLedId::KeymapGreenLED, Led::tr("Keymap Green") },
    { razer_test::RazerLedId::KeymapBlueLED, Led::tr("Keymap Blue") },
    { razer_test::RazerLedId::RightSideLED, Led::tr("Right Side") },
    { razer_test::RazerLedId::LeftSideLED, Led::tr("Left Side") }
};

}

#endif // LIBRAZER_H
