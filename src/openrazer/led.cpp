// Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "device_p.h"
#include "led_p.h"
#include "libopenrazer.h"
#include "libopenrazer_private.h"

#include <QDBusReply>

#define RGB_TO_QVARIANT(c)            \
    QVariant::fromValue(c.r),         \
            QVariant::fromValue(c.g), \
            QVariant::fromValue(c.b)

namespace libopenrazer {

namespace openrazer {

Led::Led(Device *device, QDBusObjectPath objectPath, ::openrazer::LedId ledId, QString lightingLocation)
{
    d = new LedPrivate();
    d->mParent = this;
    d->device = device;
    d->mObjectPath = objectPath;
    d->ledId = ledId;
    d->lightingLocation = lightingLocation;

    // Leave lightingLocationMethod empty in case it's Chroma
    if (lightingLocation == "Chroma") {
        d->interface = "razer.device.lighting.chroma";
    } else if (d->isProfileLed()) {
        d->lightingLocationMethod = lightingLocation;
        d->interface = "razer.device.lighting.profile_led";
    } else {
        d->lightingLocationMethod = lightingLocation;
        d->interface = "razer.device.lighting." + fromCamelCase(lightingLocation);
    }

    d->setupCapabilities();
}

/*
 * Destructor
 */
Led::~Led() = default;

void LedPrivate::setupCapabilities()
{
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "None"))
        supportedFx.append(::openrazer::Effect::Off);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "On"))
        supportedFx.append(::openrazer::Effect::On);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Static"))
        supportedFx.append(::openrazer::Effect::Static);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Blinking"))
        supportedFx.append(::openrazer::Effect::Blinking);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathSingle"))
        supportedFx.append(::openrazer::Effect::Breathing);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathDual"))
        supportedFx.append(::openrazer::Effect::BreathingDual);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathRandom"))
        supportedFx.append(::openrazer::Effect::BreathingRandom);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathMono"))
        supportedFx.append(::openrazer::Effect::BreathingMono);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Spectrum"))
        supportedFx.append(::openrazer::Effect::Spectrum);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Wave"))
        supportedFx.append(::openrazer::Effect::Wave);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Wheel"))
        supportedFx.append(::openrazer::Effect::Wheel);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Reactive"))
        supportedFx.append(::openrazer::Effect::Reactive);

    if (supportedFx.isEmpty() && device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Active")) {
        supportedFx.append(::openrazer::Effect::Off);
        supportedFx.append(::openrazer::Effect::On);
    }

    if (device->d->hasCapabilityInternal("razer.device.lighting.profile_led", "set" + lightingLocationMethod)) {
        supportedFx.append(::openrazer::Effect::Off);
        supportedFx.append(::openrazer::Effect::On);
    }

    // No-color static/breathing variants
    if (device->d->hasCapabilityInternal("razer.device.lighting.bw2013", "setStatic"))
        supportedFx.append(::openrazer::Effect::Static);
    if (device->d->hasCapabilityInternal("razer.device.lighting.bw2013", "setPulsate"))
        supportedFx.append(::openrazer::Effect::Breathing);

    if (device->d->hasCapabilityInternal("razer.device.lighting.custom", "setRipple"))
        supportedFx.append(::openrazer::Effect::Ripple);
    if (device->d->hasCapabilityInternal("razer.device.lighting.custom", "setRippleRandomColour"))
        supportedFx.append(::openrazer::Effect::RippleRandom);

    if (lightingLocation == "Chroma") {
        if (device->d->hasCapabilityInternal("razer.device.lighting.brightness", "setBrightness"))
            supportsBrightness = true;
    } else {
        if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Brightness"))
            supportsBrightness = true;
    }
}

QDBusObjectPath Led::getObjectPath()
{
    return d->mObjectPath;
}

bool Led::hasBrightness()
{
    return d->supportsBrightness;
}

bool Led::hasFx(::openrazer::Effect fx)
{
    return d->supportedFx.contains(fx);
}

