/*
 * Copyright (C) 2016-2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
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

bool Led::hasFx(::openrazer::RazerEffect fx)
{
    QString fxStr;
    switch (fx) {
    case ::openrazer::RazerEffect::Off:
        fxStr = "off";
        break;
    case ::openrazer::RazerEffect::On:
        fxStr = "on";
        break;
    case ::openrazer::RazerEffect::Static:
        fxStr = "static";
        break;
    case ::openrazer::RazerEffect::Breathing:
        fxStr = "breathing";
        break;
    case ::openrazer::RazerEffect::BreathingDual:
        fxStr = "breathing_dual";
        break;
    case ::openrazer::RazerEffect::BreathingRandom:
        fxStr = "breathing_random";
        break;
    case ::openrazer::RazerEffect::Blinking:
        fxStr = "blinking";
        break;
    case ::openrazer::RazerEffect::Spectrum:
        fxStr = "spectrum";
        break;
    case ::openrazer::RazerEffect::Wave:
        fxStr = "wave";
        break;
    case ::openrazer::RazerEffect::Reactive:
        fxStr = "reactive";
        break;
    case ::openrazer::RazerEffect::Ripple:
    case ::openrazer::RazerEffect::RippleRandom:
        // TODO Needs implementation
        return false;
    }
    return d->hasFx(fxStr);
}

::openrazer::RazerEffect Led::getCurrentEffect()
{
    QVariant reply = d->ledIface()->property("CurrentEffect");
    return handleDBusVariant<::openrazer::RazerEffect>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

QVector<::openrazer::RGB> Led::getCurrentColors()
{
    QVariant reply = d->ledIface()->property("CurrentColors");
    return handleDBusVariant<QVector<::openrazer::RGB>>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

::openrazer::RazerLedId Led::getLedId()
{
    QVariant reply = d->ledIface()->property("LedId");
    return handleDBusVariant<::openrazer::RazerLedId>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

bool Led::setOff()
{
    QDBusReply<bool> reply = d->ledIface()->call("setOff");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setOn()
{
    QDBusReply<bool> reply = d->ledIface()->call("setOn");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setStatic(::openrazer::RGB color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setStatic", QVariant::fromValue(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBreathing(::openrazer::RGB color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathing", QVariant::fromValue(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBreathingDual(::openrazer::RGB color, ::openrazer::RGB color2)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathingDual", QVariant::fromValue(color), QVariant::fromValue(color2));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBreathingRandom()
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathingRandom");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBlinking(::openrazer::RGB color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBlinking", QVariant::fromValue(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setSpectrum()
{
    QDBusReply<bool> reply = d->ledIface()->call("setSpectrum");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setWave(::openrazer::WaveDirection direction)
{
    QDBusReply<bool> reply = d->ledIface()->call("setWave", QVariant::fromValue(direction));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setReactive(::openrazer::RGB color, ::openrazer::ReactiveSpeed speed)
{
    QDBusReply<bool> reply = d->ledIface()->call("setReactive", QVariant::fromValue(speed), QVariant::fromValue(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setRipple(::openrazer::RGB color)
{
    // TODO Needs implementation
    return false;
}

bool Led::setRippleRandom()
{
    // TODO Needs implementation
    return false;
}

bool Led::setBrightness(uchar brightness)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBrightness", QVariant::fromValue(brightness));
    return handleDBusReply(reply, Q_FUNC_INFO);
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
