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
#  "release.py": Script to create the released self extracting archive
#                of Pipo-BBS and its data files.
#
#  History (Author, Date, Change):
#   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
#     Initial source written. 
#----------------------------------------------------------------------

import os
import sys

os.chdir('src')
if not os.system('python build.py') == 0:
    sys.exit(1)
os.chdir('..')
try:
    os.mkdir('release')
    os.mkdir('release/Pipo-BBS')
except:
    pass
if os.name == 'nt':
    os.system('xcopy /e /s /y doc release\\Pipo-BBS\\doc\\')
    os.system('xcopy /e /s /y data release\\Pipo-BBS\\data\\')
    os.system('xcopy /e /s /y doors release\\Pipo-BBS\\doors\\')
    os.system('copy /y src\\config.bbs release\\Pipo-BBS\\config.bbs')
    os.system('copy /y src\\Pipo-BBS.exe release\\Pipo-BBS\\Pipo-BBS.exe')
    os.system('copy /y src\\Emu.class release\\Pipo-BBS\\data\\Emu.class')
else:
    os.system('cp -rf doc release/Pipo-BBS/doc')
    os.system('cp -rf data release/Pipo-BBS/data')
    os.system('cp -rf doors release/Pipo-BBS/doors')
    os.system('cp -f src/config.bbs release/Pipo-BBS/config.bbs')
    os.system('cp -f src/Pipo-BBS release/Pipo-BBS/Pipo-BBS')
    os.system('cp -f src/Emu.class release/Pipo-BBS/data/Emu.class')
os.chdir('archiver')
os.system('python archiver.py ../release')
os.chdir('..')
if os.name == 'nt':
    os.system('move archiver\\installer.exe .')
    print 'Pipo-BBS release ready as "installer.exe".'
else:
    os.system('mv archiver/installer .')
    print 'Pipo-BBS release ready as "installer".'
    