/**
  ******************************************************************************
  * @file    com_manager.c
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
#include "com_manager.h"
#include "stdlib.h"
#include "string.h"

volatile COM_Device_t COM_device;
volatile uint8_t com_status = HS_DATALOG_IDLE;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Add Sensor to Db
  * @param None
  * @retval Sensor unique sID
  */
int32_t COM_AddSensor(void)
{
  uint32_t ii = COM_device.deviceDescriptor.nSensor;

  COM_device.sensors[ii] = calloc(1, sizeof(COM_Sensor_t));

  if (COM_device.sensors[ii] == NULL)
  {
    return -1;
  }

  COM_device.sensors[ii]->sensorDescriptor.id = ii;
  COM_device.deviceDescriptor.nSensor++;
  return COM_device.deviceDescriptor.nSensor - 1;
}

/**
  * @brief Get Device Struct
  * @param None
  * @retval whole device Structure
  */
COM_Device_t *COM_GetDevice(void)
{
  return (COM_Device_t *)&COM_device;
}

/**
  * @brief Get Device Descriptor
  * @param None
  * @retval Device Descriptor
  */
COM_DeviceDescriptor_t *COM_GetDeviceDescriptor(void)
{
  return (COM_DeviceDescriptor_t *) & (COM_device.deviceDescriptor);
}

/**
  * @brief Get Sensor
  * @param None
  * @retval Sensor unique sID
  */
COM_Sensor_t *COM_GetSensor(uint8_t sID)
{
  return COM_device.sensors[sID];
}

/**
  * @brief Get Sensor Descriptor
  * @param None
  * @retval Sensor unique sID
  */
COM_SensorDescriptor_t *COM_GetSensorDescriptor(uint8_t sID)
{
  return &(COM_device.sensors[sID]->sensorDescriptor);
}

/**
  * @brief Get Sensor Status
  * @param None
  * @retval Sensor unique sID
  */
COM_SensorStatus_t *COM_GetSensorStatus(uint8_t sID)
{
  return &(COM_device.sensors[sID]->sensorStatus);
}


/**
  * @brief Set Sensor Config
  * @param None
  * @retval Sensor unique ID
  */
void COM_SetSensorStatus(uint8_t sID, COM_SensorStatus_t *source)
{
  memcpy(&(COM_device.sensors[sID]->sensorStatus), source, sizeof(COM_SensorStatus_t));
}

/**
  * @brief Get Sensor Descriptor
  * @param None
  * @retval Sensor unique sID
  */
COM_SubSensorDescriptor_t *COM_GetSubSensorDescriptor(uint8_t sID, uint8_t ssID)
{
  return &(COM_device.sensors[sID]->sensorDescriptor.subSensorDescriptor[ssID]);
}

/**
  * @brief Get Sensor Status
  * @param None
  * @retval Sensor unique sID
  */
COM_SubSensorStatus_t *COM_GetSubSensorStatus(uint8_t sID, uint8_t ssID)
{
  return &(COM_device.sensors[sID]->sensorStatus.subSensorStatus[ssID]);
}


/**
  * @brief Get Sensor Status
  * @param Sensor unique sID
  * @retval number of subsensors
  */
uint8_t COM_GetSubSensorNumber(uint8_t sID)
{
  return COM_device.sensors[sID]->sensorDescriptor.nSubSensors;
}

uint8_t COM_IsFsLegal(float value, uint8_t sID, uint8_t ssID)
{
  uint16_t i = 0;
  uint8_t ret = 0;
  float *list = COM_device.sensors[sID]->sensorDescriptor.subSensorDescriptor[ssID].FS;
  while (list[i] != COM_END_OF_LIST_FLOAT)
  {
    if (list[i] == value)
    {
      ret = 1;
    }
    i++;
  }

  return ret;
}

uint8_t COM_IsOdrLegal(float value, uint8_t sID)
{
  uint16_t i = 0;
  uint8_t ret = 0;
  float *list = COM_device.sensors[sID]->sensorDescriptor.ODR;
  while (list[i] != COM_END_OF_LIST_FLOAT)
  {
    if (list[i] == value)
    {
      ret = 1;
    }
    i++;
  }

  return ret;
}





