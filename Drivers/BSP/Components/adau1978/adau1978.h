/**
******************************************************************************
* @file    adau1978.h
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   ADAU1978 header driver file
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the 
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ADAU1978_H
#define ADAU1978_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "adau1978_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup ADAU1978 ADAU1978
 * @{
 */

/** @defgroup ADAU1978_Exported_Types ADAU1978 Exported Types
 * @{
 */

typedef int32_t (*ADAU1978_Init_Func)(void);
typedef int32_t (*ADAU1978_DeInit_Func)(void);
typedef int32_t (*ADAU1978_GetTick_Func)(void);
typedef int32_t (*ADAU1978_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ADAU1978_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  ADAU1978_Init_Func          Init;
  ADAU1978_DeInit_Func        DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI-3-Wires */
  uint8_t                   Address;
  ADAU1978_WriteReg_Func      WriteReg;
  ADAU1978_ReadReg_Func       ReadReg;
  ADAU1978_GetTick_Func       GetTick;
} ADAU1978_IO_t;

typedef struct
{
  ADAU1978_IO_t        IO;
  adau1978_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            audio_is_enabled;
  uint32_t           SampleRate;
  uint32_t           ChannelNbr;
  uint32_t           bit_resolution;
  uint8_t               channel_map[4];
  
} ADAU1978_Object_t;

typedef struct
{
  int32_t (*Init)(ADAU1978_Object_t *, void *);
  int32_t (*DeInit)(ADAU1978_Object_t *);
  int32_t (*ReadID)(ADAU1978_Object_t *, uint32_t *);  
  int32_t (*Play)(ADAU1978_Object_t *);
  int32_t (*Pause)(ADAU1978_Object_t *);
  int32_t (*Resume)(ADAU1978_Object_t *);
  int32_t (*Stop)(ADAU1978_Object_t *, uint32_t);
  int32_t (*SetFrequency)(ADAU1978_Object_t *, uint32_t);
  int32_t (*GetFrequency   )(ADAU1978_Object_t*);  
  int32_t (*SetVolume)(ADAU1978_Object_t *, uint32_t, uint8_t);
  int32_t (*GetVolume      )(ADAU1978_Object_t*, uint32_t, uint8_t*);
  int32_t (*SetMute)(ADAU1978_Object_t *, uint32_t);  
  int32_t (*SetOutputMode)(ADAU1978_Object_t *, uint8_t);
  int32_t (*SetResolution  )(ADAU1978_Object_t*, uint32_t);
  int32_t (*GetResolution  )(ADAU1978_Object_t*, uint32_t*);  
  int32_t (*SetProtocol    )(ADAU1978_Object_t*, uint32_t);
  int32_t (*GetProtocol    )(ADAU1978_Object_t*, uint32_t*);
  int32_t (*Reset)(ADAU1978_Object_t *);
} ADAU1978_AUDIO_Drv_t;


/**
 * @}
 */

/** @defgroup ADAU1978_Exported_Constants ADAU1978 Exported Constants
 * @{
 */
#define ADAU1978_I2C_BUS           0U

/** ADAU1978 error codes  **/
#define ADAU1978_OK                 0
#define ADAU1978_ERROR             -1
   
