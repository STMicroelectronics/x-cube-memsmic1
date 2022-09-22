/**
  ******************************************************************************
  * @file    usbd_cdc_interface.c
  * @author  SRA
  * @version v1.3.1
  * @date    17-Jul-2020
  * @brief   Source file for USBD CDC interface
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
#include "usbd_cdc_interface.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_RX_DATA_SIZE  512

extern uint16_t PCM_Buffer[];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


static uint8_t USB_RxBuffer[APP_RX_DATA_SIZE];/*  */
static uint8_t USB_TxBuffer[N_IN_ENDPOINTS][APP_TX_DATA_SIZE];
static uint32_t USB_TxBuffIdx[N_IN_ENDPOINTS];
static uint8_t USB_TxBuffStatus[N_IN_ENDPOINTS];
static uint8_t dummy[ APP_TX_DATA_SIZE / 2];

/* TIM handler declaration */
TIM_HandleTypeDef  TimHandle;
/* USB handler declaration */
extern USBD_HandleTypeDef  USBD_Device;

/* Private function prototypes -----------------------------------------------*/
static int8_t SS_WCID_Itf_Init(void);
static int8_t SS_WCID_Itf_DeInit(void);
static int8_t SS_WCID_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t SS_WCID_Itf_Receive(uint8_t *pbuf, uint32_t *Len);

static void Error_Handler(void);
static void TIM_Config(void);

USBD_SS_WCID_ItfTypeDef USBD_SS_WCID_fops =
{
  SS_WCID_Itf_Init,
  SS_WCID_Itf_DeInit,
  SS_WCID_Itf_Control,
  SS_WCID_Itf_Receive
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SS_WCID_Itf_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t SS_WCID_Itf_Init(void)
{
  /*##-2- Enable TIM peripherals Clock #######################################*/
  TIMx_CLK_ENABLE();

  /*##-3- Configure the NVIC for TIMx ########################################*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(TIMx_IRQn, 0x6, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);

  /*##-3- Configure the TIM Base generation  #################################*/
  TIM_Config();

  /*##-4- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  /*##-5- Set Application Buffers ############################################*/
  USBD_SS_WCID_SetRxBuffer(&USBD_Device, USB_RxBuffer);

  return (USBD_OK);
}

/**
  * @brief  SS_WCID_Itf_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t SS_WCID_Itf_DeInit(void)
{
  return (USBD_OK);
}


/**
  * @brief  SS_WCID_Itf_Control
  *         Manage the CDC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t SS_WCID_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  switch (cmd)
  {
    case 0x00:
      /* Add your code here */
      break;
    default:
      break;
  }

  return (USBD_OK);
}


uint8_t SS_WCID_Fill_TxBuffer(uint8_t epNumber, uint8_t *Buf, uint32_t TotalLen)
{
  uint32_t i;
  /*Choose the right buffer depending on endpoint number*/
  epNumber &= 0x7F;
  epNumber -= 1;

  uint8_t *txBuffptr = USB_TxBuffer[epNumber];
  uint32_t txBuffIdx = USB_TxBuffIdx[epNumber];

  for (i = 0; i < TotalLen; i++)
  {
    txBuffptr[txBuffIdx] = Buf[i];
    txBuffIdx = (txBuffIdx + 1) % APP_TX_DATA_SIZE;
    if (txBuffIdx == (APP_TX_DATA_SIZE / 2))
    {
      USB_TxBuffStatus[epNumber] = 1;
    }
    else if (txBuffIdx == 0)
    {
      USB_TxBuffStatus[epNumber] = 2;
    }
  }
  USB_TxBuffIdx[epNumber] = txBuffIdx;
  return USBD_OK;
}

uint8_t SS_WCID_Periodic_Tx_Call(void)
{

  int i = 0;

  volatile uint32_t *TX_States = ((USBD_SS_WCID_HandleTypeDef *)(USBD_Device.pClassData))->TXStates;

  for (i = 0; i < N_IN_ENDPOINTS; i++)
  {

    if (!TX_States[i])
    {

      if (USB_TxBuffStatus[i] == 1)
      {
        USB_TxBuffStatus[i] = 0;
        /*send*/
        USBD_SS_WCID_SetTxBuffer(&USBD_Device, (uint8_t *) & (USB_TxBuffer[i][0]), APP_TX_DATA_SIZE / 2);
        if (USBD_SS_WCID_TransmitPacket(&USBD_Device, (i + 1) | 0x80) == USBD_OK)
        {
        }
      }
      else if (USB_TxBuffStatus[i] == 2)
      {
        USB_TxBuffStatus[i] = 0;
        /*send*/
        USBD_SS_WCID_SetTxBuffer(&USBD_Device, (uint8_t *) & (USB_TxBuffer[i][APP_TX_DATA_SIZE / 2]), APP_TX_DATA_SIZE / 2);
        if (USBD_SS_WCID_TransmitPacket(&USBD_Device, (i + 1) | 0x80) == USBD_OK)
        {
        }
      }
    }
  }
  return USBD_OK;
}


/**
  * @brief  SS_WCID_Itf_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  * @param  Buf: Buffer of data to be transmitted
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t SS_WCID_Itf_Receive(uint8_t *Buf, uint32_t *Len)
{
  /*Very basic communication protocol*/
  /*Start Sensors Acquisition*/
  if (USB_RxBuffer[0] == 0x01)
  {
    volatile uint32_t *TX_States = ((USBD_SS_WCID_HandleTypeDef *)(USBD_Device.pClassData))->TXStates;
    int i = 0;

    for (i = 0; i < N_IN_ENDPOINTS; i++)
    {
      TX_States[i] = 0;
      USB_TxBuffIdx[i] = 0;
      USB_TxBuffStatus[i] = 0;
    }
  }
  /*Stop Acquisition*/
  else if (USB_RxBuffer[0] == 0x02)
  {
    int i = 0;
    volatile uint32_t *TX_States = ((USBD_SS_WCID_HandleTypeDef *)(USBD_Device.pClassData))->TXStates;

    for (i = 0; i < N_IN_ENDPOINTS; i++)
    {
      TX_States[i] = 0;
      USB_TxBuffIdx[i] = 0;
      USB_TxBuffStatus[i] = 0;
    }
  }
  /*Prepare for the next receive*/
  USBD_SS_WCID_ReceivePacket(&USBD_Device);
  return (USBD_OK);
}


/**
  * @brief  TIM_Config: Configure TIMx timer
  * @param  None.
  * @retval None.
  */
static void TIM_Config(void)
{
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;

  /* Initialize TIM3 peripheral as follow:
  + Period = 10000 - 1
  + Prescaler = ((SystemCoreClock/2)/10000) - 1
  + ClockDivision = 0
  + Counter direction = Up
  */
  TimHandle.Init.Period = (SS_WCID_POLLING_INTERVAL * 1000) - 1;
  TimHandle.Init.Prescaler = 84 - 1;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Add your own code here */
}


