/**
  ******************************************************************************
  * @file    audio_application.h
  * @author  SRA
  * @version v1.3.1
  * @date    17-Jul-2020
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
#include "cube_hal.h"
#include "STWIN_audio.h"

/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup Microphones_Acquisition
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup AUDIO_APPLICATION_Exported_Defines
  * @{
  */


/*Comment this define if you want to configure and start acquisition
depending on USB functionalities implemented by user*/
#define DISABLE_USB_DRIVEN_ACQUISITION


/**
  * @}
  */
/* Exported functions ------------------------------------------------------- */
void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t Device, uint32_t ChnlNbr);
void Start_Acquisition(void);
void Error_Handler(void);
void AudioProcess(void);

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


