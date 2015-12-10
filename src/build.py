#######################################################################
#                                                                     #
#      PPPP  III PPPP   OOO      BBBB  BBBB   SSSS          222       #
#      p   P  I  P   P O   O     B   B B   B S             2   2      #
#      P   P  I  P   P O   O --- BBBB  BBBB   SSS              2      #
#      PPPP   I  PPP   O   O     B   B B   B     S   v   v   22       #
#      P      I  P     O   O     B   B B   B     S    v v   2         #
#      P     III P      OOO      BBBB  BBBB  SSSS      v   22222      #
#                                                                     #
#             A reimplementation of the original Pipo-BBS             #
#######################################################################
#
# Copyright (C) 2004 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the
#      Free Software Foundation, Inc.,
#      59 Temple Place, Suite 330, Boston, MA
#      02111-1307  USA
#
#----------------------------------------------------------------------
#  "build.py": Script compile pipo-bbs executable and Emu.class.
#
#  History (Author, Date, Change):
#   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
#     Initial source written. 
#----------------------------------------------------------------------

import os, os.path
import sys

version = '2.0-alpha-1'
debug = False

if os.name == 'nt':
    if os.path.exists('include/text_constants.h'):
        os.system('del include\\text_constants.h')
    if os.path.exists('pipo-bbs.exe'):
        os.system('del pipo-bbs.exe')
    if os.path.exists('emu.class'):
        os.system('del emu.class')
else:
    if os.path.exists('include/text_constants.h'):
        os.system('rm include/text_constants.h')
    if os.path.exists('pipo-bbs'):
        os.system('rm pipo-bbs')
    if os.path.exists('emu.class'):
        os.system('rm emu.class')

if len(sys.argv) > 1:
    if os.path.exists('include/text_constants_' + sys.argv[1].lower() + '.h'):
        if os.name == 'nt':
            os.system('copy include\\text_constants_' +
                sys.argv[1].lower() + '.h include\\text_constants.h > __x')
            os.system('del __x')
        else:
            os.system('cp include/text_constants_' +
                sys.argv[1].lower() + '.h include/text_constants.h')
else:
    if os.name == 'nt':
        os.system('copy include\\text_constants_en.h ' +
            'include\\text_constants.h > __x')
        os.system('del __x')
    else:
        os.system('cp include/text_constants_en.h include/text_constants.h')

if debug == False:
    javac_command = 'javac -g:none Emu.java'
else:
    javac_command = 'javac -g Emu.java'
if os.system(javac_command) != 0:
    print 'Error when compiling applet.  Do you have "javac" installed?'
    print 'Continuing build without java.'

source_files = """
emu_applet.c
fetcher.c
initialize.c
load_settings.c
main.c
parse_arguments.c
start_server.c
thread.c
utilities.c
""".split('\n')[1:-1]

if debug == True:
    cflags = '-g'
else:
    cflags = '-s -O3'

if os.name == 'nt':
    ldflags = '-lwsock32'
else:
    ldflags = '-lpthread'

cflags = ' %s ' % (cflags)
ldflags = ' %s ' % (ldflags)
defines = ' -DVERSION="\\"%s\\"" ' % (version)

print 'Building Pipo-BBS...',
build_command = ('gcc -o Pipo-BBS -Iinclude' + defines + cflags +
    ' '.join(source_files) + ldflags)
if os.system(build_command) == 0:
    print 'Pipo-BBS built successfully.'
else:
    print 'error during build with the following command:'
    print '  ' + build_command
    if os.name == 'nt':
        os.system('del *.o')
    else:
        os.system('rm *.o')
    sys.exit(1)
        