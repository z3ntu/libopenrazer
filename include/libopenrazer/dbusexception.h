// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
