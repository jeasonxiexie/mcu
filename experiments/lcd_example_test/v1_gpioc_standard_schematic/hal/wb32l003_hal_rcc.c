#include "wb32l003_hal.h"

/* RCC Registers Bits Definition */
#define RCC_CR_HSION_Pos                         (0U)
#define RCC_CR_HSION_Msk                         (0x1UL << RCC_CR_HSION_Pos)
#define RCC_CR_HSION                             RCC_CR_HSION_Msk
#define RCC_CR_HSIRDY_Pos                        (1U)
#define RCC_CR_HSIRDY_Msk                        (0x1UL << RCC_CR_HSIRDY_Pos)
#define RCC_CR_HSIRDY                            RCC_CR_HSIRDY_Msk
#define RCC_CR_HSEON_Pos                         (16U)
#define RCC_CR_HSEON_Msk                         (0x1UL << RCC_CR_HSEON_Pos)
#define RCC_CR_HSEON                             RCC_CR_HSEON_Msk
#define RCC_CR_HSERDY_Pos                        (17U)
#define RCC_CR_HSERDY_Msk                        (0x1UL << RCC_CR_HSERDY_Pos)
#define RCC_CR_HSERDY                            RCC_CR_HSERDY_Msk
#define RCC_CR_HSEBYP_Pos                        (18U)
#define RCC_CR_HSEBYP_Msk                        (0x1UL << RCC_CR_HSEBYP_Pos)
#define RCC_CR_HSEBYP                            RCC_CR_HSEBYP_Msk
#define RCC_CR_PLLON_Pos                         (24U)
#define RCC_CR_PLLON_Msk                         (0x1UL << RCC_CR_PLLON_Pos)
#define RCC_CR_PLLON                             RCC_CR_PLLON_Msk
#define RCC_CR_PLLRDY_Pos                        (25U)
#define RCC_CR_PLLRDY_Msk                        (0x1UL << RCC_CR_PLLRDY_Pos)
#define RCC_CR_PLLRDY                            RCC_CR_PLLRDY_Msk

#define RCC_CFGR_SW_Pos                          (0U)
#define RCC_CFGR_SW_Msk                          (0x3UL << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW                              RCC_CFGR_SW_Msk
#define RCC_CFGR_SW_HSI                          0x00000000U
#define RCC_CFGR_SW_HSE                          0x00000001U
#define RCC_CFGR_SW_PLL                          0x00000002U
#define RCC_CFGR_SWS_Pos                         (2U)
#define RCC_CFGR_SWS_Msk                         (0x3UL << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS                             RCC_CFGR_SWS_Msk
#define RCC_CFGR_HPRE_Pos                        (4U)
#define RCC_CFGR_HPRE_Msk                        (0xFUL << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_HPRE                            RCC_CFGR_HPRE_Msk
#define RCC_CFGR_PPRE_Pos                        (8U)
#define RCC_CFGR_PPRE_Msk                        (0x7UL << RCC_CFGR_PPRE_Pos)
#define RCC_CFGR_PPRE                            RCC_CFGR_PPRE_Msk
#define RCC_CFGR_PLLSRC_Pos                      (16U)
#define RCC_CFGR_PLLSRC_Msk                      (0x1UL << RCC_CFGR_PLLSRC_Pos)
#define RCC_CFGR_PLLSRC                          RCC_CFGR_PLLSRC_Msk
#define RCC_CFGR_PLLXTPRE_Pos                    (17U)
#define RCC_CFGR_PLLXTPRE_Msk                    (0x1UL << RCC_CFGR_PLLXTPRE_Pos)
#define RCC_CFGR_PLLXTPRE                        RCC_CFGR_PLLXTPRE_Msk
#define RCC_CFGR_PLLMUL_Pos                      (18U)
#define RCC_CFGR_PLLMUL_Msk                      (0xFUL << RCC_CFGR_PLLMUL_Pos)
#define RCC_CFGR_PLLMUL                          RCC_CFGR_PLLMUL_Msk

#define RCC_AHBENR_GPIOAEN_Pos                   (17U)
#define RCC_AHBENR_GPIOAEN_Msk                   (0x1UL << RCC_AHBENR_GPIOAEN_Pos)
#define RCC_AHBENR_GPIOAEN                       RCC_AHBENR_GPIOAEN_Msk
#define RCC_AHBENR_GPIOBEN_Pos                   (18U)
#define RCC_AHBENR_GPIOBEN_Msk                   (0x1UL << RCC_AHBENR_GPIOBEN_Pos)
#define RCC_AHBENR_GPIOBEN                       RCC_AHBENR_GPIOBEN_Msk
#define RCC_AHBENR_GPIOCEN_Pos                   (19U)
#define RCC_AHBENR_GPIOCEN_Msk                   (0x1UL << RCC_AHBENR_GPIOCEN_Pos)
#define RCC_AHBENR_GPIOCEN                       RCC_AHBENR_GPIOCEN_Msk

