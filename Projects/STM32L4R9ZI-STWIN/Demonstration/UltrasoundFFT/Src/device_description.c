/**
  ******************************************************************************
  * @file    device_description.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   This file provides a set of functions to handle the COM structure
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
#include "device_description.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern uint8_t mp23abs1_com_id;
extern SM_Init_Param_t MP23ABS1_Init_Param;
#define WRITE_BUFFER_SIZE_MP23ABS1      (uint32_t)(65000)

#define DEVICE_ID_REG_1  (0x1FFF7590)
#define DEVICE_ID_REG_2  (0x1FFF7594)
#define DEVICE_ID_REG_3  (0x1FFF7598)

extern uint32_t maxWriteTimeSensor[];


static void get_unique_id(char *id)
{
  uint32_t deviceserial[3];

  deviceserial[0] = *(uint32_t *)DEVICE_ID_REG_1;
  deviceserial[1] = *(uint32_t *)DEVICE_ID_REG_2;
  deviceserial[2] = *(uint32_t *)DEVICE_ID_REG_3;

  uint16_t y = (deviceserial[0] & 0xFFFF);
  uint16_t x = ((deviceserial[0] >> 16) & 0xFFFF);
  uint8_t waf_num = (deviceserial[1] & 0xFF);

  id[0] = (deviceserial[1] >> 8) & 0xFF;
  id[1] = (deviceserial[1] >> 16) & 0xFF;
  id[2] = (deviceserial[1] >> 24) & 0xFF;
  id[3] = (deviceserial[2]) & 0xFF;
  id[4] = (deviceserial[2] >> 8) & 0xFF;
  id[5] = (deviceserial[2] >> 16) & 0xFF;
  id[6] = (deviceserial[2] >> 24) & 0xFF;

  int n;
  char *p = &id[7];

  n = sprintf(p, "%03u", waf_num);
  p += n;

  n = sprintf(p, "%05u", x);
  p += n;

  n = sprintf(p, "%05u", y);
  p += n;

  *p = '\0';
  /*OUT: | LOT NUMBER [7] | WAF NUMBER [3] | X ON WAF [5] | Y ON WAF [5] | */
}

/**
  * @brief Set default device description
  * @param None
  * @retval None
  */
void set_default_description(void)
{
  COM_Sensor_t *tempSensor;
  COM_DeviceDescriptor_t *tempDeviceDescriptor;
  tempDeviceDescriptor = COM_GetDeviceDescriptor();

  get_unique_id(tempDeviceDescriptor->serialNumber);
  strcpy(tempDeviceDescriptor->alias, "STWIN_001");

  /**********/

  /* MP23ABS1 */
  mp23abs1_com_id = COM_AddSensor();

  tempSensor = COM_GetSensor(mp23abs1_com_id);

  /* SENSOR DESCRIPTOR */
  strcpy(tempSensor->sensorDescriptor.name, "MP23ABS1");
  tempSensor->sensorDescriptor.dataType = DATA_TYPE_INT16;
  tempSensor->sensorDescriptor.ODR[0] = 192000.0f;
  tempSensor->sensorDescriptor.ODR[1] = COM_END_OF_LIST_FLOAT;
  tempSensor->sensorDescriptor.samplesPerTimestamp[0] = 0;
  tempSensor->sensorDescriptor.samplesPerTimestamp[1] = 1000;
  tempSensor->sensorDescriptor.nSubSensors = 1;

  /* SENSOR STATUS */
  tempSensor->sensorStatus.ODR = 192000.0f;
  tempSensor->sensorStatus.measuredODR = 0.0f;
  tempSensor->sensorStatus.initialOffset = 0.0f;
  tempSensor->sensorStatus.samplesPerTimestamp = 0;
  tempSensor->sensorStatus.isActive = 0;
  tempSensor->sensorStatus.usbDataPacketSize = 4096;
  tempSensor->sensorStatus.sdWriteBufferSize = WRITE_BUFFER_SIZE_MP23ABS1;
  tempSensor->sensorStatus.comChannelNumber = -1;

  /* SUBSENSOR 0 DESCRIPTOR */
  tempSensor->sensorDescriptor.subSensorDescriptor[0].id = 0;
  tempSensor->sensorDescriptor.subSensorDescriptor[0].sensorType = COM_TYPE_MIC;
  tempSensor->sensorDescriptor.subSensorDescriptor[0].dataPerSample = 1;
  strcpy(tempSensor->sensorDescriptor.subSensorDescriptor[0].unit, "Waveform");
  tempSensor->sensorDescriptor.subSensorDescriptor[0].FS[0] = 130.0f;
  tempSensor->sensorDescriptor.subSensorDescriptor[0].FS[1] = COM_END_OF_LIST_FLOAT;

  /* SUBSENSOR 0 STATUS */
  tempSensor->sensorStatus.subSensorStatus[0].FS = 130.0f;
  tempSensor->sensorStatus.subSensorStatus[0].isActive = 1;
  tempSensor->sensorStatus.subSensorStatus[0].sensitivity = 1.0;

  MP23ABS1_Init_Param.ODR = tempSensor->sensorStatus.ODR;
  MP23ABS1_Init_Param.FS[0] = tempSensor->sensorStatus.subSensorStatus[0].FS;
  MP23ABS1_Init_Param.subSensorActive[0] = tempSensor->sensorStatus.subSensorStatus[0].isActive;

  maxWriteTimeSensor[mp23abs1_com_id] = 1000 * WRITE_BUFFER_SIZE_MP23ABS1 / (uint32_t)(MP23ABS1_Init_Param.ODR * 2);

}

