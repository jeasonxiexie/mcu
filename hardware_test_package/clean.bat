@echo off
echo Cleaning build files...
del /Q output\*.o 2>nul
del /Q output\*.elf 2>nul
del /Q output\*.hex 2>nul
del /Q output\*.bin 2>nul
del /Q output\*.map 2>nul
echo Clean complete!
pause