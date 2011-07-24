/*
    roxterm - GTK+ 2.0 terminal emulator with tabs
    Copyright (C) 2004-2011 Tony Houghton <h@realh.co.uk>

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


#include "defns.h"

#include <ctype.h>
#include <string.h>

#include "dlg.h"
#include "uri.h"

static char *uri_find_first_listed_in_path(char const *const *programs)
{
    int i;

    for (i = 0; programs[i]; ++i)
    {
        char *prog_name = g_strdup(programs[i]);
        char *args = strchr(prog_name, ' ');
        char *program;
        
        if (args)
            *args++ = 0;
        program = g_find_program_in_path(prog_name);

        g_free(prog_name);
        if (program)
        {
            if (args)
            {
                prog_name = program;
                program = g_strdup_printf("%s %s", prog_name, args);
                g_free(prog_name);
            }
            return program;
        }
    }
    return NULL;
}

/* Tries to find a browser the user likes. May include stuff like %s after the
 * command name so bear that in mind */
static char *uri_get_preferred_browser(void)
{
    char const *browsers[] = { "x-www-browser", "firefox", "iceweasel",
        "chromium", "chrome", "opera", "gnome-www-browser", "epiphany",
        "konqueror", "mozilla", "netscape", NULL
    };
    int i;
    char *default_choices_path = NULL;
    char **choices_branches;
    char *browser = NULL;
    const char *env = g_getenv("BROWSER");

    if (env)
    {
        return g_strdup(env);
    }

    /* Try to find text-html handler in ROX options */
    env = g_getenv("CHOICESPATH");
    if (!env)
    {
        const char *home = g_get_home_dir();

        default_choices_path = g_strconcat(home, "/.config:",
            home, "/Choices:/usr/local/share/Choices:/usr/share/Choices", NULL);
        env = default_choices_path;
    }
    choices_branches = g_strsplit(env, ":", 0);
    if (default_choices_path)
        g_free(default_choices_path);
    for (i = 0; choices_branches[i]; ++i)
    {
        char *mime_handler = g_strconcat(choices_branches[i],
            "/MIME-types/text_html", NULL);

        /* Check for ROX appdir first */
        if (g_file_test(mime_handler, G_FILE_TEST_IS_DIR))
        {
            char *tmp = mime_handler;

            mime_handler = g_strconcat(mime_handler, "/AppRun", NULL);
            g_free(tmp);
        }
        if (g_file_test(mime_handler, G_FILE_TEST_IS_EXECUTABLE))
        {
            browser = mime_handler;
            break;
        }
        g_free(mime_handler);
    }
    g_strfreev(choices_branches);
    if (browser)
        return browser;

    browser = uri_find_first_listed_in_path(browsers);

    if (!browser)
    {
        dlg_warning(NULL, _("Unable to find a web browser"));
    }

    return browser;
}

char *uri_get_browser_command(const char *url, const char *browser)
{
    char *preferred_browser = NULL;
    char *cmd = NULL;

    if (!browser || !browser[0])
        browser = preferred_browser = uri_get_preferred_browser();
    if (!browser)
        return NULL;

    if (strstr(browser, "%s"))
        cmd = g_strdup_printf(browser, url);
    else
        cmd = g_strdup_printf("%s '%s'", browser, url);
    if (preferred_browser)
        g_free(preferred_browser);
    return cmd;
}

char *uri_get_mailer_command(const char *address, const char *mailer)
{
    char *preferred_mailer = NULL;
    char *cmd = NULL;
    const char *mailers[] = { "claws-mail", "thunderbird", "balsa",
        "evolution", "mutt", "pine", "elm", "mozilla", "mail", NULL
    };

    if (!mailer || !mailer[0])
        mailer = preferred_mailer = uri_find_first_listed_in_path(mailers);
    if (!mailer)
        return NULL;

    if (strstr(mailer, "%s"))
        cmd = g_strdup_printf(mailer, address);
    else
        cmd = g_strjoin(" ", mailer, address, NULL);
    if (preferred_mailer)
        g_free(preferred_mailer);
    return cmd;
}

char *uri_get_filer_command(const char *address, const char *filer)
{
    char *preferred_filer = NULL;
    char *cmd = NULL;
    const char *filers[] = { "rox", "thunar", "nautilus -n --no-desktop",
        "dolphin", "konqueror", NULL
    };

    if (!filer || !filer[0])
        filer = preferred_filer = uri_find_first_listed_in_path(filers);
    if (!filer)
        return NULL;

    if (strstr(filer, "%s"))
        cmd = g_strdup_printf(filer, address);
    else
        cmd = g_strjoin(" ", filer, address, NULL);
    if (preferred_filer)
        g_free(preferred_filer);
    return cmd;
}

/* vi:set sw=4 ts=4 noet cindent cino= */
