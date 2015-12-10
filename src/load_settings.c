/*********************************************************************\
|*                                                                   *|
|*     PPPP  III PPPP   OOO      BBBB  BBBB   SSSS          222      *|
|*     p   P  I  P   P O   O     B   B B   B S             2   2     *|
|*     P   P  I  P   P O   O --- BBBB  BBBB   SSS              2     *|
|*     PPPP   I  PPP   O   O     B   B B   B     S   v   v   22      *|
|*     P      I  P     O   O     B   B B   B     S    v v   2        *|
|*     P     III P      OOO      BBBB  BBBB  SSSS      v   22222     *|
|*                                                                   *|
|*            A reimplementation of the original Pipo-BBS            *|
|*********************************************************************/
/*
 * Copyright (C) 2004 Free Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the
 *      Free Software Foundation, Inc.,
 *      59 Temple Place, Suite 330, Boston, MA
 *      02111-1307  USA
 *
 *---------------------------------------------------------------------
 *  "load_settings.c":  Loads BBS struct settings from resource files.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

#include "pipo.h"

Server server;

int DEFAULT_PORT = 7170;
char *DEFAULT_BBS_NAME = "Your BBS";
char *DEFAULT_DATA_PATH = "data";
char *DEFAULT_COUNTRY_CODE = "en";

void parse_setting (char *setting);
int get_number (char *setting);
char * get_string (char *setting);

void
load_settings (void)
{
    FILE *config_file;
    char config_buf [2048];
    int n, x;
    
    server.port = DEFAULT_PORT;
    server.bbs_name = DEFAULT_BBS_NAME;
    server.data_path = DEFAULT_DATA_PATH;
    server.country_code = DEFAULT_COUNTRY_CODE;
    
    assertion (((config_file = fopen ("config.bbs", "r")) != NULL),
        TEXT_COULD_NOT_OPEN_CONFIG);
    while (fgets (config_buf, 2048, config_file) != NULL)
      {
        parse_setting (config_buf);
      }
    if (strlen (server.data_path) > 0)
      {
        strcpy (config_buf, "cd ");
        strcat (config_buf, server.data_path);
        assertion ((system (config_buf) == 0), TEXT_DATA_PATH_ERROR);
      }
    fclose (config_file);
    /* preload these files */
    fetch_filedata ("Emu.class", &x);
    fetch_filedata ("login", &x);
}

void
parse_setting (char *setting)
{
    int i, n;
    
    n = strlen (setting);
    if (n < 1 ||
        setting[0] == '\r' ||
        setting[0] == '\n' ||
        setting[0] == '#')
      {
        return;
      }
    for (i = 0; i < n; i++)
      {
        if (setting[i] > 'z' ||
            setting[i] < 'A' ||
            (setting[i] > 'Z' && setting['i'] < 'a'))
          {
            if (setting[i] != '_')
              break;
          }
        if (setting[i] >= 'a')
          setting[i] -= 32;
      }
    if (strncmp (setting, "PORT", 4) == 0)
      {
        n = get_number(&(setting[4]));
        if (n > 0 && n < 65536)
          server.port = n;
      }
    else if (strncmp (setting, "BBS_NAME", 8) == 0)
      {
        if ((setting = get_string (&(setting[8]))) != NULL)
          {
            server.bbs_name = (char *) malloc (strlen (setting) + 1);
            strcpy (server.bbs_name, setting);
          }
      }
    else if (strncmp (setting, "DATA_PATH", 9) == 0)
      {
        if ((setting = get_string (&(setting[9]))) != NULL)
          {
            server.data_path = (char *) malloc (strlen (setting) + 1);
            strcpy (server.data_path, setting);
          }
      }
    else if (strncmp (setting, "COUNTRY_CODE", 12) == 0)
      {
        if ((setting = get_string (&(setting[12]))) != NULL)
          {
            server.country_code = (char *) malloc (strlen (setting) + 1);
            strcpy (server.country_code, setting);
          }
      }
    return;
}

int
get_number (char *setting)
{
    int n;
    
    if (setting == NULL)
      return 0;
    
    while (setting[0] != '\0' &&
        (setting[0] == ' ' ||
        setting[0] == '=' ||
        setting[0] >= '0' ||
        setting[0] <= '9'))
      {
        if (setting[0] >= '0' && setting[0] <= '9')
          if (sscanf (setting, "%d", &n) != 1)
            return 0;
          else
            return n;
        setting++;
      }
    return 0;
}

char *
get_string (char *setting)
{
    int n;
    
    if (setting == NULL)
      return NULL;

    while (setting[0] != '\0' &&
        (setting[0] == ' ' ||
        setting[0] == '=' ||
        setting[0] == '"'))
      {
        if (setting[0] == '"')
          {
            setting++;
            if (((void *) (n = (int) strchr (setting, '"'))) == NULL)
              return NULL;
            setting[n - (int) setting] = '\0';
            return setting;
          }
        setting++;
      }
    return NULL;
}
