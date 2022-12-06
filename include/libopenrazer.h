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

#ifndef LIBOPENRAZER_H
#define LIBOPENRAZER_H

#include "libopenrazer/dbusexception.h"
#include "libopenrazer/device.h"
#include "libopenrazer/led.h"
#include "libopenrazer/manager.h"
#include "libopenrazer/misc.h"
#include "libopenrazer/openrazer.h"
#include "libopenrazer/razercapability.h"

// NOTE: DBus types -> Qt/C++ types: http://doc.qt.io/qt-5/qdbustypesystem.html#primitive-types

/*!
 * \namespace libopenrazer
 *
 * \brief C++/Qt bindings for the OpenRazer D-Bus interface.
 */
namespace libopenrazer {

/*!
 * List of effects provided by OpenRazer.
 */
const QList<RazerCapability> ledFxList {
    RazerCapability(::openrazer::RazerEffect::Off, Led::tr("Off"), 0),
    RazerCapability(::openrazer::RazerEffect::On, Led::tr("On"), 0),
    RazerCapability(::openrazer::RazerEffect::Static, Led::tr("Static"), 1),
    RazerCapability(::openrazer::RazerEffect::Breathing, Led::tr("Breathing"), 1),
    RazerCapability(::openrazer::RazerEffect::BreathingDual, Led::tr("Breathing Dual"), 2),
    RazerCapability(::openrazer::RazerEffect::BreathingRandom, Led::tr("Breathing Random"), 0),
    RazerCapability(::openrazer::RazerEffect::Blinking, Led::tr("Blinking"), 1),
    RazerCapability(::openrazer::RazerEffect::Spectrum, Led::tr("Spectrum"), 0),
    RazerCapability(::openrazer::RazerEffect::Wave, Led::tr("Wave"), true),
    RazerCapability(::openrazer::RazerEffect::Reactive, Led::tr("Reactive"), 1),
};

/*!
 * Mapping from openrazer::RazerLedId to a human readable string
 */
const QHash<::openrazer::RazerLedId, QString> ledIdToStringTable {
    { ::openrazer::RazerLedId::Unspecified, Led::tr("Unspecified") },
    { ::openrazer::RazerLedId::ScrollWheelLED, Led::tr("Scroll Wheel") },
    { ::openrazer::RazerLedId::BatteryLED, Led::tr("Battery") },
    { ::openrazer::RazerLedId::LogoLED, Led::tr("Logo") },
    { ::openrazer::RazerLedId::BacklightLED, Led::tr("Backlight") },
    { ::openrazer::RazerLedId::MacroRecordingLED, Led::tr("Macro Recording") },
    { ::openrazer::RazerLedId::GameModeLED, Led::tr("Game Mode") },
    { ::openrazer::RazerLedId::KeymapRedLED, Led::tr("Keymap Red") },
    { ::openrazer::RazerLedId::KeymapGreenLED, Led::tr("Keymap Green") },
    { ::openrazer::RazerLedId::KeymapBlueLED, Led::tr("Keymap Blue") },
    { ::openrazer::RazerLedId::RightSideLED, Led::tr("Right Side") },
    { ::openrazer::RazerLedId::LeftSideLED, Led::tr("Left Side") }
};

}

#endif // LIBOPENRAZER_H
