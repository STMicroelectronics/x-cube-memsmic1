/**
******************************************************************************
* @file    AMic_Array_audio.c
* @author  SRA
* @brief   This file provides the Audio driver for the AMic_Array board
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
#include "AMic_Array_audio.h"
#include "audio.h"

/** @addtogroup BSP
* @{
*/

/** @addtogroup AMIC_ARRAY
* @{
*/

/** @addtogroup AMIC_ARRAY_AUDIO_IN
* @brief This file provides set of firmware functions to manage MEMS microphones
*        initialization on STWIN Kit from STMicroelectronics.
* @{
*/ 

/** @defgroup AMIC_ARRAY_AUDIO_IN_Private_Types
* @{
*/ 

/**
* @}
*/ 

/** @defgroup AMIC_ARRAY_AUDIO_IN_Private_Defines 
* @{
*/ 

/**
* @}
*/ 

/** @defgroup AMIC_ARRAY_AUDIO_IN_Private_Macros 
* @{
*/

#define SAI_CLOCK_DIVIDER(__FREQUENCY__) \
        ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))  ? (12U) \
      : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K)) ? (2U) \
      : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K)) ? (6U) \
      : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K)) ? (1U) \
      : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K)) ? (3U) \
      : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K)) ? (0U) \
      : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K)) ? (2U) : (1U)

/**
* @}
*/ 

/** @defgroup AMIC_ARRAY_AUDIO_IN_Private_Variables
* @{
*/
/* Recording context */
AUDIO_IN_Ctx_t                         MicArrayCouponCtx[AUDIO_IN_INSTANCES_NBR] = {0};
static AUDIO_Drv_t                     *AudioDrv = NULL;
static void                            *CompObj = NULL;

/* AMic Array */
SAI_HandleTypeDef AMic_Array_SAI;
		  
/**
* @}
*/ 

/** @defgroup AMIC_ARRAY_AUDIO_IN_Private_Function_Prototypes 
* @{
*/ 
HAL_StatusTypeDef MX_SAI_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate);
HAL_StatusTypeDef MX_SAI_Init(SAI_HandleTypeDef* hsai, MX_SAI_Config *MXConfig);
static void SAI_MspInit(SAI_HandleTypeDef *hsai);
static void SAI_MspDeInit(SAI_HandleTypeDef *hsai);
static int32_t ADAU1978_Probe(void);
/**
* @}
*/ 

/** @defgroup AMIC_ARRAY_AUDIO_IN_Exported_Functions 
* @{
*/  
            
