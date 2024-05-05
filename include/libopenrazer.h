// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LIBOPENRAZER_H
#define LIBOPENRAZER_H

#include "libopenrazer/dbusexception.h"
#include "libopenrazer/device.h"
#include "libopenrazer/led.h"
#include "libopenrazer/manager.h"
#include "libopenrazer/misc.h"
#include "libopenrazer/openrazer.h"
#include "libopenrazer/capability.h"

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
const QVector<Capability> ledFxList {
    Capability(::openrazer::Effect::Off, Led::tr("Off"), 0),
    Capability(::openrazer::Effect::On, Led::tr("On"), 0),
    Capability(::openrazer::Effect::Static, Led::tr("Static"), 1),
    Capability(::openrazer::Effect::Breathing, Led::tr("Breathing"), 1),
    Capability(::openrazer::Effect::BreathingDual, Led::tr("Breathing Dual"), 2),
    Capability(::openrazer::Effect::BreathingRandom, Led::tr("Breathing Random"), 0),
    Capability(::openrazer::Effect::BreathingMono, Led::tr("Breathing"), 0),
    Capability(::openrazer::Effect::Blinking, Led::tr("Blinking"), 1),
    Capability(::openrazer::Effect::Spectrum, Led::tr("Spectrum"), 0),
    Capability(::openrazer::Effect::Wave, Led::tr("Wave"), 0),
    Capability(::openrazer::Effect::Wheel, Led::tr("Wheel"), 0),
    Capability(::openrazer::Effect::Reactive, Led::tr("Reactive"), 1),
    Capability(::openrazer::Effect::Ripple, Led::tr("Ripple"), 1),
    Capability(::openrazer::Effect::RippleRandom, Led::tr("Ripple Random"), 0),
};

/*!
 * Mapping from openrazer::LedId to a human readable string
 */
const QHash<::openrazer::LedId, QString> ledIdToStringTable {
    { ::openrazer::LedId::Unspecified, "" },
    { ::openrazer::LedId::ScrollWheelLED, Led::tr("Scroll Wheel") },
    { ::openrazer::LedId::BatteryLED, Led::tr("Battery") },
    { ::openrazer::LedId::LogoLED, Led::tr("Logo") },
    { ::openrazer::LedId::BacklightLED, Led::tr("Backlight") },
    { ::openrazer::LedId::MacroRecordingLED, Led::tr("Macro Recording") },
    { ::openrazer::LedId::GameModeLED, Led::tr("Game Mode") },
    { ::openrazer::LedId::KeymapRedLED, Led::tr("Keymap Red") },
    { ::openrazer::LedId::KeymapGreenLED, Led::tr("Keymap Green") },
    { ::openrazer::LedId::KeymapBlueLED, Led::tr("Keymap Blue") },
    { ::openrazer::LedId::RightSideLED, Led::tr("Right Side") },
    { ::openrazer::LedId::LeftSideLED, Led::tr("Left Side") },
    { ::openrazer::LedId::ChargingLED, Led::tr("Charging") },
    { ::openrazer::LedId::FastChargingLED, Led::tr("Fast Charging") },
    { ::openrazer::LedId::FullyChargedLED, Led::tr("Fully Charged") },
};

}

#endif // LIBOPENRAZER_H
