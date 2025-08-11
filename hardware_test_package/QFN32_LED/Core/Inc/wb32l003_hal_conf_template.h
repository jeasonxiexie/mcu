/* Additional macros and definitions for compilation */

/* Assert parameter macros */
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                       ((INSTANCE) == GPIOB) || \
                                       ((INSTANCE) == GPIOC))

#define IS_GPIO_PIN(PIN) ((((PIN) & GPIO_PIN_All) != 0x00U) && \
                         (((PIN) & ~GPIO_PIN_All) == 0x00U))

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_INPUT)           || \
                           ((MODE) == GPIO_MODE_OUTPUT_PP)        || \
                           ((MODE) == GPIO_MODE_OUTPUT_OD)        || \
                           ((MODE) == GPIO_MODE_AF_PP)            || \
                           ((MODE) == GPIO_MODE_AF_OD)            || \
                           ((MODE) == GPIO_MODE_ANALOG))

#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL)   || \
                           ((PULL) == GPIO_PULLUP)     || \
                           ((PULL) == GPIO_PULLDOWN))

#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || \
                                   ((ACTION) == GPIO_PIN_SET))

/* GPIO register bit definitions */
#define GPIO_MODER_MODER0_Pos            (0U)
#define GPIO_MODER_MODER0_Msk            (0x3UL << GPIO_MODER_MODER0_Pos)
#define GPIO_MODER_MODER0                GPIO_MODER_MODER0_Msk
#define GPIO_OTYPER_OT_0                (0x00000001U)
#define GPIO_OSPEEDER_OSPEEDR0_Pos      (0U)
#define GPIO_OSPEEDER_OSPEEDR0_Msk      (0x3UL << GPIO_OSPEEDER_OSPEEDR0_Pos)
#define GPIO_OSPEEDER_OSPEEDR0           GPIO_OSPEEDER_OSPEEDR0_Msk
#define GPIO_PUPDR_PUPDR0_Pos            (0U)
#define GPIO_PUPDR_PUPDR0_Msk            (0x3UL << GPIO_PUPDR_PUPDR0_Pos)
#define GPIO_PUPDR_PUPDR0                GPIO_PUPDR_PUPDR0_Msk

/* SPI macros */
#define IS_SPI_ALL_INSTANCE(INSTANCE) ((INSTANCE) == SPI1)
#define IS_SPI_MODE(MODE) (((MODE) == SPI_MODE_SLAVE) || \
                          ((MODE) == SPI_MODE_MASTER))
#define IS_SPI_DIRECTION(DIRECTION) (((DIRECTION) == SPI_DIRECTION_2LINES) || \
                                    ((DIRECTION) == SPI_DIRECTION_2LINES_RXONLY) || \
                                    ((DIRECTION) == SPI_DIRECTION_1LINE))
#define IS_SPI_DATASIZE(DATASIZE) (((DATASIZE) == SPI_DATASIZE_8BIT) || \
                                  ((DATASIZE) == SPI_DATASIZE_16BIT))
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_POLARITY_LOW) || \
                          ((CPOL) == SPI_POLARITY_HIGH))
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_PHASE_1EDGE) || \
                          ((CPHA) == SPI_PHASE_2EDGE))
#define IS_SPI_NSS(NSS) (((NSS) == SPI_NSS_SOFT) || \
                        ((NSS) == SPI_NSS_HARD_INPUT) || \
                        ((NSS) == SPI_NSS_HARD_OUTPUT))
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER) (((PRESCALER) == SPI_BAUDRATEPRESCALER_2) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_4) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_8) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_16) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_32) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_64) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_128) || \
                                             ((PRESCALER) == SPI_BAUDRATEPRESCALER_256))
#define IS_SPI_FIRST_BIT(BIT) (((BIT) == SPI_FIRSTBIT_MSB) || \
                              ((BIT) == SPI_FIRSTBIT_LSB))

