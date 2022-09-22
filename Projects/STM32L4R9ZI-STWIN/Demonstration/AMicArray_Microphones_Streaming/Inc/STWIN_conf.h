/**
  ******************************************************************************
  * @file    STWIN_conf.h
  * @author  SRA
  * @version v1.0.1
  * @date    17-Jul-2020
  * @brief   This file contains definitions for the components bus interfaces
  *          This file should be copied to the application folder and renamed
  *          to STWIN_conf.h.
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
#ifndef __STWIN_CONF_H__
#define __STWIN_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32l4xx_hal.h"
#include "STWIN_bus.h"
#include "STWIN_errno.h"

/* AMic_Array */

#define AUDIO_IN_CHANNELS 4
#define AUDIO_IN_SAMPLING_FREQUENCY 96000
#define AUDIO_VOLUME_INPUT 80

#define BSP_ADAU1978_Init BSP_I2C2_Init
#define BSP_ADAU1978_DeInit BSP_I2C2_DeInit
#define BSP_ADAU1978_ReadReg BSP_I2C2_ReadReg
#define BSP_ADAU1978_WriteReg BSP_I2C2_WriteReg

/* Battery Charger */

/* Define 1 to use already implemented callback; 0 to implement callback
   into an application file */

#define USE_BC_TIM_IRQ_CALLBACK         1U
#define USE_BC_GPIO_IRQ_HANDLER         1U
#define USE_BC_GPIO_IRQ_CALLBACK        1U

/* MOTION and ENV sensors */

#define USE_MOTION_SENSOR_IIS2DH_0      0U
#define USE_MOTION_SENSOR_IIS2MDC_0     0U
#define USE_MOTION_SENSOR_IIS3DWB_0     0U
#define USE_MOTION_SENSOR_ISM330DHCX_0  0U
#define USE_ENV_SENSOR_HTS221_0         0U
#define USE_ENV_SENSOR_LPS22HH_0        0U
#define USE_ENV_SENSOR_STTS751_0        0U

/* iis2dh */
#define BSP_IIS2DH_CS_GPIO_CLK_ENABLE() __GPIOD_CLK_ENABLE()
#define BSP_IIS2DH_CS_PORT GPIOD
#define BSP_IIS2DH_CS_PIN GPIO_PIN_15

/* iis3dwb */
#define BSP_IIS3DWB_CS_GPIO_CLK_ENABLE() __GPIOF_CLK_ENABLE()
#define BSP_IIS3DWB_CS_PORT GPIOF
#define BSP_IIS3DWB_CS_PIN GPIO_PIN_5

#define BSP_IIS3DWB_INT1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define BSP_IIS3DWB_INT1_PORT               GPIOE
#define BSP_IIS3DWB_INT1_PIN                GPIO_PIN_14
#define BSP_IIS3DWB_INT1_EXTI_IRQn          EXTI15_10_IRQn
#define BSP_IIS3DWB_INT1_EXTI_IRQ_PP        1
#define BSP_IIS3DWB_INT1_EXTI_IRQ_SP        0
#define BSP_IIS3DWB_INT1_EXTI_IRQHandler    EXTI15_10_IRQHandler

#define BSP_IIS3DWB_INT2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_IIS3DWB_INT2_PORT               GPIOB
#define BSP_IIS3DWB_INT2_PIN                GPIO_PIN_2
#define BSP_IIS3DWB_INT2_EXTI_IRQn          EXTI2_IRQn
#define BSP_IIS3DWB_INT2_EXTI_IRQ_PP        1
#define BSP_IIS3DWB_INT2_EXTI_IRQ_SP        0
#define BSP_IIS3DWB_INT2_EXTI_IRQHandler    EXTI2_IRQHandler

/* ISM330DHCX */
#define BSP_ISM330DHCX_CS_GPIO_CLK_ENABLE() __GPIOF_CLK_ENABLE()
#define BSP_ISM330DHCX_CS_PORT GPIOF
#define BSP_ISM330DHCX_CS_PIN GPIO_PIN_13

#define BSP_ISM330DHCX_INT2_GPIO_CLK_ENABLE() __GPIOF_CLK_ENABLE()
#define BSP_ISM330DHCX_INT2_PORT GPIOF
#define BSP_ISM330DHCX_INT2_PIN GPIO_PIN_4
#define BSP_ISM330DHCX_INT2_EXTI_IRQn           EXTI4_IRQn

/* stts751 */
#define BSP_STTS751_INT_GPIO_CLK_ENABLE() __GPIOE_CLK_ENABLE()
#define BSP_STTS751_INT_PORT GPIOE
#define BSP_STTS751_INT_PIN GPIO_PIN_15
#define BSP_STTS751_INT_EXTI_IRQn           EXTI15_10_IRQn


#ifdef __cplusplus
}
#endif

#endif /* __STWIN_CONF_H__*/



