/**
******************************************************************************
* @file    acoustic_sl.h
* @author  SRA
* @brief   This file contains Acoustic Sound Source Localization library definitions.
******************************************************************************
* @attention
*
* Copyright (c) 2022 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*                        
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ACOUSTIC_SL_H
#define __ACOUSTIC_SL_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/** @addtogroup MIDDLEWARES
* @{
*/

/** @defgroup ACOUSTIC_SL ACOUSTIC_SL
* @{
*/

/* Exported constants --------------------------------------------------------*/

/** @defgroup Acoustic_SL_Exported_Constants AcousticSL Exported Constants
* @{
*/  

/** @defgroup Acoustic_SL_algorithm_type
* @brief    Source Localization algorithm type 
* @{
*/ 
#define ACOUSTIC_SL_ALGORITHM_XCORR                	((uint32_t)0x00000001)
#define ACOUSTIC_SL_ALGORITHM_GCCP                 	((uint32_t)0x00000002)
#define ACOUSTIC_SL_ALGORITHM_BMPH                 	((uint32_t)0x00000004)
/**
* @}
*/

/** @defgroup Acoustic_SL_errors
* @brief    Source Localization errors 
* @{
*/ 
#define ACOUSTIC_SL_ALGORITHM_ERROR                	((uint32_t)0x00000001)
#define ACOUSTIC_SL_PTR_CHANNELS_ERROR             	((uint32_t)0x00000002)
#define ACOUSTIC_SL_CHANNEL_NUMBER_ERROR           	((uint32_t)0x00000004)
#define ACOUSTIC_SL_SAMPLING_FREQ_ERROR            	((uint32_t)0x00000008)
#define ACOUSTIC_SL_RESOLUTION_ERROR               	((uint32_t)0x00000010)
#define ACOUSTIC_SL_THRESHOLD_ERROR                	((uint32_t)0x00000020)
#define ACOUSTIC_SL_DISTANCE_ERROR                 	((uint32_t)0x00000040)
#define ACOUSTIC_SL_NUM_OF_SAMPLES_ERROR           	((uint32_t)0x00000080)
#define ACOUSTIC_SL_PROCESSING_ERROR               	((uint32_t)0x00000100)

#ifndef ACOUSTIC_LOCK_ERROR
#define ACOUSTIC_LOCK_ERROR                      	((uint32_t)0x10000000)
#endif 
/**
* @}
*/  
#define ACOUSTIC_SL_NO_AUDIO_DETECTED              -100  
/**
* @}
*/

/* Exported types ------------------------------------------------------------*/

/** @defgroup Acoustic_SL_Exported_Types AcousticSL Exported Types
* @{
*/
/**
* @brief  Library handler. It keeps track of the static parameters
*         and it handles the internal state of the algorithm.
*/
typedef struct
{
  uint32_t algorithm;                           /*!< Specifies the algorithm to be used between XCORR and GCC-PHAT. This parameter can be a value of @ref 
  Acoustic_SL_algorithm_type. Default value is ACOUSTIC_BF_TYPE_CARDIOID_BASIC */
  
  uint32_t sampling_frequency;                  /*!< Specifies the sampling frequency - for future use */
  
  uint32_t channel_number;                      /*!< Specifies the number of channels, can be 2 for 180� estimation, 4 for 360� estimation. Default value is 
  2. */  
  uint8_t ptr_M1_channels;                      /*!< Number of channels in the stream of Microphone 1. Deafult value is 1. */  
  uint8_t ptr_M2_channels;                      /*!< Number of channels in the stream of Microphone 2. Deafult value is 1. */ 
  uint8_t ptr_M3_channels;                      /*!< Number of channels in the stream of Microphone 3. Deafult value is 1. */
  uint8_t ptr_M4_channels;                      /*!< Number of channels in the stream of Microphone 4. Deafult value is 1. */
  
  uint16_t M12_distance;                        /*!< Distance between Mic1 and Mic2 in decimals of a millimeter. Deafult value is 150. */
  uint16_t M34_distance;                        /*!< Distance between Mic3 and Mic4 in decimals of a millimeter. Deafult value is 150. */
  
  uint32_t internal_memory_size;                /*!< Keeps track of the amount of memory required for the current setup.
  It's filled by the AcousticSL_getMemorySize() function and must
  be used to allocate the right amount of RAM */
  uint32_t * pInternalMemory;                   /*!< Pointer to the memory allocated by the user */
  int16_t samples_to_process;                   /*!< Specifies the number of samples to be processed at a time */      
  
} AcousticSL_Handler_t;

