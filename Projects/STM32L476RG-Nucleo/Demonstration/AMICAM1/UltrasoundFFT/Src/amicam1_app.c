/**
  ******************************************************************************
  * @file    amicam1_app.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file provides a set of functions to handle amicam1 microphone
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
#include "amicam1_app.h"

SM_Init_Param_t AMICAM1_Init_Param;
SM_Sensor_State_t AMICAM1_Sensor_State = SM_SENSOR_STATE_INITIALIZING;

static int16_t amic_mem[((AUDIO_IN_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2)];

osThreadId AMICAM1_Thread_Id;

static void AMICAM1_Thread(void const *argument);

osMessageQId amicDreadyQueue_id;
osMessageQDef(amicdreadyqueue, 1, int);

osPoolId amicPool_id;
osPoolDef(amicPool, 1, uint32_t);

BSP_AUDIO_Init_t MicParams;

static volatile uint32_t tim_value = 0, tim_value_old = 0, period = 0;
static volatile uint64_t ts_amicam1;

/**
  * @brief IIS3DWB GPIO Initialization Function
  * @param None
  * @retval None
  */
int32_t AMICAM1_Peripheral_Init(uint32_t AudioFreq, uint32_t ChnlNbrIn)
{
  /* Configure Audio Input peripheral*/
  MicParams.BitsPerSample = 16;
  MicParams.ChannelsNbr = ChnlNbrIn;
  MicParams.Device = AUDIO_IN_MICS;
  MicParams.SampleRate = AudioFreq;
  MicParams.Volume = AUDIO_VOLUME_INPUT;

  if (AMICAM1_AUDIO_IN_Init(AUDIO_IN_INSTANCE, &MicParams) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_NO_INIT;
  }

  return BSP_ERROR_NONE;
}



void AMICAM1_OS_Init(void)
{
  /* Thread definition */
  osThreadDef(AMICAM1_RD_USR_THREAD, AMICAM1_Thread, AMICAM1_THREAD_PRIO, 1, configMINIMAL_STACK_SIZE);
  /* Start thread */
  AMICAM1_Thread_Id = osThreadCreate(osThread(AMICAM1_RD_USR_THREAD), NULL);
  /* Suspend thread */
  osThreadSuspend(AMICAM1_Thread_Id);

  amicDreadyQueue_id = osMessageCreate(osMessageQ(amicdreadyqueue), NULL);
  amicPool_id = osPoolCreate(osPool(amicPool));

}


static void AMICAM1_Thread(void const *argument)
{
  (void) argument;

#ifdef DATA_TEST
  static uint16_t usbTestData = 0;
#endif

  osEvent evt;
  for (;;)
  {
    if (AMICAM1_Sensor_State == SM_SENSOR_STATE_INITIALIZING)
    {
      tim_value_old = 0;

      /* Start Microphone acquisition */
      AMICAM1_AUDIO_IN_Record(AUDIO_IN_INSTANCE, (uint8_t *) amic_mem, AUDIO_IN_NOT_USED);
      AMICAM1_Sensor_State = SM_SENSOR_STATE_RUNNING;
    }
    else if (AMICAM1_Sensor_State == SM_SENSOR_STATE_RUNNING)
    {
      evt = osMessageGet(amicDreadyQueue_id, osWaitForever);

      if (AMICAM1_Sensor_State == SM_SENSOR_STATE_RUNNING) /* Change of state can happen while task blocked */
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
        ts_amicam1 += period;

        void *data_ptr = evt.value.p;  /* void since it is independent from data format*/

        AMICAM1_Data_Ready((uint8_t *)data_ptr, (AUDIO_IN_SAMPLING_FREQUENCY / 1000) * AMIC_MS * 2,
                           (double)ts_amicam1 / (double)(SystemCoreClock));
        osPoolFree(amicPool_id, data_ptr);
      }
    }
    else if (AMICAM1_Sensor_State == SM_SENSOR_STATE_SUSPENDING)
    {
#ifdef DATA_TEST
      usbTestData = 0;
#endif
      AMICAM1_Sensor_State = SM_SENSOR_STATE_SUSPENDED;
      ts_amicam1 = 0;
      AMICAM1_AUDIO_IN_Stop(AUDIO_IN_INSTANCE);
      osThreadSuspend(AMICAM1_Thread_Id);
    }
  }
}



/**
  * @brief  Regular conversion complete callback.
  * @retval None
  */

void AMICAM1_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  tim_value = hsm_tim.Instance->CNT;
  void *data_ptr = osPoolAlloc(amicPool_id);
  data_ptr = (void *)amic_mem;
  osMessagePut(amicDreadyQueue_id, (uint32_t)(data_ptr), osWaitForever);


}

/**
  * @brief  Half regular conversion complete callback.
  * @retval None
  */
void AMICAM1_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  tim_value = hsm_tim.Instance->CNT;
  void *data_ptr = osPoolAlloc(amicPool_id);
  data_ptr = (void *)amic_mem;
  osMessagePut(amicDreadyQueue_id, (uint32_t)(data_ptr), osWaitForever);
}



void AMICAM1_Set_State(SM_Sensor_State_t newState)
{
  AMICAM1_Sensor_State = newState;
}

void AMICAM1_Set_ODR(float newODR)
{
  AMICAM1_Init_Param.ODR = newODR;
}

void AMICAM1_Set_FS(float newFS1, float newFS2)
{
  AMICAM1_Init_Param.FS[0] = newFS1;
  AMICAM1_Init_Param.FS[0] = newFS2;
}

void AMICAM1_Start(void)
{
  AMICAM1_Set_State(SM_SENSOR_STATE_INITIALIZING);
  osThreadResume(AMICAM1_Thread_Id);
}

void AMICAM1_Stop(void)
{
  AMICAM1_Set_State(SM_SENSOR_STATE_SUSPENDING);
}

__weak void AMICAM1_Data_Ready(uint8_t *buf, uint16_t size, double timeStamp)
{

}