/* HAL version */
#define __WB32L003_HAL_VERSION_MAIN   (0x01U)
#define __WB32L003_HAL_VERSION_SUB1   (0x00U)
#define __WB32L003_HAL_VERSION_SUB2   (0x00U)
#define __WB32L003_HAL_VERSION_RC     (0x00U)
#define __WB32L003_HAL_VERSION         ((__WB32L003_HAL_VERSION_MAIN << 24U) |\
                                       (__WB32L003_HAL_VERSION_SUB1 << 16U) |\
                                       (__WB32L003_HAL_VERSION_SUB2 << 8U)  |\
                                       (__WB32L003_HAL_VERSION_RC))

/* HAL SPI States */
#define HAL_SPI_STATE_RESET              0x00000000U
#define HAL_SPI_STATE_READY              0x00000001U
#define HAL_SPI_STATE_BUSY               0x00000002U
#define HAL_SPI_STATE_BUSY_TX            0x00000003U
#define HAL_SPI_STATE_BUSY_RX            0x00000004U
#define HAL_SPI_STATE_BUSY_TX_RX         0x00000005U
#define HAL_SPI_STATE_ERROR              0x00000006U
#define HAL_SPI_STATE_ABORT              0x00000007U

/* HAL SPI Error Codes */
#define HAL_SPI_ERROR_NONE               0x00000000U
#define HAL_SPI_ERROR_MODF               0x00000001U
#define HAL_SPI_ERROR_CRC                0x00000002U
#define HAL_SPI_ERROR_OVR                0x00000004U
#define HAL_SPI_ERROR_FRE                0x00000008U
#define HAL_SPI_ERROR_DMA                0x00000010U
#define HAL_SPI_ERROR_FLAG               0x00000020U
#define HAL_SPI_ERROR_ABORT              0x00000040U

#define __HAL_SPI_CLEAR_OVRFLAG(__HANDLE__) do { \
                                              __IO uint32_t tmpreg_ovr = 0x00U; \
                                              tmpreg_ovr = ((SPI_TypeDef *)((__HANDLE__)->Instance))->DR; \
                                              tmpreg_ovr = ((SPI_TypeDef *)((__HANDLE__)->Instance))->SR; \
                                              UNUSED(tmpreg_ovr); \
                                            } while(0U)

/* ADC States and errors */
#define HAL_ADC_STATE_RESET             0x00000000U
#define HAL_ADC_STATE_READY             0x00000001U
#define HAL_ADC_STATE_BUSY_INTERNAL     0x00000002U
#define HAL_ADC_STATE_BUSY              0x00000003U
#define HAL_ADC_STATE_BUSY_REG          0x00000004U
#define HAL_ADC_STATE_BUSY_INJ          0x00000005U
#define HAL_ADC_STATE_BUSY_INJ_REG      0x00000006U
#define HAL_ADC_STATE_TIMEOUT           0x00000007U
#define HAL_ADC_STATE_ERROR_INTERNAL    0x00000008U
#define HAL_ADC_STATE_ERROR_CONFIG      0x00000009U
#define HAL_ADC_STATE_ERROR_DMA         0x0000000AU

#define HAL_ADC_ERROR_NONE              0x00U
#define HAL_ADC_ERROR_INTERNAL          0x01U
#define HAL_ADC_ERROR_OVR               0x02U
#define HAL_ADC_ERROR_DMA               0x04U

#define IS_ADC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == ADC1)
#define IS_ADC_CHANNEL(CHANNEL) (((CHANNEL) >= ADC_CHANNEL_0) && ((CHANNEL) <= ADC_CHANNEL_17))
#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SAMPLETIME_1CYCLE_5) || \
                                 ((TIME) == ADC_SAMPLETIME_7CYCLES_5) || \
                                 ((TIME) == ADC_SAMPLETIME_13CYCLES_5) || \
                                 ((TIME) == ADC_SAMPLETIME_28CYCLES_5) || \
                                 ((TIME) == ADC_SAMPLETIME_41CYCLES_5) || \
                                 ((TIME) == ADC_SAMPLETIME_55CYCLES_5) || \
                                 ((TIME) == ADC_SAMPLETIME_71CYCLES_5) || \
                                 ((TIME) == ADC_SAMPLETIME_239CYCLES_5))

