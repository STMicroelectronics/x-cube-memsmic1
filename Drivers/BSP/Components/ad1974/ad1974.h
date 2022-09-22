/**
******************************************************************************
* @file    ad1974.h
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   AD1974 header driver file
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
#ifndef AD1974_H
#define AD1974_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ad1974_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Components Components
 * @{
 */

/** @addtogroup AD1974 AD1974
 * @{
 */

/** @defgroup AD1974_Exported_Types AD1974 Exported Types
 * @{
 */

typedef int32_t (*AD1974_Init_Func)(void);
typedef int32_t (*AD1974_DeInit_Func)(void);
typedef int32_t (*AD1974_GetTick_Func)(void);
typedef int32_t (*AD1974_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*AD1974_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  AD1974_Init_Func          Init;
  AD1974_DeInit_Func        DeInit;
  uint32_t                  BusType; /*0 means I2C, 1 means SPI-3-Wires, 2 means SPI-4-Wires */
  uint8_t                   Address;
  AD1974_WriteReg_Func      WriteReg;
  AD1974_ReadReg_Func       ReadReg;
  AD1974_GetTick_Func       GetTick;
} AD1974_IO_t;

typedef struct
{
  AD1974_IO_t        IO;
  ad1974_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            audio_is_enabled;
  uint32_t           SampleRate;
  uint32_t           ChannelNbr;
  uint32_t           bit_resolution;
  uint8_t               channel_map[4];
  
} AD1974_Object_t;

typedef struct
{
  int32_t (*Init)(AD1974_Object_t *, void *);
  int32_t (*DeInit)(AD1974_Object_t *);
  int32_t (*ReadID)(AD1974_Object_t *, uint32_t *);  
  int32_t (*Play)(AD1974_Object_t *);
  int32_t (*Pause)(AD1974_Object_t *);
  int32_t (*Resume)(AD1974_Object_t *);
  int32_t (*Stop)(AD1974_Object_t *, uint32_t);
  int32_t (*SetFrequency)(AD1974_Object_t *, uint32_t);
  int32_t (*GetFrequency   )(AD1974_Object_t*);  
  int32_t (*SetVolume)(AD1974_Object_t *, uint32_t, uint8_t);
  int32_t (*GetVolume      )(AD1974_Object_t*, uint32_t, uint8_t*);
  int32_t (*SetMute)(AD1974_Object_t *, uint32_t);  
  int32_t (*SetOutputMode)(AD1974_Object_t *, uint8_t);
  int32_t (*SetResolution  )(AD1974_Object_t*, uint32_t);
  int32_t (*GetResolution  )(AD1974_Object_t*, uint32_t*);  
  int32_t (*SetProtocol    )(AD1974_Object_t*, uint32_t);
  int32_t (*GetProtocol    )(AD1974_Object_t*, uint32_t*);
  int32_t (*Reset)(AD1974_Object_t *);
} AD1974_AUDIO_Drv_t;


/**
 * @}
 */

/** @defgroup AD1974_Exported_Constants AD1974 Exported Constants
 * @{
 */
#define AD1974_SPI_BUS           2U

/** AD1974 error codes  **/
#define AD1974_OK                0
#define AD1974_ERROR            -1
#define AD1974_NOT_IMPLEMENTED  -2
   
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
#ifndef AUDIO_RESOLUTION_20b
#define AUDIO_RESOLUTION_20b                20U
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

/** @addtogroup AD1974_Exported_Functions AD1974 Exported Functions
 * @{
 */

int32_t AD1974_RegisterBusIO(AD1974_Object_t *pObj, AD1974_IO_t *pIO);
int32_t AD1974_Init(AD1974_Object_t *pObj, void *params);
int32_t AD1974_DeInit(AD1974_Object_t *pObj);
int32_t AD1974_ReadID(AD1974_Object_t *pObj, uint32_t *Id);

int32_t AD1974_Play(AD1974_Object_t *pObj);
int32_t AD1974_Pause(AD1974_Object_t *pObj);
int32_t AD1974_Resume(AD1974_Object_t *pObj);
int32_t AD1974_Stop(AD1974_Object_t *pObj, uint32_t Cmd);
int32_t AD1974_SetVolume(AD1974_Object_t *pObj, uint32_t Cmd, uint8_t Volume);
int32_t AD1974_SetMute(AD1974_Object_t *pObj, uint32_t Cmd);
int32_t AD1974_SetOutputMode(AD1974_Object_t *pObj, uint8_t Output);
int32_t AD1974_SetFrequency(AD1974_Object_t *pObj, uint32_t AudioFreq);
int32_t AD1974_Reset(AD1974_Object_t *pObj);
int32_t AD1974_SetResolution(AD1974_Object_t *pObj, uint32_t BitRes);
int32_t AD1974_GetResolution(AD1974_Object_t *pObj, uint32_t* BitRes);  
int32_t AD1974_SetProtocol(AD1974_Object_t *pObj, uint32_t params);
int32_t AD1974_GetProtocol(AD1974_Object_t *pObj, uint32_t* params);
int32_t AD1974_GetVolume(AD1974_Object_t *pObj, uint32_t params, uint8_t* value);
int32_t AD1974_GetFrequency(AD1974_Object_t *pObj);  

int32_t AD1974_Read_Reg(AD1974_Object_t *pObj, uint8_t Reg, uint8_t *Data);
int32_t AD1974_Write_Reg(AD1974_Object_t *pObj, uint8_t Reg, uint8_t Data);

/**
 * @}
 */

/** @addtogroup AD1974_Exported_Variables AD1974 Exported Variables
 * @{
 */

extern AD1974_AUDIO_Drv_t AD1974_AUDIO_Driver;

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
