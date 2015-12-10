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
 *  "decompress.c": Self extracting bzip2 archive.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "bzlib.h"

void error_exit (void)
{
    fprintf (stderr, "An error happened while unarchiving contents.\n");
    exit (1);
}

int main (int argc, char **argv)
{
    FILE *self_file;
    FILE *archive_file;
    FILE *new_file;
    BZFILE *bz2_file;
    char buf [2048];
    char altbuf [2048] = "";
    char *ptr;
    int n, x;

    strcpy (buf, argv[0]);
    #ifdef _WIN32    
    if (strstr (argv[0], ".exe") == NULL)
      {
        strcat (buf, ".exe");
      }
    #endif /* _WIN32 */
    if ((self_file = fopen (buf, "rb")) == NULL)
      {
        error_exit ();
      }
    n = 7170;
    while (n > 0)
      {
        if ((x = fread (buf, 1, (n > 2048 ? 2048 : n), self_file)) == 0)
          {
            break;
          }
        n -= x;
      }
    if ((archive_file = fopen ("_archive.bz2", "wb")) == NULL)
      {
        error_exit ();
      }
    while ((n = fread (buf, 1, 2048, self_file)) > 0)
      {
        fwrite (buf, 1, n, archive_file);
      }
    fclose (self_file);
    fclose (archive_file);
    if ((bz2_file = BZ2_bzopen ("_archive.bz2", "r")) == NULL)
      {
        #ifdef _WIN32
        system ("del _archive.bz2");
        #else  /* not _WIN32 */
        system ("rm _archive.bz2");
        #endif /* not _WIN32 */
        error_exit ();
      }
    if ((archive_file = fopen ("_archive", "wb")) == NULL)
      {
        #ifdef _WIN32
        system ("del _archive.bz2");
        #else  /* not _WIN32 */
        system ("rm _archive.bz2");
        #endif /* not _WIN32 */
        error_exit ();
      }
    while ((n = BZ2_bzread (bz2_file, buf, 2048)) > 0)
      {
        fwrite (buf, 1, n, archive_file);
      }
    BZ2_bzclose (bz2_file);
    fclose (archive_file);
    #ifdef _WIN32
    system ("del _archive.bz2");
    #else  /* not _WIN32 */
    system ("rm _archive.bz2");
    #endif /* not _WIN32 */

    if ((archive_file = fopen ("_archive", "rb")) == NULL)
      {
        #ifdef _WIN32
        system ("del _archive");
        #else  /* not _WIN32 */
        system ("rm _archive");
        #endif /* not _WIN32 */
        error_exit ();
      }
    while (fread (&n, 1, 4, archive_file) == 4)
      {
        if (fread (buf, 1, n, archive_file) != n)
          {
            #ifdef _WIN32
            system ("del _archive");
            #else  /* not _WIN32 */
            // system ("rm _archive");
            #endif /* not _WIN32 */
            error_exit ();
          }
        buf[n] = '\0';
        #ifdef _WIN32
        if ((ptr = strrchr (buf, '\\')) != NULL)
          {
        #else /* not _WIN32 */
        if ((ptr = strrchr (buf, '/')) != NULL)
          {
        #endif /* not _WIN32 */
            ptr[0] = '\0';
            if (altbuf[0] == '\0' || strcmp (buf, altbuf) != 0)
              {
                #ifdef _WIN32
                strcpy (altbuf, "mkdir ");
                #else /* not _WIN32 */
                strcpy (altbuf, "mkdir -p ");
                #endif /* not _WIN32 */
                strcat (altbuf, buf);
                system (altbuf);
                strcpy (altbuf, buf);
              }
            #ifdef _WIN32
            ptr[0] = '\\';
            #else /* not _WIN32 */
            ptr[0] = '/';
            #endif /* not _WIN32 */
          }
        if ((new_file = fopen (buf, "wb")) == NULL ||
            fread (&n, 1, 4, archive_file) != 4)
          {
            #ifdef _WIN32
            system ("del _archive");
            #else  /* not _WIN32 */
            system ("rm _archive");
            #endif /* not _WIN32 */
            error_exit ();
          }
        while ((x = fread (buf, 1, (2048 > n ? n : 2048), archive_file)) > 0)
          {
            if (fwrite (buf, 1, x, new_file) != x)
              {
                #ifdef _WIN32
                system ("del _archive");
                #else  /* not _WIN32 */
                system ("rm _archive");
                #endif /* not _WIN32 */
                error_exit ();
              }
            n -= x;
          }
        fclose (new_file);  
      }
    fclose (archive_file);
    #ifdef _WIN32
    system ("del _archive");
    #else  /* not _WIN32 */
    system ("rm _archive");
    #endif /* not _WIN32 */
    return 0;
}