/* AUDIO FREQUENCY */
#ifndef AUDIO_FREQUENCY_192K
#define AUDIO_FREQUENCY_192K     (uint32_t)192000U
#endif
#ifndef AUDIO_FREQUENCY_176K  
#define AUDIO_FREQUENCY_176K     (uint32_t)176400U
#endif
#ifndef AUDIO_FREQUENCY_96K
#define AUDIO_FREQUENCY_96K       (uint32_t)96000U
#endif
#ifndef AUDIO_FREQUENCY_88K
#define AUDIO_FREQUENCY_88K       (uint32_t)88200U
#endif
#ifndef AUDIO_FREQUENCY_48K
#define AUDIO_FREQUENCY_48K       (uint32_t)48000U
#endif
#ifndef AUDIO_FREQUENCY_44K  
#define AUDIO_FREQUENCY_44K       (uint32_t)44100U
#endif
#ifndef AUDIO_FREQUENCY_32K
#define AUDIO_FREQUENCY_32K       (uint32_t)32000U
#endif
#ifndef AUDIO_FREQUENCY_22K
#define AUDIO_FREQUENCY_22K       (uint32_t)22050U
#endif
#ifndef AUDIO_FREQUENCY_16K
#define AUDIO_FREQUENCY_16K       (uint32_t)16000U
#endif
#ifndef AUDIO_FREQUENCY_11K
#define AUDIO_FREQUENCY_11K       (uint32_t)11025U
#endif
#ifndef AUDIO_FREQUENCY_8K
#define AUDIO_FREQUENCY_8K         (uint32_t)8000U 
#endif
   
/* AUDIO RESOLUTION */   
#ifndef AUDIO_RESOLUTION_16b
#define AUDIO_RESOLUTION_16b                16U
#endif
#ifndef AUDIO_RESOLUTION_24b
#define AUDIO_RESOLUTION_24b                24U
#endif
#ifndef AUDIO_RESOLUTION_32b
#define AUDIO_RESOLUTION_32b                32U
#endif

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

/**
 * @}
 */

/** @addtogroup ADAU1978_Exported_Functions ADAU1978 Exported Functions
 * @{
 */

int32_t ADAU1978_RegisterBusIO(ADAU1978_Object_t *pObj, ADAU1978_IO_t *pIO);
int32_t ADAU1978_Init(ADAU1978_Object_t *pObj, void *params);
int32_t ADAU1978_DeInit(ADAU1978_Object_t *pObj);
int32_t ADAU1978_ReadID(ADAU1978_Object_t *pObj, uint32_t *Id);

int32_t ADAU1978_Play(ADAU1978_Object_t *pObj);
int32_t ADAU1978_Pause(ADAU1978_Object_t *pObj);
int32_t ADAU1978_Resume(ADAU1978_Object_t *pObj);
int32_t ADAU1978_Stop(ADAU1978_Object_t *pObj, uint32_t Cmd);
int32_t ADAU1978_SetVolume(ADAU1978_Object_t *pObj, uint32_t Cmd, uint8_t Volume);
int32_t ADAU1978_SetMute(ADAU1978_Object_t *pObj, uint32_t Cmd);
int32_t ADAU1978_SetOutputMode(ADAU1978_Object_t *pObj, uint8_t Output);
int32_t ADAU1978_SetFrequency(ADAU1978_Object_t *pObj, uint32_t AudioFreq);
int32_t ADAU1978_Reset(ADAU1978_Object_t *pObj);
int32_t ADAU1978_SetResolution(ADAU1978_Object_t *pObj, uint32_t BitRes);
int32_t ADAU1978_GetResolution(ADAU1978_Object_t *pObj, uint32_t* BitRes);  
int32_t ADAU1978_SetProtocol(ADAU1978_Object_t *pObj, uint32_t params);
int32_t ADAU1978_GetProtocol(ADAU1978_Object_t *pObj, uint32_t* params);
int32_t ADAU1978_GetVolume(ADAU1978_Object_t *pObj, uint32_t params, uint8_t* value);
int32_t ADAU1978_GetFrequency(ADAU1978_Object_t *pObj);  

int32_t ADAU1978_SetChannelMapping(ADAU1978_Object_t *pObj, uint8_t * map);  


int32_t ADAU1978_Read_Reg(ADAU1978_Object_t *pObj, uint8_t Reg, uint8_t *Data);
int32_t ADAU1978_Write_Reg(ADAU1978_Object_t *pObj, uint8_t Reg, uint8_t Data);

/**
 * @}
 */

/** @addtogroup ADAU1978_Exported_Variables ADAU1978 Exported Variables
 * @{
 */

extern ADAU1978_AUDIO_Drv_t ADAU1978_AUDIO_Driver;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
