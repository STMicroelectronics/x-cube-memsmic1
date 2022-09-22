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
#ifndef __DEVICE_DESCRIPTION_H
#define __DEVICE_DESCRIPTION_H

/* Includes ------------------------------------------------------------------*/
#include "com_manager.h"
#include "sensors_manager.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


void set_default_description(void);
void update_sensorStatus(COM_SensorStatus_t *oldSensorStatus, COM_SensorStatus_t *newSensorStatus, uint8_t sID);
void update_sensors_config(void);





#endif /* __DEVICE_DESCRIPTION_H */


