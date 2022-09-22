/**
  ******************************************************************************
  * @file    usbd_cdc_interface.h
  * @author  SRA
  * @version v1.3.1
  * @date    17-Jul-2020
  * @brief   Header for usbd_cdc_interface.c file.
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
#ifndef __USBD_SS_WCID_IF_H
#define __USBD_SS_WCID_IF_H

/* Includes ------------------------------------------------------------------*/
#include "SensorStreaming_WCID.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USB_RxBufferDim                         2048

/* Definition for TIMx clock resources */
#define TIMx                             TIM3
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
#define TIMx_FORCE_RESET()               __HAL_RCC_USART3_FORCE_RESET()
#define TIMx_RELEASE_RESET()             __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                        TIM3_IRQn
#define TIMx_IRQHandler                  TIM3_IRQHandler

//#define APP_TX_DATA_SIZE    4096
#define USB_PACKET_SIZE 2048
#define APP_TX_DATA_SIZE    (USB_PACKET_SIZE*2)

/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on SS_WCID_POLLING_INTERVAL */
#define SS_WCID_POLLING_INTERVAL            5 /* in ms. The max is 65 and the min is 1 */

extern USBD_SS_WCID_ItfTypeDef  USBD_SS_WCID_fops;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t SS_WCID_Fill_Buffer_Sensors(uint8_t *Buf, uint32_t TotalLen);
uint8_t SS_WCID_Fill_Buffer_Audio(uint8_t *Buf, uint32_t TotalLen);
uint8_t SS_WCID_Fill_TxBuffer(uint8_t epNumber, uint8_t *Buf, uint32_t TotalLen);
uint8_t SS_WCID_Periodic_Tx_Call(void);


#endif /* __USBD_SS_WCID_IF_H */