__weak int32_t AMIC_ARRAY_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t* AudioInit)
{
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
  else
  {
    /* Store the audio record context */
    MicArrayCouponCtx[Instance].Device          = AudioInit->Device;
    MicArrayCouponCtx[Instance].ChannelsNbr     = AudioInit->ChannelsNbr;  
    MicArrayCouponCtx[Instance].SampleRate      = AudioInit->SampleRate; 
    MicArrayCouponCtx[Instance].BitsPerSample   = AudioInit->BitsPerSample;
    MicArrayCouponCtx[Instance].Volume          = AudioInit->Volume;
    MicArrayCouponCtx[Instance].State           = AUDIO_IN_STATE_RESET;

      if (MicArrayCouponCtx[Instance].Device == AMIC_ARRAY)
      {
        /* PLL clock SETUP */ 
        if(MX_SAI_ClockConfig(&AMic_Array_SAI, (uint32_t)NULL) != HAL_OK)
        {
          return BSP_ERROR_CLOCK_FAILURE;
        }
        
        /* SAI data transfer preparation:
        Prepare the Media to be used for the audio transfer from memory to SAI peripheral */
        AMic_Array_SAI.Instance = AMIC_ARRAY_INSTANCE;
        
        SAI_MspInit(&AMic_Array_SAI);        
        MX_SAI_Config mx_sai_config;
        
        /* Prepare AMic_Array_SAI handle */
        
        if (AudioInit->ChannelsNbr == 1U)
        {
          mx_sai_config.MonoStereoMode = SAI_MONOMODE;
        }
        else
        {  
          mx_sai_config.MonoStereoMode = SAI_STEREOMODE;
        }
        
        if ((AudioInit->ChannelsNbr == 1U) || (AudioInit->ChannelsNbr == 2U))
        {
          mx_sai_config.SlotNumber     = 2;
          mx_sai_config.SlotActive     = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;
          mx_sai_config.FrameLength       = 32; 
          mx_sai_config.ActiveFrameLength = 16;
        }
        else if (AudioInit->ChannelsNbr == 4U)
        {
          mx_sai_config.SlotNumber     = AudioInit->ChannelsNbr;
          mx_sai_config.SlotActive     = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_2 | SAI_SLOTACTIVE_3;
          mx_sai_config.FrameLength       = 16U*AudioInit->ChannelsNbr; 
          mx_sai_config.ActiveFrameLength = 8U*AudioInit->ChannelsNbr;
        }
        else
        {
          return BSP_ERROR_WRONG_PARAM;
        }
        
        mx_sai_config.Mckdiv            = SAI_CLOCK_DIVIDER(AudioInit->SampleRate);
        mx_sai_config.AudioFrequency    = AudioInit->SampleRate;
        mx_sai_config.AudioMode         = SAI_MODEMASTER_RX;
        mx_sai_config.ClockStrobing     = SAI_CLOCKSTROBING_RISINGEDGE;
        mx_sai_config.DataSize          = SAI_DATASIZE_16;
        mx_sai_config.OutputDrive       = SAI_OUTPUTDRIVE_DISABLE;
        mx_sai_config.Synchro           = SAI_ASYNCHRONOUS;
        
        /* SAI peripheral initialization: this __weak function can be redefined by the application  */
        if(MX_SAI_Init(&AMic_Array_SAI, &mx_sai_config) != HAL_OK)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
        
        if(ADAU1978_Probe()!= BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
        
      }
      else
      {
        return BSP_ERROR_WRONG_PARAM;
      }
  }
  
  /* Update BSP AUDIO IN state */     
  MicArrayCouponCtx[Instance].State = AUDIO_IN_STATE_STOP; 
  /* Return BSP status */
  return BSP_ERROR_NONE; 
}


/**
* @brief  Deinit the audio IN peripherals.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @retval BSP status
*/

__weak int32_t AMIC_ARRAY_AUDIO_IN_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }	
    if (MicArrayCouponCtx[Instance].Device == AMIC_ARRAY)
    {
      if (AMic_Array_SAI.Instance != NULL)
      {
        SAI_MspDeInit(&AMic_Array_SAI);
        /* Call the Media layer stop function */
        if(AudioDrv->DeInit(CompObj) != 0)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }   
        if(HAL_OK != HAL_SAI_DMAStop(&AMic_Array_SAI))
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
  MicArrayCouponCtx[Instance].State = AUDIO_IN_STATE_RESET; 
  return ret;
}


/**
* @brief  Start audio recording.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  pbuf     Main buffer pointer for the recorded data storing  
* @param  Size     Size of the record buffer
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_Record(uint32_t Instance, uint8_t* pBuf, uint32_t NbrOfBytes)
{
  int32_t ret = BSP_ERROR_NONE;
  MicArrayCouponCtx[Instance].pBuff = (uint16_t*)pBuf;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  if (MicArrayCouponCtx[Instance].Device == AMIC_ARRAY)
  {
    /* Call the Media layer play function */
    if(AudioDrv->Play(CompObj) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      (void)HAL_SAI_Init(&AMic_Array_SAI);
      
      if (HAL_OK != HAL_SAI_Receive_DMA(&AMic_Array_SAI, (uint8_t *)MicArrayCouponCtx[Instance].pBuff, (uint16_t)(NbrOfBytes)))
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }
  else
  {
    ret =  BSP_ERROR_PERIPH_FAILURE;
  }	  
  
  
  /* Update BSP AUDIO IN state */     
  MicArrayCouponCtx[Instance].State = AUDIO_IN_STATE_RECORDING;
  return ret;  
}


