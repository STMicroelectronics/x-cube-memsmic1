/**
  ******************************************************************************
  * @file    com_manager.h
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Header for com_manager.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COM_MANAGER_H
#define __COM_MANAGER_H

#define COM_MAX_SENSORS 10

#define COM_TYPE_ACC    1
#define COM_TYPE_MAG    2
#define COM_TYPE_GYRO   3
#define COM_TYPE_TEMP   4
#define COM_TYPE_PRESS  5
#define COM_TYPE_HUM    6
#define COM_TYPE_MIC    7

#define N_MAX_SESNSOR_COMBO 4
#define N_MAX_SUPPORTED_ODR 16
#define N_MAX_SUPPORTED_FS 16

#define COM_END_OF_LIST_INT -1
#define COM_END_OF_LIST_FLOAT -1.0f

#define COM_LIST_SEPARATOR_INT -2
#define COM_LIST_SEPARATOR_FLOAT -2.0f


/* Commands */
#define CMD_DEVICE                  (uint8_t)(0x00)
#define CMD_DEVICE_HANDLER          (uint8_t)(0x01)
#define CMD_SENSOR_HANDLER          (uint8_t)(0x02)
#define CMD_SENSOR_CONFIG           (uint8_t)(0x03)
#define CMD_SENSOR_STATUS           (uint8_t)(0x04)
#define CMD_SENSOR_UTILITY          (uint8_t)(0x05)
#define CMD_DEVICE_LEN            (uint8_t)(0x06)
#define CMD_DEVICE_HANDLER_LEN    (uint8_t)(0x07)
#define CMD_SENSOR_HANDLER_LEN    (uint8_t)(0x08)
#define CMD_SENSOR_CONFIG_LEN     (uint8_t)(0x09)
#define CMD_SENSOR_STATUS_LEN     (uint8_t)(0x0A)
#define CMD_SENSOR_UTILITY_LEN    (uint8_t)(0x0B)

#define CMD_START_ACQUISITION    (uint8_t)(0x83)
#define CMD_STOP_ACQUISITION     (uint8_t)(0x84)
#define CMD_SERIAL_CMD           (uint8_t)(0x85)
#define CMD_RESET                (uint8_t)(0x87)

#define DATA_TYPE_UINT8     (uint8_t)(0x00)
#define DATA_TYPE_INT8      (uint8_t)(0x01)
#define DATA_TYPE_UINT16    (uint8_t)(0x02)
#define DATA_TYPE_INT16     (uint8_t)(0x03)
#define DATA_TYPE_UINT32    (uint8_t)(0x04)
#define DATA_TYPE_INT32     (uint8_t)(0x05)
#define DATA_TYPE_FLOAT     (uint8_t)(0x06)

/* To be used in COM_Command_t */
#define COM_COMMAND_SET           (uint8_t)(0x00)
#define COM_COMMAND_GET           (uint8_t)(0x01)
#define COM_COMMAND_START         (uint8_t)(0x02)
#define COM_COMMAND_STOP          (uint8_t)(0x03)

#define COM_REQUEST_DEVICE        (uint8_t)(0x00)
#define COM_REQUEST_DEVICE_INFO   (uint8_t)(0x01)
#define COM_REQUEST_DESCRIPTOR    (uint8_t)(0x02)
#define COM_REQUEST_STATUS        (uint8_t)(0x03)

#define COM_COMMAND_ERROR         -1


/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

typedef struct
{
  float             FS;
  float             sensitivity;
  uint8_t                     isActive;
}
COM_SubSensorStatus_t;


typedef struct
{
  uint8_t           id;
  uint8_t               sensorType;
  uint8_t                     dataPerSample;
  char              unit[16];
  float                 FS[N_MAX_SUPPORTED_FS];
}
COM_SubSensorDescriptor_t;


typedef struct
{
  float      ODR;
  float      measuredODR;
  float      initialOffset;
  uint16_t     samplesPerTimestamp;
  uint8_t      isActive;
  uint16_t               usbDataPacketSize;
  uint32_t               sdWriteBufferSize;
  int16_t                comChannelNumber;
  COM_SubSensorStatus_t  subSensorStatus[N_MAX_SESNSOR_COMBO];
}
COM_SensorStatus_t;


typedef struct
{
  uint8_t                                           id;
  char                  name[16];
  uint8_t           dataType;
  float                         ODR[N_MAX_SUPPORTED_ODR];
  uint16_t            samplesPerTimestamp[2];
  uint8_t                                               nSubSensors;
  COM_SubSensorDescriptor_t                         subSensorDescriptor[N_MAX_SESNSOR_COMBO];
}
COM_SensorDescriptor_t;

typedef struct
{
  COM_SensorDescriptor_t sensorDescriptor;
  COM_SensorStatus_t     sensorStatus;
}
COM_Sensor_t;

typedef struct
{
  char     serialNumber[21];
  char     alias[16];
  uint32_t nSensor;
}
COM_DeviceDescriptor_t;


typedef struct
{
  COM_DeviceDescriptor_t deviceDescriptor;
  COM_Sensor_t  *sensors[COM_MAX_SENSORS];
}
COM_Device_t;

typedef struct
{
  int8_t command;
  int8_t request;
  int8_t sensorId;
  int8_t subSensorId;
}
COM_Command_t;


#define HS_DATALOG_IDLE          (uint8_t)(0x00)
#define HS_DATALOG_USB_STARTED   (uint8_t)(0x01)
#define HS_DATALOG_SD_STARTED    (uint8_t)(0x02)

extern volatile uint8_t com_status;


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

int32_t COM_AddSensor(void);
COM_Device_t *COM_GetDevice(void);
COM_DeviceDescriptor_t *COM_GetDeviceDescriptor(void);

COM_Sensor_t *COM_GetSensor(uint8_t sID);

COM_SensorDescriptor_t *COM_GetSensorDescriptor(uint8_t sID);
COM_SensorStatus_t *COM_GetSensorStatus(uint8_t sID);
void COM_SetSensorStatus(uint8_t sID, COM_SensorStatus_t *source);

COM_SubSensorDescriptor_t *COM_GetSubSensorDescriptor(uint8_t sID, uint8_t ssID);
COM_SubSensorStatus_t *COM_GetSubSensorStatus(uint8_t sID, uint8_t ssID);

uint8_t COM_IsFsLegal(float value, uint8_t sID, uint8_t ssID);
uint8_t COM_IsOdrLegal(float value, uint8_t sID);
uint8_t COM_GetSubSensorNumber(uint8_t sID);




#endif /* __COM_MANAGER_H */


