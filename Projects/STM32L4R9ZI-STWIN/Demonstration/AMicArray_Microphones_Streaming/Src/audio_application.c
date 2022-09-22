/**
  ******************************************************************************
  * @file    audio_application.c
  * @author  SRA
  * @version v1.2.1
  * @date    17-Jul-2020
  * @brief   Audio  application.
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

/* Includes ------------------------------------------------------------------*/
#include "audio_application.h"

/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup Microphones_Acquisition
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** @defgroup AUDIO_APPLICATION_Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Private_Variables
  * @{
  */

#define AUDIO_OUTPUT_BUFF_SIZE ((AUDIO_IN_SAMPLING_FREQUENCY/1000)*AUDIO_IN_CHANNELS*2)
static int16_t Audio_output_buffer[AUDIO_OUTPUT_BUFF_SIZE];
BSP_AUDIO_Init_t MicParams;

/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Exported_Function
  * @{
  */

/**
  * @brief  Initializes all the required peripherals using the BSP Init function.
  * @param  None
  * @retval BSP_ERROR_NONE if no problem during initialization, AUDIO_ERROR otherwise
  */
int32_t Init_Audio_Device(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{
  /* Configure Audio Input peripheral*/
  MicParams.BitsPerSample = 16;
  MicParams.ChannelsNbr = ChnlNbrOut;
  MicParams.Device = ChnlNbrIn;
  MicParams.SampleRate = AudioFreq;
  MicParams.Volume = AUDIO_VOLUME_INPUT;

  if (AMIC_ARRAY_AUDIO_IN_Init(AMIC_ARRAY_AUDIO_IN_INSTANCE, &MicParams) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_NO_INIT;
  }
  if (AMIC_ARRAY_AUDIO_IN_SetVolume(AMIC_ARRAY_AUDIO_IN_INSTANCE, MicParams.Volume) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Starts audio output.
  * @param  None
  * @retval BSP_ERROR_NONE if no problem during initialization, AUDIO_ERROR otherwise
  */
int32_t Start_Audio_Device(void)
{
  return AMIC_ARRAY_AUDIO_IN_Record(AMIC_ARRAY_AUDIO_IN_INSTANCE, (uint8_t *) Audio_output_buffer,
                                    AUDIO_OUTPUT_BUFF_SIZE);
}

/**
  * @brief  Manages the DMA full Transfer complete event.
  * @param  OutputDevice: device to be used
  *         This parameter can be a value of @ref SOUNDTERMINAL_devices
  * @retval None
  */
void AMIC_ARRAY_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  Send_Audio_to_USB((int16_t *)&Audio_output_buffer[AUDIO_OUTPUT_BUFF_SIZE / 2], AUDIO_OUTPUT_BUFF_SIZE / 2);
  BSP_LED_On(LED1);
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  OutputDevice: device to be used
  *         This parameter can be a value of @ref SOUNDTERMINAL_devices
  * @retval None
  */
void AMIC_ARRAY_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  Send_Audio_to_USB((int16_t *)Audio_output_buffer, AUDIO_OUTPUT_BUFF_SIZE / 2);
  BSP_LED_Off(LED1);
}


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


