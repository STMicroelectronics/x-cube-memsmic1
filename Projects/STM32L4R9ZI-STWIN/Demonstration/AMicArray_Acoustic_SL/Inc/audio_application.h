/**
  ******************************************************************************
  * @file    audio_application.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Header for audio_application.c module.
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
#ifndef __AUDIO_APPLICATION_H
#define __AUDIO_APPLICATION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "audio_application.h"
#include "AMic_Array_audio.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio_in.h"
#include "usbd_audio_if.h"
#include "STWIN.h"

/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup ACOUSTIC_SL_Example
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @{
  */


/** @defgroup AUDIO_APPLICATION_Private_Types
  * @{
  */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup AUDIO_APPLICATION_Exported_Defines
  * @{
  */

#define AUDIO_BUFF_SIZE ((AUDIO_IN_SAMPLING_FREQUENCY/1000)*AUDIO_IN_CHANNELS*2)
#define TOP_LEFT_MIC 1
#define TOP_RIGHT_MIC 3
#define BOTTOM_LEFT_MIC 0
#define BOTTOM_RIGHT_MIC 2
#define SIDE 40
#define DIAGONAL 57

/**
  * @}
  */
/* Exported functions ------------------------------------------------------- */
void Audio_Libraries_Init(void);
void Error_Handler(void);
void AudioProcess(void);
void SW_IRQ_Tasks_Init(void);
void SW_Task1_Callback(void);
void SW_Task2_Callback(void);
void SW_Task1_Start(void);
void SW_Task2_Start(void);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */



#endif /* __AUDIO_APPLICATION_H */


