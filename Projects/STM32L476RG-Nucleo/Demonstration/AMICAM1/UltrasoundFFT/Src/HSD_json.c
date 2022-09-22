/**
  ******************************************************************************
  * @file    HSD_json.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   High Speed DataLog Json Interpreter
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
#include "HSD_json.h"
#include "parson.h"

/* Private variables ---------------------------------------------------------*/

static void (*JSON_free_function)(void *);

/* Private function prototypes -----------------------------------------------*/
static int32_t get_JSON_from_Device(COM_Device_t *device, char **serialized_string, uint8_t pretty);
static int32_t get_JSON_from_DeviceInfo(COM_DeviceDescriptor_t *device_descriptor, char **serialized_string);
static int32_t get_JSON_from_Sensor(COM_Sensor_t *sensor, char **serialized_string);
static int32_t get_JSON_from_SensorDescriptor(COM_SensorDescriptor_t *sensor_descriptor, char **serialized_string);
static int32_t get_JSON_from_SensorStatus(COM_SensorStatus_t *sensor_status, char **serialized_string);
static int32_t get_JSON_from_SubSensorDescriptor(COM_SubSensorDescriptor_t *sub_sensor_descriptor,
                                                 char **serialized_string);
static int32_t get_JSON_from_SubSensorStatus(COM_SubSensorStatus_t *sub_sensor_status, char **serialized_string);
static int32_t parse_Device_from(char *SerializedJSON, COM_Device_t *Device);
static int32_t parse_Command_from(char *SerializedJSON, COM_Command_t *outCommand);
static int32_t parse_Status_from(char *SerializedJSON, COM_SensorStatus_t *sensorStatus);

static void create_JSON_DeviceInfo(COM_DeviceDescriptor_t *device_descriptor, JSON_Value *tempJSON);
static void create_JSON_Sensor(COM_Sensor_t *sensor, JSON_Value *tempJSON);
static void create_JSON_SensorDescriptor(COM_SensorDescriptor_t *sensor_descriptor, JSON_Value *tempJSON);
static void create_JSON_SensorStatus(COM_SensorStatus_t *sensor_status, JSON_Value *tempJSON);
static void create_JSON_SubSensorDescriptor(COM_SubSensorDescriptor_t *sub_sensor_descriptor, JSON_Value *tempJSON);
static void create_JSON_SubSensorStatus(COM_SubSensorStatus_t *sub_sensor_status, JSON_Value *tempJSON);

/* Public function -----------------------------------------------------------*/

/**
  * @brief  Set malloc() and free() Callbacks for
  * @param  malloc_function: malloc() implementation
  * @param  free_function: free() implementation
  * @retval 0: no error
  */
int32_t HSD_JSON_set_allocation_functions(void *(*malloc_function)(size_t), void (*free_function)(void *))
{
  json_set_allocation_functions(malloc_function, free_function);
  JSON_free_function = free_function;
  return 0;
}

int32_t HSD_JSON_free(void *mem)
{
  JSON_free_function(mem);
  return 0;
}

/**
  * @brief  Set malloc() and free() Callbacks for
  * @param  Device: COM_Device_t struct instance to be serialized
  * @param  SerializedJSON: free() implementation
  * @retval 0: no error
  */
int32_t HSD_JSON_serialize_Device(COM_Device_t *Device, char **SerializedJSON, uint8_t pretty)
{
  return get_JSON_from_Device(Device, SerializedJSON, pretty);
}

int32_t HSD_JSON_serialize_DeviceInfo(COM_DeviceDescriptor_t *DeviceInfo, char **SerializedJSON)
{
  return get_JSON_from_DeviceInfo(DeviceInfo, SerializedJSON);
}

int32_t HSD_JSON_serialize_Sensor(COM_Sensor_t *Sensor, char **SerializedJSON)
{
  return get_JSON_from_Sensor(Sensor, SerializedJSON);
}

int32_t HSD_JSON_serialize_SensorDescriptor(COM_SensorDescriptor_t *SensorDescriptor, char **SerializedJSON)
{
  return get_JSON_from_SensorDescriptor(SensorDescriptor, SerializedJSON);
}

int32_t HSD_JSON_serialize_SensorStatus(COM_SensorStatus_t *SensorStatus, char **SerializedJSON)
{
  return get_JSON_from_SensorStatus(SensorStatus, SerializedJSON);
}