#define RCC_APB2ENR_ADCEN_Pos                    (9U)
#define RCC_APB2ENR_ADCEN_Msk                    (0x1UL << RCC_APB2ENR_ADCEN_Pos)
#define RCC_APB2ENR_ADCEN                        RCC_APB2ENR_ADCEN_Msk
#define RCC_APB2ENR_SPI1EN_Pos                   (12U)
#define RCC_APB2ENR_SPI1EN_Msk                   (0x1UL << RCC_APB2ENR_SPI1EN_Pos)
#define RCC_APB2ENR_SPI1EN                       RCC_APB2ENR_SPI1EN_Msk

#define RCC_APB1ENR_TIM2EN_Pos                   (0U)
#define RCC_APB1ENR_TIM2EN_Msk                   (0x1UL << RCC_APB1ENR_TIM2EN_Pos)
#define RCC_APB1ENR_TIM2EN                       RCC_APB1ENR_TIM2EN_Msk
#define RCC_APB1ENR_PWREN_Pos                    (28U)
#define RCC_APB1ENR_PWREN_Msk                    (0x1UL << RCC_APB1ENR_PWREN_Pos)
#define RCC_APB1ENR_PWREN                        RCC_APB1ENR_PWREN_Msk

#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)

/* Private macros */
#define __HAL_RCC_HSI_ENABLE()           SET_BIT(RCC->CR, RCC_CR_HSION)
#define __HAL_RCC_HSI_DISABLE()          CLEAR_BIT(RCC->CR, RCC_CR_HSION)
#define __HAL_RCC_HSE_CONFIG(__STATE__)                                     \
                    do{                                                     \
                      if ((__STATE__) == RCC_HSE_ON)                       \
                      {                                                     \
                        SET_BIT(RCC->CR, RCC_CR_HSEON);                    \
                      }                                                     \
                      else if ((__STATE__) == RCC_HSE_OFF)                 \
                      {                                                     \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEON);                  \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);                 \
                      }                                                     \
                      else if ((__STATE__) == RCC_HSE_BYPASS)              \
                      {                                                     \
                        SET_BIT(RCC->CR, RCC_CR_HSEBYP);                   \
                        SET_BIT(RCC->CR, RCC_CR_HSEON);                    \
                      }                                                     \
                      else                                                  \
                      {                                                     \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEON);                  \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);                 \
                      }                                                     \
                    }while(0U)

#define __HAL_RCC_PLL_ENABLE()           SET_BIT(RCC->CR, RCC_CR_PLLON)
#define __HAL_RCC_PLL_DISABLE()          CLEAR_BIT(RCC->CR, RCC_CR_PLLON)

#define __HAL_RCC_APB1_FORCE_RESET()     WRITE_REG(RCC->APB1RSTR, 0xFFFFFFFFU)
#define __HAL_RCC_APB1_RELEASE_RESET()   WRITE_REG(RCC->APB1RSTR, 0x00000000U)
#define __HAL_RCC_APB2_FORCE_RESET()     WRITE_REG(RCC->APB2RSTR, 0xFFFFFFFFU)
#define __HAL_RCC_APB2_RELEASE_RESET()   WRITE_REG(RCC->APB2RSTR, 0x00000000U)
#define __HAL_RCC_AHB_FORCE_RESET()      WRITE_REG(RCC->AHBRSTR, 0xFFFFFFFFU)
#define __HAL_RCC_AHB_RELEASE_RESET()    WRITE_REG(RCC->AHBRSTR, 0x00000000U)
#define __HAL_RCC_PWR_FORCE_RESET()      SET_BIT(RCC->APB1RSTR, RCC_APB1RSTR_PWRRST)
#define __HAL_RCC_PWR_RELEASE_RESET()    CLEAR_BIT(RCC->APB1RSTR, RCC_APB1RSTR_PWRRST)

HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct)
{
  uint32_t tickstart;
  uint32_t pll_config;

  /* Check Null pointer */
  if (RCC_OscInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_OSCILLATORTYPE(RCC_OscInitStruct->OscillatorType));

  /*------------------------------- HSE Configuration ------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSE(RCC_OscInitStruct->HSEState));

    /* When the HSE is used as system clock or clock source for PLL in these cases it is not allowed to be disabled */
    if ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSE)
        || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)))
    {
      if ((__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET) && (RCC_OscInitStruct->HSEState == RCC_HSE_OFF))
      {
        return HAL_ERROR;
      }
    }
    else
    {
      /* Set the new HSE configuration ---------------------------------------*/
      __HAL_RCC_HSE_CONFIG(RCC_OscInitStruct->HSEState);

      /* Check the HSE State */
      if (RCC_OscInitStruct->HSEState != RCC_HSE_OFF)
      {
        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till HSE is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till HSE is disabled */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*----------------------------- HSI Configuration --------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSI(RCC_OscInitStruct->HSIState));
    assert_param(IS_RCC_CALIBRATION_VALUE(RCC_OscInitStruct->HSICalibrationValue));

    /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */
    if ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI)
        || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI)))
    {
      /* When HSI is used as system clock it will not disabled */
      if ((__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) && (RCC_OscInitStruct->HSIState != RCC_HSI_ON))
      {
        return HAL_ERROR;
      }
      /* Otherwise, just the calibration is allowed */
      else
      {
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
    }
    else
    {
      /* Check the HSI State */
      if (RCC_OscInitStruct->HSIState != RCC_HSI_OFF)
      {
        /* Enable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_ENABLE();

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till HSI is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
      else
      {
        /* Disable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_DISABLE();

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till HSI is disabled */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*------------------------------ LSI Configuration -------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LSI(RCC_OscInitStruct->LSIState));

    /* Check the LSI State */
    if (RCC_OscInitStruct->LSIState != RCC_LSI_OFF)
    {
      /* Enable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_ENABLE();

      /* Get Start Tick */
      tickstart = HAL_GetTick();

      /* Wait till LSI is ready */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
      {
        if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Disable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_DISABLE();

      /* Get Start Tick */
      tickstart = HAL_GetTick();

      /* Wait till LSI is disabled */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET)
      {
        if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
  }
  /*------------------------------ LSE Configuration -------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE)
  {
    FlagStatus pwrclkchanged = RESET;

    /* Check the parameters */
    assert_param(IS_RCC_LSE(RCC_OscInitStruct->LSEState));

    /* Update LSE configuration in Backup Domain control register    */
    /* Requires to enable write access to Backup Domain of necessary */
    if (__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      pwrclkchanged = SET;
    }

    if (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
    {
      /* Enable write access to Backup domain */
      SET_BIT(PWR->CR, PWR_CR_DBP);

      /* Wait for Backup domain Write protection disable */
      tickstart = HAL_GetTick();

      while (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
      {
        if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* Set the new LSE configuration -----------------------------------------*/
    __HAL_RCC_LSE_CONFIG(RCC_OscInitStruct->LSEState);
    /* Check the LSE State */
    if (RCC_OscInitStruct->LSEState != RCC_LSE_OFF)
    {
      /* Get Start Tick */
      tickstart = HAL_GetTick();

      /* Wait till LSE is ready */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Get Start Tick */
      tickstart = HAL_GetTick();

      /* Wait till LSE is disabled */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* Restore clock configuration if changed */
    if (pwrclkchanged == SET)
    {
      __HAL_RCC_PWR_CLK_DISABLE();
    }
  }

  /*-------------------------------- PLL Configuration -----------------------*/
  /* Check the parameters */
  assert_param(IS_RCC_PLL(RCC_OscInitStruct->PLL.PLLState));
  if ((RCC_OscInitStruct->PLL.PLLState) != RCC_PLL_NONE)
  {
    /* Check if the PLL is used as system clock or not */
    if (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    {
      if ((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_ON)
      {
        /* Check the parameters */
        assert_param(IS_RCC_PLLSOURCE(RCC_OscInitStruct->PLL.PLLSource));
        assert_param(IS_RCC_PLL_MUL(RCC_OscInitStruct->PLL.PLLMUL));

        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till PLL is disabled */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* Configure the main PLL clock source and multiplication factor. */
        __HAL_RCC_PLL_CONFIG(RCC_OscInitStruct->PLL.PLLSource,
                             RCC_OscInitStruct->PLL.PLLMUL);
        /* Enable the main PLL. */
        __HAL_RCC_PLL_ENABLE();

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till PLL is ready */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till PLL is disabled */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    else
    {
      /* Check if there is a request to disable the PLL used as System clock source */
      if ((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF)
      {
        return HAL_ERROR;
      }
      else
      {
        /* Do not return HAL_ERROR if request repeats the current configuration */
        pll_config = RCC->CFGR;
        if ((READ_BIT(pll_config, RCC_CFGR_PLLSRC) != RCC_OscInitStruct->PLL.PLLSource) ||
            (READ_BIT(pll_config, RCC_CFGR_PLLMUL) != RCC_OscInitStruct->PLL.PLLMUL))
        {
          return HAL_ERROR;
        }
      }
    }
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency)
{
  uint32_t tickstart;

  /* Check Null pointer */
  if (RCC_ClkInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_CLOCKTYPE(RCC_ClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
  must be correctly programmed according to the frequency of the CPU clock
    (HCLK) of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if (FLatency > __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /*-------------------------- HCLK Configuration --------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    /* Set the highest APBx dividers in order to ensure that we do not go through
       a non-spec phase whatever we decrease or increase HCLK. */
    if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
    {
      MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_HCLK_DIV16);
    }

    /* Set the new HCLK clock divider */
    assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
  }

  /*------------------------- SYSCLK Configuration ---------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
  {
    assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));

    /* HSE is selected as System Clock Source */
    if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
    {
      /* Check the HSE ready flag */
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* PLL is selected as System Clock Source */
    else if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      /* Check the PLL ready flag */
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* HSI is selected as System Clock Source */
    else
    {
      /* Check the HSI ready flag */
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))
    {
      if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }

  /* Decreasing the number of wait states because of lower CPU frequency */
  if (FLatency < __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /*-------------------------- PCLK1 Configuration ---------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_ClkInitStruct->APB1CLKDivider);
  }

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];

  /* Configure the source of time base considering new system clocks settings*/
  HAL_InitTick(uwTickPrio);

  return HAL_OK;
}