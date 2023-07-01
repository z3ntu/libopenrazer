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

Led::Led(Device *device, QDBusObjectPath objectPath, ::openrazer::RazerLedId ledId, QString lightingLocation)
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
        supportedFx.append(::openrazer::RazerEffect::Off);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "On"))
        supportedFx.append(::openrazer::RazerEffect::On);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Static"))
        supportedFx.append(::openrazer::RazerEffect::Static);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Blinking"))
        supportedFx.append(::openrazer::RazerEffect::Blinking);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathSingle"))
        supportedFx.append(::openrazer::RazerEffect::Breathing);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathDual"))
        supportedFx.append(::openrazer::RazerEffect::BreathingDual);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathRandom"))
        supportedFx.append(::openrazer::RazerEffect::BreathingRandom);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "BreathMono"))
        supportedFx.append(::openrazer::RazerEffect::BreathingMono);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Spectrum"))
        supportedFx.append(::openrazer::RazerEffect::Spectrum);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Wave"))
        supportedFx.append(::openrazer::RazerEffect::Wave);
    if (device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Reactive"))
        supportedFx.append(::openrazer::RazerEffect::Reactive);

    if (supportedFx.isEmpty() && device->d->hasCapabilityInternal(interface, "set" + lightingLocationMethod + "Active")) {
        supportedFx.append(::openrazer::RazerEffect::Off);
        supportedFx.append(::openrazer::RazerEffect::On);
    }

    if (device->d->hasCapabilityInternal("razer.device.lighting.profile_led", "set" + lightingLocationMethod)) {
        supportedFx.append(::openrazer::RazerEffect::Off);
        supportedFx.append(::openrazer::RazerEffect::On);
    }

    // No-color static/breathing variants
    if (device->d->hasCapabilityInternal("razer.device.lighting.bw2013", "setStatic"))
        supportedFx.append(::openrazer::RazerEffect::Static);
    if (device->d->hasCapabilityInternal("razer.device.lighting.bw2013", "setPulsate"))
        supportedFx.append(::openrazer::RazerEffect::Breathing);

    if (device->d->hasCapabilityInternal("razer.device.lighting.custom", "setRipple"))
        supportedFx.append(::openrazer::RazerEffect::Ripple);
    if (device->d->hasCapabilityInternal("razer.device.lighting.custom", "setRippleRandomColour"))
        supportedFx.append(::openrazer::RazerEffect::RippleRandom);

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

bool Led::hasFx(::openrazer::RazerEffect fx)
{
    return d->supportedFx.contains(fx);
}

::openrazer::RazerEffect Led::getCurrentEffect()
{
    // OpenRazer doesn't expose get*Effect when there's no effect supported.
    if (!d->hasFx()) {
        return ::openrazer::RazerEffect::Off;
    }

    // Devices with On/Off effects need special handling, except for when
    // openrazer already supports the "On" effect. Then we can treat it
    // standard.
    if (hasFx(::openrazer::RazerEffect::On) &&
            !d->device->d->hasCapabilityInternal(d->interface, "set" + d->lightingLocationMethod + "On")) {
        QDBusReply<bool> reply;
        if (d->isProfileLed())
            reply = d->ledIface()->call("get" + d->lightingLocationMethod);
        else
            reply = d->ledIface()->call("get" + d->lightingLocationMethod + "Active");
        bool on = handleDBusReply(reply, Q_FUNC_INFO);
        return on ? ::openrazer::RazerEffect::On : ::openrazer::RazerEffect::Off;
    }

    QDBusReply<QString> reply = d->ledIface()->call("get" + d->lightingLocationMethod + "Effect");
    QString effect = handleDBusReply(reply, Q_FUNC_INFO);
    // TODO:
    // * breathTriple
    // * starlightSingle
    // * starlightDual
    // * starlightRandom
    if (effect == "none") {
        return ::openrazer::RazerEffect::Off;
    } else if (effect == "on") {
        return ::openrazer::RazerEffect::On;
    } else if (effect == "static") {
        return ::openrazer::RazerEffect::Static;
    } else if (effect == "breathSingle" || effect == "pulsate") {
        return ::openrazer::RazerEffect::Breathing;
    } else if (effect == "breathDual") {
        return ::openrazer::RazerEffect::BreathingDual;
    } else if (effect == "breathRandom") {
        return ::openrazer::RazerEffect::BreathingRandom;
    } else if (effect == "breathMono") {
        return ::openrazer::RazerEffect::BreathingMono;
    } else if (effect == "blinking") {
        return ::openrazer::RazerEffect::Blinking;
    } else if (effect == "spectrum") {
        return ::openrazer::RazerEffect::Spectrum;
    } else if (effect == "wave") {
        return ::openrazer::RazerEffect::Wave;
    } else if (effect == "reactive") {
        return ::openrazer::RazerEffect::Reactive;
    } else if (effect == "ripple") {
        return ::openrazer::RazerEffect::Ripple;
    } else if (effect == "rippleRandomColour") {
        return ::openrazer::RazerEffect::RippleRandom;
    } else {
        qWarning("libopenrazer: Unhandled effect in getCurrentEffect: %s, defaulting to Spectrum", qUtf8Printable(effect));
        return ::openrazer::RazerEffect::Spectrum;
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

::openrazer::RazerLedId Led::getLedId()
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
    return ledId == ::openrazer::RazerLedId::KeymapRedLED
            || ledId == ::openrazer::RazerLedId::KeymapGreenLED
            || ledId == ::openrazer::RazerLedId::KeymapBlueLED;
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