int32_t HSD_JSON_serialize_SubSensorDescriptor(COM_SubSensorDescriptor_t *SubSensorDescriptor, char **SerializedJSON)
{
  return get_JSON_from_SubSensorDescriptor(SubSensorDescriptor, SerializedJSON);
}

int32_t HSD_JSON_serialize_SubSensorStatus(COM_SubSensorStatus_t *SubSensorStatus, char **SerializedJSON)
{
  return get_JSON_from_SubSensorStatus(SubSensorStatus, SerializedJSON);
}

int32_t HSD_JSON_parse_Device(char *SerializedJSON, COM_Device_t *Device)
{
  return parse_Device_from(SerializedJSON, Device);
}

int32_t HSD_JSON_parse_Command(char *SerializedJSON, COM_Command_t *Command)
{
  return parse_Command_from(SerializedJSON, Command);
}

int32_t HSD_JSON_parse_Status(char *SerializedJSON, COM_SensorStatus_t *SensorStatus)
{
  return parse_Status_from(SerializedJSON, SensorStatus);
}


/* Private function ----------------------------------------------------------*/
static int32_t get_JSON_from_Device(COM_Device_t *device, char **serialized_string, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Value *tempJSON1;
  JSON_Object *JSON_Device;
  JSON_Array *JSON_SensorArray;
  uint32_t size;
  uint32_t i;

  tempJSON = json_value_init_object();
  JSON_Device = json_value_get_object(tempJSON);

  json_object_dotset_string(JSON_Device, "device.deviceInfo.serialNumber", device->deviceDescriptor.serialNumber);
  json_object_dotset_string(JSON_Device, "device.deviceInfo.alias", device->deviceDescriptor.alias);
  json_object_dotset_number(JSON_Device, "device.deviceInfo.nSensor", device->deviceDescriptor.nSensor);

  json_object_dotset_value(JSON_Device, "device.sensor", json_value_init_array());
  JSON_SensorArray = json_object_dotget_array(JSON_Device, "device.sensor");

  for (i = 0; i < device->deviceDescriptor.nSensor; i++)
  {
    tempJSON1 = json_value_init_object();
    create_JSON_Sensor(device->sensors[i], tempJSON1);
    json_array_append_value(JSON_SensorArray, tempJSON1);
  }

  /* convert to a json string and write to file */
  if (pretty == 1)
  {
    *serialized_string = json_serialize_to_string_pretty(tempJSON);
  }
  else
  {
    *serialized_string = json_serialize_to_string(tempJSON);
  }
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t get_JSON_from_DeviceInfo(COM_DeviceDescriptor_t *device_descriptor, char **serialized_string)
{
  int32_t size = 0;

  JSON_Value *tempJSON = json_value_init_object();
  create_JSON_DeviceInfo(device_descriptor, tempJSON);

  /* convert to a json string and write as string */
  *serialized_string = json_serialize_to_string(tempJSON);
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t get_JSON_from_Sensor(COM_Sensor_t *sensor, char **serialized_string)
{
  int32_t size = 0;

  JSON_Value *tempJSON = json_value_init_object();
  create_JSON_Sensor(sensor, tempJSON);

  /* convert to a json string and write as string */
  *serialized_string = json_serialize_to_string(tempJSON);
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t get_JSON_from_SensorDescriptor(COM_SensorDescriptor_t *sensor_descriptor, char **serialized_string)
{
  int32_t size = 0;

  JSON_Value *tempJSON = json_value_init_object();
  create_JSON_SensorDescriptor(sensor_descriptor, tempJSON);

  /* convert to a json string and write as string */
  *serialized_string = json_serialize_to_string(tempJSON);
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t get_JSON_from_SensorStatus(COM_SensorStatus_t *sensor_status, char **serialized_string)
{
  int32_t size = 0;

  JSON_Value *tempJSON = json_value_init_object();
  create_JSON_SensorStatus(sensor_status, tempJSON);

  /* convert to a json string and write as string */
  *serialized_string = json_serialize_to_string(tempJSON);
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t get_JSON_from_SubSensorDescriptor(COM_SubSensorDescriptor_t *sub_sensor_descriptor,
                                                 char **serialized_string)
{
  int32_t size = 0;

  JSON_Value *tempJSON = json_value_init_object();
  create_JSON_SubSensorDescriptor(sub_sensor_descriptor, tempJSON);

  /* convert to a json string and write as string */
  *serialized_string = json_serialize_to_string(tempJSON);
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t get_JSON_from_SubSensorStatus(COM_SubSensorStatus_t *sub_sensor_status, char **serialized_string)
{
  int32_t size = 0;

  JSON_Value *tempJSON = json_value_init_object();
  create_JSON_SubSensorStatus(sub_sensor_status, tempJSON);

  /* convert to a json string and write as string */
  *serialized_string = json_serialize_to_string(tempJSON);
  size = json_serialization_size(tempJSON);

  json_value_free(tempJSON);

  return size;
}


static int32_t parse_Device_from(char *SerializedJSON, COM_Device_t *Device)
{
  JSON_Object *JSON_subSensorObj;
  JSON_Array *JSON_subSensorParser;
  uint32_t ii, size;
  char *StatusText;

  JSON_Value *tempJSON = json_parse_string(SerializedJSON);
  JSON_Object *JSON_ParseHandler = json_value_get_object(tempJSON);

  if (json_object_dothas_value(JSON_ParseHandler, "device.sensor"))
  {
    JSON_subSensorParser = json_object_dotget_array(JSON_ParseHandler, "device.sensor");
    size = (uint32_t)json_array_get_count(JSON_subSensorParser);

    for (ii = 0; ii < size; ii++)
    {
      JSON_subSensorObj = json_array_get_object(JSON_subSensorParser, ii);
      if (json_object_dothas_value(JSON_subSensorObj, "sensorStatus"))
      {
        StatusText = json_serialize_to_string(json_object_get_value(JSON_subSensorObj, "sensorStatus"));
        parse_Status_from(StatusText, &Device->sensors[ii]->sensorStatus);
      }
    }
  }

  json_value_free(tempJSON);

  return 0;
}

static int32_t parse_Command_from(char *SerializedJSON, COM_Command_t *outCommand)
{
  JSON_Value *tempJSON = json_parse_string(SerializedJSON);
  JSON_Object *JSON_ParseHandler = json_value_get_object(tempJSON);

  if (json_object_dothas_value(JSON_ParseHandler, "command"))
  {
    if (strcmp(json_object_dotget_string(JSON_ParseHandler, "command"), "GET") == 0)
    {
      outCommand->command = COM_COMMAND_GET;
    }
    else if (strcmp(json_object_dotget_string(JSON_ParseHandler, "command"), "SET") == 0)
    {
      outCommand->command = COM_COMMAND_SET;
    }
    else if (strcmp(json_object_dotget_string(JSON_ParseHandler, "command"), "START") == 0)
    {
      outCommand->command = COM_COMMAND_START;
    }
    else if (strcmp(json_object_dotget_string(JSON_ParseHandler, "command"), "STOP") == 0)
    {
      outCommand->command = COM_COMMAND_STOP;
    }
    else
    {
      outCommand->command = COM_COMMAND_ERROR;
    }
  }
  else
  {
    return COM_COMMAND_ERROR;
  }

  if (json_object_dothas_value(JSON_ParseHandler, "request"))
  {
    if (strcmp(json_object_dotget_string(JSON_ParseHandler, "request"), "device") == 0)
    {
      outCommand->request = COM_REQUEST_DEVICE;
    }
    else if (strcmp(json_object_dotget_string(JSON_ParseHandler, "request"), "deviceInfo") == 0)
    {
      outCommand->request = COM_REQUEST_DEVICE_INFO;
    }
    else if (strcmp(json_object_dotget_string(JSON_ParseHandler, "request"), "descriptor") == 0)
    {
      outCommand->request = COM_REQUEST_DESCRIPTOR;
    }
    else if (strcmp(json_object_dotget_string(JSON_ParseHandler, "request"), "status") == 0)
    {
      outCommand->request = COM_REQUEST_STATUS;
    }
    else
    {
      outCommand->request = COM_COMMAND_ERROR;
    }
  }
  else
  {
    outCommand->request = COM_COMMAND_ERROR;
  }

  if (json_object_dothas_value(JSON_ParseHandler, "sensorId"))
  {
    outCommand->sensorId = (int8_t)json_object_dotget_number(JSON_ParseHandler, "sensorId");
  }
  else
  {
    outCommand->sensorId = COM_COMMAND_ERROR;
  }

  if (json_object_dothas_value(JSON_ParseHandler, "subSensorId"))
  {
    outCommand->subSensorId = (int8_t)json_object_dotget_number(JSON_ParseHandler, "subSensorId");
  }
  else
  {
    outCommand->subSensorId = COM_COMMAND_ERROR;
  }

  json_value_free(tempJSON);

  return 0;
}
volatile  int8_t subid = -1;
static int32_t parse_Status_from(char *SerializedJSON, COM_SensorStatus_t *sensorStatus)
{
  JSON_Object *JSON_subSensorObj;
  JSON_Array *JSON_subSensorParser;
  uint32_t ii, size;

  JSON_Value *tempJSON = json_parse_string(SerializedJSON);
  JSON_Object *JSON_ParseHandler = json_value_get_object(tempJSON);

  if (json_object_dothas_value(JSON_ParseHandler, "odr"))
  {
    sensorStatus->ODR = json_object_dotget_number(JSON_ParseHandler, "odr");
  }
  if (json_object_dothas_value(JSON_ParseHandler, "usbDataPacketSize"))
  {
    sensorStatus->usbDataPacketSize = (uint16_t) json_object_dotget_number(JSON_ParseHandler, "usbDataPacketSize");
  }
  if (json_object_dothas_value(JSON_ParseHandler, "sdWriteBufferSize"))
  {
    sensorStatus->sdWriteBufferSize = (uint32_t) json_object_dotget_number(JSON_ParseHandler, "sdWriteBufferSize");
  }
  if (json_object_dothas_value(JSON_ParseHandler, "comChannelNumber"))
  {
    sensorStatus->comChannelNumber = (int16_t) json_object_dotget_number(JSON_ParseHandler, "comChannelNumber");
  }
  if (json_object_dothas_value(JSON_ParseHandler, "samplesPerTs"))
  {
    sensorStatus->samplesPerTimestamp = (uint16_t)json_object_dotget_number(JSON_ParseHandler, "samplesPerTs");
  }
  if (json_object_dothas_value(JSON_ParseHandler, "isActive"))
  {
    if (json_object_dotget_boolean(JSON_ParseHandler, "isActive") == 1)
    {
      sensorStatus->isActive = 1;
    }
    else
    {
      sensorStatus->isActive = 0;
    }
  }
  if (json_object_dothas_value(JSON_ParseHandler, "subSensorStatus"))
  {
    JSON_subSensorParser = json_object_dotget_array(JSON_ParseHandler, "subSensorStatus");
    size = (uint32_t)json_array_get_count(JSON_subSensorParser);

    for (ii = 0; ii < size; ii++)
    {
      JSON_subSensorObj = json_array_get_object(JSON_subSensorParser, ii);

      if (json_object_dothas_value(JSON_subSensorObj, "id"))
      {
        subid = (int8_t)json_object_dotget_number(JSON_subSensorObj, "id");
      }
      else
      {
        subid = ii;
      }

      if (json_object_dothas_value(JSON_subSensorObj, "fs"))
      {
        sensorStatus->subSensorStatus[subid].FS = json_object_dotget_number(JSON_subSensorObj, "fs");
      }
      if (json_object_dothas_value(JSON_subSensorObj, "sensitivity"))
      {
        sensorStatus->subSensorStatus[subid].sensitivity = json_object_dotget_number(JSON_subSensorObj, "sensitivity");
      }
      if (json_object_dothas_value(JSON_subSensorObj, "isActive"))
      {
        if (json_object_dotget_boolean(JSON_subSensorObj, "isActive") == 1)
        {
          sensorStatus->subSensorStatus[subid].isActive = 1;
        }
        else
        {
          sensorStatus->subSensorStatus[subid].isActive = 0;
        }
      }
    }
  }
  json_value_free(tempJSON);

  return 0;
}

static void create_JSON_DeviceInfo(COM_DeviceDescriptor_t *device_descriptor, JSON_Value *tempJSON)
{
  JSON_Object *JSON_DeviceHandler = json_value_get_object(tempJSON);

  json_object_dotset_string(JSON_DeviceHandler, "serialNumber", device_descriptor->serialNumber);
  json_object_dotset_string(JSON_DeviceHandler, "alias", device_descriptor->alias);
  json_object_dotset_number(JSON_DeviceHandler, "nSensor", device_descriptor->nSensor);

}


static void create_JSON_Sensor(COM_Sensor_t *sensor, JSON_Value *tempJSON)
{
  JSON_Value *tempJSON1;
  JSON_Value *tempJSON2;
  JSON_Array *JSON_SensorArray1;
  JSON_Array *JSON_SensorArray2;
  uint32_t ii = 0;
  uint8_t nSubSensors = sensor->sensorDescriptor.nSubSensors;
  uint8_t nSensor = sensor->sensorDescriptor.id;

  JSON_Object *JSON_Sensor = json_value_get_object(tempJSON);

  json_object_dotset_number(JSON_Sensor, "id", nSensor);

  JSON_Value *DescriptorJSON = json_value_init_object();
  json_object_set_value(JSON_Sensor, "sensorDescriptor", DescriptorJSON);
  create_JSON_SensorDescriptor(&sensor->sensorDescriptor, DescriptorJSON);

  json_object_dotset_value(JSON_Sensor, "sensorDescriptor.subSensorDescriptor", json_value_init_array());

  JSON_SensorArray1 = json_object_dotget_array(JSON_Sensor, "sensorDescriptor.subSensorDescriptor");
  for (ii = 0; ii < nSubSensors; ii++)
  {
    tempJSON1 = json_value_init_object();
    create_JSON_SubSensorDescriptor(&sensor->sensorDescriptor.subSensorDescriptor[ii], tempJSON1);
    json_array_append_value(JSON_SensorArray1, tempJSON1);
  }

  JSON_Value *statusJSON = json_value_init_object();
  json_object_set_value(JSON_Sensor, "sensorStatus", statusJSON);
  create_JSON_SensorStatus(&sensor->sensorStatus, statusJSON);

  json_object_dotset_value(JSON_Sensor, "sensorStatus.subSensorStatus", json_value_init_array());
  JSON_SensorArray2 = json_object_dotget_array(JSON_Sensor, "sensorStatus.subSensorStatus");
  for (ii = 0; ii < nSubSensors; ii++)
  {
    tempJSON2 = json_value_init_object();
    create_JSON_SubSensorStatus(&sensor->sensorStatus.subSensorStatus[ii], tempJSON2);
    json_array_append_value(JSON_SensorArray2, tempJSON2);
  }
}


static void create_JSON_SensorDescriptor(COM_SensorDescriptor_t *sensor_descriptor, JSON_Value *tempJSON)
{
  uint32_t ii = 0;

  JSON_Object *JSON_SensorDescriptor = json_value_get_object(tempJSON);
  JSON_Array *JSON_SensorArray1;
  JSON_Value *tempJSON1;

  json_object_dotset_string(JSON_SensorDescriptor, "name", sensor_descriptor->name);

  switch (sensor_descriptor->dataType)
  {
    case DATA_TYPE_UINT8:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "uint8_t");
      break;
    case DATA_TYPE_INT8:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "int8_t");
      break;
    case DATA_TYPE_UINT16:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "uint16_t");
      break;
    case DATA_TYPE_INT16:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "int16_t");
      break;
    case DATA_TYPE_UINT32:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "uint32_t");
      break;
    case DATA_TYPE_INT32:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "int32_t");
      break;
    case DATA_TYPE_FLOAT:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "float");
      break;
    default:
      json_object_dotset_string(JSON_SensorDescriptor, "dataType", "NA");
      break;
  }

  json_object_dotset_number(JSON_SensorDescriptor, "samplesPerTs.min", sensor_descriptor->samplesPerTimestamp[0]);
  json_object_dotset_number(JSON_SensorDescriptor, "samplesPerTs.max", sensor_descriptor->samplesPerTimestamp[1]);
  json_object_dotset_string(JSON_SensorDescriptor, "samplesPerTs.dataType", "int16_t");

  json_object_dotset_value(JSON_SensorDescriptor, "odr.values", json_value_init_array());
  JSON_Array *JSON_SensorArray = json_object_dotget_array(JSON_SensorDescriptor, "odr.values");

  while (sensor_descriptor->ODR[ii] > 0)
  {
    json_array_append_number(JSON_SensorArray, sensor_descriptor->ODR[ii]);
    ii++;
  }

  json_object_dotset_value(JSON_SensorDescriptor, "subSensorDescriptor", json_value_init_array());
  JSON_SensorArray1 = json_object_dotget_array(JSON_SensorDescriptor, "subSensorDescriptor");
  for (ii = 0; ii < sensor_descriptor->nSubSensors; ii++)
  {
    tempJSON1 = json_value_init_object();
    create_JSON_SubSensorDescriptor(&sensor_descriptor->subSensorDescriptor[ii], tempJSON1);
    json_array_append_value(JSON_SensorArray1, tempJSON1);
  }
}


