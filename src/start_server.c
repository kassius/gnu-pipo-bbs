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
 *  "start_server.c": Begins the server's main handling loop.
 *               collection.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

#include "pipo.h"

extern Server server;

int continue_running = 1;

void
start_server (void)
{
    int server_socket;
    struct sockaddr_in server_address;
    int client_socket;
    struct sockaddr_in client_address;
    int x;
    Thread *thread_ptr;
    
    assertion (((server_socket = socket (AF_INET, SOCK_STREAM, 0)) != -1),
        TEXT_SOCKET_FAILED);
#ifndef _WIN32        
    x = 1;
    assertion (((setsockopt (server_socket, SOL_SOCKET, SO_REUSEADDR, &x,
        sizeof(int))) != -1), TEXT_SETSOCKOPT_FAILED);
#endif /* not _WIN32 */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons (server.port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    memset (&(server_address.sin_zero), '\0', 8);
    assertion ((bind (server_socket, (struct sockaddr *) &server_address,
        sizeof (struct sockaddr)) != -1), TEXT_BIND_FAILED);
    assertion ((listen (server_socket, 10) != -1), TEXT_LISTEN_FAILED);
    
    while (continue_running)
      {
        x = sizeof (struct sockaddr_in);
        if ((client_socket = accept (server_socket, (struct sockaddr *)
            &client_address, &x)) == -1)
          {
            fprintf (stderr, TEXT_ACCEPT_FAILED);
            continue;
          }
        assertion (((thread_ptr = (Thread *) calloc (1,
            sizeof (Thread))) != NULL), TEXT_OUT_OF_MEMORY);
        thread_ptr->socket = client_socket;
        memcpy ((void *) &(thread_ptr->address), (void *) &client_address,
            sizeof (struct sockaddr));
        pthread_create (&(thread_ptr->thread), NULL,
            (void * (*) (void *)) start_thread, thread_ptr);
        pthread_detach (thread_ptr->thread);
      }
    return;
}
