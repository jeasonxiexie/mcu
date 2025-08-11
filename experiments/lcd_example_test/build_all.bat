@echo off
setlocal EnableDelayedExpansion

REM LCD Example Test - 全版本编译脚本 (Windows)
REM 用于编译所有LCD驱动版本并输出到统一目录
REM 日期: 2024-08-06

REM 获取脚本所在目录
set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

REM 定义输出目录
set "OUTPUT_DIR=%SCRIPT_DIR%\firmware_output"
set "VERSION1_OUTPUT=%OUTPUT_DIR%\version1_gpioc"
set "VERSION2_OUTPUT=%OUTPUT_DIR%\version2_gpiob"

REM 版本目录
set "VERSION1_DIR=%SCRIPT_DIR%\v1_gpioc_standard_schematic"
set "VERSION2_DIR=%SCRIPT_DIR%\v2_gpiob_actual_hardware"

REM 获取时间戳
for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /value') do set "dt=%%I"
set "TIMESTAMP=%dt:~0,8%_%dt:~8,6%"
set "BUILD_DATE=%date% %time%"

REM 初始化状态
set "VERSION1_STATUS=FAILED"
set "VERSION2_STATUS=FAILED"

echo =========================================
echo     LCD Example Test 全版本编译脚本
echo =========================================
echo.

REM 检查工具链
echo [INFO] 检查ARM工具链...
where arm-none-eabi-gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] 未找到 arm-none-eabi-gcc
    echo [INFO] 请安装ARM工具链:
    echo [INFO]   下载 ARM GNU Toolchain from https://developer.arm.com/
    echo [INFO]   并将其添加到系统PATH环境变量
    pause
    exit /b 1
)

REM 显示工具链版本
for /f "tokens=*" %%i in ('arm-none-eabi-gcc --version ^| findstr gcc') do set "GCC_VERSION=%%i"
echo [INFO] 工具链版本: %GCC_VERSION%

REM 清理旧的输出目录
echo [INFO] 清理旧的输出目录...
if exist "%OUTPUT_DIR%" rd /s /q "%OUTPUT_DIR%"

REM 创建输出目录
echo [INFO] 创建输出目录结构...
mkdir "%VERSION1_OUTPUT%" 2>nul
mkdir "%VERSION2_OUTPUT%" 2>nul

REM ========================================
REM 编译 Version 1
REM ========================================
echo.
echo =========================================
echo 编译 Version 1 (GPIOC - 标准原理图版本)
echo =========================================

cd /d "%VERSION1_DIR%"

REM 清理
echo [INFO] 清理旧的编译文件...
call make clean >nul 2>&1

REM 编译
echo [INFO] 开始编译...
call make
if %errorlevel% equ 0 (
    echo [SUCCESS] Version 1 编译成功
    set "VERSION1_STATUS=SUCCESS"
    
    REM 复制输出文件
    echo [INFO] 复制固件到输出目录...
    copy /y "lcd_test_v1.bin" "%VERSION1_OUTPUT%\lcd_test_v1_%TIMESTAMP%.bin" >nul
    copy /y "lcd_test_v1.hex" "%VERSION1_OUTPUT%\lcd_test_v1_%TIMESTAMP%.hex" >nul
    copy /y "lcd_test_v1.elf" "%VERSION1_OUTPUT%\lcd_test_v1_%TIMESTAMP%.elf" >nul
    
    REM 复制为最新版本
    copy /y "lcd_test_v1.bin" "%VERSION1_OUTPUT%\lcd_test_v1_latest.bin" >nul
    copy /y "lcd_test_v1.hex" "%VERSION1_OUTPUT%\lcd_test_v1_latest.hex" >nul
    copy /y "lcd_test_v1.elf" "%VERSION1_OUTPUT%\lcd_test_v1_latest.elf" >nul
    
    REM 获取固件大小
    arm-none-eabi-size lcd_test_v1.elf > "%TEMP%\size_v1.txt"
    
    REM 保存编译信息
    (
        echo LCD Test Version 1 - GPIOC ^(标准原理图版本^)
        echo =====================================
        echo 编译时间: %BUILD_DATE%
        echo 时间戳: %TIMESTAMP%
        echo GPIO端口: GPIOC
        echo 参考文档: 0.96寸插接式原理图.pdf
        echo.
        echo 固件文件:
        echo - lcd_test_v1_%TIMESTAMP%.bin
        echo - lcd_test_v1_%TIMESTAMP%.hex
        echo - lcd_test_v1_%TIMESTAMP%.elf
        echo.
        echo 固件大小:
        type "%TEMP%\size_v1.txt"
        echo.
        echo 引脚映射:
        echo - LCD_RST: PC6
        echo - LCD_RS:  PC5
        echo - LCD_SCL: PC3
        echo - LCD_CS:  PC1
        echo - LCD_SDA: PC0
    ) > "%VERSION1_OUTPUT%\build_info.txt"
    
    echo [SUCCESS] Version 1 文件已保存到: %VERSION1_OUTPUT%
) else (
    echo [ERROR] Version 1 编译失败
)

REM ========================================
REM 编译 Version 2
REM ========================================
echo.
echo =========================================
echo 编译 Version 2 (GPIOB - 实际PCB版本)
echo =========================================

cd /d "%VERSION2_DIR%"

REM 清理
echo [INFO] 清理旧的编译文件...
call make clean >nul 2>&1