static void create_JSON_SensorStatus(COM_SensorStatus_t *sensor_status, JSON_Value *tempJSON)
{
  JSON_Object *JSON_SensorStatus = json_value_get_object(tempJSON);

  json_object_dotset_number(JSON_SensorStatus, "odr", sensor_status->ODR);
  json_object_dotset_number(JSON_SensorStatus, "odrMeasured", sensor_status->measuredODR);
  json_object_dotset_number(JSON_SensorStatus, "initialOffset", sensor_status->initialOffset);
  json_object_dotset_boolean(JSON_SensorStatus, "isActive", sensor_status->isActive);
  json_object_dotset_number(JSON_SensorStatus, "samplesPerTs", sensor_status->samplesPerTimestamp);
  json_object_dotset_number(JSON_SensorStatus, "usbDataPacketSize", sensor_status->usbDataPacketSize);
  json_object_dotset_number(JSON_SensorStatus, "sdWriteBufferSize", sensor_status->sdWriteBufferSize);
  json_object_dotset_number(JSON_SensorStatus, "comChannelNumber", sensor_status->comChannelNumber);
}


static void create_JSON_SubSensorDescriptor(COM_SubSensorDescriptor_t *sub_sensor_descriptor, JSON_Value *tempJSON)
{
  uint32_t ii = 0;

  JSON_Value *tempJSONarray = json_value_init_object();
  JSON_Array *JSON_SensorArray = json_value_get_array(tempJSONarray);
  JSON_Object *JSON_SubSensorDescriptor = json_value_get_object(tempJSON);

  json_object_dotset_number(JSON_SubSensorDescriptor, "id", sub_sensor_descriptor->id);

  switch (sub_sensor_descriptor->sensorType)
  {
    case COM_TYPE_ACC:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "ACC");
      break;
    case COM_TYPE_MAG:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "MAG");
      break;
    case COM_TYPE_GYRO:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "GYRO");
      break;
    case COM_TYPE_TEMP:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "TEMP");
      break;
    case COM_TYPE_PRESS:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "PRESS");
      break;
    case COM_TYPE_HUM:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "HUM");
      break;
    case COM_TYPE_MIC:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "MIC");
      break;
    default:
      json_object_dotset_string(JSON_SubSensorDescriptor, "sensorType", "NA");
      break;
  }

  json_object_dotset_number(JSON_SubSensorDescriptor, "dataPerSample", sub_sensor_descriptor->dataPerSample);
  json_object_dotset_string(JSON_SubSensorDescriptor, "unit", sub_sensor_descriptor->unit);
  json_object_dotset_value(JSON_SubSensorDescriptor, "fs.values", json_value_init_array());
  JSON_SensorArray = json_object_dotget_array(JSON_SubSensorDescriptor, "fs.values");

  while (sub_sensor_descriptor->FS[ii] > 0)
  {
    json_array_append_number(JSON_SensorArray, sub_sensor_descriptor->FS[ii]);
    ii++;
  }
  json_value_free(tempJSONarray);
}


static void create_JSON_SubSensorStatus(COM_SubSensorStatus_t *sub_sensor_status, JSON_Value *tempJSON)
{
  JSON_Object *JSON_SubSensorStatus = json_value_get_object(tempJSON);

  json_object_dotset_number(JSON_SubSensorStatus, "fs", sub_sensor_status->FS);
  json_object_dotset_number(JSON_SubSensorStatus, "sensitivity", sub_sensor_status->sensitivity);
  json_object_dotset_boolean(JSON_SubSensorStatus, "isActive", sub_sensor_status->isActive);
}



