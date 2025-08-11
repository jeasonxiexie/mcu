#include "lcd_init_correct.h"
#include "hal/wb32l003_hal.h"

// 延时函数
static void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

// GPIO初始化 - 根据RTF文档的引脚映射
void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能所有需要的GPIO时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    // ========== 步骤1：配置电源控制引脚 ==========
    
    // PA3 - LCD电源控制
    GPIO_InitStruct.Pin = LCD_PWR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_PWR_PORT, &GPIO_InitStruct);
    LCD_PWR_On();  // 立即使能LCD电源
    
    // PC6 - CPU电源控制（保持高电平）
    GPIO_InitStruct.Pin = CPU_PWR_PIN;
    HAL_GPIO_Init(CPU_PWR_PORT, &GPIO_InitStruct);
    CPU_PWR_On();
    
    // PC5 - 5V升压控制
    GPIO_InitStruct.Pin = RF_PWR_PIN;
    HAL_GPIO_Init(RF_PWR_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(RF_PWR_PORT, RF_PWR_PIN, GPIO_PIN_SET);
    
    delay_ms(10);  // 等待电源稳定
    
    // ========== 步骤2：配置LCD接口引脚（GPIOB） ==========
    
    // PB3-PB7: LCD接口（PB7 用作硬件复位）
    GPIO_InitStruct.Pin = LCD_SDA_PIN | LCD_CS_PIN | LCD_SCL_PIN | 
                          LCD_RS_PIN | LCD_RST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // 设置初始状态
    LCD_CS_Set();   // CS高（未选中）
    LCD_RS_Set();   // RS高（数据模式）
    LCD_SCL_Set();  // 时钟高
    LCD_SDA_Set();  // 数据高
    LCD_RST_Set();  // 复位高
    
    // ========== 步骤3：配置背光控制（GPIOB） ==========
    
    // PB1, PB2: 背光控制
    GPIO_InitStruct.Pin = LCD_BL1_PIN | LCD_BL2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // 初始背光设置（先供电低亮，PB2 低=使能）
    LCD_BL2_Enable();    // PB2 LOW = 使能背光
    LCD_BL1_Low();       // PB1 LOW = 低亮（初始低亮）
    
    // ========== 步骤4：配置音频控制 ==========
    
    // PA4 - 静音控制（初始静音）
    GPIO_InitStruct.Pin = CON_MUTE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CON_MUTE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_MUTE_PORT, CON_MUTE_PIN, GPIO_PIN_RESET);  // LOW = 静音
    
    // ========== 步骤5：配置LED ==========
    
    // PC0, PC1: LED
    GPIO_InitStruct.Pin = LED_RED_PIN | LED_GREEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // LED初始状态（关闭）
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);    // HIGH = 关
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET); // HIGH = 关
}

// 软件SPI写入一个字节
void LCD_Writ_Bus(uint8_t dat)
{
    uint8_t i;
    LCD_CS_Clr();
    
    for(i = 0; i < 8; i++)
    {
        LCD_SCL_Clr();
        if(dat & 0x80)
        {
            LCD_SDA_Set();
        }
        else
        {
            LCD_SDA_Clr();
        }
        LCD_SCL_Set();
        dat <<= 1;
    }
    
    LCD_CS_Set();
}

// 写入8位数据
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}

// 写入16位数据
void LCD_WR_DATA(uint16_t dat)
{
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
}

// 写入寄存器/命令
void LCD_WR_REG(uint8_t dat)
{
    LCD_RS_Clr();  // 命令模式
    LCD_Writ_Bus(dat);
    LCD_RS_Set();  // 数据模式
}

// 设置地址范围
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if(USE_HORIZONTAL == 0)
    {
        LCD_WR_REG(0x2a);  // 列地址设置
        LCD_WR_DATA(x1 + 24);
        LCD_WR_DATA(x2 + 24);
        LCD_WR_REG(0x2b);  // 行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);  // 内存写入
    }
    else if(USE_HORIZONTAL == 1)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1 + 24);
        LCD_WR_DATA(x2 + 24);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);
    }
    else if(USE_HORIZONTAL == 2)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1 + 24);
        LCD_WR_DATA(y2 + 24);
        LCD_WR_REG(0x2c);
    }
    else
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1 + 24);
        LCD_WR_DATA(y2 + 24);
        LCD_WR_REG(0x2c);
    }
}

// LCD初始化
void LCD_Init(void)
{
    LCD_GPIO_Init();  // 初始化GPIO
    
    // 硬件复位
    LCD_RST_Clr();
    delay_ms(100);
    LCD_RST_Set();
    delay_ms(100);
    
    // 根据文档设置背光
    // 上电稳定 → 供电（Q7 导通）→ 切换高亮
    delay_ms(80);        // 电源/升压稳定（建议 50-100ms）
    LCD_BL1_Low();       // PB1 LOW = Q7 导通（供电）
    delay_ms(20);
    LCD_BL2_HighBrightness();   // PB2 HIGH = 高亮
    
    // ST7735S初始化序列
    LCD_WR_REG(0x11);  // Sleep out
    delay_ms(120);
    
    // Frame rate control
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    
    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    
    LCD_WR_REG(0xB3);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    
    LCD_WR_REG(0xB4);  // Dot inversion
    LCD_WR_DATA8(0x03);
    
    // Power control
    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x04);
    
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0xC5);
    
    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x00);
    
    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x2A);
    
    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);
    
    LCD_WR_REG(0xC5);  // VCOM
    LCD_WR_DATA8(0x06);
    
    // Memory access control
    LCD_WR_REG(0x36);
    if(USE_HORIZONTAL == 0) LCD_WR_DATA8(0x08);
    else if(USE_HORIZONTAL == 1) LCD_WR_DATA8(0xC8);
    else if(USE_HORIZONTAL == 2) LCD_WR_DATA8(0x78);
    else LCD_WR_DATA8(0xA8);
    
    // Color format
    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x55);  // 16-bit color
    
    // Gamma correction
    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x0b);
    LCD_WR_DATA8(0x17);
    LCD_WR_DATA8(0x0a);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x1a);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x1d);
    LCD_WR_DATA8(0x21);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x3c);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x10);
    
    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x0c);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x1b);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x15);
    LCD_WR_DATA8(0x1d);
    LCD_WR_DATA8(0x21);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x3E);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x10);
    
    delay_ms(120);
    LCD_WR_REG(0x29);  // Display on
    
    // 开机完成后解除静音
    HAL_GPIO_WritePin(CON_MUTE_PORT, CON_MUTE_PIN, GPIO_PIN_SET);  // HIGH = 解除静音
}

// 填充矩形区域
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
    
    for(i = ysta; i < yend; i++)
    {
        for(j = xsta; j < xend; j++)
        {
            LCD_WR_DATA(color);
        }
    }
}