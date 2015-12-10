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
#  "archiver.py": Script to create self extracting bzip2 archive.
#
#  History (Author, Date, Change):
#   + chrootstrap (chrootstrap@yahoo.com),  2-2004,
#     Initial source written. 
#----------------------------------------------------------------------

import os, os.path
import struct
import sys

if len(sys.argv) < 2:
    print 'Usage:  python archiver.py PATH'
    sys.exit(1)

if os.name == 'nt':
    map(lambda x:os.system('del ' + x),
        filter(lambda y:os.path.exists(y), ['compressor.exe', 'installer.exe',
        'decompressor.exe', '_archive', '_archive.bz2', '_decompressor.c']))
else:
    map(lambda x:os.system('del ' + x),
        filter(lambda y:os.path.exists(y), ['compressor', 'installer'
        'decompressor', '_archive', '_archive.bz2', '_decompressor.c']))

print 'Creating compressor...'

os.system('gcc -o compressor compressor.c -Ibzip2 bzip2/blocksort.c ' +
    'bzip2/bzlib.c bzip2/compress.c bzip2/crctable.c bzip2/decompress.c ' +
    'bzip2/huffman.c bzip2/randtable.c')

print 'Creating uncompressed archive...'
cwd = os.getcwd()
os.chdir(sys.argv[1])
Contents = []
def append_file(arg, dir, filenames):
    Contents.extend(filter(lambda path: not os.path.isdir(path),
        map(lambda name: (dir + '/' + name)[2:], filenames)))
os.path.walk('.', append_file, None)
output_file = open(cwd + '/_archive', 'wb')
for input_filename in Contents:
    output_file.write(struct.pack('I', len(input_filename)))
    if os.name == 'nt':
        output_file.write(input_filename.replace('/', '\\'))
    else:
        output_file.write(input_filename.replace('\\', '/'))
    output_file.write(struct.pack('I', os.path.getsize(input_filename)))
    input_file = open(input_filename, 'rb')
    output_file.write(input_file.read())
    input_file.close()
output_file.close()
os.chdir(cwd)

print 'Compressing archive...'
if os.name == 'nt':
    os.system('compressor _archive _archive.bz2')
    os.system('del compressor.exe')
    os.system('del _archive')
else:
    os.system('./compressor _archive _archive.bz2')
    os.system('rm compressor')
    os.system('rm _archive')

print 'Creating decompressor...'
os.system('gcc -s -Os -o decompressor decompressor.c -DSKINNY -Ibzip2 ' +
    'bzip2/bzlib.c bzip2/crctable.c bzip2/decompress.c bzip2/huffman.c ' +
    'bzip2/randtable.c')
if os.name == 'nt':
    compiled_size = os.path.getsize('decompressor.exe')
else:
    compiled_size = os.path.getsize('decompressor')
input_file = open('decompressor.c', 'rb')
output_file = open('_decompressor.c', 'wb')
output_file.write(input_file.read().replace('7170', str(compiled_size)))
input_file.close()
output_file.close()

print 'Preparing decompressor with archive...'
os.system('gcc -s -Os -o decompressor _decompressor.c -DSKINNY -Ibzip2 ' +
    'bzip2/bzlib.c bzip2/crctable.c bzip2/decompress.c bzip2/huffman.c ' +
    'bzip2/randtable.c')
input_file = open('_archive.bz2', 'rb')
if os.name == 'nt':
    output_file = open('decompressor.exe', 'ab')
else:
    output_file = open('decompressor', 'ab')
output_file.write(input_file.read())
input_file.close()
output_file.close()
if os.name == 'nt':
    os.system('del _decompressor.c')
    os.system('del _archive.bz2')
    os.system('rename decompressor.exe installer.exe')
else:
    os.system('rm _decompressor.c')
    os.system('rm _archive.bz2')
    os.system('mv decompressor installer')

print 'Archiving completed.'
