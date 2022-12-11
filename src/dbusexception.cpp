// Copyright (C) 2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer/dbusexception.h"

namespace libopenrazer {

DBusException::DBusException(const QDBusError &error)
    : m_name(error.name()), m_message(error.message())
{
}

DBusException::DBusException(const QString &name, const QString &message)
    : m_name(name), m_message(message)
{
}

void DBusException::raise() const
{
    throw *this;
}

DBusException *DBusException::clone() const
{
    return new DBusException(*this);
}

const char *DBusException::what() const noexcept
{
    return QString("%1 - %2").arg(m_name, m_message).toLatin1().data();
}

QString DBusException::name() const
{
    return m_name;
}

QString DBusException::message() const
{
    return m_message;
}

}
