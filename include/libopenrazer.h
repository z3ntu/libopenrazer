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
const QVector<RazerCapability> ledFxList {
    RazerCapability(::openrazer::RazerEffect::Off, Led::tr("Off"), 0),
    RazerCapability(::openrazer::RazerEffect::On, Led::tr("On"), 0),
    RazerCapability(::openrazer::RazerEffect::Static, Led::tr("Static"), 1),
    RazerCapability(::openrazer::RazerEffect::Breathing, Led::tr("Breathing"), 1),
    RazerCapability(::openrazer::RazerEffect::BreathingDual, Led::tr("Breathing Dual"), 2),
    RazerCapability(::openrazer::RazerEffect::BreathingRandom, Led::tr("Breathing Random"), 0),
    RazerCapability(::openrazer::RazerEffect::BreathingMono, Led::tr("Breathing"), 0),
    RazerCapability(::openrazer::RazerEffect::Blinking, Led::tr("Blinking"), 1),
    RazerCapability(::openrazer::RazerEffect::Spectrum, Led::tr("Spectrum"), 0),
    RazerCapability(::openrazer::RazerEffect::Wave, Led::tr("Wave"), 0),
    RazerCapability(::openrazer::RazerEffect::Wheel, Led::tr("Wheel"), 0),
    RazerCapability(::openrazer::RazerEffect::Reactive, Led::tr("Reactive"), 1),
    RazerCapability(::openrazer::RazerEffect::Ripple, Led::tr("Ripple"), 1),
    RazerCapability(::openrazer::RazerEffect::RippleRandom, Led::tr("Ripple Random"), 0),
};

/*!
 * Mapping from openrazer::RazerLedId to a human readable string
 */
const QHash<::openrazer::RazerLedId, QString> ledIdToStringTable {
    { ::openrazer::RazerLedId::Unspecified, "" },
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
    { ::openrazer::RazerLedId::LeftSideLED, Led::tr("Left Side") },
    { ::openrazer::RazerLedId::ChargingLED, Led::tr("Charging") },
    { ::openrazer::RazerLedId::FastChargingLED, Led::tr("Fast Charging") },
    { ::openrazer::RazerLedId::FullyChargedLED, Led::tr("Fully Charged") },
};

}

#endif // LIBOPENRAZER_H
