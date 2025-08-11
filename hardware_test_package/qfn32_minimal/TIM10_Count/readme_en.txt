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

@par Example Description 

This example describes how to se the counting function of TIM10.

In this example, Main clock is configured at 24MHz.

When this routine runs, TIM1 outputs 200K PWM signal through PC6 pin, and TIM10 \n
counts through the falling edge of PD4 input, using the reloaded counting mode, \n
Reload value 65435, count value to 65535 generates interrupt flip PC4 pin. Connect \n
PD4 to PC6 pin and output a waveform with a flip speed of 2K on PC4 pin.


@par Directory contents 

  - TIM10/TIM10_Count/TIM10_Count.uvprojx   MDK5 project file
  - TIM10/TIM10_Count/TIM10_Count.uvoptx    MDK5 project options file
  - TIM10/TIM10_Count/JLinkSettings.ini     Jlink settings file
  - TIM10/TIM10_Count/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - The chip pins are connected as follows：
      - PD4 pin is connected to PC6 pin
      - PC4 pin is connection to oscilloscope


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM10_Count.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2023 Westberry Technology</center></h3>
 */
