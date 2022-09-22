/**
  ******************************************************************************
  * @file    fft_app.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Header for fft_app.c module.
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
#ifndef __FFT_APP_H
#define __FFT_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

#include "cmsis_os.h"
#include "threadConfig.h"

#include "com_manager.h"
#include "usbd_wcid_interface.h"

#include "fft.h"
#include "amicam1_conf.h"


#define FFT_RAW                 0
#define FFT_AVERAGE             1
#define FFT_WEIGHTED_AVERAGE    2

/* Select one from the list above */
#define FFT_OUTPUT      FFT_RAW


extern osThreadId FFT_Thread_Id;

void FFT_OS_Init(void);

void FFT_Lib_Data_Input(int16_t *data, uint32_t len);


#ifdef __cplusplus
}
#endif

#endif /* __FFT_APP_H */