/* TIM macros */
#define IS_TIM_INSTANCE(INSTANCE) (((INSTANCE) == TIM1)  || \
                                  ((INSTANCE) == TIM2)  || \
                                  ((INSTANCE) == TIM3)  || \
                                  ((INSTANCE) == TIM14) || \
                                  ((INSTANCE) == TIM16) || \
                                  ((INSTANCE) == TIM17))

#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                                       ((INSTANCE) == TIM2) || \
                                                       ((INSTANCE) == TIM3))

#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE) (((INSTANCE) == TIM1)  || \
                                                 ((INSTANCE) == TIM2)  || \
                                                 ((INSTANCE) == TIM3)  || \
                                                 ((INSTANCE) == TIM14) || \
                                                 ((INSTANCE) == TIM16) || \
                                                 ((INSTANCE) == TIM17))

#define IS_TIM_REPETITION_COUNTER_INSTANCE(INSTANCE) ((INSTANCE) == TIM1)

#define IS_TIM_CCXN_INSTANCE(INSTANCE, CHANNEL) ((((INSTANCE) == TIM1) && \
                                                 (((CHANNEL) == TIM_CHANNEL_1) || \
                                                  ((CHANNEL) == TIM_CHANNEL_2) || \
                                                  ((CHANNEL) == TIM_CHANNEL_3))))

#define IS_TIM_BREAK_INSTANCE(INSTANCE) ((INSTANCE) == TIM1)

#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) \
    ((((INSTANCE) == TIM1) && \
     (((CHANNEL) == TIM_CHANNEL_1) || \
      ((CHANNEL) == TIM_CHANNEL_2) || \
      ((CHANNEL) == TIM_CHANNEL_3) || \
      ((CHANNEL) == TIM_CHANNEL_4))) \
    || \
    (((INSTANCE) == TIM2) && \
     (((CHANNEL) == TIM_CHANNEL_1) || \
      ((CHANNEL) == TIM_CHANNEL_2) || \
      ((CHANNEL) == TIM_CHANNEL_3) || \
      ((CHANNEL) == TIM_CHANNEL_4))) \
    || \
    (((INSTANCE) == TIM3) && \
     (((CHANNEL) == TIM_CHANNEL_1) || \
      ((CHANNEL) == TIM_CHANNEL_2) || \
      ((CHANNEL) == TIM_CHANNEL_3) || \
      ((CHANNEL) == TIM_CHANNEL_4))) \
    || \
    (((INSTANCE) == TIM14) && \
     (((CHANNEL) == TIM_CHANNEL_1))) \
    || \
    (((INSTANCE) == TIM16) && \
     (((CHANNEL) == TIM_CHANNEL_1))) \
    || \
    (((INSTANCE) == TIM17) && \
     (((CHANNEL) == TIM_CHANNEL_1))))

#define IS_TIM_COUNTER_MODE(MODE) (((MODE) == TIM_COUNTERMODE_UP)              || \
                                  ((MODE) == TIM_COUNTERMODE_DOWN)            || \
                                  ((MODE) == TIM_COUNTERMODE_CENTERALIGNED1)  || \
                                  ((MODE) == TIM_COUNTERMODE_CENTERALIGNED2)  || \
                                  ((MODE) == TIM_COUNTERMODE_CENTERALIGNED3))

#define IS_TIM_CLOCKDIVISION_DIV(DIV) (((DIV) == TIM_CLOCKDIVISION_DIV1) || \
                                      ((DIV) == TIM_CLOCKDIVISION_DIV2) || \
                                      ((DIV) == TIM_CLOCKDIVISION_DIV4))

#define IS_TIM_AUTORELOAD_PRELOAD(PRELOAD) (((PRELOAD) == TIM_AUTORELOAD_PRELOAD_DISABLE) || \
                                           ((PRELOAD) == TIM_AUTORELOAD_PRELOAD_ENABLE))

#define IS_TIM_CHANNELS(CHANNEL) (((CHANNEL) == TIM_CHANNEL_1) || \
                                 ((CHANNEL) == TIM_CHANNEL_2) || \
                                 ((CHANNEL) == TIM_CHANNEL_3) || \
                                 ((CHANNEL) == TIM_CHANNEL_4) || \
                                 ((CHANNEL) == TIM_CHANNEL_ALL))

