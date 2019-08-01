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

#include "led_p.h"
#include "libopenrazer.h"
#include "libopenrazer_private.h"

#include <QColor>

#define QCOLOR_TO_QVARIANT(c)                      \
    QVariant::fromValue(                           \
            razer_test::RGB {                      \
                    static_cast<uchar>(c.red()),   \
                    static_cast<uchar>(c.green()), \
                    static_cast<uchar>(c.blue()) })

namespace libopenrazer {

Led::Led(QDBusObjectPath objectPath)
{
    d = new LedPrivate();
    d->mParent = this;
    d->mObjectPath = objectPath;
}

Led::~Led() = default;

QDBusObjectPath Led::getObjectPath()
{
    return d->mObjectPath;
}

razer_test::RazerEffect Led::getCurrentEffect()
{
    QVariant reply = d->ledIface()->property("CurrentEffect");
    return handleDBusVariant<razer_test::RazerEffect>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

QList<razer_test::RGB> Led::getCurrentColors()
{
    QVariant reply = d->ledIface()->property("CurrentColors");
    return handleDBusVariant<QList<razer_test::RGB>>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

razer_test::RazerLedId Led::getLedId()
{
    QVariant reply = d->ledIface()->property("LedId");
    return handleDBusVariant<razer_test::RazerLedId>(reply, d->ledIface()->lastError(), Q_FUNC_INFO);
}

bool Led::setOff()
{
    QDBusReply<bool> reply = d->ledIface()->call("setOff");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setStatic(QColor color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setStatic", QCOLOR_TO_QVARIANT(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBreathing(QColor color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathing", QCOLOR_TO_QVARIANT(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBreathingDual(QColor color, QColor color2)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathingDual", QCOLOR_TO_QVARIANT(color), QCOLOR_TO_QVARIANT(color2));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBreathingRandom()
{
    QDBusReply<bool> reply = d->ledIface()->call("setBreathingRandom");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setBlinking(QColor color)
{
    QDBusReply<bool> reply = d->ledIface()->call("setBlinking", QCOLOR_TO_QVARIANT(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setSpectrum()
{
    QDBusReply<bool> reply = d->ledIface()->call("setSpectrum");
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setWave(razer_test::WaveDirection direction)
{
    QDBusReply<bool> reply = d->ledIface()->call("setWave", QVariant::fromValue(direction));
    return handleDBusReply(reply, Q_FUNC_INFO);
}

bool Led::setReactive(QColor color, razer_test::ReactiveSpeed speed)
{
    QDBusReply<bool> reply = d->ledIface()->call("setReactive", QVariant::fromValue(speed), QCOLOR_TO_QVARIANT(color));
    return handleDBusReply(reply, Q_FUNC_INFO);
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

QDBusInterface *LedPrivate::ledIface()
{
    if (iface == nullptr) {
        iface = new QDBusInterface(OPENRAZER_SERVICE_NAME, mObjectPath.path(), "io.github.openrazer1.Led",
                                   RAZER_TEST_DBUS_BUS, mParent);
    }
    if (!iface->isValid()) {
        fprintf(stderr, "%s\n",
                qPrintable(RAZER_TEST_DBUS_BUS.lastError().message()));
    }
    return iface;
}

}
