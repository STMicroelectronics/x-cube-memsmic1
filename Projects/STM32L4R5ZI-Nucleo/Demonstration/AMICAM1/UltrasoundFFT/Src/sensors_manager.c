/**
  ******************************************************************************
  * @file    sensors_manager.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file provides a set of functions to handle the sensors
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
#include "sensors_manager.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef hsm_tim;

/* Private function prototypes -----------------------------------------------*/

void SM_TIM_Init(void);

uint32_t reg_after_release = 0;


void SM_Peripheral_Init(void)
{
  SM_TIM_Init();
}

void SM_TIM_Init(void)
{
  SM_TIMx_CLK_ENABLE();
  /* Set TIMx instance */
  hsm_tim.Instance = SM_TIMx;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  hsm_tim.Init.Period            = 0xFFFFFFFF;
  hsm_tim.Init.Prescaler         = 0;
  hsm_tim.Init.ClockDivision     = 0;
  hsm_tim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  hsm_tim.Init.RepetitionCounter = 0;

  if (HAL_TIM_Base_Init(&hsm_tim) != HAL_OK)
  {
    /* Initialization Error */
//    Error_Handler();
  }
}

void SM_TIM_Start(void)
{
  /*##-2- Start the TIM Base generation ####################*/
  if (HAL_TIM_Base_Start(&hsm_tim) != HAL_OK)
  {
    /* Starting Error */
//    Error_Handler();
  }
}

void SM_TIM_Stop(void)
{
  /*##-2- Start the TIM Base generation ####################*/
  if (HAL_TIM_Base_Stop(&hsm_tim) != HAL_OK)
  {
    /* Starting Error */
//    Error_Handler();
  }
  hsm_tim.Instance->CNT = 0;
}

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


