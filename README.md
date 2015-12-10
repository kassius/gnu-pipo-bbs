# GNU Pipo BBS 2.0

BBS core, written in C.

https://www.gnu.org/software/pipo/

https://directory.fsf.org/wiki/GNU_Pipo

http://sourceforge.net/projects/pipo-bbs/

## Building

> In order to build, Python ( http://www.python.org ) is required. At 
this point, I believe we are compatibile with Python 1.6, but in the 
future 2.0+ may be required. A Java compiler is optional, but recommended.
Swing is used (though it may very possibly just be AWT eventually), but 
gcj's support of Swing is improving and my GCC 3.3.1 gcj produces 
output with the source that works as well as Sun's javac in this case.

`$ python release.py`

## Running after changing the code

```
$ ./installer

$ cd Pipo-BBS

$ ./Pipo-BBS
```
