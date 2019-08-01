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
 *
 */

#ifndef MISC_H
#define MISC_H

namespace libopenrazer {

/*!
 * The possible statuses of the OpenRazer daemon as detected by libopenrazer
 */
enum DaemonStatus {
    /** The OpenRazer daemon systemd unit is enabled i.e. the daemon gets autostarted at login. */
    Enabled,
    /** The OpenRazer daemon systemd unit is disabled i.e. the daemon does not get started at login. */
    Disabled,
    /** The daemon is not installed (systemd unit and/or daemon executable is missing). */
    NotInstalled,
    /** The daemon is installed but 'systemctl' is not available but the daemon executable exists. */
    NoSystemd,
    /** The detection mechanism didn't detect the status. */
    Unknown,
};

}

#endif // MISC_H
