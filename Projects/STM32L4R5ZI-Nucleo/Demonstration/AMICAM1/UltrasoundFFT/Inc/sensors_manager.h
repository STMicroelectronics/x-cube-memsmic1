/**
  ******************************************************************************
  * @file    sensors_manager.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Header for sensors_manager.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORS_MANAGER_H
#define __SENSORS_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

typedef enum
{
  SM_SENSOR_STATE_RUNNING,
  SM_SENSOR_STATE_SUSPENDING,
  SM_SENSOR_STATE_SUSPENDED,
  SM_SENSOR_STATE_INITIALIZING,
} SM_Sensor_State_t;

typedef struct
{
  float ODR;
  float FS[4]; /* size = 4, in case of combo devices */
  uint8_t subSensorActive[4];
} SM_Init_Param_t;

extern TIM_HandleTypeDef hsm_tim;

typedef struct
{
  uint8_t WhoAmI;
  uint8_t I2C_address;
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
  osSemaphoreId *sem;
} sensor_handle_t;

typedef struct
{
  sensor_handle_t *sensorHandler;
  uint8_t isRead;
  uint8_t *dataPtr;
  uint8_t regAddr;
  uint16_t readSize;
} SM_Message_t;



#define SM_TIMx                           TIM5
#define SM_TIMx_CLK_ENABLE()              __HAL_RCC_TIM5_CLK_ENABLE()

void SM_Peripheral_Init(void);
void SM_Error_Handler(void);
void SM_TIM_Init(void);
void SM_TIM_Start(void);
void SM_TIM_Stop(void);

#ifdef __cplusplus
}
#endif

#endif /* __SENSORS_MANAGER_H */