REM 编译
echo [INFO] 开始编译...
call make
if %errorlevel% equ 0 (
    echo [SUCCESS] Version 2 编译成功
    set "VERSION2_STATUS=SUCCESS"
    
    REM 复制输出文件
    echo [INFO] 复制固件到输出目录...
    copy /y "lcd_test_v2.bin" "%VERSION2_OUTPUT%\lcd_test_v2_%TIMESTAMP%.bin" >nul
    copy /y "lcd_test_v2.hex" "%VERSION2_OUTPUT%\lcd_test_v2_%TIMESTAMP%.hex" >nul
    copy /y "lcd_test_v2.elf" "%VERSION2_OUTPUT%\lcd_test_v2_%TIMESTAMP%.elf" >nul
    
    REM 复制为最新版本
    copy /y "lcd_test_v2.bin" "%VERSION2_OUTPUT%\lcd_test_v2_latest.bin" >nul
    copy /y "lcd_test_v2.hex" "%VERSION2_OUTPUT%\lcd_test_v2_latest.hex" >nul
    copy /y "lcd_test_v2.elf" "%VERSION2_OUTPUT%\lcd_test_v2_latest.elf" >nul
    
    REM 获取固件大小
    arm-none-eabi-size lcd_test_v2.elf > "%TEMP%\size_v2.txt"
    
    REM 保存编译信息
    (
        echo LCD Test Version 2 - GPIOB ^(实际PCB版本^)
        echo =====================================
        echo 编译时间: %BUILD_DATE%
        echo 时间戳: %TIMESTAMP%
        echo GPIO端口: GPIOB
        echo 参考文档: WB32L003 耳放固件.rtf
        echo.
        echo 固件文件:
        echo - lcd_test_v2_%TIMESTAMP%.bin
        echo - lcd_test_v2_%TIMESTAMP%.hex
        echo - lcd_test_v2_%TIMESTAMP%.elf
        echo.
        echo 固件大小:
        type "%TEMP%\size_v2.txt"
        echo.
        echo 引脚映射:
        echo - LCD_RST: PB7
        echo - LCD_RS:  PB6
        echo - LCD_SCL: PB5
        echo - LCD_CS:  PB4
        echo - LCD_SDA: PB3
        echo - LCD_BL1: PB1 ^(背光控制^)
        echo - LCD_BL2: PB2 ^(背光亮度^)
    ) > "%VERSION2_OUTPUT%\build_info.txt"
    
    echo [SUCCESS] Version 2 文件已保存到: %VERSION2_OUTPUT%
) else (
    echo [ERROR] Version 2 编译失败
)

REM ========================================
REM 生成编译报告
REM ========================================
echo.
echo [INFO] 生成编译报告...

(
    echo # LCD驱动固件编译报告
    echo.
    echo **编译时间**: %BUILD_DATE%
    echo **时间戳**: %TIMESTAMP%
    echo **工具链**: %GCC_VERSION%
    echo.
    echo ## 编译结果
    echo.
    echo ### Version 1 - GPIOC ^(标准原理图版本^)
    if "%VERSION1_STATUS%"=="SUCCESS" (
        echo - **状态**: 成功
    ) else (
        echo - **状态**: 失败
    )
    echo - **输出目录**: `version1_gpioc/`
    echo - **最新固件**: `lcd_test_v1_latest.bin`
    echo - **参考文档**: 0.96寸插接式原理图.pdf
    echo.
    echo ### Version 2 - GPIOB ^(实际PCB版本^)
    if "%VERSION2_STATUS%"=="SUCCESS" (
        echo - **状态**: 成功
    ) else (
        echo - **状态**: 失败
    )
    echo - **输出目录**: `version2_gpiob/`
    echo - **最新固件**: `lcd_test_v2_latest.bin`
    echo - **参考文档**: WB32L003 耳放固件.rtf
    echo.
    echo ## 使用说明
    echo.
    echo ### 烧录固件
    echo 1. 选择对应的版本（根据硬件）
    echo 2. 使用ISP烧录器连接到目标板
    echo 3. 烧录对应的.hex或.bin文件
    echo.
    echo ### 版本选择指南
    echo - **Version 1**: 用于标准0.96寸LCD模块开发板
    echo - **Version 2**: 用于项目实际PCB板
    echo.
    echo ## 注意事项
    echo - 版本不对LCD不亮
    echo - 请确认您的硬件连接后选择对应版本
    echo.
    echo ---
    echo *此报告由自动编译脚本生成*
) > "%OUTPUT_DIR%\BUILD_REPORT.md"

echo [SUCCESS] 编译报告已生成: %OUTPUT_DIR%\BUILD_REPORT.md

REM ========================================
REM 显示最终结果
REM ========================================
echo.
echo =========================================
echo               编译完成
echo =========================================
echo [INFO] 输出目录: %OUTPUT_DIR%
if "%VERSION1_STATUS%"=="SUCCESS" (
    echo [INFO] Version 1 ^(GPIOC^): 成功
) else (
    echo [INFO] Version 1 ^(GPIOC^): 失败
)
if "%VERSION2_STATUS%"=="SUCCESS" (
    echo [INFO] Version 2 ^(GPIOB^): 成功
) else (
    echo [INFO] Version 2 ^(GPIOB^): 失败
)
echo.

if "%VERSION1_STATUS%"=="SUCCESS" if "%VERSION2_STATUS%"=="SUCCESS" (
    echo [SUCCESS] 所有版本编译成功！
    echo.
    pause
    exit /b 0
) else (
    echo [WARNING] 部分版本编译失败，请检查错误信息
    echo.
    pause
    exit /b 1
)