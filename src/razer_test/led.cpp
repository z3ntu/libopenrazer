// Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "device_p.h"
#include "led_p.h"
#include "libopenrazer.h"
#include "libopenrazer_private.h"

namespace libopenrazer {

namespace razer_test {

Led::Led(Device *device, QDBusObjectPath objectPath)
{
    d = new LedPrivate();
    d->mParent = this;
    d->device = device;
    d->mObjectPath = objectPath;
}

Led::~Led() = default;

QDBusObjectPath Led::getObjectPath()
{
    return d->mObjectPath;
}

bool Led::hasBrightness()
{
    return d->hasFx("brightness");
}

bool Led::hasFx(::openrazer::Effect fx)
{
    QString fxStr;
    switch (fx) {
    case ::openrazer::Effect::Off:
        fxStr = "off";
        break;
    case ::openrazer::Effect::On:
        fxStr = "on";
        break;
    case ::openrazer::Effect::Static:
        fxStr = "static";
        break;
    case ::openrazer::Effect::Breathing:
        fxStr = "breathing";
        break;
    case ::openrazer::Effect::BreathingDual:
        fxStr = "breathing_dual";
        break;
    case ::openrazer::Effect::BreathingRandom:
        fxStr = "breathing_random";
        break;
    case ::openrazer::Effect::Blinking:
        fxStr = "blinking";
        break;
    case ::openrazer::Effect::Spectrum:
        fxStr = "spectrum";
        break;
    case ::openrazer::Effect::Wave:
        fxStr = "wave";
        break;
    case ::openrazer::Effect::Reactive:
        fxStr = "reactive";
        break;
    case ::openrazer::Effect::Wheel:
    case ::openrazer::Effect::BreathingMono:
    case ::openrazer::Effect::Ripple:
    case ::openrazer::Effect::RippleRandom:
        // TODO Needs implementation
        return false;
    }
    return d->hasFx(fxStr);
}

::openrazer::Effect Led::getCurrentEffect()
{
    QVariant reply = d->ledIface()->property("CurrentEffect");
    return handleDBusVariant<::openrazer::Effect>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

QVector<::openrazer::RGB> Led::getCurrentColors()
{
    QVariant reply = d->ledIface()->property("CurrentColors");
    return handleDBusVariant<QVector<::openrazer::RGB>>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

::openrazer::WaveDirection Led::getWaveDirection()
{
    return ::openrazer::WaveDirection::RIGHT_TO_LEFT; // TODO Needs implementation
}

::openrazer::LedId Led::getLedId()
{
    QVariant reply = d->ledIface()->property("LedId");
    return handleDBusVariant<::openrazer::LedId>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

void Led::setOff()
{
    QDBusReply<bool> reply = d->ledIface()->call("setOff");
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setOn()
{
    QDBusReply<bool> reply = d->ledIface()->call("setOn");
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setStatic(::openrazer::RGB color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setStatic", QVariant::fromValue(color));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathing(::openrazer::RGB color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathing", QVariant::fromValue(color));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathingDual", QVariant::fromValue(color), QVariant::fromValue(color2));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathingRandom()
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathingRandom");
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setBreathingMono()
{
    // TODO Needs implementation
}

void Led::setBlinking(::openrazer::RGB color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBlinking", QVariant::fromValue(color));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setSpectrum()
{
    QDBusReply<bool> reply = d->ledIface()->call("setSpectrum");
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setWave(::openrazer::WaveDirection direction)
{
    QDBusReply<bool> reply = d->ledIface()->call("setWave", QVariant::fromValue(direction));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setWheel(::openrazer::WheelDirection direction)
{
    // TODO Needs implementation
}

void Led::setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed)
{
    QDBusReply<bool> reply = d->ledIface()->call("setReactive", QVariant::fromValue(speed), QVariant::fromValue(color));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

void Led::setRipple(::openrazer::RGB color)
{
    // TODO Needs implementation
}

void Led::setRippleRandom()
{
    // TODO Needs implementation
}

void Led::setBrightness(uchar brightness)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBrightness", QVariant::fromValue(brightness));
    handleVoidDBusReply(reply, Q_FUNC_INFO);
}

uchar Led::getBrightness()
{
    QDBusReply<uchar> reply = d->ledIface()->call("getBrightness");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool LedPrivate::hasFx(const QString &fxStr)
{
    return device->d->supportedFx.contains(fxStr);
}

QDBusInterface *LedPrivate::ledIface()
{
    if (iface == nullptr) {
        iface = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "io.github.openrazer1.Led",
                                   OPENRAZER_DBUS_BUS, mParent);
    }
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(OPENRAZER_DBUS_BUS.lastError().message()));
    }
    return iface;
}

}

}
