/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  SRA
  * @version v1.3.1
  * @date    17-Jul-2020
  * @brief   General low level driver configuration
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
#ifndef __USBD_CONF_H
#define __USBD_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern PCD_HandleTypeDef hpcd;
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Common Config */
#define USBD_MAX_NUM_INTERFACES               2
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 200
#define USBD_SUPPORT_USER_STRING              0
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      0

/* AUDIO Class Config */
#define USBD_AUDIO_FREQ                       48000
#define AUDIO_TOTAL_IF_NUM              0x02

/* Exported macro ------------------------------------------------------------*/
/* Memory management macros */


#define USBD_memset               memset
#define USBD_memcpy               memcpy

#ifdef USE_STATIC_ALLOCATION

#define USBD_free                 USBD_static_free
#define USBD_malloc               USBD_static_malloc
#define MAX_STATIC_ALLOC_SIZE     1600/4
#else
#define USBD_free                 free
#define USBD_malloc               malloc
#define MAX_STATIC_ALLOC_SIZE     4
#endif  /* USE_STATIC_ALLOCATION */

/* DEBUG macros */
#if (USBD_DEBUG_LEVEL > 0)
#define USBD_UsrLog(...)   printf(__VA_ARGS__);\
  printf("\n");
#else
#define USBD_UsrLog(...)
#endif  /* (USBD_DEBUG_LEVEL > 0) */

#if (USBD_DEBUG_LEVEL > 1)

#define  USBD_ErrLog(...)   printf("ERROR: ") ;\
  printf(__VA_ARGS__);\
  printf("\n");
#else
#define USBD_ErrLog(...)
#endif  /* (USBD_DEBUG_LEVEL > 1) */

#if (USBD_DEBUG_LEVEL > 2)
#define  USBD_DbgLog(...)   printf("DEBUG : ") ;\
  printf(__VA_ARGS__);\
  printf("\n");
#else
#define USBD_DbgLog(...)
#endif  /* (USBD_DEBUG_LEVEL > 2) */

/* Exported functions ------------------------------------------------------- */

#endif /* __USBD_CONF_H */