#define IS_TIM_PWM_MODE(MODE) (((MODE) == TIM_OCMODE_PWM1) || \
                              ((MODE) == TIM_OCMODE_PWM2))

#define IS_TIM_OC_POLARITY(POLARITY) (((POLARITY) == TIM_OCPOLARITY_HIGH) || \
                                     ((POLARITY) == TIM_OCPOLARITY_LOW))

#define IS_TIM_FAST_STATE(STATE) (((STATE) == TIM_OCFAST_DISABLE) || \
                                 ((STATE) == TIM_OCFAST_ENABLE))

#define IS_TIM_OCN_POLARITY(POLARITY) (((POLARITY) == TIM_OCNPOLARITY_HIGH) || \
                                      ((POLARITY) == TIM_OCNPOLARITY_LOW))

#define IS_TIM_OCIDLE_STATE(STATE) (((STATE) == TIM_OCIDLESTATE_SET) || \
                                   ((STATE) == TIM_OCIDLESTATE_RESET))

#define IS_TIM_OCNIDLE_STATE(STATE) (((STATE) == TIM_OCNIDLESTATE_SET) || \
                                    ((STATE) == TIM_OCNIDLESTATE_RESET))

#define TIM_OCNPOLARITY_HIGH            0x00000000U
#define TIM_OCNPOLARITY_LOW             0x00000008U
#define TIM_OCIDLESTATE_SET             0x00000100U
#define TIM_OCIDLESTATE_RESET           0x00000000U
#define TIM_OCNIDLESTATE_SET            0x00000200U
#define TIM_OCNIDLESTATE_RESET          0x00000000U

#define TIM_CCx_ENABLE                  0x00000001U
#define TIM_CCx_DISABLE                 0x00000000U

#define __HAL_TIM_ENABLE(__HANDLE__)    (((TIM_TypeDef *)((__HANDLE__)->Instance))->CR1|=(TIM_CR1_CEN))

/* TIM register definitions */
#define TIM_CCMR1_CC1S_Pos          (0U)
#define TIM_CCMR1_CC1S_Msk          (0x3UL << TIM_CCMR1_CC1S_Pos)
#define TIM_CCMR1_CC1S              TIM_CCMR1_CC1S_Msk
#define TIM_CCMR1_OC1PE_Pos         (3U)
#define TIM_CCMR1_OC1PE_Msk         (0x1UL << TIM_CCMR1_OC1PE_Pos)
#define TIM_CCMR1_OC1PE             TIM_CCMR1_OC1PE_Msk
#define TIM_CCMR1_OC2M_Pos          (12U)
#define TIM_CCMR1_OC2M_Msk          (0x7UL << TIM_CCMR1_OC2M_Pos)
#define TIM_CCMR1_OC2M              TIM_CCMR1_OC2M_Msk
#define TIM_CCMR1_CC2S_Pos          (8U)
#define TIM_CCMR1_CC2S_Msk          (0x3UL << TIM_CCMR1_CC2S_Pos)
#define TIM_CCMR1_CC2S              TIM_CCMR1_CC2S_Msk
#define TIM_CCMR1_OC2PE_Pos         (11U)
#define TIM_CCMR1_OC2PE_Msk         (0x1UL << TIM_CCMR1_OC2PE_Pos)
#define TIM_CCMR1_OC2PE             TIM_CCMR1_OC2PE_Msk
#define TIM_CCMR1_OC2FE_Pos         (10U)
#define TIM_CCMR1_OC2FE_Msk         (0x1UL << TIM_CCMR1_OC2FE_Pos)
#define TIM_CCMR1_OC2FE             TIM_CCMR1_OC2FE_Msk

