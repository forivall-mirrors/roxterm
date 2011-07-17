#ifndef URI_H
#define URI_H
/*
    roxterm - GTK+ 2.0 terminal emulator with tabs
    Copyright (C) 2004 Tony Houghton <h@realh.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/* URI handling */

#ifndef DEFNS_H
#include "defns.h"
#endif

/* Returns a command for launching a URL in the given browser, which may
 * contain '%s for URL substitution, otherwise the URL is appended. If browser
 * is null, it tries to find one. Result should be freed after use. */
char *uri_get_browser_command(const char *url, const char *browser);

char *uri_get_mailer_command(const char *address, const char *mailer);

char *uri_get_filer_command(const char *address, const char *filer);

#endif /* URI_H */

/* vi:set sw=4 ts=4 noet cindent cino= */
