/* Includes ------------------------------------------------------------------*/
#include "wb32l003.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// QFN-32 引脚映射
#define LED_RED_PORT    GPIOC
#define LED_RED_PIN     GPIO_Pin_0    // Pin 17 - 低电平点亮
#define LED_GREEN_PORT  GPIOC
#define LED_GREEN_PIN   GPIO_Pin_1    // Pin 18 - 低电平点亮

#define UART_TX_PORT    GPIOD
#define UART_TX_PIN     GPIO_Pin_5    // Pin 30 - UART2_TX

#define POW_CPU_PORT    GPIOC
#define POW_CPU_PIN     GPIO_Pin_6    // Pin 23 - 系统电源控制
#define POW_LCD_PORT    GPIOA
#define POW_LCD_PIN     GPIO_Pin_3    // Pin 7 - LCD电源
#define POW_RF_PORT     GPIOC
#define POW_RF_PIN      GPIO_Pin_5    // Pin 22 - RF电源

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t SystemTickCount = 0;

/* Private function prototypes -----------------------------------------------*/
void System_Init(void);
void Power_Control_Init(void);
void LED_Init(void);
void UART2_Init(void);
void UART2_SendString(const char* str);
void delay_ms(uint32_t ms);

/* Private functions ---------------------------------------------------------*/

void delay_ms(uint32_t ms)
{
  uint32_t ticks = (SystemCoreClock / 1000) * ms / 4; // 估算延时
  while (ticks--)
  {
    __asm__("nop");
  }
}

int main(void)
{
  uint8_t led_state = 0;
  uint32_t counter = 0;
  
  // 系统初始化
  SystemCoreClockUpdate();
  System_Init();
  
  // 第一步：电源控制（必须在150ms内）
  Power_Control_Init();
  
  // 初始化LED
  LED_Init();
  
  // 初始化UART2
  UART2_Init();
  
  // 启动信号：快闪3次
  UART2_SendString("\r\n=== QFN-32 WB32L003 Test Starting ===\r\n");
  for(int i = 0; i < 3; i++)
  {
    GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);   // 红灯亮
    GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN); // 绿灯亮
    delay_ms(100);
    GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);      // 红灯灭
    GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);   // 绿灯灭
    delay_ms(100);
  }
  
  UART2_SendString("System initialized successfully!\r\n");
  UART2_SendString("LED alternating every 500ms...\r\n");
  
  /* 主循环：LED交替闪烁 + UART输出 */
  while (1)
  {
    // LED交替闪烁
    if(led_state == 0)
    {
      GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);   // 红灯亮
      GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);  // 绿灯灭
      led_state = 1;
    }
    else
    {
      GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);      // 红灯灭
      GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN); // 绿灯亮
      led_state = 0;
    }
    
    // 每秒输出一次调试信息
    counter++;
    if(counter >= 2)  // 500ms * 2 = 1秒
    {
      counter = 0;
      UART2_SendString("[Tick] System running... LED state: ");
      if(led_state)
        UART2_SendString("RED\r\n");
      else
        UART2_SendString("GREEN\r\n");
    }
    
    delay_ms(500);
  }
}

/* 系统初始化 */
void System_Init(void)
{
  // 启用所需的时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);
}

/* 电源控制初始化 - QFN-32引脚 */
void Power_Control_Init(void)
{
  // PC6 - CPU电源控制 (Pin 23) - 必须首先设置！
  GPIO_Init(POW_CPU_PORT, POW_CPU_PIN, 
            GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(POW_CPU_PORT, POW_CPU_PIN);  // HIGH = 开机
  
  // PA3 - LCD电源 (Pin 7)
  GPIO_Init(POW_LCD_PORT, POW_LCD_PIN, 
            GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(POW_LCD_PORT, POW_LCD_PIN);  // HIGH = 开机
  
  // PC5 - RF电源 (Pin 22)
  GPIO_Init(POW_RF_PORT, POW_RF_PIN, 
            GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(POW_RF_PORT, POW_RF_PIN);   // HIGH = 开机
}

/* LED初始化 - QFN-32引脚 */
void LED_Init(void)
{
  // PC0 - 红色LED (Pin 17) - 低电平点亮
  GPIO_Init(LED_RED_PORT, LED_RED_PIN, 
            GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);  // 初始状态：灭
  
  // PC1 - 绿色LED (Pin 18) - 低电平点亮
  GPIO_Init(LED_GREEN_PORT, LED_GREEN_PIN, 
            GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);  // 初始状态：灭
}

/* UART2初始化 - PD5作为TX (Pin 30) */
void UART2_Init(void)
{
  UART_InitTypeDef UART_InitStructure;
  
  // 配置PD5为UART2_TX (AF0)
  GPIO_Init(UART_TX_PORT, UART_TX_PIN, 
            GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH | GPIO_AF0);
  
  // 使能UART2时钟
  RCC_APBPeriphClockCmd(RCC_APBPeriph_UART2, ENABLE);
  
  // UART2配置: 115200, 8N1
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Tx;  // 只发送
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  
  UART_Init(UART2, &UART_InitStructure);
  UART_Cmd(UART2, ENABLE);
}

/* UART2发送字符串 */
void UART2_SendString(const char* str)
{
  while(*str)
  {
    // 等待发送缓冲区空
    while(UART_GetFlagStatus(UART2, UART_FLAG_TXE) == RESET);
    UART_SendData(UART2, (uint8_t)*str++);
  }
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
