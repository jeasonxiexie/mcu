@echo off
echo ========================================
echo WB32L003 测试程序编译脚本
echo ========================================
echo.

REM 检查是否安装了ARM GCC
where arm-none-eabi-gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo [错误] 未找到 arm-none-eabi-gcc！
    echo.
    echo 请安装 ARM GCC 工具链：
    echo 1. 下载: https://developer.arm.com/downloads/-/gnu-rm
    echo 2. 安装后将bin目录加入PATH环境变量
    echo 3. 或者使用Keil MDK编译
    echo.
    pause
    exit /b 1
)

echo [信息] 找到ARM GCC编译器
arm-none-eabi-gcc --version | findstr "gcc"
echo.

REM 设置编译参数
set CC=arm-none-eabi-gcc
set OBJCOPY=arm-none-eabi-objcopy
set SIZE=arm-none-eabi-size

set CFLAGS=-mcpu=cortex-m0plus -mthumb -O2 -g -Wall
set LDFLAGS=-nostdlib -nostartfiles -ffreestanding

REM 编译源文件
echo [编译] simple_test.c ...
%CC% %CFLAGS% -c source/simple_test.c -o simple_test.o
if %errorlevel% neq 0 (
    echo [错误] 编译失败！
    pause
    exit /b 1
)

echo [编译] startup.s ...
%CC% %CFLAGS% -c source/startup.s -o startup.o 2>nul
if %errorlevel% neq 0 (
    echo [警告] 没有找到startup.s，使用简化启动
    echo [创建] 最小启动代码...
    
    REM 创建最小的启动代码
    echo .syntax unified > min_startup.s
    echo .cpu cortex-m0plus >> min_startup.s
    echo .thumb >> min_startup.s
    echo. >> min_startup.s
    echo .section .vectors, "a" >> min_startup.s
    echo .word 0x20001000 >> min_startup.s
    echo .word Reset_Handler >> min_startup.s
    echo. >> min_startup.s
    echo .section .text >> min_startup.s
    echo .thumb_func >> min_startup.s
    echo .global Reset_Handler >> min_startup.s
    echo Reset_Handler: >> min_startup.s
    echo     ldr r0, =0x20001000 >> min_startup.s
    echo     mov sp, r0 >> min_startup.s
    echo     bl main >> min_startup.s
    echo     b . >> min_startup.s
    
    %CC% %CFLAGS% -c min_startup.s -o startup.o
)

REM 链接
echo [链接] 生成ELF文件...
%CC% %CFLAGS% %LDFLAGS% -Ttext=0x08000000 startup.o simple_test.o -o simple_test.elf
if %errorlevel% neq 0 (
    echo [错误] 链接失败！
    pause
    exit /b 1
)

REM 生成HEX文件
echo [转换] 生成HEX文件...
%OBJCOPY% -O ihex simple_test.elf simple_test.hex
if %errorlevel% neq 0 (
    echo [错误] HEX生成失败！
    pause
    exit /b 1
)

REM 显示大小信息
echo.
echo [信息] 编译成功！
%SIZE% simple_test.elf
echo.
echo 输出文件：simple_test.hex
echo.

REM 清理临时文件
del *.o 2>nul
del min_startup.s 2>nul

echo ========================================
echo 编译完成！
echo HEX文件：simple_test.hex
echo.
echo 下一步：
echo 1. 使用SWD烧录器将 simple_test.hex 烧录到MCU
echo 2. 观察LED是否闪烁
echo 3. 连接串口查看输出（PA9, 115200）
echo ========================================
pause