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
uint8_t amicam1_com_id;

uint8_t sensor_first_dataReady[] =  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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
  /* enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();

  /* Configure the system clock */
  SystemClock_Config();

  /* Setup dynamic allocation function for JSON parser */
  HSD_JSON_set_allocation_functions(HSD_malloc, HSD_free);

  /* Start USB */
  MX_USB_DEVICE_Init();

  /* Set default device description */
  set_default_description();

  /* Sensor Manager initialization, only using PID (Platform Indepentent Drivers) */
  SM_Peripheral_Init();

  /* Initialize AMICAM1 peripherals and threads */
  AMICAM1_Peripheral_Init(AUDIO_IN_SAMPLING_FREQUENCY, AUDIO_IN_CHANNELS);
  AMICAM1_OS_Init();

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
  * @brief  System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE
                                     | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {

  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {

  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_DFSDM1 | RCC_PERIPHCLK_USB;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_MSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {

  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {

  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}


/**
  * @brief  Start sensor thread
  * @param  id: Sensor id
  * @retval 0: no error
  */
uint8_t StartSensorThread(uint32_t id)
{
  if (id == amicam1_com_id)
  {
    AMICAM1_Start();
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
  if (id == amicam1_com_id)
  {
    AMICAM1_Stop();
  }
  sensor_first_dataReady[id] = 0;
  return 0;
}


/*  ---------- Sensors data ready functions ----------- */

void AMICAM1_Data_Ready(uint8_t *buf, uint16_t size, double timeStamp)
{
  FFT_Lib_Data_Input((int16_t *)buf, size / 2);
}

/*  --------------------------------------------------- */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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