/**
* @brief  Stop audio recording.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_Stop(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR) 
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
    if (MicArrayCouponCtx[Instance].Device == AMIC_ARRAY)
    {
      /* Call the Media layer stop function */
      if(AudioDrv->Stop(CompObj, 0) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        if(HAL_SAI_DMAStop(&AMic_Array_SAI)!= HAL_OK)
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
  MicArrayCouponCtx[Instance].State = AUDIO_IN_STATE_STOP;
  return ret; 	
}


/**
* @brief  Pause the audio file stream.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_Pause(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR) 
  {
    return BSP_ERROR_WRONG_PARAM;  
  }
    if (MicArrayCouponCtx[Instance].Device == AMIC_ARRAY)
    {
      /* Call the Media layer stop function */
      if(AudioDrv->Stop(CompObj, 0) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        if(HAL_SAI_DMAStop(&AMic_Array_SAI)!= HAL_OK)
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
  MicArrayCouponCtx[Instance].State = AUDIO_IN_STATE_PAUSE;
  return ret; 	
}


/**
* @brief  Resume the audio file stream.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_Resume(uint32_t Instance)
{  
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }	
    if (MicArrayCouponCtx[Instance].Device == AMIC_ARRAY)
    {
      (void)HAL_SAI_Init(&AMic_Array_SAI);
      /* Call the Media layer stop function */
      if(AudioDrv->Resume(CompObj) != 0) 
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      } 
      if (HAL_OK != HAL_SAI_Receive_DMA(&AMic_Array_SAI, (uint8_t *)MicArrayCouponCtx[Instance].pBuff, (uint16_t)MicArrayCouponCtx[Instance].Size))
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }	  
  
  /* Update BSP AUDIO IN state */     
  MicArrayCouponCtx[Instance].State = AUDIO_IN_STATE_RECORDING;
  return ret;  
}


/**
* @brief  Starts audio recording.
* @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
* @param  pBuf      Main buffer pointer for the recorded data storing
* @param  size      Size of the recorded buffer
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes)
{
  UNUSED(Instance);
  UNUSED(pBuf);
  UNUSED(NbrOfBytes);
  return BSP_NOT_IMPLEMENTED;
}


/**
* @brief  Stop audio recording.
* @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
* @param  Device    Digital input device to be stopped
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device)
{
  UNUSED(Instance);
  UNUSED(Device);
  return BSP_NOT_IMPLEMENTED;
}


/**
* @brief  Pause the audio file stream.
* @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
* @param  Device    Digital mic to be paused
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device)
{
  UNUSED(Instance);
  UNUSED(Device);
  return BSP_NOT_IMPLEMENTED;
}


/**
* @brief  Resume the audio file stream
* @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
* @param  Device    Digital mic to be resumed
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device)
{
  UNUSED(Instance);
  UNUSED(Device);
  return BSP_NOT_IMPLEMENTED;
}


/**
* @brief  Set Audio In device
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  Device    The audio input device to be used
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device)
{  
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (AMIC_ARRAY_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }
    
    audio_init.Device = Device;
    audio_init.ChannelsNbr   = MicArrayCouponCtx[Instance].ChannelsNbr;  
    audio_init.SampleRate    = MicArrayCouponCtx[Instance].SampleRate;   
    audio_init.BitsPerSample = MicArrayCouponCtx[Instance].BitsPerSample;
    audio_init.Volume        = MicArrayCouponCtx[Instance].Volume;
    
    if(AMIC_ARRAY_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In device
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  Device    The audio input device used
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Return audio Input Device */
    *Device = MicArrayCouponCtx[Instance].Device;
  }
  return ret;
}


