// Copyright (C) 2018-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENRAZER_H
#define OPENRAZER_H

#include <QDBusArgument>
#include <QDBusMetaType>
#include <QDebug>

namespace openrazer {
Q_NAMESPACE

enum class RazerLedId : uchar {
    Unspecified = 0x00,
    ScrollWheelLED = 0x01,
    BatteryLED = 0x03,
    LogoLED = 0x04,
    BacklightLED = 0x05,
    MacroRecordingLED = 0x07,
    GameModeLED = 0x08,
    KeymapRedLED = 0x0C,
    KeymapGreenLED = 0x0D,
    KeymapBlueLED = 0x0E,
    RightSideLED = 0x10,
    LeftSideLED = 0x11,
    ChargingLED = 0x20,
    FastChargingLED = 0x21,
    FullyChargedLED = 0x22,
};
Q_ENUM_NS(RazerLedId)

// Marshall the RazerLedId data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RazerLedId &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the RazerLedId data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RazerLedId &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<RazerLedId>(a);
    return argument;
}
inline uint qHash(RazerLedId key, uint seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

enum class WaveDirection : uchar {
    LEFT_TO_RIGHT = 0x01,
    RIGHT_TO_LEFT = 0x02
};
Q_ENUM_NS(WaveDirection)

// Marshall the WaveDirection data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const WaveDirection &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the WaveDirection data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, WaveDirection &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<WaveDirection>(a);
    return argument;
}

enum class WheelDirection : uchar {
    CLOCKWISE = 0x01,
    COUNTER_CLOCKWISE = 0x02
};
Q_ENUM_NS(WheelDirection)

// Marshall the WheelDirection data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const WheelDirection &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the WheelDirection data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, WheelDirection &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<WheelDirection>(a);
    return argument;
}

enum class ReactiveSpeed : uchar {
    _500MS = 0x01,
    _1000MS = 0x02,
    _1500MS = 0x03,
    _2000MS = 0x04
};
Q_ENUM_NS(ReactiveSpeed)

// Marshall the ReactiveSpeed data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const ReactiveSpeed &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the ReactiveSpeed data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, ReactiveSpeed &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<ReactiveSpeed>(a);
    return argument;
}

enum class RazerEffect {
    Off,
    On,
    Static,
    Breathing,
    BreathingDual,
    BreathingRandom,
    BreathingMono,
    Blinking,
    Spectrum,
    Wave,
    Wheel,
    Reactive,
    Ripple,
    RippleRandom,
};
Q_ENUM_NS(RazerEffect)

// Marshall the RazerEffect data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RazerEffect &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the RazerEffect data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RazerEffect &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<RazerEffect>(a);
    return argument;
}
inline uint qHash(RazerEffect key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

struct RazerDPI {
    ushort dpi_x;
    ushort dpi_y;
};

// Marshall the RazerDPI data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RazerDPI &value)
{
    argument.beginStructure();
    argument << value.dpi_x << value.dpi_y;
    argument.endStructure();
    return argument;
}

// Retrieve the RazerDPI data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RazerDPI &value)
{
    argument.beginStructure();
    argument >> value.dpi_x >> value.dpi_y;
    argument.endStructure();
    return argument;
}

inline QDebug operator<<(QDebug dbg, const RazerDPI &value)
{
    dbg.nospace() << "RazerDPI(" << value.dpi_x << ", " << value.dpi_y << ")";
    return dbg.maybeSpace();
}

struct MatrixDimensions {
    uchar x;
    uchar y;
};

// Marshall the MatrixDimensions data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const MatrixDimensions &value)
{
    argument.beginStructure();
    argument << value.x << value.y;
    argument.endStructure();
    return argument;
}

// Retrieve the MatrixDimensions data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, MatrixDimensions &value)
{
    argument.beginStructure();
    argument >> value.x >> value.y;
    argument.endStructure();
    return argument;
}

inline QDebug operator<<(QDebug dbg, const MatrixDimensions &value)
{
    dbg.nospace() << "MatrixDimensions(" << value.x << ", " << value.y << ")";
    return dbg.maybeSpace();
}

struct RGB {
    uchar r;
    uchar g;
    uchar b;
};

// Marshall the RGB data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RGB &value)
{
    argument.beginStructure();
    argument << value.r << value.g << value.b;
    argument.endStructure();
    return argument;
}

// Retrieve the RGB data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RGB &value)
{
    argument.beginStructure();
    argument >> value.r >> value.g >> value.b;
    argument.endStructure();
    return argument;
}

inline QDebug operator<<(QDebug dbg, const RGB &value)
{
    dbg.nospace() << "RGB(" << value.r << ", " << value.g << ", " << value.b << ")";
    return dbg.maybeSpace();
}

}

Q_DECLARE_METATYPE(openrazer::RazerLedId)
Q_DECLARE_METATYPE(openrazer::WaveDirection)
Q_DECLARE_METATYPE(openrazer::WheelDirection)
Q_DECLARE_METATYPE(openrazer::ReactiveSpeed)
Q_DECLARE_METATYPE(openrazer::RazerEffect)
Q_DECLARE_METATYPE(openrazer::RazerDPI)
Q_DECLARE_METATYPE(openrazer::MatrixDimensions)
Q_DECLARE_METATYPE(openrazer::RGB)

namespace openrazer {

inline void registerMetaTypes()
{
    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    qRegisterMetaType<WheelDirection>("WheelDirection");
    qDBusRegisterMetaType<WheelDirection>();

    qRegisterMetaType<RazerDPI>("RazerDPI");
    qDBusRegisterMetaType<RazerDPI>();
    qRegisterMetaType<QVector<RazerDPI>>("QVector<RazerDPI>");
    qDBusRegisterMetaType<QVector<RazerDPI>>();

    qRegisterMetaType<ReactiveSpeed>("ReactiveSpeed");
    qDBusRegisterMetaType<ReactiveSpeed>();

    qRegisterMetaType<MatrixDimensions>("MatrixDimensions");
    qDBusRegisterMetaType<MatrixDimensions>();

    qRegisterMetaType<RGB>("RGB");
    qDBusRegisterMetaType<RGB>();
    qRegisterMetaType<QVector<RGB>>("QVector<RGB>");
    qDBusRegisterMetaType<QVector<RGB>>();

    qRegisterMetaType<RazerEffect>("RazerEffect");
    qDBusRegisterMetaType<RazerEffect>();

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();
}

}

#endif // OPENRAZER_H
