/**
  ******************************************************************************
  * @file    amicam1_conf.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file contains definitions for the MEMSMIC1 applications.
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
#ifndef AMICAM1_CONF_H__
#define AMICAM1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_bus.h"
#include "stm32l4xx_nucleo_errno.h"


/* The N_MS value defines the number of millisecond to be processed at each AudioProcess call,
that must be consistent with the N_MS_PER_INTERRUPT defined in the audio driver
(amicam1_audio.h).
The default value of the N_MS_PER_INTERRUPT directive in the driver is set to 1,
for backward compatibility: leaving this values as it is allows to avoid any
modification in the application layer developed with the older versions of the driver */

#define N_MS        (N_MS_PER_INTERRUPT)

#define AUDIO_IN_CHANNELS   1
#define AUDIO_IN_SAMPLING_FREQUENCY 192000
#define AUDIO_IN_INSTANCE           AUDIO_IN_INSTANCE_ADC_EXT
#define AUDIO_IN_MICS               AUDIO_IN_ANALOG_MIC1

#define AUDIO_IN_NOT_USED               0
#define AUDIO_IN_BUFFER_SIZE            DEFAULT_AUDIO_IN_BUFFER_SIZE
#define AUDIO_VOLUME_INPUT              64U
#define AMICAM1_AUDIO_IN_IT_PRIORITY    6U




#define AD1974_BUS_Init                 BSP_SPI1_Init
#define AD1974_BUS_DeInit               BSP_SPI1_DeInit
#define AD1974_BUS_Recv                 BSP_SPI1_Recv
#define AD1974_BUS_Send                 BSP_SPI1_Send
#define AD1974_BUS_SendRecv             BSP_SPI1_SendRecv
#define AD1974_BUS_CS_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define AD1974_BUS_CS_PORT              GPIOC
#define AD1974_BUS_CS_PIN               GPIO_PIN_9

#ifdef __cplusplus
}
#endif

#endif /* AMICAM1_CONF_H__*/