::openrazer::Effect Led::getCurrentEffect()
{
    // OpenRazer doesn't expose get*Effect when there's no effect supported.
    if (!d->hasFx()) {
        return ::openrazer::Effect::Off;
    }

    // Devices with On/Off effects need special handling, except for when
    // openrazer already supports the "On" effect. Then we can treat it
    // standard.
    if (hasFx(::openrazer::Effect::On) &&
            !d->device->d->hasCapabilityInternal(d->interface, "set" + d->lightingLocationMethod + "On")) {
        QDBusReply<bool> reply;
        if (d->isProfileLed())
            reply = d->ledIface()->call("get" + d->lightingLocationMethod);
        else
            reply = d->ledIface()->call("get" + d->lightingLocationMethod + "Active");
        bool on = handleDBusReply(reply, Q_FUNC_INFO);
        return on ? ::openrazer::Effect::On : ::openrazer::Effect::Off;
    }

    QDBusReply<QString> reply = d->ledIface()->call("get" + d->lightingLocationMethod + "Effect");
    QString effect = handleDBusReply(reply, Q_FUNC_INFO);
    // TODO:
    // * breathTriple
    // * starlightSingle
    // * starlightDual
    // * starlightRandom
    if (effect == "none") {
        return ::openrazer::Effect::Off;
    } else if (effect == "on") {
        return ::openrazer::Effect::On;
    } else if (effect == "static") {
        return ::openrazer::Effect::Static;
    } else if (effect == "breathSingle" || effect == "pulsate") {
        return ::openrazer::Effect::Breathing;
    } else if (effect == "breathDual") {
        return ::openrazer::Effect::BreathingDual;
    } else if (effect == "breathRandom") {
        return ::openrazer::Effect::BreathingRandom;
    } else if (effect == "breathMono") {
        return ::openrazer::Effect::BreathingMono;
    } else if (effect == "blinking") {
        return ::openrazer::Effect::Blinking;
    } else if (effect == "spectrum") {
        return ::openrazer::Effect::Spectrum;
    } else if (effect == "wave") {
        return ::openrazer::Effect::Wave;
    } else if (effect == "wheel") {
        return ::openrazer::Effect::Wheel;
    } else if (effect == "reactive") {
        return ::openrazer::Effect::Reactive;
    } else if (effect == "ripple") {
        return ::openrazer::Effect::Ripple;
    } else if (effect == "rippleRandomColour") {
        return ::openrazer::Effect::RippleRandom;
    } else {
        qWarning("libopenrazer: Unhandled effect in getCurrentEffect: %s, defaulting to Spectrum", qUtf8Printable(effect));
        return ::openrazer::Effect::Spectrum;
    }
}

QVector<::openrazer::RGB> Led::getCurrentColors()
{
    // OpenRazer doesn't expose get*EffectColors when there's no effect supported.
    // Also profile LEDs don't support any color
    if (!d->hasFx() || d->isProfileLed()) {
        return {};
    }

    QDBusReply<QByteArray> reply = d->ledIface()->call("get" + d->lightingLocationMethod + "EffectColors");
    QByteArray values = handleDBusReply(reply, Q_FUNC_INFO);
    if (values.size() % 3 != 0) {
        throw DBusException("Invalid return array from EffectColors", "The EffectColors return array has an invalid size.");
    }
    QVector<::openrazer::RGB> colors;
    for (int i = 0; i < values.size() / 3; i++) {
        colors.append({ static_cast<uchar>(values[i * 3]),
                        static_cast<uchar>(values[i * 3 + 1]),
                        static_cast<uchar>(values[i * 3 + 2]) });
    }
    return colors;
}

::openrazer::WaveDirection Led::getWaveDirection()
{
    // OpenRazer doesn't expose get*WaveDir when there's no effect supported.
    // Also profile LEDs don't support any wave direction
    if (!d->hasFx() || d->isProfileLed()) {
        return ::openrazer::WaveDirection::LEFT_TO_RIGHT;
    }

    QDBusReply<int> reply = d->ledIface()->call("get" + d->lightingLocationMethod + "WaveDir");
    int value = handleDBusReply(reply, Q_FUNC_INFO);
    return static_cast<::openrazer::WaveDirection>(value);
}

::openrazer::LedId Led::getLedId()
{
    return d->ledId;
}

