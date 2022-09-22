/**
  ******************************************************************************
  * @file    AMic_Array_app.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file provides a set of functions to handle AMic_Array microphone
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
#include "AMic_Array_app.h"

SM_Init_Param_t AMIC_ARRAY_Init_Param;
SM_Sensor_State_t AMIC_ARRAY_Sensor_State = SM_SENSOR_STATE_INITIALIZING;

static int16_t amic_mem[((AUDIO_IN_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2)];

osThreadId AMIC_ARRAY_Thread_Id;

static void AMIC_ARRAY_Thread(void const *argument);

osMessageQId amicDreadyQueue_id;
osMessageQDef(amicdreadyqueue, 1, int);

osPoolId amicPool_id;
osPoolDef(amicPool, 1, uint32_t);

BSP_AUDIO_Init_t MicParams;

static volatile uint32_t tim_value = 0, tim_value_old = 0, period = 0;
static volatile uint64_t ts_AMic_Array;

/**
  * @brief IIS3DWB GPIO Initialization Function
  * @param None
  * @retval None
  */
int32_t AMIC_ARRAY_Peripheral_Init(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
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



void AMIC_ARRAY_OS_Init(void)
{
  /* Thread definition */
  osThreadDef(AMIC_ARRAY_RD_USR_THREAD, AMIC_ARRAY_Thread, AMIC_ARRAY_THREAD_PRIO, 1, configMINIMAL_STACK_SIZE);
  /* Start thread */
  AMIC_ARRAY_Thread_Id = osThreadCreate(osThread(AMIC_ARRAY_RD_USR_THREAD), NULL);
  /* Suspend thread */
  osThreadSuspend(AMIC_ARRAY_Thread_Id);

  amicDreadyQueue_id = osMessageCreate(osMessageQ(amicdreadyqueue), NULL);
  amicPool_id = osPoolCreate(osPool(amicPool));

}


static void AMIC_ARRAY_Thread(void const *argument)
{
  (void) argument;

#if (configUSE_APPLICATION_TASK_TAG == 1 && defined(TASK_AMIC_ARRAY_DEBUG_PIN))
  vTaskSetApplicationTaskTag(NULL, (TaskHookFunction_t)TASK_AMIC_ARRAY_DEBUG_PIN);
#endif

#ifdef DATA_TEST
  static uint16_t usbTestData = 0;
#endif

  osEvent evt;
  for (;;)
  {
    if (AMIC_ARRAY_Sensor_State == SM_SENSOR_STATE_INITIALIZING)
    {
      tim_value_old = 0;

      /* Start Microphone acquisition */
      AMIC_ARRAY_AUDIO_IN_Record(AMIC_ARRAY_AUDIO_IN_INSTANCE, (uint8_t *) amic_mem,
                                 (AUDIO_IN_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2);
      AMIC_ARRAY_Sensor_State = SM_SENSOR_STATE_RUNNING;
    }
    else if (AMIC_ARRAY_Sensor_State == SM_SENSOR_STATE_RUNNING)
    {
      evt = osMessageGet(amicDreadyQueue_id, osWaitForever);

      if (AMIC_ARRAY_Sensor_State == SM_SENSOR_STATE_RUNNING) /* Change of state can happen while task blocked */
      {
        if (tim_value >= tim_value_old)
        {
          period = tim_value - tim_value_old;
        }
        else
        {
          period = tim_value + (0xFFFFFFFF - tim_value_old);
        }

        tim_value_old = tim_value;
        ts_AMic_Array += period;

        void *data_ptr = evt.value.p;  /* void since it is independent from data format*/

        AMIC_ARRAY_Data_Ready((uint8_t *)data_ptr, (AUDIO_IN_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2,
                              (double)ts_AMic_Array / (double)(SystemCoreClock));
        osPoolFree(amicPool_id, data_ptr);
      }
    }
    else if (AMIC_ARRAY_Sensor_State == SM_SENSOR_STATE_SUSPENDING)
    {
#ifdef DATA_TEST
      usbTestData = 0;
#endif
      AMIC_ARRAY_Sensor_State = SM_SENSOR_STATE_SUSPENDED;
      ts_AMic_Array = 0;
      AMIC_ARRAY_AUDIO_IN_Pause(AMIC_ARRAY_AUDIO_IN_INSTANCE);
      osThreadSuspend(AMIC_ARRAY_Thread_Id);
    }
  }
}



/**
  * @brief  Regular conversion complete callback.
  * @retval None
  */

void AMIC_ARRAY_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  tim_value = hsm_tim.Instance->CNT;
  void *data_ptr = osPoolAlloc(amicPool_id);
  data_ptr = (void *)&amic_mem[((AUDIO_IN_SAMPLING_FREQUENCY / 1000) * AMIC_MS)];
  osMessagePut(amicDreadyQueue_id, (uint32_t)(data_ptr), osWaitForever);


}

/**
  * @brief  Half regular conversion complete callback.
  * @retval None
  */
void AMIC_ARRAY_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  tim_value = hsm_tim.Instance->CNT;
  void *data_ptr = osPoolAlloc(amicPool_id);
  data_ptr = (void *)amic_mem;
  osMessagePut(amicDreadyQueue_id, (uint32_t)(data_ptr), osWaitForever);
}


void AMIC_ARRAY_Set_State(SM_Sensor_State_t newState)
{
  AMIC_ARRAY_Sensor_State = newState;
}

void AMIC_ARRAY_Set_ODR(float newODR)
{
  AMIC_ARRAY_Init_Param.ODR = newODR;
}

void AMIC_ARRAY_Set_FS(float newFS1, float newFS2)
{
  AMIC_ARRAY_Init_Param.FS[0] = newFS1;
  AMIC_ARRAY_Init_Param.FS[0] = newFS2;
}

void AMIC_ARRAY_Start(void)
{
  AMIC_ARRAY_Set_State(SM_SENSOR_STATE_INITIALIZING);
  osThreadResume(AMIC_ARRAY_Thread_Id);
}

void AMIC_ARRAY_Stop(void)
{
  AMIC_ARRAY_Set_State(SM_SENSOR_STATE_SUSPENDING);
}

__weak void AMIC_ARRAY_Data_Ready(uint8_t *buf, uint16_t size, double timeStamp)
{

}