#define TIM_CCER_CC2E_Pos           (4U)
#define TIM_CCER_CC2E_Msk           (0x1UL << TIM_CCER_CC2E_Pos)
#define TIM_CCER_CC2E               TIM_CCER_CC2E_Msk
#define TIM_CCER_CC2P_Pos           (5U)
#define TIM_CCER_CC2P_Msk           (0x1UL << TIM_CCER_CC2P_Pos)
#define TIM_CCER_CC2P               TIM_CCER_CC2P_Msk
#define TIM_CCER_CC1NE_Pos          (2U)
#define TIM_CCER_CC1NE_Msk          (0x1UL << TIM_CCER_CC1NE_Pos)
#define TIM_CCER_CC1NE              TIM_CCER_CC1NE_Msk
#define TIM_CCER_CC1NP_Pos          (3U)
#define TIM_CCER_CC1NP_Msk          (0x1UL << TIM_CCER_CC1NP_Pos)
#define TIM_CCER_CC1NP              TIM_CCER_CC1NP_Msk
#define TIM_CCER_CC2NE_Pos          (6U)
#define TIM_CCER_CC2NE_Msk          (0x1UL << TIM_CCER_CC2NE_Pos)
#define TIM_CCER_CC2NE              TIM_CCER_CC2NE_Msk
#define TIM_CCER_CC2NP_Pos          (7U)
#define TIM_CCER_CC2NP_Msk          (0x1UL << TIM_CCER_CC2NP_Pos)
#define TIM_CCER_CC2NP              TIM_CCER_CC2NP_Msk

#define TIM_CR2_OIS1_Pos            (0U)
#define TIM_CR2_OIS1_Msk            (0x1UL << TIM_CR2_OIS1_Pos)
#define TIM_CR2_OIS1                TIM_CR2_OIS1_Msk
#define TIM_CR2_OIS1N_Pos           (1U)
#define TIM_CR2_OIS1N_Msk           (0x1UL << TIM_CR2_OIS1N_Pos)
#define TIM_CR2_OIS1N               TIM_CR2_OIS1N_Msk
#define TIM_CR2_OIS2_Pos            (2U)
#define TIM_CR2_OIS2_Msk            (0x1UL << TIM_CR2_OIS2_Pos)
#define TIM_CR2_OIS2                TIM_CR2_OIS2_Msk
#define TIM_CR2_OIS2N_Pos           (3U)
#define TIM_CR2_OIS2N_Msk           (0x1UL << TIM_CR2_OIS2N_Pos)
#define TIM_CR2_OIS2N               TIM_CR2_OIS2N_Msk

#define TIM_EGR_UG_Pos              (0U)
#define TIM_EGR_UG_Msk              (0x1UL << TIM_EGR_UG_Pos)
#define TIM_EGR_UG                  TIM_EGR_UG_Msk

/* PWR macros */
#define IS_PWR_WAKEUP_PIN(PIN) (((PIN) == PWR_WAKEUP_PIN1) || \
                               ((PIN) == PWR_WAKEUP_PIN2))

#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_MAINREGULATOR_ON) || \
                                    ((REGULATOR) == PWR_LOWPOWERREGULATOR_ON))

#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPENTRY_WFI) || \
                                  ((ENTRY) == PWR_SLEEPENTRY_WFE))

#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPENTRY_WFI) || \
                                 ((ENTRY) == PWR_STOPENTRY_WFE))

/* Cortex-M0+ SCB register definitions */
#define SCB_SCR_SLEEPDEEP_Pos       2U
#define SCB_SCR_SLEEPDEEP_Msk       (1UL << SCB_SCR_SLEEPDEEP_Pos)
#define SCB_SCR_SLEEPONEXIT_Pos     1U
#define SCB_SCR_SLEEPONEXIT_Msk     (1UL << SCB_SCR_SLEEPONEXIT_Pos)
#define SCB_SCR_SEVONPEND_Pos       4U
#define SCB_SCR_SEVONPEND_Msk       (1UL << SCB_SCR_SEVONPEND_Pos)

/* Flash registers */
typedef struct
{
  __IO uint32_t ACR;
  __IO uint32_t KEYR;
  __IO uint32_t OPTKEYR;
  __IO uint32_t SR;
  __IO uint32_t CR;
  __IO uint32_t AR;
  __IO uint32_t RESERVED;
  __IO uint32_t OBR;
  __IO uint32_t WRPR;
} FLASH_TypeDef;

#define FLASH_ACR_LATENCY_Pos        (0U)
#define FLASH_ACR_LATENCY_Msk        (0x7UL << FLASH_ACR_LATENCY_Pos)
#define FLASH_ACR_LATENCY            FLASH_ACR_LATENCY_Msk

