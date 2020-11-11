/*
 * Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
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

#ifndef DBUSEXCEPTION_H
#define DBUSEXCEPTION_H

#include <QDBusError>
#include <QException>

namespace libopenrazer {

/*!
 * \brief Custom exception containing a name and message
 *
 * This provides an exception which will be thrown by libopenrazer in case of errors.
 *
 * You can get information about this exception using the getName() and getMessage() methods.
 */
class DBusException : public QException
{
public:
    /// @cond
    DBusException(const QDBusError &error);
    DBusException(const QString &name, const QString &message);

    void raise() const override;
    DBusException *clone() const override;
    const char *what() const noexcept override;
    /// @endcond

    /*!
     * Returns the "title" of this exception
     */
    QString name() const;

    /*!
     * Returns a detailed message from this exception.
     */
    QString message() const;

private:
    QString m_name;
    QString m_message;
};

}

#endif // DBUSEXCEPTION_H
