// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LIBOPENRAZER_H
#define LIBOPENRAZER_H

#include "libopenrazer/capability.h"
#include "libopenrazer/dbusexception.h"
#include "libopenrazer/device.h"
#include "libopenrazer/led.h"
#include "libopenrazer/manager.h"
#include "libopenrazer/misc.h"
#include "libopenrazer/openrazer.h"

#include <QTranslator>
#include <QtGlobal>

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
    Capability(::openrazer::Effect::Off, QT_TRANSLATE_NOOP("libopenrazer", "Off"), 0),
    Capability(::openrazer::Effect::On, QT_TRANSLATE_NOOP("libopenrazer", "On"), 0),
    Capability(::openrazer::Effect::Static, QT_TRANSLATE_NOOP("libopenrazer", "Static"), 1),
    Capability(::openrazer::Effect::Breathing, QT_TRANSLATE_NOOP("libopenrazer", "Breathing"), 1),
    Capability(::openrazer::Effect::BreathingDual, QT_TRANSLATE_NOOP("libopenrazer", "Breathing Dual"), 2),
    Capability(::openrazer::Effect::BreathingRandom, QT_TRANSLATE_NOOP("libopenrazer", "Breathing Random"), 0),
    Capability(::openrazer::Effect::BreathingMono, QT_TRANSLATE_NOOP("libopenrazer", "Breathing"), 0),
    Capability(::openrazer::Effect::Blinking, QT_TRANSLATE_NOOP("libopenrazer", "Blinking"), 1),
    Capability(::openrazer::Effect::Spectrum, QT_TRANSLATE_NOOP("libopenrazer", "Spectrum"), 0),
    Capability(::openrazer::Effect::Wave, QT_TRANSLATE_NOOP("libopenrazer", "Wave"), 0),
    Capability(::openrazer::Effect::Wheel, QT_TRANSLATE_NOOP("libopenrazer", "Wheel"), 0),
    Capability(::openrazer::Effect::Reactive, QT_TRANSLATE_NOOP("libopenrazer", "Reactive"), 1),
    Capability(::openrazer::Effect::Ripple, QT_TRANSLATE_NOOP("libopenrazer", "Ripple"), 1),
    Capability(::openrazer::Effect::RippleRandom, QT_TRANSLATE_NOOP("libopenrazer", "Ripple Random"), 0),
};

/*!
 * Mapping from openrazer::LedId to a human readable string
 *
 * This needs to be translated by the user!
 *   qApp->translate("libopenrazer", foo)
 */
const QHash<::openrazer::LedId, const char *> ledIdToStringTable {
    { ::openrazer::LedId::Unspecified, "" },
    { ::openrazer::LedId::ScrollWheelLED, QT_TRANSLATE_NOOP("libopenrazer", "Scroll Wheel") },
    { ::openrazer::LedId::BatteryLED, QT_TRANSLATE_NOOP("libopenrazer", "Battery") },
    { ::openrazer::LedId::LogoLED, QT_TRANSLATE_NOOP("libopenrazer", "Logo") },
    { ::openrazer::LedId::BacklightLED, QT_TRANSLATE_NOOP("libopenrazer", "Backlight") },
    { ::openrazer::LedId::MacroRecordingLED, QT_TRANSLATE_NOOP("libopenrazer", "Macro Recording") },
    { ::openrazer::LedId::GameModeLED, QT_TRANSLATE_NOOP("libopenrazer", "Game Mode") },
    { ::openrazer::LedId::KeymapRedLED, QT_TRANSLATE_NOOP("libopenrazer", "Keymap Red") },
    { ::openrazer::LedId::KeymapGreenLED, QT_TRANSLATE_NOOP("libopenrazer", "Keymap Green") },
    { ::openrazer::LedId::KeymapBlueLED, QT_TRANSLATE_NOOP("libopenrazer", "Keymap Blue") },
    { ::openrazer::LedId::RightSideLED, QT_TRANSLATE_NOOP("libopenrazer", "Right Side") },
    { ::openrazer::LedId::LeftSideLED, QT_TRANSLATE_NOOP("libopenrazer", "Left Side") },
    { ::openrazer::LedId::ChargingLED, QT_TRANSLATE_NOOP("libopenrazer", "Charging") },
    { ::openrazer::LedId::FastChargingLED, QT_TRANSLATE_NOOP("libopenrazer", "Fast Charging") },
    { ::openrazer::LedId::FullyChargedLED, QT_TRANSLATE_NOOP("libopenrazer", "Fully Charged") },
};

/*!
 * Load the translations into the given QTranslator object.
 *
 * This then needs to be installed into the QApplication!
 */
bool loadTranslations(QTranslator *translator);

}

#endif // LIBOPENRAZER_H