#define __HAL_FLASH_GET_LATENCY()    (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY))
#define __HAL_FLASH_SET_LATENCY(__LATENCY__)    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (__LATENCY__))

#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_LATENCY_0) || \
                                  ((LATENCY) == FLASH_LATENCY_1))

/* RCC additional macros */
#define __HAL_RCC_GET_SYSCLK_SOURCE() (RCC->CFGR & RCC_CFGR_SWS)
#define __HAL_RCC_GET_PLL_OSCSOURCE() (RCC->CFGR & RCC_CFGR_PLLSRC)
#define __HAL_RCC_GET_FLAG(FLAG) ((((FLAG) >> 5U) == 1U) ? (RCC->CR & (1U << ((FLAG) & 0x1FU))) : \
                                  (((FLAG) >> 5U) == 2U) ? (RCC->BDCR & (1U << ((FLAG) & 0x1FU))) : \
                                  (RCC->CSR & (1U << ((FLAG) & 0x1FU))))

#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_LSERDY                  ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)

#define RCC_SYSCLKSOURCE_STATUS_HSI      RCC_CFGR_SWS_HSI
#define RCC_SYSCLKSOURCE_STATUS_HSE      RCC_CFGR_SWS_HSE
#define RCC_SYSCLKSOURCE_STATUS_PLLCLK   RCC_CFGR_SWS_PLL

#define __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(VALUE) (MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, (VALUE) << RCC_CR_HSITRIM_Pos))
#define __HAL_RCC_LSI_ENABLE()           SET_BIT(RCC->CSR, RCC_CSR_LSION)
#define __HAL_RCC_LSI_DISABLE()          CLEAR_BIT(RCC->CSR, RCC_CSR_LSION)
#define __HAL_RCC_LSE_CONFIG(STATE)      do { \
                                           if((STATE) == RCC_LSE_ON) { \
                                             SET_BIT(RCC->BDCR, RCC_BDCR_LSEON); \
                                           } else if((STATE) == RCC_LSE_OFF) { \
                                             CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON); \
                                             CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP); \
                                           } else if((STATE) == RCC_LSE_BYPASS) { \
                                             SET_BIT(RCC->BDCR, RCC_BDCR_LSEON); \
                                             SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP); \
                                           } \
                                         } while(0U)
#define __HAL_RCC_PWR_IS_CLK_DISABLED()  ((RCC->APB1ENR & RCC_APB1ENR_PWREN) == 0U)
#define __HAL_RCC_PLL_CONFIG(SOURCE, MUL) MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL, \
                                                     (SOURCE) | ((MUL) << RCC_CFGR_PLLMUL_Pos))
#define __HAL_RCC_SYSCLK_CONFIG(SOURCE)  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (SOURCE))

#define RCC_CR_HSITRIM_Pos               (3U)
#define RCC_CR_HSITRIM_Msk               (0x1FUL << RCC_CR_HSITRIM_Pos)
#define RCC_CR_HSITRIM                   RCC_CR_HSITRIM_Msk
#define RCC_CSR_LSION_Pos                (0U)
#define RCC_CSR_LSION_Msk                (0x1UL << RCC_CSR_LSION_Pos)
#define RCC_CSR_LSION                    RCC_CSR_LSION_Msk
#define RCC_CSR_LSIRDY_Pos               (1U)
#define RCC_CSR_LSIRDY_Msk               (0x1UL << RCC_CSR_LSIRDY_Pos)
#define RCC_CSR_LSIRDY                   RCC_CSR_LSIRDY_Msk
#define RCC_BDCR_LSEON_Pos               (0U)
#define RCC_BDCR_LSEON_Msk               (0x1UL << RCC_BDCR_LSEON_Pos)
#define RCC_BDCR_LSEON                   RCC_BDCR_LSEON_Msk
#define RCC_BDCR_LSERDY_Pos              (1U)
#define RCC_BDCR_LSERDY_Msk              (0x1UL << RCC_BDCR_LSERDY_Pos)
#define RCC_BDCR_LSERDY                  RCC_BDCR_LSERDY_Msk
#define RCC_BDCR_LSEBYP_Pos              (2U)
#define RCC_BDCR_LSEBYP_Msk              (0x1UL << RCC_BDCR_LSEBYP_Pos)
#define RCC_BDCR_LSEBYP                  RCC_BDCR_LSEBYP_Msk
#define RCC_CFGR_PLLSRC_HSE_PREDIV       0x00010000U
#define RCC_CFGR2_PREDIV_Pos             (0U)
#define RCC_CFGR2_PREDIV_Msk             (0xFUL << RCC_CFGR2_PREDIV_Pos)
#define RCC_CFGR2_PREDIV                 RCC_CFGR2_PREDIV_Msk
#define RCC_APB1RSTR_PWRRST_Pos          (28U)
#define RCC_APB1RSTR_PWRRST_Msk          (0x1UL << RCC_APB1RSTR_PWRRST_Pos)
#define RCC_APB1RSTR_PWRRST              RCC_APB1RSTR_PWRRST_Msk

