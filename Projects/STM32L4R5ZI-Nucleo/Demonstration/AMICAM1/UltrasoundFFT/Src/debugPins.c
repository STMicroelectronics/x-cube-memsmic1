/**
  ******************************************************************************
  * @file    debugPins.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file provides a set of functions for debugging purpose
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
#include "debugPins.h"

const uint16_t  DEBUG_PIN[DEBUG_PINn] =  {DEBUG_PIN_7_PIN,
                                          DEBUG_PIN_8_PIN,
                                          DEBUG_PIN_9_PIN,
                                          DEBUG_PIN_10_PIN,
                                          DEBUG_PIN_11_PIN,
                                          DEBUG_PIN_12_PIN,
                                          DEBUG_PIN_13_PIN,
                                          DEBUG_PIN_14_PIN,
                                          DEBUG_PIN_17_PIN,
                                          DEBUG_PIN_18_PIN,
                                          DEBUG_PIN_19_PIN,
                                          DEBUG_PIN_20_PIN
                                         };

GPIO_TypeDef  *DEBUG_PIN_PORT[DEBUG_PINn] =   {DEBUG_PIN_7_GPIO_PORT,
                                               DEBUG_PIN_8_GPIO_PORT,
                                               DEBUG_PIN_9_GPIO_PORT,
                                               DEBUG_PIN_10_GPIO_PORT,
                                               DEBUG_PIN_11_GPIO_PORT,
                                               DEBUG_PIN_12_GPIO_PORT,
                                               DEBUG_PIN_13_GPIO_PORT,
                                               DEBUG_PIN_14_GPIO_PORT,
                                               DEBUG_PIN_17_GPIO_PORT,
                                               DEBUG_PIN_18_GPIO_PORT,
                                               DEBUG_PIN_19_GPIO_PORT,
                                               DEBUG_PIN_20_GPIO_PORT
                                              };


/**
  * @brief  Configure general purpose debug pins on STWIN STMOD+ connector
  *         Pin name also identifies the pin number on STMOD+.
  * @param  None
  * @retval None
  */
void DEBUG_PIN_Init_All(void)
{
  DEBUG_PIN_Off(DEBUG_PIN7);
  DEBUG_PIN_Off(DEBUG_PIN8);
  DEBUG_PIN_Off(DEBUG_PIN9);
  DEBUG_PIN_Off(DEBUG_PIN10);
  DEBUG_PIN_Off(DEBUG_PIN11);
  DEBUG_PIN_Off(DEBUG_PIN12);
  DEBUG_PIN_Off(DEBUG_PIN13);
  DEBUG_PIN_Off(DEBUG_PIN14);
  DEBUG_PIN_Off(DEBUG_PIN17);
  DEBUG_PIN_Off(DEBUG_PIN18);
  DEBUG_PIN_Off(DEBUG_PIN19);
  DEBUG_PIN_Off(DEBUG_PIN20);

  DEBUG_PIN_Init(DEBUG_PIN7); // Pin7 is for IDLE thread
  DEBUG_PIN_Init(DEBUG_PIN8);
  DEBUG_PIN_Init(DEBUG_PIN9);
  DEBUG_PIN_Init(DEBUG_PIN10);
  DEBUG_PIN_Init(DEBUG_PIN11);
  DEBUG_PIN_Init(DEBUG_PIN12);
  DEBUG_PIN_Init(DEBUG_PIN13);
  DEBUG_PIN_Init(DEBUG_PIN14);
  DEBUG_PIN_Init(DEBUG_PIN17);
  DEBUG_PIN_Init(DEBUG_PIN18);
  DEBUG_PIN_Init(DEBUG_PIN19);
  DEBUG_PIN_Init(DEBUG_PIN20);
}


void DEBUG_PIN_Init(Debug_Pin_TypeDef Pin)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO clock */
  DEBUG_PINx_GPIO_CLK_ENABLE(Pin);

  /* Configure the GPIO pin */
  GPIO_InitStructure.Pin = DEBUG_PIN[Pin];
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_WritePin(DEBUG_PIN_PORT[Pin], GPIO_InitStructure.Pin, GPIO_PIN_RESET);

  HAL_GPIO_Init(DEBUG_PIN_PORT[Pin], &GPIO_InitStructure);
}

void  DEBUG_PIN_DeInit(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_DeInit(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin]);
  DEBUG_PINx_GPIO_CLK_DISABLE(Pin);
}

void DEBUG_PIN_On(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_WritePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin], GPIO_PIN_SET);
}

void DEBUG_PIN_Off(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_WritePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin], GPIO_PIN_RESET);
}

void DEBUG_PIN_Toggle(Debug_Pin_TypeDef Pin)
{
  HAL_GPIO_TogglePin(DEBUG_PIN_PORT[Pin], DEBUG_PIN[Pin]);
}


