/**
  ******************************************************************************
  * @file    main.c
  * @author  SRA
  * @version v1.2.0
  * @date    17-Sep-2021
  * @brief   Main program body
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
#include "main.h"
#include "fft_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USBD_HandleTypeDef USBD_Device;
uint8_t AMic_Array_com_id;

uint8_t sensor_first_dataReady[] =  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

extern uint8_t SD_is_logging;
extern uint8_t SD_present;
uint32_t maxWriteTimeSensor[COM_MAX_SENSORS];

EXTI_HandleTypeDef BC_exti;
volatile uint32_t t_stwin = 0;

uint32_t t_start = 0;

extern volatile COM_Device_t COM_device;
COM_Command_t test;

uint8_t *p = 0;

/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup Microphones_Acquisition
  * @{
  */

/** @defgroup MAIN
  * @{
  */

/** @defgroup MAIN_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Functions
  * @{
  */
/* Private function prototypes -----------------------------------------------*/
static void BattChrg_Init(void);
static void BC_Int_Callback(void);
static void Error_Handler(void);
void PVD_Config(void);
void SystemClock_Config(void);
/**
  * @}
  */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32l4xx HAL library initialization:
  - Configure the Flash prefetch, instruction and Data caches
  - Configure the Systick to generate an interrupt each 1 msec
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();

  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddUSB();
  HAL_PWREx_EnableVddIO2();
  BSP_Enable_DCDC2();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the Battery Charger */
  BattChrg_Init();

  /* Configure Power Voltage Detector(PVD) to detect if battery voltage is low */
  PVD_Config();

  /* Configure DEBUG PIN and LED */
  BSP_DEBUG_PIN_Init_All();
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HSD_JSON_set_allocation_functions(HSD_malloc, HSD_free);

  /* Start USB */
  MX_USB_DEVICE_Init();

  /* Set default device description */
  set_default_description();

  /* PWR Button initialization */
  BSP_PB_PWR_Init();

  /* Sensor Manager initialization, only using PID (Platform Indepentent Drivers) */
  SM_Peripheral_Init();

  /* Initialize AMIC_ARRAY peripherals and threads */
  AMIC_ARRAY_Peripheral_Init(AUDIO_IN_SAMPLING_FREQUENCY, AMIC_ARRAY, 1);
  AMIC_ARRAY_OS_Init();

  FFT_OS_Init();

  /* Start scheduler */
  osKernelStart();

  while (1);
}


/* Private functions ---------------------------------------------------------*/

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
  USBD_Init(&USBD_Device, &WCID_STREAMING_Desc, 0);
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_WCID_STREAMING_CLASS);
  /* Add Interface callbacks for AUDIO and CDC Class */
  USBD_WCID_STREAMING_RegisterInterface(&USBD_Device, &USBD_WCID_STREAMING_fops);
  /* Start Device Process */
  USBD_Start(&USBD_Device);
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV5;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_I2C2
                                       | RCC_PERIPHCLK_DFSDM1 | RCC_PERIPHCLK_USB | RCC_PERIPHCLK_SDMMC1
                                       | RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_DFSDM1AUDIO;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  PeriphClkInit.Dfsdm1AudioClockSelection = RCC_DFSDM1AUDIOCLKSOURCE_SAI1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLP;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 5;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 96;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV25;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK | RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


void BattChrg_Init(void)
{
  BSP_BC_Init();
  BSP_BC_BatMS_Init();
  BSP_BC_CmdSend(BATMS_ON);

  HAL_EXTI_GetHandle(&BC_exti, EXTI_LINE_10);
  HAL_EXTI_RegisterCallback(&BC_exti,  HAL_EXTI_COMMON_CB_ID, BC_Int_Callback);

  t_stwin = HAL_GetTick();
}

/**
  * @brief  Battery Charger Interrupt callback
  * @param  None
  * @retval None
  */
void BC_Int_Callback(void)
{
  if (HAL_GetTick() - t_stwin > 4000)
  {
    BSP_BC_CmdSend(SHIPPING_MODE_ON);
  }
}


/**
  * @brief  Configures the PVD resources.
  * @param  None
  * @retval None
  */
void PVD_Config(void)
{
  PWR_PVDTypeDef sConfigPVD;

  /*##-1- Enable Power Clock #################################################*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /*##-2- Configure the NVIC for PVD #########################################*/
  HAL_NVIC_SetPriority(PVD_PVM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PVD_PVM_IRQn);

  /* Configure the PVD Level to 6 and generate an interrupt on falling
  edge(PVD detection level set to 2.9V, refer to the electrical characteristics
  of you device datasheet for more details) */
  sConfigPVD.PVDLevel = PWR_PVDLEVEL_6;
  sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING;
  HAL_PWR_ConfigPVD(&sConfigPVD);

  /* Enable the PVD Output */
  HAL_PWR_EnablePVD();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}


/**
  * @brief  This function is executed in case of error occurrence
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while (1);
}



void vApplicationIdleHook(void)
{
  if (com_status == HS_DATALOG_IDLE)
  {
    if (!(HAL_GetTick() % 1000))
    {
      BSP_LED_On(LED_ORANGE);
    }
    else
    {
      if (!(HAL_GetTick() % 50))
      {
        BSP_LED_Off(LED_ORANGE);
      }
    }
  }
  else if (com_status == HS_DATALOG_USB_STARTED)
  {
    if (!(HAL_GetTick() % 200))
    {
      BSP_LED_On(LED_ORANGE);
    }
    else
    {
      if (!(HAL_GetTick() % 50))
      {
        BSP_LED_Off(LED_ORANGE);
      }
    }
  }
}

/**
  * @brief  Start sensor thread
  * @param  id: Sensor id
  * @retval 0: no error
  */
uint8_t StartSensorThread(uint32_t id)
{
  if (id == AMic_Array_com_id)
  {
    AMIC_ARRAY_Start();
  }
  sensor_first_dataReady[id] = 1;
  return 0;
}

/**
  * @brief  Stop sensor thread
  * @param  id: Sensor id
  * @retval 0: no error
  */
uint8_t StopSensorThread(uint32_t id)
{
  if (id == AMic_Array_com_id)
  {
    AMIC_ARRAY_Stop();
  }
  sensor_first_dataReady[id] = 0;
  return 0;
}


/*  ---------- Sensors data ready functions ----------- */

void AMIC_ARRAY_Data_Ready(uint8_t *buf, uint16_t size, double timeStamp)
{
  FFT_Lib_Data_Input((int16_t *)buf, size / 2);
}

/*  --------------------------------------------------- */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

