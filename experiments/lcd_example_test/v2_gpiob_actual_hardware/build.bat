@echo off

REM Version 2 (GPIOB) 独立编译脚本
REM 基于实际PCB的LCD驱动版本

echo =====================================
echo   Version 2 (GPIOB) 固件编译
echo =====================================
echo.

REM 检查工具链
where arm-none-eabi-gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] 未找到ARM工具链
    echo 请从 https://developer.arm.com/ 下载安装
    pause
    exit /b 1
)

REM 清理
echo 清理旧文件...
call make clean >nul 2>&1

REM 编译
echo 开始编译...
call make
if %errorlevel% equ 0 (
    echo [SUCCESS] 编译成功!
    echo.
    echo 生成的固件文件:
    echo   - lcd_test_v2.bin ^(烧录文件^)
    echo   - lcd_test_v2.hex ^(烧录文件,可选^)
    echo   - lcd_test_v2.elf ^(调试文件^)
    echo.
    arm-none-eabi-size lcd_test_v2.elf
    pause
) else (
    echo [ERROR] 编译失败
    pause
    exit /b 1
)