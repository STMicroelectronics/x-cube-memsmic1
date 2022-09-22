/**
  ******************************************************************************
  * @file    amicam1_app.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Header for amicam1_app.c module.
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
#ifndef __AMICAM1_APP_H
#define __AMICAM1_APP_H

#ifdef __cplusplus
extern "C" {
#endif


#include "cmsis_os.h"
#include "threadConfig.h"
#include "sensors_manager.h"
#include "amicam1_audio.h"

#define AMIC_MS                                 (uint32_t)(1)
#define FFT_LEN_AMIC                            (uint32_t)(1024)
#define OVLP                                    (float)(0.25)

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


int32_t AMICAM1_Peripheral_Init(uint32_t AudioFreq, uint32_t ChnlNbrIn);
void AMICAM1_OS_Init(void);
void AMICAM1_Data_Ready(uint8_t *buf, uint16_t size, double timeStamp);
void AMICAM1_Set_State(SM_Sensor_State_t newState);
void AMICAM1_Set_ODR(float newODR);
void AMICAM1_Set_FS(float newFS1, float newFS2);
void AMICAM1_Start(void);
void AMICAM1_Stop(void);


#ifdef __cplusplus
}
#endif

#endif /* __AMICAM1_APP_H */


