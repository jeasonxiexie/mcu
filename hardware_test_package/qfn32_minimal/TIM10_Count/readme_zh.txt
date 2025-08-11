/**
  @page TIM10_Count TIM10_Count example
  
  @verbatim
  ****************** (C) COPYRIGHT 2023 Westberry Technology *******************
  * @file    TIM10/TIM10_Count/readme.txt 
  * @author  Westberry Application Team
  * @version V0.0.4
  * @date    23-April-2023
  * @brief   Description of the TIM10 TIM10_Count example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WESTBERRY SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM
  * THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par 例程描述 

本例程演示如何使用TIM10的计数功能。

本例程系统时钟工作在24Mhz。

本例程运行时TIM1通过PC6管脚输出200K的PWM信号，TIM10通过PD4输入的下降沿进行计数，采用重装载计数方式， \n
重装载值65435，计数值到65535时产生中断翻转PC4管脚。将PD4连接到PC6管脚，在PC4管脚上将输出翻转速度为2K的波形。


@par 目录内容 

  - TIM10/TIM10_Count/TIM10_Count.uvprojx   MDK5工程文件
  - TIM10/TIM10_Count/TIM10_Count.uvoptx    MDK5工程配置文件
  - TIM10/TIM10_Count/JLinkSettings.ini     Jlink配置文件
  - TIM10/TIM10_Count/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - 芯片管脚连接如下：
      - PD4 管脚连接到 PC6管脚
      - PC4 管脚连接示波器


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM10_Count.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2023 Westberry Technology</center></h3>
 */