void Led::setOff()
{
    QDBusReply<void> reply;
    if (d->isProfileLed())
        reply = d->ledIface()->call("set" + d->lightingLocationMethod, false);
    else if (d->device->d->hasCapabilityInternal(d->interface, "set" + d->lightingLocationMethod + "Active"))
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Active", false);
    else
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "None");
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setOn()
{
    QDBusReply<void> reply;
    if (d->isProfileLed())
        reply = d->ledIface()->call("set" + d->lightingLocationMethod, true);
    else if (d->device->d->hasCapabilityInternal(d->interface, "set" + d->lightingLocationMethod + "Active"))
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Active", true);
    else
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "On");
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setStatic(::openrazer::RGB color)
{
    QDBusReply<void> reply;
    if (d->device->d->hasCapabilityInternal("razer.device.lighting.bw2013", "setStatic"))
        reply = d->ledBw2013Iface()->call("setStatic");
    else
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Static", RGB_TO_QVARIANT(color));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathing(::openrazer::RGB color)
{
    QDBusReply<void> reply;
    if (d->device->d->hasCapabilityInternal("razer.device.lighting.bw2013", "setPulsate"))
        reply = d->ledBw2013Iface()->call("setPulsate");
    else
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "BreathSingle", RGB_TO_QVARIANT(color));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2)
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "BreathDual", RGB_TO_QVARIANT(color), RGB_TO_QVARIANT(color2));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathingRandom()
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "BreathRandom");
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathingMono()
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "BreathMono");
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBlinking(::openrazer::RGB color)
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Blinking", RGB_TO_QVARIANT(color));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setSpectrum()
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Spectrum");
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setWave(::openrazer::WaveDirection direction)
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Wave", static_cast<int>(direction));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setWheel(::openrazer::WheelDirection direction)
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Wheel", static_cast<int>(direction));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed)
{
    QDBusReply<void> reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Reactive", RGB_TO_QVARIANT(color), static_cast<uchar>(speed));
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setRipple(::openrazer::RGB color)
{
    QDBusReply<void> reply = d->ledCustomIface()->call("setRipple", RGB_TO_QVARIANT(color), 0.05);
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setRippleRandom()
{
    QDBusReply<void> reply = d->ledCustomIface()->call("setRippleRandomColour", 0.05);
    handleDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBrightness(uchar brightness)
{
    double dbusBrightness = (double)brightness / 255 * 100;
    QDBusReply<void> reply;
    if (d->lightingLocation == "Chroma")
        reply = d->ledBrightnessIface()->call("setBrightness", QVariant::fromValue(dbusBrightness));
    else
        reply = d->ledIface()->call("set" + d->lightingLocationMethod + "Brightness", QVariant::fromValue(dbusBrightness));
    handleDBusReply(reply, Q_FUNC_INFO);
}

uchar Led::getBrightness()
{
    QDBusReply<double> reply;
    if (d->lightingLocation == "Chroma")
        reply = d->ledBrightnessIface()->call("getBrightness");
    else
        reply = d->ledIface()->call("get" + d->lightingLocationMethod + "Brightness");

    double value = handleDBusReply(reply, Q_FUNC_INFO);
    return value / 100 * 255;
}

bool LedPrivate::hasFx()
{
    return !supportedFx.isEmpty();
}

bool LedPrivate::isProfileLed()
{
    return ledId == ::openrazer::LedId::KeymapRedLED
            || ledId == ::openrazer::LedId::KeymapGreenLED
            || ledId == ::openrazer::LedId::KeymapBlueLED;
}

QDBusInterface *LedPrivate::ledIface()
{
    if (iface == nullptr) {
        iface = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), interface,
                                   OPENRAZER_DBUS_BUS, mParent);
    }
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return iface;
}

QDBusInterface *LedPrivate::ledBrightnessIface()
{
    if (ifaceBrightness == nullptr) {
        ifaceBrightness = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.brightness",
                                             OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceBrightness->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceBrightness;
}

QDBusInterface *LedPrivate::ledBw2013Iface()
{
    if (ifaceBw2013 == nullptr) {
        ifaceBw2013 = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.bw2013",
                                         OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceBw2013->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceBw2013;
}

QDBusInterface *LedPrivate::ledCustomIface()
{
    if (ifaceCustom == nullptr) {
        ifaceCustom = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "razer.device.lighting.custom",
                                         OPENRAZER_DBUS_BUS, mParent);
    }
    if (!ifaceCustom->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return ifaceCustom;
}

}

}