#define IS_RCC_OSCILLATORTYPE(TYPE) ((TYPE) <= 0xFU)
#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || \
                        ((HSE) == RCC_HSE_ON) || \
                        ((HSE) == RCC_HSE_BYPASS))
#define IS_RCC_HSI(HSI) (((HSI) == RCC_HSI_OFF) || \
                        ((HSI) == RCC_HSI_ON))
#define IS_RCC_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1FU)
#define IS_RCC_LSI(LSI) (((LSI) == RCC_LSI_OFF) || \
                        ((LSI) == RCC_LSI_ON))
#define IS_RCC_LSE(LSE) (((LSE) == RCC_LSE_OFF) || \
                        ((LSE) == RCC_LSE_ON) || \
                        ((LSE) == RCC_LSE_BYPASS))
#define IS_RCC_PLL(PLL) (((PLL) == RCC_PLL_NONE) || \
                        ((PLL) == RCC_PLL_OFF) || \
                        ((PLL) == RCC_PLL_ON))
#define IS_RCC_PLLSOURCE(SOURCE) (((SOURCE) == RCC_PLLSOURCE_HSI) || \
                                 ((SOURCE) == RCC_PLLSOURCE_HSE))
#define IS_RCC_PLL_MUL(MUL) (((MUL) == RCC_PLL_MUL2) || \
                            ((MUL) == RCC_PLL_MUL3) || \
                            ((MUL) == RCC_PLL_MUL4) || \
                            ((MUL) == RCC_PLL_MUL5) || \
                            ((MUL) == RCC_PLL_MUL6) || \
                            ((MUL) == RCC_PLL_MUL7) || \
                            ((MUL) == RCC_PLL_MUL8) || \
                            ((MUL) == RCC_PLL_MUL9) || \
                            ((MUL) == RCC_PLL_MUL10) || \
                            ((MUL) == RCC_PLL_MUL11) || \
                            ((MUL) == RCC_PLL_MUL12) || \
                            ((MUL) == RCC_PLL_MUL13) || \
                            ((MUL) == RCC_PLL_MUL14) || \
                            ((MUL) == RCC_PLL_MUL15) || \
                            ((MUL) == RCC_PLL_MUL16))
#define IS_RCC_CLOCKTYPE(TYPE) ((TYPE) <= 0x7U)
#define IS_RCC_HCLK(DIV) (((DIV) == RCC_SYSCLK_DIV1) || \
                         ((DIV) == RCC_SYSCLK_DIV2) || \
                         ((DIV) == RCC_SYSCLK_DIV4) || \
                         ((DIV) == RCC_SYSCLK_DIV8) || \
                         ((DIV) == RCC_SYSCLK_DIV16) || \
                         ((DIV) == RCC_SYSCLK_DIV64) || \
                         ((DIV) == RCC_SYSCLK_DIV128) || \
                         ((DIV) == RCC_SYSCLK_DIV256) || \
                         ((DIV) == RCC_SYSCLK_DIV512))
#define IS_RCC_PCLK(DIV) (((DIV) == RCC_HCLK_DIV1) || \
                         ((DIV) == RCC_HCLK_DIV2) || \
                         ((DIV) == RCC_HCLK_DIV4) || \
                         ((DIV) == RCC_HCLK_DIV8) || \
                         ((DIV) == RCC_HCLK_DIV16))
