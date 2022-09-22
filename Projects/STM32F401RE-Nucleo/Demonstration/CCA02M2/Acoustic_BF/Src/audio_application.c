/**
  ******************************************************************************
  * @file    audio_application.c
  * @author  SRA
  * @version v1.3.0
  * @date    17-Sep-2021
  * @brief   Beam forming example.
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
#include "acoustic_bf.h"


/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup ACOUSTIC_BF_Example
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
uint16_t PDM_Buffer[((((AUDIO_IN_CHANNELS * AUDIO_IN_SAMPLING_FREQUENCY) / 1000) * MAX_DECIMATION_FACTOR) / 16)* N_MS ];
/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Private_Variables
  * @{
  */
/* Private variables ---------------------------------------------------------*/

/*Handler and Config structure for BeamForming*/
AcousticBF_Handler_t libBeamforming_Handler_Instance;
AcousticBF_Config_t lib_Beamforming_Config_Instance;
int16_t USBOUT[16 * 4];
/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Exported_Function
  * @{
  */

/**
  * @brief  Half Transfer user callback, called by BSP functions.
  * @param  None
  * @retval None
  */
void CCA02M2_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  UNUSED(Instance);
  AudioProcess();
}

/**
  * @brief  Transfer Complete user callback, called by BSP functions.
  * @param  None
  * @retval None
  */
void CCA02M2_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  UNUSED(Instance);
  AudioProcess();
}

/**
  * @brief  User function that is called when 1 ms of PDM data is available.
  *       In this application only PDM to PCM conversion and USB streaming
  *                  is performed.
  *       User can add his own code here to perform some DSP or audio analysis.
  *                In this case audio libraries are used
  * @param  none
  * @retval None
  */
void AudioProcess(void)
{
  if (AcousticBF_FirstStep(&((uint8_t *)(PDM_Buffer))[0], &((uint8_t *)(PDM_Buffer))[1], (int16_t *)USBOUT,
                           &libBeamforming_Handler_Instance) == 1U)
  {
    SW_Task1_Start();
  }

  Send_Audio_to_USB((int16_t *)USBOUT, AUDIO_IN_SAMPLING_FREQUENCY / 1000 * AUDIO_IN_CHANNELS);
}


/**
  * @brief  Initialize the audio libraries adopted
  * @param  None
  * @retval None
  */
void Audio_Libraries_Init(void)
{
  __IO uint32_t error_value = 0;
  /* Enable CRC peripheral to unlock the library */
  __CRC_CLK_ENABLE();
  /*Setup Beamforming static parameters*/
  error_value = 0;
  libBeamforming_Handler_Instance.algorithm_type_init = ACOUSTIC_BF_TYPE_STRONG;
  libBeamforming_Handler_Instance.ref_mic_enable = ACOUSTIC_BF_REF_ENABLE;
  libBeamforming_Handler_Instance.ptr_out_channels = 2;
  libBeamforming_Handler_Instance.data_format = ACOUSTIC_BF_DATA_FORMAT_PDM;
  libBeamforming_Handler_Instance.sampling_frequency = PDM_FREQ_16K;
  libBeamforming_Handler_Instance.ptr_M1_channels = 2;
  libBeamforming_Handler_Instance.ptr_M2_channels = 2;
  libBeamforming_Handler_Instance.delay_enable = 1;
  (void)AcousticBF_getMemorySize(&libBeamforming_Handler_Instance);
  libBeamforming_Handler_Instance.pInternalMemory =
    (uint32_t *)malloc(libBeamforming_Handler_Instance.internal_memory_size);
  error_value = AcousticBF_Init(&libBeamforming_Handler_Instance);

  /*Error Management*/
  if (error_value != 0U)
  {
    Error_Handler();
  }

  /*Malloc Failure*/
  if (libBeamforming_Handler_Instance.pInternalMemory == NULL)
  {
    Error_Handler();
  }

  /*Setup Beamforming dynamic parameters*/
  lib_Beamforming_Config_Instance.algorithm_type = ACOUSTIC_BF_TYPE_STRONG;
  lib_Beamforming_Config_Instance.M2_gain = 0.0f;
  lib_Beamforming_Config_Instance.mic_distance = 150;
  lib_Beamforming_Config_Instance.volume = 24;
  error_value = AcousticBF_setConfig(&libBeamforming_Handler_Instance, &lib_Beamforming_Config_Instance);

  /*Error Management*/
  if (error_value != 0U)
  {
    Error_Handler();
  }
}

/**
  * @brief  Initializes two SW interrupt with different priorities
  * @param  None
  * @retval None
  */
void SW_IRQ_Tasks_Init(void)
{
  HAL_NVIC_SetPriority((IRQn_Type)EXTI1_IRQn, 0x0D, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI1_IRQn);

  HAL_NVIC_SetPriority((IRQn_Type)EXTI2_IRQn, 0x0E, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI2_IRQn);
}

/**
  * @brief  Highest priority interrupt handler routine
  * @param  None
  * @retval None
  */
void SW_Task1_Callback(void)
{
  (void)AcousticBF_SecondStep(&libBeamforming_Handler_Instance);
}

/**
  * @brief Lower priority interrupt handler routine
  * @param  None
  * @retval None
  */
void SW_Task2_Callback(void)
{

}

/**
  * @brief Throws Highest priority interrupt
  * @param  None
  * @retval None
  */
void SW_Task1_Start(void)
{
  HAL_NVIC_SetPendingIRQ(EXTI1_IRQn);
}

/**
  * @brief Throws Lower priority interrupt
  * @param  None
  * @retval None
  */
void SW_Task2_Start(void)
{
  HAL_NVIC_SetPendingIRQ(EXTI2_IRQn);
}

void Error_Handler(void)
{
  while (1);
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

