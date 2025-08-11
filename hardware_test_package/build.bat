@echo off
echo ===================================
echo WB32L003 Hardware Test Builder
echo ===================================
echo.

REM 检查工具是否存在
if not exist "tools\arm-none-eabi\bin\arm-none-eabi-gcc.exe" (
    echo [ERROR] ARM compiler not found!
    echo Please download ARM toolchain and extract to tools\arm-none-eabi\
    echo Download from: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
    pause
    exit /b 1
)

REM 创建输出目录
if not exist "output" mkdir output

REM 清理旧文件
echo Cleaning old files...
del /Q output\*.o 2>nul
del /Q output\*.elf 2>nul
del /Q output\*.hex 2>nul
del /Q output\*.bin 2>nul

REM 设置编译器路径
set CC=tools\arm-none-eabi\bin\arm-none-eabi-gcc.exe
set AS=tools\arm-none-eabi\bin\arm-none-eabi-gcc.exe
set LD=tools\arm-none-eabi\bin\arm-none-eabi-ld.exe
set OBJCOPY=tools\arm-none-eabi\bin\arm-none-eabi-objcopy.exe
set SIZE=tools\arm-none-eabi\bin\arm-none-eabi-size.exe

REM 编译参数
set CFLAGS=-mcpu=cortex-m0plus -mthumb -O2 -g -Wall -fdata-sections -ffunction-sections
set LDFLAGS=-TWB32L003K8Ux_FLASH_nolibc.ld --gc-sections

REM 编译main.c
echo Compiling main.c...
%CC% %CFLAGS% -c source\main.c -o output\main.o
if errorlevel 1 (
    echo [ERROR] Failed to compile main.c
    pause
    exit /b 1
)

REM 编译startup.s
echo Compiling startup.s...
%AS% %CFLAGS% -c source\startup_wb32l003.s -o output\startup.o
if errorlevel 1 (
    echo [ERROR] Failed to compile startup.s
    pause
    exit /b 1
)

REM 编译stubs.c
echo Compiling stubs.c...
%CC% %CFLAGS% -c source\stubs.c -o output\stubs.o
if errorlevel 1 (
    echo [ERROR] Failed to compile stubs.c
    pause
    exit /b 1
)

REM 链接
echo Linking...
cd source
..\%LD% %LDFLAGS% ..\output\startup.o ..\output\main.o ..\output\stubs.o -o ..\output\test.elf
cd ..
if errorlevel 1 (
    echo [ERROR] Failed to link
    pause
    exit /b 1
)

REM 生成HEX文件
echo Generating HEX file...
%OBJCOPY% -O ihex output\test.elf output\test.hex
if errorlevel 1 (
    echo [ERROR] Failed to generate HEX file
    pause
    exit /b 1
)

REM 生成BIN文件
%OBJCOPY% -O binary output\test.elf output\test.bin

REM 显示大小信息
echo.
echo Build complete! Size information:
%SIZE% output\test.elf
echo.
echo ===================================
echo Output files:
echo   - output\test.hex (for programming)
echo   - output\test.bin (binary format)
echo ===================================
echo.
echo Next steps:
echo 1. Connect your SWD programmer
echo 2. Flash output\test.hex to MCU
echo 3. Check LED flashing and UART output
echo.
pause