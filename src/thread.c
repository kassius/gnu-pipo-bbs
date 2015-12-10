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
 *  "thread.c": Thread functions for handling client requests.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

#include "pipo.h"

extern char *HTTP_HEADER;
extern char *EMU_HTML;
extern char *EMU_CLASS_HTTP;
extern char *EMU_CLASS_DATA;
extern int EMU_CLASS_DATA_SIZE;

int number_of_running_threads = 0;

void client_login (Thread *thread);

int test () { puts("here"); return 1;}

void *
start_thread (Thread *thread)
{
    int x, y, z;
    char *filedata;
    
    number_of_running_threads++;

    assertion (((thread->iobuf = (char *) malloc (sizeof (char) *
        IOBUF_SIZE)) != NULL), TEXT_OUT_OF_MEMORY);
    
    if (send (thread->socket, HTTP_HEADER, strlen (HTTP_HEADER), 0) !=
        strlen (HTTP_HEADER))
      {
        closesocket (thread->socket);
        number_of_running_threads--;
        pthread_exit (NULL);
      }
    
    if ((filedata = (char *) fetch_filedata ("login", &x)) != NULL)
      {
        z = 0;
        while ((y = send (thread->socket, &(filedata[z]), x - z, 0)) > 0)
          z += y;
        if (y < 0)
          {
            closesocket (thread->socket);
            number_of_running_threads--;
            pthread_exit (NULL);
          }
        release_filedata (filedata);
      }
    recv (thread->socket, thread->iobuf, IOBUF_SIZE, 0);
    if (strncmp (thread->iobuf, "GET /Emu.class", 14) == 0)
      {
        if (send (thread->socket, EMU_CLASS_HTTP,
            strlen (EMU_CLASS_HTTP), 0) != strlen (EMU_CLASS_HTTP))
          {
            closesocket (thread->socket);
            number_of_running_threads--;
            pthread_exit (NULL);
          }
        if ((filedata = (char *) fetch_filedata ("Emu.class", &x)) != NULL)
          {
            z = 0;
            while ((y = send (thread->socket, &(filedata[z]), x - z, 0)) > 0)
              z += y;
            if (y < 0)
              {
                closesocket (thread->socket);
                number_of_running_threads--;
                pthread_exit (NULL);
              }
            release_filedata (filedata);
          }
      }
    else if (strncmp (thread->iobuf, "GET /", 5) == 0)
      send (thread->socket, EMU_HTML, strlen (EMU_HTML), 0);
    else
      client_login (thread);
    closesocket (thread->socket);
    number_of_running_threads--;
    pthread_exit (NULL);
}

void
client_login (Thread *thread)
{
    READ_STRING ();
    /* etc... */
}
