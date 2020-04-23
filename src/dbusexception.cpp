/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2019  Luca Weiss <luca@z3ntu.xyz>
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
 */

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

QString DBusException::name() const
{
    return m_name;
}

QString DBusException::message() const
{
    return m_message;
}

}
