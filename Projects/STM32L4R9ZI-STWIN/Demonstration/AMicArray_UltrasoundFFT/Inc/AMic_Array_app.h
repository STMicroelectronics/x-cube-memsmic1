/**
  ******************************************************************************
  * @file    AMic_Array_app.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Header for AMic_Array_app.c module.
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
#ifndef __AMIC_ARRAY_APP_H
#define __AMIC_ARRAY_APP_H

#ifdef __cplusplus
extern "C" {
#endif


#include "cmsis_os.h"
#include "threadConfig.h"
#include "sensors_manager.h"
#include "AMic_Array_audio.h"
#include "STWIN.h"

#define AMIC_MS                                 (uint32_t)(1)
#define FFT_LEN_AMIC                            (uint32_t)(1024)
#define OVLP                                    (float)(0.25)

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


int32_t AMIC_ARRAY_Peripheral_Init(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
void AMIC_ARRAY_OS_Init(void);
void AMIC_ARRAY_Data_Ready(uint8_t *buf, uint16_t size, double timeStamp);
void AMIC_ARRAY_Set_State(SM_Sensor_State_t newState);
void AMIC_ARRAY_Set_ODR(float newODR);
void AMIC_ARRAY_Set_FS(float newFS1, float newFS2);
void AMIC_ARRAY_Start(void);
void AMIC_ARRAY_Stop(void);


#ifdef __cplusplus
}
#endif

#endif /* __AMIC_ARRAY_APP_H */