/**
* @brief  Set Audio In frequency
* @param  Instance  Audio IN instance
*					AMic_Array can support only Instance 0
* @param  SampleRate  Input frequency to be set
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t  SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {	
    if (AMIC_ARRAY_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }	
    
    audio_init.Device 		 = MicArrayCouponCtx[Instance].Device;
    audio_init.ChannelsNbr   = MicArrayCouponCtx[Instance].ChannelsNbr;  
    audio_init.SampleRate    = SampleRate;   
    audio_init.BitsPerSample = MicArrayCouponCtx[Instance].BitsPerSample;
    audio_init.Volume        = MicArrayCouponCtx[Instance].Volume;
    
    if(AMIC_ARRAY_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In frequency
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  SampleRate  Audio Input frequency to be returned
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio in frequency */
    *SampleRate = MicArrayCouponCtx[Instance].SampleRate;
  }
  
  /* Return BSP status */  
  return ret;
}


/**
* @brief  Set Audio In Resolution
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  BitsPerSample  Input resolution to be set
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {	
    if (AMIC_ARRAY_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }	
    
    audio_init.Device 		 = MicArrayCouponCtx[Instance].Device;
    audio_init.ChannelsNbr   = MicArrayCouponCtx[Instance].ChannelsNbr;  
    audio_init.SampleRate    = MicArrayCouponCtx[Instance].SampleRate; 
    audio_init.BitsPerSample = BitsPerSample;
    audio_init.Volume        = MicArrayCouponCtx[Instance].Volume;
    
    if(AMIC_ARRAY_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In Resolution
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  BitsPerSample  Input resolution to be returned
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Return audio in resolution */
    *BitsPerSample = MicArrayCouponCtx[Instance].BitsPerSample;
  }
  return ret;
}


/**
* @brief  Set Audio In Channel number
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  ChannelNbr  Channel number to be used
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;
  BSP_AUDIO_Init_t audio_init;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {	
    if (AMIC_ARRAY_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUSY;
    }	
    
    audio_init.Device 	     = MicArrayCouponCtx[Instance].Device;
    audio_init.ChannelsNbr   = ChannelNbr;  
    audio_init.SampleRate    = MicArrayCouponCtx[Instance].SampleRate; 
    audio_init.BitsPerSample = MicArrayCouponCtx[Instance].BitsPerSample;
    audio_init.Volume        = MicArrayCouponCtx[Instance].Volume;
    
    if(AMIC_ARRAY_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}


/**
* @brief  Get Audio In Channel number
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  ChannelNbr  Channel number to be used
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Channel number to be returned */
    *ChannelNbr = MicArrayCouponCtx[Instance].ChannelsNbr;
  }
  return ret;	
}


/**
* @brief  Set the current audio in volume level.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  Volume    Volume level to be returned
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
    uint32_t counter;
    for (counter = 0U; counter <  MicArrayCouponCtx[Instance].ChannelsNbr; counter ++)
    {
      /* Call the codec volume control function with converted volume value */
      if(AudioDrv->SetVolume(CompObj, counter, Volume) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    } 
  
  /* Update AudioIn Context */
  MicArrayCouponCtx[Instance].Volume = Volume;
  /* Return BSP status */
  return ret;  
}


/**
* @brief  Get the current audio in volume level.
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  Volume    Volume level to be returned
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }  
  else
  {
    /* Input Volume to be returned */
    *Volume = MicArrayCouponCtx[Instance].Volume;
  }
  /* Return BSP status */
  return ret;  
}


/**
* @brief  Get Audio In device
* @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
*					AMic_Array can support only Instance 0
* @param  State     Audio in state
* @retval BSP status
*/
int32_t AMIC_ARRAY_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* Input State to be returned */
    *State = MicArrayCouponCtx[Instance].State;
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
  AMIC_ARRAY_AUDIO_IN_TransferComplete_CallBack(0);
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
  AMIC_ARRAY_AUDIO_IN_HalfTransfer_CallBack(0);
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


/**
* @brief  User callback when record buffer is filled.
* @retval None
*/
__weak void AMIC_ARRAY_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
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
__weak void AMIC_ARRAY_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
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
__weak void AMIC_ARRAY_AUDIO_IN_Error_CallBack(uint32_t Instance)
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
  * @note   This API is called by BSP_AUDIO_OUT_Init() and BSP_AUDIO_OUT_SetFrequency()
  *         Being __weak it can be overwritten by the application     
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_SAI_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);
  UNUSED(SampleRate);
  
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;  
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
  
  /* SAI clock config */
  RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_SAI1;
  RCC_ExCLKInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI2;
  RCC_ExCLKInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_SYSCLK;
  
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2Source = RCC_PLLSOURCE_HSE;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2M = 2;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2N = 43;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2P = RCC_PLLP_DIV7;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2R = RCC_PLLR_DIV2;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2Q = RCC_PLLQ_DIV2;
  RCC_ExCLKInitStruct.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_SAI2CLK;
  
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1M = 2;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1N = 12;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  
  return ret;
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
  
  /* Configure SAI1_Block_A */
  hsai->Init.MonoStereoMode       = MXConfig->MonoStereoMode;
  hsai->Init.AudioFrequency       = MXConfig->AudioFrequency;
  hsai->Init.AudioMode            = MXConfig->AudioMode;
  hsai->Init.NoDivider            = SAI_MASTERDIVIDER_ENABLE;
  hsai->Init.Protocol             = SAI_FREE_PROTOCOL;
  hsai->Init.DataSize             = MXConfig->DataSize;
  hsai->Init.FirstBit             = SAI_FIRSTBIT_MSB;
  hsai->Init.ClockStrobing        = MXConfig->ClockStrobing;
  hsai->Init.Synchro              = MXConfig->Synchro;
  hsai->Init.OutputDrive          = MXConfig->OutputDrive;
  hsai->Init.FIFOThreshold        = SAI_FIFOTHRESHOLD_1QF;
  hsai->Init.Mckdiv               = MXConfig->Mckdiv;
  
  /* Configure SAI_Block_x Frame */
  hsai->FrameInit.FrameLength       = MXConfig->FrameLength; 
  hsai->FrameInit.ActiveFrameLength = MXConfig->ActiveFrameLength;  
  hsai->FrameInit.FSDefinition      = SAI_FS_STARTFRAME;
  hsai->FrameInit.FSPolarity        = SAI_FS_ACTIVE_HIGH;
  hsai->FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;
  
  /* Configure SAI Block_x Slot */
  hsai->SlotInit.FirstBitOffset     = 0;
  hsai->SlotInit.SlotSize           = SAI_SLOTSIZE_16B;
  hsai->SlotInit.SlotNumber         = MXConfig->SlotNumber;  
  hsai->SlotInit.SlotActive         = MXConfig->SlotActive;
  
  if(HAL_SAI_Init(hsai) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  
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
  
  if(hsai->Instance == AMIC_ARRAY_INSTANCE)
  {
    /* Enable SAI clock */
    AMIC_ARRAY_CLK_ENABLE();
    
    /* Enable GPIO clock */
    AMIC_ARRAY_SD_CLK_ENABLE(); 
    AMIC_ARRAY_SCK_CLK_ENABLE();
    AMIC_ARRAY_MCK_CLK_ENABLE();
    AMIC_ARRAY_FS_CLK_ENABLE();
    
    /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins ------------------*/
    GPIO_InitStruct.Pin         = AMIC_ARRAY_SCK_PIN ; 
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull        = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate   = AMIC_ARRAY_SCK_SD_WS_MCK_AF;
    HAL_GPIO_Init(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin         = AMIC_ARRAY_SD_PIN ;
    HAL_GPIO_Init(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, &GPIO_InitStruct);     
    
    GPIO_InitStruct.Pin         = AMIC_ARRAY_FS_PIN ;
    HAL_GPIO_Init(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, &GPIO_InitStruct); 
    
    GPIO_InitStruct.Pin         = AMIC_ARRAY_MCLK_PIN; 
    HAL_GPIO_Init(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, &GPIO_InitStruct);  
    
    /* Enable the DMA clock */
    AMIC_ARRAY_DMAx_CLK_ENABLE(); 
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    
    hdma_SaiRx_1.Instance                 = AMIC_ARRAY_DMAx_INSTANCE;  
    hdma_SaiRx_1.Init.Request             = AMIC_ARRAY_DMAx_REQUEST;  
    hdma_SaiRx_1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_SaiRx_1.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_SaiRx_1.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_SaiRx_1.Init.PeriphDataAlignment = AMIC_ARRAY_DMAx_PERIPH_DATA_SIZE;
    hdma_SaiRx_1.Init.MemDataAlignment    = AMIC_ARRAY_DMAx_MEM_DATA_SIZE;
    hdma_SaiRx_1.Init.Mode                = DMA_CIRCULAR;
    hdma_SaiRx_1.Init.Priority            = DMA_PRIORITY_HIGH;      
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmarx, hdma_SaiRx_1);
    
    /* Deinitialize the Stream for new transfer */
    (void)HAL_DMA_DeInit(&hdma_SaiRx_1);
    
    /* Configure the DMA Stream */
    (void)HAL_DMA_Init(&hdma_SaiRx_1);      
    
    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AMIC_ARRAY_DMAx_IRQ, AMIC_ARRAY_AUDIO_IN_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(AMIC_ARRAY_DMAx_IRQ);
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
  if(hsai->Instance == AMIC_ARRAY_INSTANCE)
  {  
    /* SAI DMA IRQ Channel deactivation */
    HAL_NVIC_DisableIRQ(AMIC_ARRAY_DMAx_IRQ);
    
    /* Deinitialize the DMA stream */
    (void)HAL_DMA_DeInit(hsai->hdmatx);
    
    /* Disable SAI peripheral */
    __HAL_SAI_DISABLE(hsai);  
    
    /* Deactivates CODEC_SAI pins FS, SCK, MCK and SD by putting them in input mode */
    gpio_init_structure.Pin = AMIC_ARRAY_FS_PIN;
    HAL_GPIO_DeInit(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, gpio_init_structure.Pin);
    
    gpio_init_structure.Pin = AMIC_ARRAY_SCK_PIN;
    HAL_GPIO_DeInit(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, gpio_init_structure.Pin);
    
    gpio_init_structure.Pin =  AMIC_ARRAY_SD_PIN;
    HAL_GPIO_DeInit(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, gpio_init_structure.Pin);
    
    gpio_init_structure.Pin = AMIC_ARRAY_MCLK_PIN;
    HAL_GPIO_DeInit(AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT, gpio_init_structure.Pin);
    
    /* Disable SAI clock */
    AMIC_ARRAY_CLK_DISABLE();
  }
}


/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t ADAU1978_Probe(void)
{
  int32_t ret;
  ADAU1978_IO_t              IOCtx;
  static ADAU1978_Object_t   ADAU1978Obj;
  
  /* Configure the audio driver */
  IOCtx.BusType     = ADAU1978_I2C_BUS;
  IOCtx.Address     = ADAU1978_I2C_ADDRESS_01;
  IOCtx.Init        = BSP_ADAU1978_Init;
  IOCtx.DeInit      = BSP_ADAU1978_DeInit;  
  IOCtx.ReadReg     = BSP_ADAU1978_ReadReg;
  IOCtx.WriteReg    = BSP_ADAU1978_WriteReg; 
  IOCtx.GetTick     = BSP_GetTick; 
  
  ADAU1978Obj.ChannelNbr = MicArrayCouponCtx[0].ChannelsNbr;
  ADAU1978Obj.SampleRate = MicArrayCouponCtx[0].SampleRate;
  
  if(ADAU1978_RegisterBusIO (&ADAU1978Obj, &IOCtx) != ADAU1978_OK)
  {
    return BSP_ERROR_BUS_FAILURE;   
  }
  else
  {
    AudioDrv = (AUDIO_Drv_t *) (void *) &ADAU1978_AUDIO_Driver;
    CompObj = &ADAU1978Obj;    
  }
  
  if (AudioDrv->Init(CompObj, NULL) != ADAU1978_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {

  uint8_t chMap[4] = {0,1, 3, 2};
  (void)ADAU1978_SetChannelMapping(&ADAU1978Obj, chMap);

    ret = BSP_ERROR_NONE;
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