void update_sensorStatus(COM_SensorStatus_t *oldSensorStatus, COM_SensorStatus_t *newSensorStatus, uint8_t sID)
{
  /* Check differencies between oldSensorStatus and newSensorStatus, act properly*/
  /* ODR */
  if (oldSensorStatus->ODR != newSensorStatus->ODR)
  {
    if (COM_IsOdrLegal(newSensorStatus->ODR, sID))
    {
      oldSensorStatus->ODR = newSensorStatus->ODR; /* Todo Setters and getters */
    }
  }

  /* isActive */
  if (oldSensorStatus->isActive != newSensorStatus->isActive)
  {
    oldSensorStatus->isActive = newSensorStatus->isActive;
  }

  /* subsensor: FS, is Active*/
  for (int i = 0; i < COM_GetSubSensorNumber(sID); i++)
  {
    if (oldSensorStatus->subSensorStatus[i].FS != newSensorStatus->subSensorStatus[i].FS)
    {
      oldSensorStatus->subSensorStatus[i].FS = newSensorStatus->subSensorStatus[i].FS;
    }

    if (oldSensorStatus->subSensorStatus[i].isActive != newSensorStatus->subSensorStatus[i].isActive)
    {
      oldSensorStatus->subSensorStatus[i].isActive = newSensorStatus->subSensorStatus[i].isActive;
    }
  }
  /* CHANNEL NUMBER */
  if (oldSensorStatus->comChannelNumber != newSensorStatus->comChannelNumber)
  {
    oldSensorStatus->comChannelNumber = newSensorStatus->comChannelNumber;
  }

  /* CHANNEL NUMBER */
  if (oldSensorStatus->samplesPerTimestamp != newSensorStatus->samplesPerTimestamp)
  {
    oldSensorStatus->samplesPerTimestamp = newSensorStatus->samplesPerTimestamp;
  }
}

void update_sensors_config(void)
{
  COM_Sensor_t *tempSensor;

  tempSensor = COM_GetSensor(mp23abs1_com_id);
  MP23ABS1_Init_Param.ODR = tempSensor->sensorStatus.ODR;
  MP23ABS1_Init_Param.FS[0] = tempSensor->sensorStatus.subSensorStatus[0].FS;
  MP23ABS1_Init_Param.subSensorActive[0] = tempSensor->sensorStatus.subSensorStatus[0].isActive;
}



