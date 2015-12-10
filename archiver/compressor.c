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
 *  "compressor.c": Very simple utility for bzipping a single file.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "bzlib.h"

char *usage = "compressor FILE_TO_BE_PACKED OUTPUT_NAME\n";

int
main (int argc, char **argv)
{
    BZFILE *bz2_file;
    FILE *archive_file;
    char buf [2048];
    int n;
    
    if (argc != 3)
      {
        fprintf (stderr, usage);
        exit (1);
      }
    if ((archive_file = fopen (argv[1], "rb")) == NULL)
      {
        fprintf (stderr, "Error opening file to be packed, '%s'.\n", argv[2]);
        exit (1);
      }
    if ((bz2_file = BZ2_bzopen (argv[2], "w")) == NULL)
      {
        fprintf (stderr, "Error opening output file, '%s'.\n", argv[1]);
        exit (1);
      }
    while ((n = fread (buf, 1, 2048, archive_file)) > 0)
      {
        BZ2_bzwrite (bz2_file, buf, n);
      }
    fclose (archive_file);
    BZ2_bzclose (bz2_file);
    return 0;
}