#define IS_RCC_SYSCLKSOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSOURCE_HSI) || \
                                    ((SOURCE) == RCC_SYSCLKSOURCE_HSE) || \
                                    ((SOURCE) == RCC_SYSCLKSOURCE_PLLCLK))

#define HSE_TIMEOUT_VALUE          100U
#define HSI_TIMEOUT_VALUE          100U
#define LSI_TIMEOUT_VALUE          100U
#define PLL_TIMEOUT_VALUE          100U
#define RCC_DBP_TIMEOUT_VALUE      100U
#define RCC_LSE_TIMEOUT_VALUE      5000U
#define CLOCKSWITCH_TIMEOUT_VALUE  5000U

#define RCC_LSI_OFF                      0x00000000U
#define RCC_LSI_ON                       0x00000001U
#define RCC_LSE_OFF                      0x00000000U
#define RCC_LSE_ON                       0x00000001U
#define RCC_LSE_BYPASS                   0x00000002U

/* Additional macros */
#define HAL_RCC_GetSysClockFreq()        SystemCoreClock

/* NVIC and Cortex macros */
#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PRIORITYGROUP_0) || \
                                      ((GROUP) == NVIC_PRIORITYGROUP_1) || \
                                      ((GROUP) == NVIC_PRIORITYGROUP_2) || \
                                      ((GROUP) == NVIC_PRIORITYGROUP_3) || \
                                      ((GROUP) == NVIC_PRIORITYGROUP_4))

#define IS_NVIC_PREEMPTION_PRIORITY(PRIORITY)  ((PRIORITY) < 16U)
#define IS_NVIC_SUB_PRIORITY(PRIORITY)         ((PRIORITY) < 16U)
#define IS_NVIC_DEVICE_IRQ(IRQ)                ((IRQ) >= (IRQn_Type)0x00U)
#define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SYSTICK_CLKSOURCE_HCLK) || \
                                      ((SOURCE) == SYSTICK_CLKSOURCE_HCLK_DIV8))

/* NVIC functions that are missing */
static inline void NVIC_SetPriorityGrouping(uint32_t PriorityGroup) 
{
  /* Cortex-M0+ does not support priority grouping */
}

static inline uint32_t NVIC_GetPriorityGrouping(void)
{
  /* Cortex-M0+ does not support priority grouping */
  return 0U;
}

static inline void NVIC_SystemReset(void)
{
  __DSB();
  SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);
  __DSB();
  for(;;) { __NOP(); }
}

static inline uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  /* Cortex-M0+ has only 2 priority bits, no sub-priority */
  return PreemptPriority & 0x3U;
}

static inline void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* const pPreemptPriority, uint32_t* const pSubPriority)
{
  /* Cortex-M0+ has only 2 priority bits, no sub-priority */
  *pPreemptPriority = Priority & 0x3U;
  *pSubPriority = 0U;
}

static inline uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
  /* Cortex-M0+ does not have NVIC->IABR */
  return 0U;
}

/* Other required macros */
#define __DSB() __asm volatile ("dsb 0xF":::"memory")
#define __WFI() __asm volatile ("wfi")
#define __WFE() __asm volatile ("wfe")
#define __SEV() __asm volatile ("sev")
#define __NOP() __asm volatile ("nop")

#define SCB_AIRCR_VECTKEY_Pos              16U
#define SCB_AIRCR_SYSRESETREQ_Pos          2U
#define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)

/* System tick macros */
#define SYSTICK_CLKSOURCE_HCLK             0x00000004U
#define IS_TICKFREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ)  || \
                          ((FREQ) == HAL_TICK_FREQ_100HZ) || \
                          ((FREQ) == HAL_TICK_FREQ_1KHZ))

/* Additional RCC register bit definitions if missing */
#define RCC_CFGR_SWS_HSI                   0x00000000U
#define RCC_CFGR_SWS_HSE                   0x00000004U
#define RCC_CFGR_SWS_PLL                   0x00000008U

#define DBGMCU                             ((DBGMCU_TypeDef *) DBGMCU_BASE)

typedef struct
{
  __IO uint32_t IDCODE;
  __IO uint32_t CR;
} DBGMCU_TypeDef;