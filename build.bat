SET PATH=C:\devkitPro\devkitPPC\bin;C:\mingw64\bin;;C:\Program Files (x86)\GnuWin32\bin;%PATH%
make
pause
rmdir /q /s build
rmdir /q /s -p
del *.elf