/**
  ******************************************************************************
  * @file    main.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file contains all the functions prototypes for the main.c
  *          file.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "STWIN_debug_pins.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define AMIC_ARRAY_THREAD_PRIO          osPriorityNormal
#define FFT_THREAD_PRIO               osPriorityNormal

/*
Set configUSE_APPLICATION_TASK_TAG to 1 in FreeRTOSConfig.h to enable the Task debugging mode.
Each time a task is executing the corresponding pin is SET otherwise is RESET
 */

// IDLE Task Pin cannot be changed
#define TASK_IDLE_DEBUG_PIN           DEBUG_PIN7
#define TASK_AMIC_ARRAY_DEBUG_PIN     DEBUG_PIN17


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */



