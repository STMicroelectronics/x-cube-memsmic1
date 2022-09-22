/**
******************************************************************************
* @file    amicam1_audio.c
* @author  SRA
* 
* 
* @brief   This file provides the Audio driver for the amicam1 board
******************************************************************************
* @attention
*
* Copyright (c) 2022 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*                        
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "amicam1_audio.h"
#include "audio.h"

/** @addtogroup BSP
* @{
*/

/** @addtogroup AMICAM1
* @{
*/

/** @addtogroup AMICAM1_AUDIO
* @{
*/ 

/** @defgroup AMICAM1_AUDIO_IN_Private_Types
* @{
*/ 

/**
* @}
*/ 

/** @defgroup AMICAM1_AUDIO_IN_Private_Defines 
* @{
*/ 
#define ADC_CLOCK ((uint32_t)(3072000))

static int32_t DFSDM_internal_buff[((MAX_FS/1000) * 2 * N_MS_PER_INTERRUPT)];
static uint16_t SAI_internal_buff[((MAX_FS/1000) * 2 * MAX_CH_NUMBER *N_MS_PER_INTERRUPT)];

/**
* @}
*/ 

/** @defgroup AMICAM1_AUDIO_IN_Private_Macros 
* @{
*/

/**
* @}
*/ 

/** @defgroup AMICAM1_AUDIO_IN_Private_Variables
* @{
*/
/* Recording context */
AUDIO_IN_Ctx_t                         amicam1_ctx[AUDIO_IN_INSTANCES_NBR] = {0};
static AUDIO_Drv_t                     *AudioDrv = NULL;
static void                            *CompObj = NULL;

SAI_HandleTypeDef hAudioInSai;
ADC_HandleTypeDef hAudioInADC;

DFSDM_Filter_HandleTypeDef   hAudioInDfsdmFilterHandle;
static DFSDM_Channel_HandleTypeDef  hAudioInDfsdmChannelHandle;
DMA_HandleTypeDef ADC_DmaHandle;


/**
* @}
*/ 

/** @defgroup AMICAM1_AUDIO_IN_Private_Function_Prototypes 
* @{
*/ 
HAL_StatusTypeDef MX_SAI_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate);
HAL_StatusTypeDef MX_SAI_Init(SAI_HandleTypeDef* hsai, MX_SAI_Config *MXConfig);

HAL_StatusTypeDef MX_ADC_ClockConfig(ADC_HandleTypeDef* hadc, uint32_t SampleRate);
HAL_StatusTypeDef MX_ADC_Init(ADC_HandleTypeDef* hadc, uint32_t ClockPrescaler);
HAL_StatusTypeDef MX_DFSDM_Init(DFSDM_Filter_HandleTypeDef* hfilter, DFSDM_Channel_HandleTypeDef * hchannel, MX_DFSDM_Config *MXConfig);

static void SAI_MspInit(SAI_HandleTypeDef *hsai);
static void SAI_MspDeInit(SAI_HandleTypeDef *hsai);

static void ADC_MspInit(ADC_HandleTypeDef *hadc);
static void DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hfilter);

static int32_t AD1974_Probe(void);
static int32_t BSP_AD1974_Init(void);
static int32_t BSP_AD1974_DeInit(void);  
static int32_t BSP_AD1974_ReadReg(uint16_t Addr, uint16_t Reg, uint8_t *pdata, uint16_t len);
static int32_t BSP_AD1974_WriteReg(uint16_t Addr, uint16_t Reg, uint8_t *pdata, uint16_t len);
/**    
* @}
*/ 

/** @defgroup AMICAM1_AUDIO_IN_Exported_Functions 
* @{
*/  

__weak int32_t AMICAM1_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t* AudioInit)
{
  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  /* Store the audio record context */
  amicam1_ctx[Instance].Device          = AudioInit->Device;
  amicam1_ctx[Instance].ChannelsNbr     = AudioInit->ChannelsNbr;  
  amicam1_ctx[Instance].SampleRate      = AudioInit->SampleRate; 
  amicam1_ctx[Instance].BitsPerSample   = AudioInit->BitsPerSample;
  amicam1_ctx[Instance].Volume          = AudioInit->Volume;
  amicam1_ctx[Instance].State           = AUDIO_IN_STATE_RESET;
  amicam1_ctx[Instance].Size            = ((AudioInit->SampleRate)/1000U) * 2U * N_MS_PER_INTERRUPT; /* Per single mic, in samples */
  
  /* Device is one of the microphones attached to the extenal ADC */
  if ((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3| AUDIO_IN_ANALOG_MIC4)) != 0U)
  {
    /* Count the microphone in the device variable */
    uint32_t mic_count = (amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC1) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC2) >> 1) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC3) >> 2) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC4) >> 3 ) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC5) >> 4);
    /* return error if counted mics number is different from ch_number or if it's not 2 or 4 */
    if ((mic_count != amicam1_ctx[Instance].ChannelsNbr))// || ((amicam1_ctx[Instance].ChannelsNbr != 2U) && (amicam1_ctx[Instance].ChannelsNbr != 4U)))
    {
      return BSP_ERROR_WRONG_PARAM;
    }
    /* PLL clock SETUP */ 
    if(MX_SAI_ClockConfig(&hAudioInSai, (uint32_t)NULL) != HAL_OK)
    {
      return BSP_ERROR_CLOCK_FAILURE;
    }
    /* SAI data transfer preparation:
    Prepare the Media to be used for the audio transfer from memory to SAI peripheral */
    hAudioInSai.Instance = AMICAM1_SAI_INSTANCE;
    
    SAI_MspInit(&hAudioInSai);
    
    MX_SAI_Config mx_sai_config;
    
    if (amicam1_ctx[Instance].ChannelsNbr == 1U)
    { 
      mx_sai_config.MonoStereoMode = SAI_MONOMODE;      
    }
    else
    {
      mx_sai_config.MonoStereoMode = SAI_STEREOMODE;
    }
    
    if (amicam1_ctx[Instance].ChannelsNbr <= 2U)
    {    
      mx_sai_config.AudioMode = SAI_MODEMASTER_RX;
      mx_sai_config.Synchro = SAI_ASYNCHRONOUS;
      mx_sai_config.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
      mx_sai_config.NoDivider = SAI_MASTERDIVIDER_ENABLE;
      mx_sai_config.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
      mx_sai_config.AudioFrequency = amicam1_ctx[Instance].SampleRate;
      mx_sai_config.SynchroExt = SAI_SYNCEXT_DISABLE;
      mx_sai_config.CompandingMode = SAI_NOCOMPANDING;
    }
    else if (amicam1_ctx[Instance].ChannelsNbr == 4U)
    {
      mx_sai_config.Protocol = SAI_FREE_PROTOCOL;
      mx_sai_config.AudioMode = SAI_MODEMASTER_RX;
      mx_sai_config.DataSize = SAI_DATASIZE_16;
      mx_sai_config.FirstBit = SAI_FIRSTBIT_MSB;
      mx_sai_config.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
      mx_sai_config.Synchro = SAI_ASYNCHRONOUS;
      mx_sai_config.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
      mx_sai_config.NoDivider = SAI_MASTERDIVIDER_ENABLE;
      mx_sai_config.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
      mx_sai_config.AudioFrequency = amicam1_ctx[Instance].SampleRate;
      mx_sai_config.SynchroExt = SAI_SYNCEXT_DISABLE;
      mx_sai_config.CompandingMode = SAI_NOCOMPANDING;      
    }
    else
    {
      return BSP_ERROR_WRONG_PARAM;
    }
    
    /* SAI peripheral initialization: this __weak function can be redefined by the application  */
    (void)MX_SAI_Init(&hAudioInSai, &mx_sai_config);
    
    if(AD1974_Probe()!= BSP_ERROR_NONE)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
    
  }      
  else if((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC5)) != 0U)
  {
    amicam1_ctx[Instance].DecimationFactor = ADC_CLOCK / amicam1_ctx[Instance].SampleRate;
    
    /* Count the microphone in the device variable */    
    uint32_t mic_count = (amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC1) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC2) >> 1) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC3) >> 2) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC4) >> 3) + ((amicam1_ctx[Instance].Device & AUDIO_IN_ANALOG_MIC5) >> 4);
    /* return error if counted mics number is different from ch_number or if it's not 1 */
    if ((mic_count != amicam1_ctx[Instance].ChannelsNbr) || (amicam1_ctx[Instance].ChannelsNbr != 1U))
    {
      return BSP_ERROR_WRONG_PARAM;
    }    
    /* PLL clock SETUP */ 
    if(MX_ADC_ClockConfig(&hAudioInADC, (uint32_t)NULL) != HAL_OK)
    {
      return BSP_ERROR_CLOCK_FAILURE;
    }
    hAudioInADC.Instance = AMICAM1_ADC_INSTANCE;    
    ADC_MspInit(&hAudioInADC);
    
    uint32_t adc_prescaler;
    
    switch(amicam1_ctx[Instance].SampleRate)
    {
    case AUDIO_FREQUENCY_192K:
      adc_prescaler = ADC_CLOCK_ASYNC_DIV1;
      break;
    case AUDIO_FREQUENCY_96K:
      adc_prescaler = ADC_CLOCK_ASYNC_DIV2;
      break;
    case AUDIO_FREQUENCY_48K:
      adc_prescaler = ADC_CLOCK_ASYNC_DIV4;
      break;   
    case AUDIO_FREQUENCY_32K:
      adc_prescaler = ADC_CLOCK_ASYNC_DIV6;
      break;   
    case AUDIO_FREQUENCY_16K:
      adc_prescaler = ADC_CLOCK_ASYNC_DIV12;
      break;      
    default:
      adc_prescaler = ADC_CLOCK_ASYNC_DIV1;
      break;
    }
    
    /* ADC peripheral initialization: this __weak function can be redefined by the application  */
    if(MX_ADC_Init(&hAudioInADC, adc_prescaler) != HAL_OK)
    {
      return BSP_ERROR_PERIPH_FAILURE;
    } 
    
    hAudioInDfsdmFilterHandle.Instance = AMICAM1_DFSDM_FILTER_INSTANCE;
    DFSDM_FilterMspInit(&hAudioInDfsdmFilterHandle);
    
    MX_DFSDM_Config mx_dfsdm_config;
    
#ifdef STM32L4R5xx
    mx_dfsdm_config.Multiplexer = DFSDM_CHANNEL_ADC_OUTPUT;
    mx_dfsdm_config.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO; 
    mx_dfsdm_config.Divider = 1;
    mx_dfsdm_config.Oversampling = 10;
    mx_dfsdm_config.DmaMode = DISABLE;
    mx_dfsdm_config.Trigger = DFSDM_FILTER_SW_TRIGGER;
#else
    mx_dfsdm_config.Multiplexer = DFSDM_CHANNEL_INTERNAL_REGISTER;
    mx_dfsdm_config.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
    mx_dfsdm_config.Divider = 80; /* N.U. */
    mx_dfsdm_config.Oversampling = 1; /* N.U. */
    mx_dfsdm_config.DmaMode = ENABLE; 
    mx_dfsdm_config.Trigger = DFSDM_FILTER_EXT_TRIGGER; /* N.U. */
#endif
    
    /* ADC peripheral initialization: this __weak function can be redefined by the application  */
    if(MX_DFSDM_Init(&hAudioInDfsdmFilterHandle, &hAudioInDfsdmChannelHandle, &mx_dfsdm_config) != HAL_OK)
    {
      return BSP_ERROR_PERIPH_FAILURE;
    }
  }
  else
  {
    return BSP_ERROR_WRONG_PARAM;
  }  
  /* Update BSP AUDIO IN state */     
  amicam1_ctx[Instance].State = AUDIO_IN_STATE_STOP; 
  /* Return BSP status */
  return BSP_ERROR_NONE; 
}


/**
* @brief  Deinit the audio IN peripherals.
* @param  Instance  AUDIO IN Instance. It can be AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @retval BSP status
*/

__weak int32_t AMICAM1_AUDIO_IN_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }	
  if ((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3| AUDIO_IN_ANALOG_MIC4)) != 0U)
  {
    if (hAudioInSai.Instance != NULL)
    {
      SAI_MspDeInit(&hAudioInSai);
      /* Call the Media layer stop function */
      if(AudioDrv->DeInit(CompObj) != 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }   
      if(HAL_OK != HAL_SAI_DMAStop(&hAudioInSai))
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }
  else
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  
  /* Update BSP AUDIO IN state */     
  amicam1_ctx[Instance].State = AUDIO_IN_STATE_RESET; 
  return ret;
}


/**
* @brief  Start audio recording.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  pbuf     Main buffer pointer for the recorded data storing  
* @param  Size     Size of the record buffer
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_Record(uint32_t Instance, uint8_t* pBuf, uint32_t NbrOfBytes)
{
  UNUSED(NbrOfBytes);
  
  int32_t ret = BSP_ERROR_NONE;
  amicam1_ctx[Instance].pBuff = (uint16_t*)pBuf;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  if ((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3| AUDIO_IN_ANALOG_MIC4)) != 0U)
  {
    if (amicam1_ctx[Instance].ChannelsNbr <= 2U)
    {    
      if(HAL_SAI_InitProtocol(&hAudioInSai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BITEXTENDED, 2) != HAL_OK)
      {
        ret = (int32_t)HAL_ERROR;
      }
    }
    else if (amicam1_ctx[Instance].ChannelsNbr == 4U)
    {     
      if (HAL_SAI_Init(&hAudioInSai) != HAL_OK)
      {
        ret = (int32_t)HAL_ERROR;
      }      
    } 
    else
    {
      ret = BSP_ERROR_WRONG_PARAM;
    }
    uint32_t total_size = (amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].Size * amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].ChannelsNbr * N_MS_PER_INTERRUPT );
    
    if (HAL_OK != HAL_SAI_Receive_DMA(&hAudioInSai, (uint8_t *)SAI_internal_buff, (uint16_t)(total_size)))
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
    
    if(AudioDrv->Play(CompObj) != 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }   
    
  }
  else
  {    
    
#ifdef STM32L4R5xx
    if (HAL_ADC_Start(&hAudioInADC) != HAL_OK)
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
#endif
    
    (void)HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilterHandle, DFSDM_internal_buff, (amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].Size));  
    
#ifdef STM32L476xx 
    uint32_t adc_size =  amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].Size * amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].DecimationFactor;   
    
    /*DMA peripheral to peripheral is not available on all the MCUs */
    if (HAL_ADC_Start_DMA(&hAudioInADC, (uint32_t *)&(hAudioInDfsdmChannelHandle.Instance->CHDATINR), adc_size) != HAL_OK)
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;  
    }    
#endif
  }	  
  
  /* Update BSP AUDIO IN state */     
  amicam1_ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
  return ret;  
}


/**
* @brief  Stop audio recording.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_Stop(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR) 
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
  if ((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3| AUDIO_IN_ANALOG_MIC4)) != 0U)
  {
    /* Call the Media layer stop function */
    if(AudioDrv->Stop(CompObj, 0) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if(HAL_SAI_DMAStop(&hAudioInSai)!= HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }
  else
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }	
  
  /* Update BSP AUDIO IN state */     
  amicam1_ctx[Instance].State = AUDIO_IN_STATE_STOP;
  return ret; 	
}


/**
* @brief  Pause the audio file stream.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_Pause(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR) 
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
  if ((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3| AUDIO_IN_ANALOG_MIC4)) != 0U)
  {
    /* Call the Media layer stop function */
    if(AudioDrv->Stop(CompObj, 0) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if(HAL_SAI_DMAStop(&hAudioInSai)!= HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }		
  }
  else
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  
  /* Update BSP AUDIO IN state */     
  amicam1_ctx[Instance].State = AUDIO_IN_STATE_PAUSE;
  return ret; 	
}


/**
* @brief  Resume the audio file stream.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_Resume(uint32_t Instance)
{  
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }	
  if ((amicam1_ctx[Instance].Device & (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3| AUDIO_IN_ANALOG_MIC4)) != 0U)
  {
    (void)HAL_SAI_Init(&hAudioInSai);
    /* Call the Media layer stop function */
    if(AudioDrv->Resume(CompObj) != 0) 
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    } 
    if (HAL_OK != HAL_SAI_Receive_DMA(&hAudioInSai, (uint8_t *)amicam1_ctx[Instance].pBuff, (uint16_t)amicam1_ctx[Instance].Size))
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  else
  {
    ret =  BSP_ERROR_PERIPH_FAILURE;
  }	  
  
  /* Update BSP AUDIO IN state */     
  amicam1_ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
  return ret;  
}


/**
* @brief  Starts audio recording.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  pBuf      Main buffer pointer for the recorded data storing
* @param  size      Size of the recorded buffer
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes)
{
  UNUSED(Instance);
  UNUSED(pBuf);
  UNUSED(NbrOfBytes);
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}


/**
* @brief  Stop audio recording.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Device    Input device to be stopped
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device)
{
  UNUSED(Instance);
  UNUSED(Device);
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}


/**
* @brief  Pause the audio file stream.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Device    Mic to be paused
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device)
{
  UNUSED(Instance);
  UNUSED(Device);
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}


/**
* @brief  Resume the audio file stream
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Device    Mic to be resumed
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device)
{
  UNUSED(Instance);
  UNUSED(Device);
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}


/**
* @brief  Set Audio In device
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Device    The audio input device to be used
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device)
{  
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (AMICAM1_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }
    
    audio_init.Device = Device;
    audio_init.ChannelsNbr   = amicam1_ctx[Instance].ChannelsNbr;  
    audio_init.SampleRate    = amicam1_ctx[Instance].SampleRate;   
    audio_init.BitsPerSample = amicam1_ctx[Instance].BitsPerSample;
    audio_init.Volume        = amicam1_ctx[Instance].Volume;
    
    if(AMICAM1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In device
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Device    The audio input device used
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Return audio Input Device */
    *Device = amicam1_ctx[Instance].Device;
  }
  return ret;
}


/**
* @brief  Set Audio In frequency
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  SampleRate  Input frequency to be set
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t  SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {	
    if (AMICAM1_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }	
    
    audio_init.Device 		 = amicam1_ctx[Instance].Device;
    audio_init.ChannelsNbr   = amicam1_ctx[Instance].ChannelsNbr;  
    audio_init.SampleRate    = SampleRate;   
    audio_init.BitsPerSample = amicam1_ctx[Instance].BitsPerSample;
    audio_init.Volume        = amicam1_ctx[Instance].Volume;
    
    if(AMICAM1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In frequency
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  SampleRate  Audio Input frequency to be returned
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio in frequency */
    *SampleRate = amicam1_ctx[Instance].SampleRate;
  }
  
  /* Return BSP status */  
  return ret;
}


/**
* @brief  Set Audio In Resolution
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  BitsPerSample  Input resolution to be set
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {	
    if (AMICAM1_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }	
    
    audio_init.Device           = amicam1_ctx[Instance].Device;
    audio_init.ChannelsNbr      = amicam1_ctx[Instance].ChannelsNbr;  
    audio_init.SampleRate       = amicam1_ctx[Instance].SampleRate; 
    audio_init.BitsPerSample    = BitsPerSample;
    audio_init.Volume           = amicam1_ctx[Instance].Volume;
    
    if(AMICAM1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In Resolution
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  BitsPerSample  Input resolution to be returned
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Return audio in resolution */
    *BitsPerSample = amicam1_ctx[Instance].BitsPerSample;
  }
  return ret;
}


/**
* @brief  Set Audio In Channel number
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  ChannelNbr  Channel number to be used
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {	
    if (AMICAM1_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }	
    
    audio_init.Device 	     = amicam1_ctx[Instance].Device;
    audio_init.ChannelsNbr   = ChannelNbr;  
    audio_init.SampleRate    = amicam1_ctx[Instance].SampleRate; 
    audio_init.BitsPerSample = amicam1_ctx[Instance].BitsPerSample;
    audio_init.Volume        = amicam1_ctx[Instance].Volume;
    
    if(AMICAM1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In Channel number
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  ChannelNbr  Channel number to be used
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Channel number to be returned */
    *ChannelNbr = amicam1_ctx[Instance].ChannelsNbr;
  }
  return ret;	
}


/**
* @brief  Set the current audio in volume level.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Volume    Volume level to be returned
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  uint32_t counter;
  for (counter = 0U; counter <  amicam1_ctx[Instance].ChannelsNbr; counter ++)
  {

  } 
  
  /* Update AudioIn Context */
  amicam1_ctx[Instance].Volume = Volume;
  /* Return BSP status */
  return ret;  
}


/**
* @brief  Get the current audio in volume level.
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  Volume    Volume level to be returned
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }  
  else
  {
    /* Input Volume to be returned */
    *Volume = amicam1_ctx[Instance].Volume;
  }
  /* Return BSP status */
  return ret;  
}


/**
* @brief  Get Audio In device
* @param  Instance  AUDIO IN Instance. AUDIO_IN_INSTANCE_ADC_INT or AUDIO_IN_INSTANCE_ADC_EXT
* @param  State     Audio in state
* @retval BSP status
*/
int32_t AMICAM1_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Input State to be returned */
    *State = amicam1_ctx[Instance].State;
  }
  return ret;
}


/**
* @brief  Rx Transfer completed callbacks.
* @param  hsai SAI handle
* @retval None
*/
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);  
  uint32_t idx;
  uint32_t total_size_per_interrupt = (amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].Size * amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].ChannelsNbr * N_MS_PER_INTERRUPT ) / 2U;
  for (idx = 0; idx < total_size_per_interrupt ;idx ++)
  {
    ((uint16_t *)(amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].pBuff))[idx] = SAI_internal_buff[total_size_per_interrupt + idx];
  }
  
  AMICAM1_AUDIO_IN_TransferComplete_CallBack(0);
}


/**
* @brief  Rx Half Transfer completed callbacks.
* @param  hsai  SAI handle
* @retval None
*/
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);
  uint32_t idx;
  uint32_t total_size_per_interrupt = (amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].Size * amicam1_ctx[AUDIO_IN_INSTANCE_ADC_EXT].ChannelsNbr * N_MS_PER_INTERRUPT ) / 2U;
  
  for (idx = 0; idx < total_size_per_interrupt ;idx ++)
  {
    ((uint16_t *)(amicam1_ctx[0].pBuff))[idx]  = SAI_internal_buff[idx];
  }
  
  AMICAM1_AUDIO_IN_HalfTransfer_CallBack(0);
}


/**
* @brief  SAI error callbacks.
* @param  hsai  SAI handle
* @retval None
*/
void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);
  
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */ 
}

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{  
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdfsdm_filter);
  
  uint32_t size = amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].Size / 2U * N_MS_PER_INTERRUPT;
  uint16_t idx;  
  int32_t * ptr = (int32_t *)&DFSDM_internal_buff[size];
  
  for (idx = 0; idx < size ;idx ++)
  {
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].Z = ((ptr[idx] /256) * (int32_t)(amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].Volume))/64;
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldOut = (0xFC * (amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldOut + amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].Z - amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldIn)) / 256;
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldIn = amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].Z;
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].pBuff[idx] = (uint16_t) SaturaLH(amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldOut, -32760, 32760);         
  }  
  
  AMICAM1_AUDIO_IN_TransferComplete_CallBack(0);
}

/**
* @brief  Half regular conversion complete callback.
* @param  hdfsdm_filter : DFSDM filter handle.
* @retval None
*/
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter) 
{  
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdfsdm_filter);
  
  uint32_t size = amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].Size / 2U * N_MS_PER_INTERRUPT;
  uint16_t idx;  
  int32_t * ptr = (int32_t *)DFSDM_internal_buff;
  
  for (idx = 0; idx < size ;idx ++)
  {
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].Z = ((ptr[idx] / 256) * (int32_t)(amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].Volume))/64;
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldOut = (0xFC * (amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldOut + amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].Z - amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldIn)) / 256;
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldIn = amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].Z;
    amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].pBuff[idx] = (uint16_t) SaturaLH(amicam1_ctx[AUDIO_IN_INSTANCE_ADC_INT].HP_Filters[1].oldOut, -32760, 32760);  
  }  
  
  AMICAM1_AUDIO_IN_TransferComplete_CallBack(0);
}


/**
* @brief  User callback when record buffer is filled.
* @retval None
*/
__weak void AMICAM1_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
* @brief  Manages the DMA Half Transfer complete event.
* @retval None
*/
__weak void AMICAM1_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
* @brief  Audio IN Error callback function.
* @retval None
*/
__weak void AMICAM1_AUDIO_IN_Error_CallBack(uint32_t Instance)
{ 
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  
  /* This function is called when an Interrupt due to transfer error on or peripheral
  error occurs. */
}


/**
* @brief  SAI clock Config.
* @param  hsai SAI handle
* @param  SampleRate  Audio frequency used to play the audio stream.
* @note   Being __weak it can be overwritten by the application     
* @retval HAL status
*/
__weak HAL_StatusTypeDef MX_SAI_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
  /* Todo: remove unused PLL */
  HAL_StatusTypeDef ret = HAL_OK;
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);
  UNUSED(SampleRate);
  
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;  
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
  
  /* SAI clock config */
  RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
  RCC_ExCLKInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI2;
  
#ifdef STM32L4R5xx  
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2Source = RCC_PLLSOURCE_HSE;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2M = 2;  
#else
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2Source = RCC_PLLSOURCE_HSI;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2M = 4;    
#endif
  
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2N = 86;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2P = RCC_PLLP_DIV7;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2R = RCC_PLLR_DIV4;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_SAI2CLK;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  
  return ret;
}

/**
* @brief  ADC clock Config.
* @param  hadc ADC handle
* @param  SampleRate  Audio frequency used to play the audio stream.
* @note   Being __weak it can be overwritten by the application
* @retval HAL status
*/
__weak HAL_StatusTypeDef MX_ADC_ClockConfig(ADC_HandleTypeDef* hadc, uint32_t SampleRate)
{
  /* Todo: remove unused PLL */
  HAL_StatusTypeDef ret = HAL_OK;
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  UNUSED(SampleRate);
  
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
  
  /* SAI clock config */
  RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  RCC_ExCLKInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  
#ifdef STM32L4R5xx  
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1M = 2;
#else
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1M = 4;
#endif
  
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1N = 69;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV6;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  
  /* ADC Periph interface clock configuration */
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_PLLSAI1);
  
  return ret;
  
}


/**
* @brief  Initializes the DFSDM.
* @param  MXConfig DFSDM configuration structure
* @note   Being __weak it can be overwritten by the application
* @retval HAL status
*/
__weak HAL_StatusTypeDef MX_ADC_Init(ADC_HandleTypeDef* hadc, uint32_t ClockPrescaler)
{ 
  static ADC_ChannelConfTypeDef sConfig;
  
  if (HAL_ADC_DeInit(hadc) != HAL_OK)
  {
    /* ADC de-initialization Error */
    return HAL_ERROR;
  }
  
  hadc->Init.ClockPrescaler = ClockPrescaler;           /* Synchronous clock mode, input ADC clock divided by 4*/
  hadc->Init.Resolution = ADC_RESOLUTION_12B;           /* 16-bit resolution for converted data */
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
  hadc->Init.ScanConvMode = (uint32_t)DISABLE;          /* Sequencer ENABLED/disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;        /* EOC flag picked-up to indicate conversion end */
  hadc->Init.LowPowerAutoWait = DISABLE;                /* Auto-delayed conversion feature disabled */
  hadc->Init.ContinuousConvMode = ENABLE;               /* Continuous mode enabled (automatic conversion restart after each conversion) */
  hadc->Init.NbrOfConversion = 1;                       /* Parameter discarded because sequencer is disabled */
  hadc->Init.DiscontinuousConvMode = DISABLE;           /* Parameter discarded because sequencer is disabled */
  hadc->Init.NbrOfDiscConversion = 1;                   /* Parameter discarded because sequencer is disabled */
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;     /* Software start to trig the 1st conversion manually, without external event */
#ifdef STM32L4R5xx
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.DMAContinuousRequests = DISABLE;                       
#else
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;    /* Parameter discarded because software trigger chosen */
  hadc->Init.DMAContinuousRequests = ENABLE;                            /* DMA circular mode selected */
#endif
  hadc->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;        /* DR register is overwritten with the last conversion result in case of overrun */
  hadc->Init.OversamplingMode = DISABLE;                /* Oversampling enabled */
  hadc->Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_NONE;                       /* Right shift of the oversampled summation */
  hadc->Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;             /* Specifies whether or not a trigger is needed for each sample */
  hadc->Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;   /* Specifies whether or not the oversampling buffer is maintained during injection sequence */
  
#ifdef STM32L4R5xx  
  hadc->Init.DFSDMConfig = ADC_DFSDM_MODE_ENABLE;
#endif  
  
  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel = ADC_CHANNEL_5;                      /* Sampled channel number */
  sConfig.Rank = ADC_REGULAR_RANK_1;                    /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;      /* Sampling time (number of clock cycles unit) */
  sConfig.SingleDiff = ADC_SINGLE_ENDED;                /* Single-ended/DIFFERENTIAL input channel */
  sConfig.OffsetNumber = ADC_OFFSET_1;                  /* Offset subtraction */
  sConfig.Offset = 2048;                                /* Parameter discarded because offset correction is disabled */
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  return HAL_OK;
}

/**
* @brief  Initialize DFSDM MSP.
* @param  hsai  DFSDM handle 
* @retval None
*/

static void ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  static DMA_HandleTypeDef DmaHandle;
  GPIO_InitTypeDef  GPIO_InitStruct;  
  
  if(hadc->Instance == AMICAM1_ADC_INSTANCE)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO clock ****************************************/
    AMICAM1_ADC_PIN_CLK_ENABLE();
    /* ADC Periph clock enable */
    AMICAM1_ADC_CLK_ENABLE();
    /* Enable DMA clock */
    AMICAM1_ADC_DMAx_CLK_ENABLE();
#ifdef STM32L4R5xx
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
#endif
    
    /*##- 2- Configure peripheral GPIO #########################################*/
    /* ADC Channel GPIO pin configuration */
    GPIO_InitStruct.Pin = AMICAM1_ADC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AMICAM1_ADC_GPIO_PORT, &GPIO_InitStruct);
    /*##- 3- Configure DMA #####################################################*/
    
    /*********************** Configure DMA parameters ***************************/
    DmaHandle.Instance                 = AMICAM1_ADC_DMAx_INSTANCE;
    DmaHandle.Init.Request             = AMICAM1_ADC_DMAx_REQUEST;
    DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
#ifdef STM32L4R5xx
    DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
#else
    DmaHandle.Init.MemInc              = DMA_MINC_DISABLE;
#endif
    DmaHandle.Init.PeriphDataAlignment = AMICAM1_ADC_DMAx_PERIPH_DATA_SIZE;
    DmaHandle.Init.MemDataAlignment    = AMICAM1_ADC_DMAx_MEM_DATA_SIZE;
    DmaHandle.Init.Mode                = DMA_CIRCULAR;
    DmaHandle.Init.Priority            = DMA_PRIORITY_MEDIUM;
    /* Deinitialize  & Initialize the DMA for new transfer */
    (void)HAL_DMA_DeInit(&DmaHandle);
    (void)HAL_DMA_Init(&DmaHandle);
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
    
#ifdef STM32L4R5xx
    /* NVIC configuration for DMA Input data interrupt */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn); 
#endif    
  }
}


/**
* @brief  Initializes the DFSDM.
* @param  MXConfig DFSDM configuration structure
* @note   Being __weak it can be overwritten by the application
* @retval HAL status
*/
__weak HAL_StatusTypeDef MX_DFSDM_Init(DFSDM_Filter_HandleTypeDef* hfilter, DFSDM_Channel_HandleTypeDef * hchannel, MX_DFSDM_Config *MXConfig)
{ 
  /* Initialize channel */
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(hchannel);
  
  hchannel->Init.Input.Multiplexer        = MXConfig->Multiplexer;
  hchannel->Init.OutputClock.Selection    = MXConfig->Selection;
  hchannel->Init.OutputClock.Divider      = MXConfig->Divider;
  hchannel->Init.Awd.Oversampling         = MXConfig->Oversampling;
  
  hchannel->Instance                      = DFSDM1_Channel0;
  hchannel->Init.OutputClock.Activation   = DISABLE;
  hchannel->Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  hchannel->Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS; 
  hchannel->Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
  hchannel->Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hchannel->Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER;
  hchannel->Init.Offset                   = 0;
  hchannel->Init.RightBitShift            = 4;
  
  if(HAL_OK != HAL_DFSDM_ChannelInit(hchannel))
  {
    return HAL_ERROR;
  }
  /* Initialize filter */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(hfilter);
  
  hfilter->Init.InjectedParam.DmaMode        = MXConfig->DmaMode; 
  hfilter->Init.InjectedParam.Trigger        = MXConfig->Trigger;
  
  hfilter->Instance                          = DFSDM1_Filter0;
  hfilter->Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  hfilter->Init.RegularParam.FastMode        = ENABLE;
  hfilter->Init.RegularParam.DmaMode         = ENABLE;
  hfilter->Init.InjectedParam.ScanMode       = DISABLE;
  hfilter->Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO;
  hfilter->Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_RISING_EDGE; 
  hfilter->Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC2_ORDER;
  hfilter->Init.FilterParam.Oversampling     = 16;
  hfilter->Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(hfilter))
  {
    return HAL_ERROR;
  }
  
  (void)HAL_DFSDM_FilterConfigRegChannel(hfilter, (DFSDM_CHANNEL_0),DFSDM_CONTINUOUS_CONV_ON);
  
  return HAL_OK;
}

/**
* @brief  Initialize DFSDM MSP.
* @param  hsai  DFSDM handle
* @retval None
*/
static void DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hfilter) 
{  
  if(hfilter->Instance == AMICAM1_DFSDM_FILTER_INSTANCE)
  {
    AMICAM1_DFSDM_CLK_ENABLE();
    AMICAM1_DFSDM_DMAx_CLK_ENABLE();
#ifdef STM32L4R5xx
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
#endif
    
    ADC_DmaHandle.Init.Request = AMICAM1_DFSDM_DMAx_REQUEST;
    ADC_DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    ADC_DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    ADC_DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
    ADC_DmaHandle.Init.PeriphDataAlignment = AMICAM1_DFSDM_DMAx_PERIPH_DATA_SIZE;
    ADC_DmaHandle.Init.MemDataAlignment = AMICAM1_DFSDM_DMAx_MEM_DATA_SIZE;
    ADC_DmaHandle.Init.Mode = DMA_CIRCULAR;
    ADC_DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
    ADC_DmaHandle.Instance = AMICAM1_DFSDM_DMAx_INSTANCE;
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hfilter, hdmaReg, ADC_DmaHandle);
    
    /* Reset DMA handle state */
    __HAL_DMA_RESET_HANDLE_STATE(&ADC_DmaHandle);
    
    /* Configure the DMA Channel */
    (void)HAL_DMA_Init(&ADC_DmaHandle);
    
    HAL_NVIC_SetPriority(AMICAM1_DFSDM_DMAx_IRQ, 5, 0);
    HAL_NVIC_EnableIRQ(AMICAM1_DFSDM_DMAx_IRQ);
  }
}


/**
* @brief  Initializes the Audio instance (SAI).
* @param  MXConfig SAI configuration structure
* @note   Being __weak it can be overwritten by the application
* @retval HAL status
*/
__weak HAL_StatusTypeDef MX_SAI_Init(SAI_HandleTypeDef* hsai, MX_SAI_Config *MXConfig)
{ 
  HAL_StatusTypeDef ret = HAL_OK;
  
  /* Disable SAI peripheral to allow access to SAI internal registers */
  __HAL_SAI_DISABLE(hsai);
  
  /* Configure SAI */
  hsai->Init.Protocol =         MXConfig->Protocol;     
  hsai->Init.DataSize =         MXConfig->DataSize;     
  hsai->Init.FirstBit =         MXConfig->FirstBit;     
  hsai->Init.ClockStrobing =    MXConfig->ClockStrobing;  
  hsai->Init.AudioMode =        MXConfig->AudioMode;     
  hsai->Init.Synchro =          MXConfig->Synchro;       
  hsai->Init.OutputDrive =      MXConfig->OutputDrive;   
  hsai->Init.NoDivider =        MXConfig->NoDivider;     
  hsai->Init.FIFOThreshold =    MXConfig->FIFOThreshold; 
  hsai->Init.AudioFrequency =   MXConfig->AudioFrequency;
  hsai->Init.SynchroExt =       MXConfig->SynchroExt;    
  hsai->Init.MonoStereoMode =   MXConfig->MonoStereoMode;
  hsai->Init.CompandingMode =   MXConfig->CompandingMode;
  
  hsai->FrameInit.FrameLength = 256;
  hsai->FrameInit.ActiveFrameLength = 128;
  hsai->FrameInit.FSDefinition = SAI_FS_STARTFRAME;
  hsai->FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai->FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
  hsai->SlotInit.FirstBitOffset = 0;
  hsai->SlotInit.SlotSize = SAI_SLOTSIZE_32B;
  hsai->SlotInit.SlotNumber = 8;
  hsai->SlotInit.SlotActive = 0x0000000F;
  
  __HAL_SAI_ENABLE(hsai);
  
  return ret;
}


/**
* @brief  Initialize SAI MSP.
* @param  hsai  SAI handle 
* @retval None
*/
static void SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  static DMA_HandleTypeDef hdma_SaiRx_1;
  GPIO_InitTypeDef  GPIO_InitStruct;  
  
  if(hsai->Instance == AMICAM1_SAI_INSTANCE)
  {
#ifdef STM32L4R5xx
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
#endif
    /* Enable SAI clock */
    AMICAM1_SAI_CLK_ENABLE();
    
    /* Enable GPIO clock */
    AMICAM1_SD_CLK_ENABLE(); 
    AMICAM1_SCK_CLK_ENABLE();
    AMICAM1_MCLK_CLK_ENABLE();
    AMICAM1_FS_CLK_ENABLE();
    
    /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins ------------------*/
    GPIO_InitStruct.Pin         = AMICAM1_SD_PIN ; 
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull        = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate   = AMICAM1_SD_AF;
    HAL_GPIO_Init(AMICAM1_SD_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin         = AMICAM1_SCK_PIN ;
    GPIO_InitStruct.Alternate   = AMICAM1_SCK_AF;
    HAL_GPIO_Init(AMICAM1_SCK_GPIO_PORT, &GPIO_InitStruct);     
    
    GPIO_InitStruct.Pin         = AMICAM1_MCLK_PIN ;
    GPIO_InitStruct.Alternate   = AMICAM1_MCLK_AF;
    HAL_GPIO_Init(AMICAM1_MCLK_GPIO_PORT, &GPIO_InitStruct);   
    
    GPIO_InitStruct.Pin         = AMICAM1_FS_PIN ;
    GPIO_InitStruct.Alternate   = AMICAM1_FS_AF;
    HAL_GPIO_Init(AMICAM1_FS_GPIO_PORT, &GPIO_InitStruct);    
    
    /* Enable the DMA clock */
    AMICAM1_DMAx_CLK_ENABLE(); 
    
    hdma_SaiRx_1.Instance                 = AMICAM1_DMAx_INSTANCE;  
    hdma_SaiRx_1.Init.Request             = AMICAM1_DMAx_REQUEST;   
    hdma_SaiRx_1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_SaiRx_1.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_SaiRx_1.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_SaiRx_1.Init.PeriphDataAlignment = AMICAM1_DMAx_PERIPH_DATA_SIZE;
    hdma_SaiRx_1.Init.MemDataAlignment    = AMICAM1_DMAx_MEM_DATA_SIZE;
    hdma_SaiRx_1.Init.Mode                = DMA_CIRCULAR;
    hdma_SaiRx_1.Init.Priority            = DMA_PRIORITY_HIGH;      
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmarx, hdma_SaiRx_1);
    
    /* Deinitialize the Stream for new transfer */
    (void)HAL_DMA_DeInit(&hdma_SaiRx_1);
    
    /* Configure the DMA Stream */
    (void)HAL_DMA_Init(&hdma_SaiRx_1);      
    
    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AMICAM1_DMAx_IRQ, AMICAM1_AUDIO_IN_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(AMICAM1_DMAx_IRQ);
  }
}


/**
* @brief  Deinitializes SAI MSP.
* @param  hsai  SAI handle 
* @retval HAL status
*/
static void SAI_MspDeInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  gpio_init_structure;
  if(hsai->Instance == AMICAM1_SAI_INSTANCE)
  {  
    /* SAI DMA IRQ Channel deactivation */
    HAL_NVIC_DisableIRQ(AMICAM1_DMAx_IRQ);
    
    /* Deinitialize the DMA stream */
    (void)HAL_DMA_DeInit(hsai->hdmatx);
    
    /* Disable SAI peripheral */
    __HAL_SAI_DISABLE(hsai);  
    
    /* Deactivates CODEC_SAI pins FS, SCK, MCK and SD by putting them in input mode */
    gpio_init_structure.Pin = AMICAM1_FS_PIN;
    HAL_GPIO_DeInit(AMICAM1_SD_GPIO_PORT, gpio_init_structure.Pin);
    
    gpio_init_structure.Pin = AMICAM1_SCK_PIN;
    HAL_GPIO_DeInit(AMICAM1_SCK_GPIO_PORT, gpio_init_structure.Pin);
    
    gpio_init_structure.Pin =  AMICAM1_SD_PIN;
    HAL_GPIO_DeInit(AMICAM1_FS_GPIO_PORT, gpio_init_structure.Pin);
    
    /* Disable SAI clock */
    AMICAM1_SAI_CLK_DISABLE();
  }
}


/**
* @brief  Register Bus IOs if component ID is OK
* @retval error status
*/
static int32_t AD1974_Probe(void)
{
  int32_t ret;
  AD1974_IO_t              IOCtx;
  static AD1974_Object_t   AD1974Obj;
  
  /* Configure the audio driver */
  IOCtx.BusType     = AD1974_SPI_BUS;
  IOCtx.Address     = 0;
  IOCtx.Init        = BSP_AD1974_Init;
  IOCtx.DeInit      = BSP_AD1974_DeInit;  
  IOCtx.ReadReg     = BSP_AD1974_ReadReg;
  IOCtx.WriteReg    = BSP_AD1974_WriteReg; 
  IOCtx.GetTick     = BSP_GetTick; 
  
  AD1974Obj.ChannelNbr = amicam1_ctx[0].ChannelsNbr;
  AD1974Obj.SampleRate = amicam1_ctx[0].SampleRate;
  
  if(AD1974_RegisterBusIO (&AD1974Obj, &IOCtx) != AD1974_OK)
  {
    return BSP_ERROR_BUS_FAILURE;   
  }
  else
  {
    AudioDrv = (AUDIO_Drv_t *) (void *) &AD1974_AUDIO_Driver;
    CompObj = &AD1974Obj;    
  }
  
  if (AudioDrv->Init(CompObj, NULL) != AD1974_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret;
} 

static int32_t BSP_AD1974_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  
  /* Enable the CS */
  AD1974_BUS_CS_CLK_ENABLE(); 
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = AD1974_BUS_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_WritePin(AD1974_BUS_CS_PORT, AD1974_BUS_CS_PIN, GPIO_PIN_SET); 
  HAL_GPIO_Init(AD1974_BUS_CS_PORT, &GPIO_InitStruct);
  
  return AD1974_BUS_Init();  
}

static int32_t BSP_AD1974_DeInit(void)
{  
  HAL_GPIO_DeInit(AD1974_BUS_CS_PORT, AD1974_BUS_CS_PIN);
  return AD1974_BUS_DeInit();
}

static int32_t BSP_AD1974_ReadReg(uint16_t Addr, uint16_t Reg, uint8_t *pdata, uint16_t len)
{
  UNUSED(Addr);
  int32_t ret = BSP_ERROR_NONE;
  uint32_t idx;
  uint8_t aTxBuffer[3];
  uint8_t aRxBuffer[3];
  
  for (idx = 0; idx < len; idx ++)
  {      
    aTxBuffer[0] = 0x09;             /* Read */
    aTxBuffer[1] = (uint8_t)(Reg + idx);        /* RegAddr */
    aTxBuffer[2] = 0x00;             /* Dummy */
    
    HAL_GPIO_WritePin(AD1974_BUS_CS_PORT, AD1974_BUS_CS_PIN, GPIO_PIN_RESET);
    ret = BSP_SPI1_SendRecv(aTxBuffer, aRxBuffer, 3);
    
    if(ret != BSP_ERROR_NONE)
    {
      return ret;
    }
    
    HAL_GPIO_WritePin(AD1974_BUS_CS_PORT, AD1974_BUS_CS_PIN, GPIO_PIN_SET);   
    pdata[idx] = aRxBuffer[2];    
  }
  return ret;
  
}

static int32_t BSP_AD1974_WriteReg(uint16_t Addr, uint16_t Reg, uint8_t *pdata, uint16_t len)
{
  UNUSED(Addr);
  int32_t ret = BSP_ERROR_NONE;
  uint32_t idx;
  uint8_t aTxBuffer[3];
  
  for (idx = 0; idx < len; idx ++)
  {      
    aTxBuffer[0] = 0x08;             /* Write */
    aTxBuffer[1] = (uint8_t)(Reg + idx);        /*RegAddr*/
    aTxBuffer[2] = pdata[idx];        /*Data*/
    
    HAL_GPIO_WritePin(AD1974_BUS_CS_PORT, AD1974_BUS_CS_PIN, GPIO_PIN_RESET); 
    ret = BSP_SPI1_Send(aTxBuffer, 3);
    
    if(ret != BSP_ERROR_NONE)
    {
      return ret;
    }
    
    HAL_GPIO_WritePin(AD1974_BUS_CS_PORT, AD1974_BUS_CS_PIN, GPIO_PIN_SET);   
  }
  return ret;
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/


