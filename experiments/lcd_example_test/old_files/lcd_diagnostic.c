/**
 * @file lcd_diagnostic.c
 * @brief LCD诊断测试程序 - 用于调试LCD不亮问题
 */

#include "wb32l003.h"
#include "wb32l003_hal.h"
#include "lcd_init.h"

// LED状态定义
#define LED_PIN     GPIO_PIN_0
#define LED_PORT    GPIOB

// 延时函数
static void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

// 非常慢的延时（用于逻辑分析仪）
static void slow_delay(void)
{
    for(volatile uint32_t i = 0; i < 10000; i++);
}

// LED闪烁函数
void LED_Blink(uint8_t times, uint32_t delay)
{
    for(uint8_t i = 0; i < times; i++)
    {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
        delay_ms(delay);
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
        delay_ms(delay);
    }
}

// 系统时钟配置
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | 
                                  RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

// LED初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

// 诊断版GPIO初始化 - 分步骤带LED指示
void Diagnostic_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 步骤1: 使能时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    LED_Blink(1, 100); // 1次闪烁表示时钟使能完成
    
    // 步骤2: 配置PA3 (LCD电源控制) - 最先配置
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 立即拉高PA3使能LCD电源
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    delay_ms(100); // 等待电源稳定
    LED_Blink(2, 100); // 2次闪烁表示LCD电源使能
    
    // 步骤3: 配置GPIOC (LCD接口)
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | 
                          GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // 设置初始状态
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);  // CS高
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);  // DC高
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // RST高
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);  // SCL高
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  // SDA高
    LED_Blink(3, 100); // 3次闪烁表示LCD接口配置完成
    
    // 步骤4: 配置背光控制
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // 测试不同的背光控制方式
    // 先关闭所有背光
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   // PB1高 = PNP三极管关闭
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // PB2低 = 直接驱动关闭
    delay_ms(500);
    
    // 尝试方式1: PB1低电平（PNP三极管导通）
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // PB1低 = PNP导通
    LED_Blink(4, 100); // 4次闪烁表示尝试PB1背光
    delay_ms(1000);
    
    // 尝试方式2: PB2高电平（直接驱动）
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   // 关闭PB1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   // PB2高 = 直接驱动开启
    LED_Blink(5, 100); // 5次闪烁表示尝试PB2背光
    delay_ms(1000);
    
    // 最终使用PB1（高亮度）
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // PB1低 = 背光开
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // PB2低 = 关闭
}

// 慢速SPI写入（便于逻辑分析仪观察）
void Slow_SPI_Write(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET); // CS低
    slow_delay();
    
    for(uint8_t i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); // SCL低
        slow_delay();
        
        if(data & 0x80)
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // SDA高
        else
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // SDA低
        
        slow_delay();
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   // SCL高
        slow_delay();
        
        data <<= 1;
    }
    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); // CS高
    slow_delay();
}

// 测试LCD复位序列
void Test_LCD_Reset(void)
{
    LED_Blink(10, 50); // 10次快速闪烁表示开始复位测试
    
    // 复位序列
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   // RST高
    delay_ms(100);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); // RST低
    delay_ms(100);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   // RST高
    delay_ms(200);
    
    LED_Blink(3, 200); // 3次慢速闪烁表示复位完成
}

// 发送测试命令
void Send_Test_Commands(void)
{
    // 发送睡眠退出命令
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // DC低 = 命令
    Slow_SPI_Write(0x11); // SLPOUT
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   // DC高 = 数据
    delay_ms(120);
    
    // 发送显示开启命令
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // DC低 = 命令
    Slow_SPI_Write(0x29); // DISPON
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   // DC高 = 数据
    delay_ms(100);
    
    // 发送测试模式命令（填充红色）
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // DC低 = 命令
    Slow_SPI_Write(0x2C); // RAMWR
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   // DC高 = 数据
    
    // 发送一些红色像素数据
    for(int i = 0; i < 100; i++)
    {
        Slow_SPI_Write(0xF8); // 红色高字节
        Slow_SPI_Write(0x00); // 红色低字节
    }
}

int main(void)
{
    // HAL初始化
    HAL_Init();
    
    // 系统时钟配置
    SystemClock_Config();
    
    // LED初始化
    LED_Init();
    
    // 启动指示：快速闪烁3次
    LED_Blink(3, 50);
    delay_ms(500);
    
    // 诊断版GPIO初始化（带LED状态指示）
    Diagnostic_GPIO_Init();
    delay_ms(500);
    
    // 测试LCD复位
    Test_LCD_Reset();
    delay_ms(500);
    
    // 发送测试命令
    Send_Test_Commands();
    
    // 主循环：持续发送测试数据
    while(1)
    {
        // 每秒发送一次测试模式
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // 命令
        Slow_SPI_Write(0x2C); // RAMWR
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   // 数据
        
        // 交替发送红色和蓝色
        static uint8_t color_toggle = 0;
        for(int i = 0; i < 50; i++)
        {
            if(color_toggle)
            {
                Slow_SPI_Write(0xF8); // 红色
                Slow_SPI_Write(0x00);
            }
            else
            {
                Slow_SPI_Write(0x00); // 蓝色
                Slow_SPI_Write(0x1F);
            }
        }
        color_toggle = !color_toggle;
        
        // LED心跳
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        delay_ms(1000);
    }
}