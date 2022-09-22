/**
  ******************************************************************************
  * @file           : stm32l4xx_nucleo_bus.h
  * @brief          : source file for the BSP BUS IO driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_nucleo_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4XX_NUCLEO
  * @{
  */

/** @defgroup STM32L4XX_NUCLEO_BUS STM32L4XX_NUCLEO BUS
  * @{
  */


/** @defgroup STM32L4XX_NUCLEO_BUS_Exported_Variables BUS Exported Variables
  * @{
  */

SPI_HandleTypeDef hspi1;
/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_BUS_Private_Variables BUS Private Variables
  * @{
  */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
static uint32_t IsSPI1MspCbValid = 0;
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

static uint32_t SPI1InitCounter = 0;
/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_BUS_Private_FunctionPrototypes  BUS Private Function
  * @{
  */

static void SPI1_MspInit(SPI_HandleTypeDef *hSPI);
static void SPI1_MspDeInit(SPI_HandleTypeDef *hSPI);
#if (USE_CUBEMX_BSP_V2 == 1)
static uint32_t SPI_GetPrescaler(uint32_t clk_src_hz, uint32_t baudrate_mbps);
#endif

/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_LOW_LEVEL_Private_Functions STM32L4XX_NUCLEO LOW LEVEL Private Functions
  * @{
  */

/** @defgroup STM32L4XX_NUCLEO_BUS_Exported_Functions STM32L4XX_NUCLEO_BUS Exported Functions
  * @{
  */

/* BUS IO driver over SPI Peripheral */
/*******************************************************************************
                            BUS OPERATIONS OVER SPI
  *******************************************************************************/
/**
  * @brief  Initializes SPI HAL.
  * @retval BSP status
  */
int32_t BSP_SPI1_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hspi1.Instance  = SPI1;

  if (SPI1InitCounter++ == 0)
  {
    if (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_RESET)
    {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      /* Init the SPI Msp */
      SPI1_MspInit(&hspi1);
#else
      if (IsSPI1MspCbValid == 0U)
      {
        if (BSP_SPI1_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
        {
          return BSP_ERROR_MSP_FAILURE;
        }
      }
#endif
      if (ret == BSP_ERROR_NONE)
      {
        /* Init the SPI */
        if (MX_SPI1_Init(&hspi1) != HAL_OK)
        {
          ret = BSP_ERROR_BUS_FAILURE;
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  DeInitializes SPI HAL.
  * @retval None
  * @retval BSP status
  */
int32_t BSP_SPI1_DeInit(void)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;
  if (SPI1InitCounter > 0)
  {
    if (--SPI1InitCounter == 0)
    {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      SPI1_MspDeInit(&hspi1);
#endif
      /* DeInit the SPI*/
      if (HAL_SPI_DeInit(&hspi1) == HAL_OK)
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}

/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Pointer to data buffer to send
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_SPI_Transmit(&hspi1, pData, Length, BUS_SPI1_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Pointer to data buffer to receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_SPI1_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_SPI_Receive(&hspi1, pData, Length, BUS_SPI1_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

/**
  * @brief  Send and Receive data to/from SPI BUS (Full duplex)
  * @param  pData: Pointer to data buffer to send/receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, Length, BUS_SPI1_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default BSP SPI1 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_SPI1_RegisterDefaultMspCallbacks(void)
{

  __HAL_SPI_RESET_HANDLE_STATE(&hspi1);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_MSPINIT_CB_ID, SPI1_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_MSPDEINIT_CB_ID, SPI1_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsSPI1MspCbValid = 1;

  return BSP_ERROR_NONE;
}

/**
  * @brief BSP SPI1 Bus Msp Callback registering
  * @param Callbacks     pointer to SPI1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_SPI1_RegisterMspCallbacks(BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_SPI_RESET_HANDLE_STATE(&hspi1);

  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  IsSPI1MspCbValid = 1;

  return BSP_ERROR_NONE;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t BSP_GetTick(void)
{
  return HAL_GetTick();
}

/* SPI1 init function */

__weak HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef ret = HAL_OK;
  hspi->Instance = SPI1;
  hspi->Init.Mode = SPI_MODE_MASTER;
  hspi->Init.Direction = SPI_DIRECTION_2LINES;
  hspi->Init.DataSize = SPI_DATASIZE_8BIT;
  hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi->Init.NSS = SPI_NSS_SOFT;
  hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi->Init.TIMode = SPI_TIMODE_DISABLE;
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial = 7;
  hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void SPI1_MspInit(SPI_HandleTypeDef *spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
  /* Enable Peripheral clock */
  __HAL_RCC_SPI1_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  /**SPI1 GPIO Configuration
  PA6     ------> SPI1_MISO
  PA7     ------> SPI1_MOSI
  PA5     ------> SPI1_SCK
    */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
}

static void SPI1_MspDeInit(SPI_HandleTypeDef *spiHandle)
{
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
  /* Peripheral clock disable */
  __HAL_RCC_SPI1_CLK_DISABLE();

  /**SPI1 GPIO Configuration
  PA6     ------> SPI1_MISO
  PA7     ------> SPI1_MOSI
  PA5     ------> SPI1_SCK
    */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
}

#if (USE_CUBEMX_BSP_V2 == 1)
/**
  * @brief  Convert the SPI baudrate into prescaler.
  * @param  clock_src_hz : SPI source clock in HZ.
  * @param  baudrate_mbps : SPI baud rate in mbps.
  * @retval Prescaler divisor
  */
static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps)
{
  uint32_t divisor = 0;
  uint32_t spi_clk = clock_src_hz;
  uint32_t presc = 0;

  static const uint32_t baudrate[] =
  {
    SPI_BAUDRATEPRESCALER_2,
    SPI_BAUDRATEPRESCALER_4,
    SPI_BAUDRATEPRESCALER_8,
    SPI_BAUDRATEPRESCALER_16,
    SPI_BAUDRATEPRESCALER_32,
    SPI_BAUDRATEPRESCALER_64,
    SPI_BAUDRATEPRESCALER_128,
    SPI_BAUDRATEPRESCALER_256,
  };

  while (spi_clk > baudrate_mbps)
  {
    presc = baudrate[divisor];
    if (++divisor > 7)
    {
      break;
    }

    spi_clk = (spi_clk >> 1);
  }

  return presc;
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

