/**
  ******************************************************************************
  * @file    fft_app.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file provides a set of functions to handle fft
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
#include "fft_app.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static osSemaphoreId fft_data_input_sem_id;
static osSemaphoreDef(fft_data_input_sem);

FFT_instance_t audio_fft_instance;
float *audio_fft_out;
float *userBuff_input;
float *FFT_Average;

extern USBD_HandleTypeDef USBD_Device;

/* Private function prototypes -----------------------------------------------*/

osThreadId FFT_Thread_Id;
static void FFT_Thread(void const *argument);
void FFT_Lib_Init(uint32_t FFTLen, float Overlap);


/**
  * @brief FFT Library Data Input
  * @param None
  * @retval None
  */
void FFT_Lib_Data_Input(int16_t *data, uint32_t len)
{
  if (FFT_Data_Input(data, len, &audio_fft_instance))
  {
    osSemaphoreRelease(fft_data_input_sem_id);
  }
}

void FFT_Lib_Init(uint32_t FFTLen, float Overlap)
{
  audio_fft_instance.init_params.FFT_len = FFTLen;
  audio_fft_instance.init_params.overlap = Overlap;
  audio_fft_instance.init_params.win_type = FFT_HANNING_WIN;
  audio_fft_instance.init_params.output_type = MAGNITUDE;
  audio_fft_instance.init_params.data_type = INT16;
  audio_fft_instance.init_params.use_direct_process = DIRECT_PROCESS_DISABLED;
#ifdef FFT_STATIC_ALLOCATION
  userBuff_input = calloc(FFT_getMemorySize(&audio_fft_instance) / sizeof(float), sizeof(float));
  audio_fft_instance.init_params.userBuffer = userBuff_input;
#endif
  FFT_Init(&audio_fft_instance);

  /* Allocate output buffer */
  audio_fft_out = calloc(audio_fft_instance.init_params.FFT_len / 2, sizeof(float));
  FFT_Average = calloc(audio_fft_instance.init_params.FFT_len / 2, sizeof(float));
}


/**
  * @brief FFT Threads Creation
  * @param None
  * @retval None
  */
void FFT_OS_Init(void)
{
  /* Data read complete semaphore initialization */
  fft_data_input_sem_id = osSemaphoreCreate(osSemaphore(fft_data_input_sem), 1);
  osSemaphoreWait(fft_data_input_sem_id, osWaitForever);

  /* Thread definition: read data */
  osThreadDef(FFT_Computation_Thread, FFT_Thread, FFT_THREAD_PRIO, 1, configMINIMAL_STACK_SIZE * 2);
  /* Start thread */
  FFT_Thread_Id = osThreadCreate(osThread(FFT_Computation_Thread), NULL);
}


static void FFT_Thread(void const *argument)
{
  (void) argument;

#if (configUSE_APPLICATION_TASK_TAG == 1 && defined(TASK_FFT_DEBUG_PIN))
  vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)TASK_FFT_DEBUG_PIN);
#endif

#ifdef DATA_TEST
  static uint16_t usbTestData = 0;
#endif

#if (FFT_OUTPUT == FFT_AVERAGE)
  uint16_t nAccTotal = 10;
  uint16_t nAcc = 0;
#endif

  FFT_Lib_Init(512, 0.5f);

  for (;;)
  {
    osSemaphoreWait(fft_data_input_sem_id,  osWaitForever);
    FFT_Process(&audio_fft_instance, audio_fft_out);

#if (FFT_OUTPUT == FFT_RAW)

    if (com_status == HS_DATALOG_USB_STARTED)
    {
      USBD_WCID_STREAMING_FillTxDataBuffer(&USBD_Device, 0, (uint8_t *) audio_fft_out,
                                           sizeof(float) * audio_fft_instance.init_params.FFT_len / 2);
    }

#elif (FFT_OUTPUT == FFT_AVERAGE)
    arm_add_f32(audio_fft_out, FFT_Average, FFT_Average, audio_fft_instance.FFT_len / 2);
    nAcc++;

    if (nAcc == nAccTotal)
    {
      arm_scale_f32(FFT_Average, 1.0f / (float)nAcc, FFT_Average, audio_fft_instance.FFT_len / 2);
      if (com_status == HS_DATALOG_USB_STARTED)
      {
        USBD_WCID_STREAMING_FillTxDataBuffer(&USBD_Device, 0, (uint8_t *)FFT_Average,
                                             sizeof(float) * audio_fft_instance.FFT_len / 2);
      }
      memset((uint8_t *)FFT_Average, 0, audio_fft_instance.FFT_len / 2);
      nAcc = 0;
    }

#elif (FFT_OUTPUT == FFT_WEIGHTED_AVERAGE)
    uint32_t i;
    for (i = 0; i < audio_fft_instance.FFT_len / 2; i++)
    {
      audio_fft_out[i] = FFT_Average[i] * 0.97f + audio_fft_out[i] * 0.03f;
      FFT_Average[i] = audio_fft_out[i];
    }
    if (com_status == HS_DATALOG_USB_STARTED)
    {
      USBD_WCID_STREAMING_FillTxDataBuffer(&USBD_Device, 0, (uint8_t *)FFT_Average,
                                           sizeof(float) * audio_fft_instance.FFT_len / 2);
    }
#endif

#ifdef DATA_TEST
    uint16_t i = 0;

    int16_t *p16 = (int16_t *)dataOut;
    for (i = 0; i < 4; i++)
    {
      *p16++ = usbTestData++;
    }
#endif

  }
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


