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
 *  "fetcher.c": Loads and caches files.  Future automatic garbage
 *               collection.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

#include "pipo.h"

extern Server server;

struct filedata_link {
    int hash;
    int count;
    char *filedata;
    int size;
    struct filedata_link *next;
    struct filedata_link *prev;
};
typedef struct filedata_link Filedata_Link;

Filedata_Link *filedata_linked_list;
int filedata_linked_list_size;

int FETCHER_LOCK = 0;

char *
fetch_filedata (const char *filename, int *size)
{
    Filedata_Link *link = NULL;
    Filedata_Link *old_list;
    int new_hash;
    int i, x;
    FILE *afile;
    struct stat statbuf;
    char filepath [1024];

    while (FETCHER_LOCK != 0)
      for (i = rand () % 7170; i > 0; i--)
        i = i * i / i;
    FETCHER_LOCK++;
    
    if (strlen (filename) < 1)
      {
        FETCHER_LOCK--;
        return NULL;
      }
    else
      new_hash = hash (filename);    
    
    if (filedata_linked_list == NULL)
      {
        assertion (((filedata_linked_list = (Filedata_Link *) malloc
            (sizeof (Filedata_Link) * 64)) != NULL), TEXT_OUT_OF_MEMORY);
        for (i = 0; i < 64; i++)
          filedata_linked_list[i].filedata = NULL;
        filedata_linked_list_size = 64;
        link = &(filedata_linked_list[0]);
        link->hash = new_hash;
        link->count = 0;
        link->next = NULL;
        link->prev = NULL;
      }
    else
      {
        link = &(filedata_linked_list[0]);
        do
          {
            if (link->hash == new_hash)
              {
                link->count++;
                *size = link->size;
                FETCHER_LOCK--;
                return link->filedata;
              }
          }
        while (link->next != NULL && (link = link->next));
        for (i = 0; i < filedata_linked_list_size; i++)
          {
            if (filedata_linked_list[i].filedata == NULL)
              {
                i = -i;
                break;
              }
          }
        if (i >= 0)
          {
            old_list = filedata_linked_list;
            filedata_linked_list_size += 64;
            assertion (((filedata_linked_list = (Filedata_Link *) malloc
                (sizeof (Filedata_Link) * filedata_linked_list_size)) !=
                NULL), TEXT_OUT_OF_MEMORY);
            for (i = filedata_linked_list_size - 64;
                i < filedata_linked_list_size; i++)
              filedata_linked_list[i].filedata = NULL;
            memcpy (filedata_linked_list, old_list, sizeof (Filedata_Link) *
                filedata_linked_list_size - 64);
            free (old_list);
            i = filedata_linked_list_size - 64;
          }
        else
          i = -i;
        link->next = &(filedata_linked_list[i]);
        filedata_linked_list[i].prev = link;
        link = &(filedata_linked_list[i]);
        link->next = NULL;
        link->hash = new_hash;
        link->count = 0;
      }
    strcpy (filepath, server.data_path);
#ifdef _WIN32
    strcat (filepath, "\\");
#else /* not _WIN32 */
    strcat (filepath, "/");
#endif /* not _WIN32 */
    strcat (filepath, filename);
    x = strlen (filepath);
    for (i = strlen (server.data_path); i < x; i++)
#ifdef _WIN32        
      if (filepath[i] == '/')
        filepath[i] = '\\';
#else /* not _WIN32 */
      if (filepath[i] == '\\')
        filepath[i] = '/';
#endif /* not _WIN32 */
    if ((afile = fopen (filepath, "rb")) == NULL ||
        fstat (fileno (afile), &statbuf) != 0)
      {
        FETCHER_LOCK--;
        return NULL;
      }
    assertion ((link->filedata = (char *) malloc (statbuf.st_size)) != NULL,
        TEXT_OUT_OF_MEMORY);
    i = 0;
    while (i < statbuf.st_size)
      {
        if ((x = fread
            (&(link->filedata[0]), 1, statbuf.st_size - i, afile)) < 1)
          {
            free (link->filedata);
            link->filedata = NULL;
            FETCHER_LOCK--;
            return NULL;
          }
        else
          i += x;
      }
    fclose (afile);
    link->count++;
    link->size = statbuf.st_size;
    *size = link->size;
    FETCHER_LOCK--;
    return link->filedata;
}

void
release_filedata (char *filedata)
{
    Filedata_Link *link;
    int i;

    while (FETCHER_LOCK != 0)
      for (i = rand () % 7170; i > 0; i--)
        i = i * i / i;
    FETCHER_LOCK++;
    
    if (filedata_linked_list == NULL || filedata == NULL)
      {
        FETCHER_LOCK--;
        return;
      }
    link = &(filedata_linked_list[0]);
    while (link != NULL)
      {
        if (link->filedata == filedata)
          {
            link->count--;
            if (link->count <= 0)
              {
                /* add freed_time member to Filedata_Link and delay free? */
                free (link->filedata);
                if (link->prev != NULL)
                  link->prev->next = link->next;
                if (link->next != NULL)
                  link->next->prev = link->prev;
                link->hash = -1;
                link->count = 0;
                FETCHER_LOCK--;
                return;
              }
          }
        link = link->next;
      }
    FETCHER_LOCK--;
    return;
}

int
hash (const char *filename)
{
    int i, x;
    
    x = filename[0] << 7;
    for (i = 1; filename[i] != '\0'; i++)
      {
        x = 1000003 * x ^ filename[i];
      }
    x ^= i;
    if (x == -1)
      return -2;
    else
      return x;
}
