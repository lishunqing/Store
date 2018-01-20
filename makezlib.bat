cd zlib
nmake -f win32/Makefile.msc clean
nmake -f win32/Makefile.msc

copy /Y zlib.lib ..\lib\zlib.lib
cd ..
