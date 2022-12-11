// Copyright (C) 2016-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
