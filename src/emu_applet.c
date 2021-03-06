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
 *  "emu_applet.c":  Contains constants for vending the emu applet.
 *                   In particular, contains the applet bytecode.
 *
 *  History (Author, Date, Change):
 *   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
 *     Initial source written. 
 *---------------------------------------------------------------------
 */

char *HTTP_HEADER =
    "HTTP/1.1 200 OK\r\n"
    "Server: Pipo-BBS " VERSION "\r\n"
    "Ducksoup: \e[2J\e[0m;\e[H";

char *EMU_HTML =
    "Connection: close\r\n"
    "Content-Type: text/html\r\n\r\n"
    "<html><head><title>Pipo-BBS</title></head><body><center>"
    "<applet code=\"Emu.class\" width=\"640\" height=\"480\">"
    "</center></body></html>\r\n";

char *EMU_CLASS_HTTP =
    "Connection: close\r\n"
    "Content-type: application/java-vm\r\n"
    "Accept-ranges: bytes\r\n\r\n";