/**
* @brief  Library dynamic configuration handler. It contains dynamic parameters.
*/
typedef struct
{
  uint16_t threshold;                           /*!< Specifies a value related to a voice-activity score. With values below the threshold, the algorithm does not act. The threshold value ranges from 0 to 1000 and the default value is 24. */
  uint32_t resolution;                          /*!< Angle resolution for the algorithms. Ignored if XCORR is used. Deafult value is 4. */
} AcousticSL_Config_t;


/**
* @}
*/

/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup Acoustic_SL_Exported_Functions AcousticSL Exported Functions
* @{
*/
/**
* @brief  Fills the "internal_memory_size" of the pHandler parameter passed as argument with a value representing the
*         right amount of memory needed by the library, depending on the specific static parameters adopted.
* @param  pHandler: AcousticSL_Handler_t filled with desired parameters.
* @retval 0 if everything is fine.
*/
uint32_t AcousticSL_getMemorySize(AcousticSL_Handler_t * pHandler);

/**
* @brief  Library initialization.
* @param  pHandler: AcousticSL_Handler_t filled with desired parameters.
* @retval 0 if everything is fine.
*         different from 0 if erroneous parameters have been passed to the Init function and the default value has been used.
*         The specific error can be recognized by checking the relative bit in the returned word.
*/
uint32_t AcousticSL_Init(AcousticSL_Handler_t * pHandler);

/**
* @brief  Library data input
* @param  pM1: pointer to an array that contains PCM samples (16 bit signed int)
*         representing 1 ms of data acquired by the first channel.
* @param  pM2: pointer to an array that contains PCM samples (16 bit signed int)
*         representing 1 ms of data acquired by the second channel.
* @param  pM3: pointer to an array that contains PCM samples (16 bit signed int)
*         representing 1 ms of data acquired by the third channel.
* @param  pM4: pointer to an array that contains PCM samples (16 bit signed int)
*         representing 1 ms of data acquired by the fourth channel.
* @param  pHandler: pointer to the handler of the curent Source Localization instance running.
* @retval 1 if data collection is finished and libSoundSourceLoc_Process must be called, 0 otherwise.
* @note   Input function reads samples skipping the required number of values depending on the Ptr_Mx_Channels configuration.
* @note   pM3 and pM4 are ignored in the case the library is setup for using 2 channels.
*/
uint32_t AcousticSL_Data_Input(void *pM1, void *pM2, void *pM3, void *pM4, AcousticSL_Handler_t * pHandler);

/**
* @brief  Library run function, performs audio analysis when all required data has been collected.
* @param  Estimated_Angle: pointer to the int32_t variable that will contain the computed value.
* @param  pHandler: pointer to the handler of the current Source Localization instance running.
* @retval 0 if everything is ok, 1 otherwise
*/
uint32_t AcousticSL_Process(int32_t * Estimated_Angle, AcousticSL_Handler_t * pHandler);

/**
* @brief  Library setup function, it sets the values for threshold and resolution. It can be called at runtime to change
*         dynamic parameters.
* @note   Only the threshold and resolution are evaluated by the SetConfig function.
* @retval 0 if everything is fine.
*         different from 0 if erroneous parameters have been passed to the Init function and the default value has been used.
*         The specific error can be recognized by checking the relative bit in the returned word.
*/
uint32_t AcousticSL_setConfig(AcousticSL_Handler_t * pHandler, AcousticSL_Config_t * pConfig);

/**
* @brief  Fills the pConfig structure with the actual dynamic parameters as they are currently used inside the library.
* @param  pHandler: pointer to the handler of the current Source Localization instance running.
* @param  pConfig: pointer to the dynamic parameters handler that will be filled with the current library configuration
* @retval 0 if everything is fine.
*/
uint32_t AcousticSL_getConfig(AcousticSL_Handler_t * pHandler, AcousticSL_Config_t * pConfig);

/**
* @brief  To be used to retrieve version information.
* @param  version char array to be filled with the current library version
* @retval 0 if everything is fine.
*/
uint32_t AcousticSL_GetLibVersion(char *version);


/**
* @}
*/
/**
* @}
*/

/**
* @}
*/
#endif /* __ACOUSTIC_SL_H */

