/**
******************************************************************************
* @file    ad1974.c
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   AD1974 driver file
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the 
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "ad1974.h"

/** @addtogroup BSP BSP
* @{
*/

/** @addtogroup Components Components
* @{
*/

/** @defgroup AD1974 AD1974
* @{
*/

/** @defgroup AD1974_Exported_Variables AD1974 Exported Variables
* @{
*/

AD1974_AUDIO_Drv_t AD1974_AUDIO_Driver =
{
  AD1974_Init,
  AD1974_DeInit,
  AD1974_ReadID,
  AD1974_Play,
  AD1974_Pause,
  AD1974_Resume,
  AD1974_Stop,
  AD1974_SetFrequency,
  AD1974_GetFrequency,
  AD1974_SetVolume,
  AD1974_GetVolume,
  AD1974_SetMute,  
  AD1974_SetOutputMode,
  AD1974_SetResolution, 
  AD1974_GetResolution,   
  AD1974_SetProtocol,   
  AD1974_GetProtocol,   
  AD1974_Reset         
};


/**
* @}
*/

/** @defgroup AD1974_Private_Function_Prototypes AD1974 Private Function Prototypes
* @{
*/

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
* @}
*/

/** @defgroup AD1974_Exported_Functions AD1974 Exported Functions
* @{
*/

/**
* @brief  Register Component Bus IO operations
* @param  pObj the device pObj
* @retval 0 in case of success, an error code otherwise
*/
int32_t AD1974_RegisterBusIO(AD1974_Object_t *pObj, AD1974_IO_t *pIO)
{
  int32_t ret;
  
  if (pObj == NULL)
  {
    ret = AD1974_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.BusType   = pIO->BusType;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;
    
    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.handle   = pObj;
    
    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = AD1974_ERROR;
    }
  }
  
  return ret;
}

/**
* @brief  Initialize the AD1974 sensor
* @param  pObj the device pObj
* @retval 0 in case of success, an error code otherwise
*/
int32_t AD1974_Init(AD1974_Object_t *pObj, void *params)
{  
  UNUSED(params);
  uint32_t sampling_freq = pObj->SampleRate;
  uint32_t channel_number = pObj->ChannelNbr;
  uint32_t bit_resolution = pObj->bit_resolution;
  
  if (pObj->is_initialized == 0U)
  {
    if (sampling_freq <= AUDIO_FREQUENCY_16K)
    {
      /* Set MCLK */
      if(ad1974_auxport_clk_sel_set(&(pObj->Ctx), AD1974_AUX_CLOCK_MCLK) != AD1974_OK)
      {
        return AD1974_ERROR;      
      } 
      
      if(ad1974_adc_clk_sel_set(&(pObj->Ctx), AD1974_ADC_CLOCK_MCLK) != AD1974_OK)
      {
        return AD1974_ERROR;      
      } 
    }
    else
    {
      if(ad1974_internal_mclk_enable_set(&(pObj->Ctx), 0) != AD1974_OK)
      {
        return AD1974_ERROR;      
      } 
      
      if(ad1974_pll_power_down_set(&(pObj->Ctx),1) != AD1974_OK)
      {
        return AD1974_ERROR;      
      }     
      
      /* Set LR clock as PLL input */ 
      if(ad1974_pll_input_set(&(pObj->Ctx), AD1974_PLL_IN_ALRCLK) != AD1974_OK)
      {
        return AD1974_ERROR;      
      }   
      
      if(ad1974_pll_mclk_in_rate_set(&(pObj->Ctx), AD1974_PLL_MCLK_IN_RATE_INX256) != AD1974_OK)
      {
        return AD1974_ERROR;      
      }   
      
      if(ad1974_pll_mclk_out_rate_set(&(pObj->Ctx), AD1974_PLL_MCLK_OUT_RATE_OFF) != AD1974_OK)
      {
        return AD1974_ERROR;      
      }
    }      
    
    /* HPF filter on */
    if(ad1974_hpf_enable_set(&(pObj->Ctx), 1) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }        
    
    (void)AD1974_SetResolution(pObj, bit_resolution);
    (void)AD1974_SetFrequency(pObj, sampling_freq);    
    
    switch (channel_number)
    {
    case 2:
      (void)ad1974_serial_format_set(&(pObj->Ctx), AD1974_SERIAL_FORMAT_STEREO);
      break; 
      
    case 4:
      (void)ad1974_serial_format_set(&(pObj->Ctx), AD1974_SERIAL_FORMAT_TDM);
      break;     
      
    default:
      break;
    } 
    
  }
  pObj->is_initialized = 1;
  
  return AD1974_OK;
}

/**
* @brief  Deinitialize the AD1974 sensor
* @param  pObj the device pObj
* @retval 0 in case of success, an error code otherwise
*/
int32_t AD1974_DeInit(AD1974_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    /* Power off */
    if(ad1974_power_down_set(&(pObj->Ctx), 1) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }    
    pObj->is_initialized = 0;
  }
  
  return AD1974_OK;
}

/**
* @brief  Get WHO_AM_I value
* @param  pObj the device pObj
* @param  Id the WHO_AM_I value
* @retval 0 in case of success, an error code otherwise
*/
int32_t AD1974_ReadID(AD1974_Object_t *pObj, uint32_t *Id)
{
  UNUSED(Id);
  UNUSED(pObj);
  return AD1974_OK;
}


int32_t AD1974_Play(AD1974_Object_t *pObj)
{
  uint8_t lock = 0;
  if (pObj->is_initialized == 1U)
  {
    /* Power down PLL */       
    if(ad1974_pll_power_down_set(&(pObj->Ctx), 0) != AD1974_OK)
    {
      return AD1974_ERROR;      
    } 
    
    while (lock == 0U)
    {
      (void)ad1974_pll_lock_get(&(pObj->Ctx), &lock);
    }
    
    (void)ad1974_internal_mclk_enable_set(&(pObj->Ctx), 1);  
    
    if(AD1974_SetMute(pObj, 0) != AD1974_OK)
    {
      return AD1974_ERROR;      
    } 
    
    pObj->audio_is_enabled = 1U;
    
    return AD1974_OK;
  }    
  return AD1974_ERROR;
}



int32_t AD1974_Pause(AD1974_Object_t *pObj)
{
  if (pObj->audio_is_enabled == 1U)
  {
    
    if(AD1974_SetMute(pObj, 1) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }    
    pObj->audio_is_enabled = 0U;
  }
  else
  {
    return AD1974_ERROR;  
  }    
  
  return AD1974_OK;
}



int32_t AD1974_Resume(AD1974_Object_t *pObj)
{
  if (pObj->audio_is_enabled == 0U)
  {
    if(AD1974_SetMute(pObj, 0) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }    
    pObj->audio_is_enabled = 1;
  }
  else
  {
    return AD1974_ERROR;  
  }    
  return AD1974_OK;
}



int32_t AD1974_Stop(AD1974_Object_t *pObj, uint32_t Cmd)
{
  UNUSED(Cmd);
  if (pObj->audio_is_enabled == 1U)
  {
    if(AD1974_SetMute(pObj, 1) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }    
    pObj->audio_is_enabled = 0;
  }
  else
  {
    return AD1974_ERROR;  
  }      
  
  return AD1974_OK;
}



int32_t AD1974_SetVolume(AD1974_Object_t *pObj, uint32_t Cmd, uint8_t Volume)
{
  /*AD1974 gain is fixed. User must change volume before or after this component*/
  
  UNUSED(Volume);
  UNUSED(Cmd);
  UNUSED(pObj);
  return AD1974_NOT_IMPLEMENTED;
}



int32_t AD1974_SetMute(AD1974_Object_t *pObj, uint32_t Cmd)
{
  if(ad1974_adc1l_mute_set(&(pObj->Ctx), (uint8_t)Cmd) != AD1974_OK)
  {
    return AD1974_ERROR;      
  }      
  if(ad1974_adc1r_mute_set(&(pObj->Ctx), (uint8_t)Cmd) != AD1974_OK)
  {
    return AD1974_ERROR;      
  }      
  if(ad1974_adc2l_mute_set(&(pObj->Ctx), (uint8_t)Cmd) != AD1974_OK)
  {
    return AD1974_ERROR;      
  }      
  if(ad1974_adc2r_mute_set(&(pObj->Ctx),(uint8_t)Cmd) != AD1974_OK)
  {
    return AD1974_ERROR;      
  }      
  return AD1974_OK;
}



int32_t AD1974_SetOutputMode(AD1974_Object_t *pObj, uint8_t Output)
{
  UNUSED(pObj);
  UNUSED(Output);
  return AD1974_ERROR;
}



int32_t AD1974_SetFrequency(AD1974_Object_t *pObj, uint32_t AudioFreq)
{
  if (AudioFreq <= AUDIO_FREQUENCY_16K)
  {
    /* LR clock used as PLL input */
    if(ad1974_out_rate_set(&(pObj->Ctx), AD1974_OUT_RATE_64_882_96) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }      
  }
  else if (AudioFreq <= AUDIO_FREQUENCY_48K)
  {
    /* LR clock used as PLL input */
    if(ad1974_out_rate_set(&(pObj->Ctx), AD1974_OUT_RATE_32_441_48) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }      
  }
  else if(AudioFreq <= AUDIO_FREQUENCY_96K)
  {
    /* LR clock used as PLL input */
    if(ad1974_out_rate_set(&(pObj->Ctx), AD1974_OUT_RATE_64_882_96) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }          
  }     
  else if(AudioFreq <= AUDIO_FREQUENCY_192K)
  {
    /* LR clock used as PLL input */
    if(ad1974_out_rate_set(&(pObj->Ctx), AD1974_OUT_RATE_128_1764_192) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }           
  }   
  else
  {
    return AD1974_ERROR;   
  } 
  return AD1974_OK;
}


int32_t AD1974_Reset(AD1974_Object_t *pObj)
{
  if(ad1974_power_down_set(&(pObj->Ctx), 1) != AD1974_OK)
  {    
    return AD1974_ERROR;      
  }
  
  if(ad1974_power_down_set(&(pObj->Ctx), 0) != AD1974_OK)
  {    
    return AD1974_ERROR;      
  }
  pObj->audio_is_enabled = 0;
  pObj->is_initialized = 0;
  return AD1974_OK;
}

int32_t AD1974_SetResolution(AD1974_Object_t *pObj, uint32_t BitRes)
{
  switch (BitRes)
  {
  case AUDIO_RESOLUTION_16b:
    if(ad1974_word_width_set(&(pObj->Ctx), AD1974_WORD_WIDTH_16) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }      
    break;
  case AUDIO_RESOLUTION_20b:
    if(ad1974_word_width_set(&(pObj->Ctx), AD1974_WORD_WIDTH_20) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }      
    break;
  case AUDIO_RESOLUTION_24b:
    if(ad1974_word_width_set(&(pObj->Ctx), AD1974_WORD_WIDTH_24) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }       
    break;   
  default:
    if(ad1974_word_width_set(&(pObj->Ctx), AD1974_WORD_WIDTH_16) != AD1974_OK)
    {
      return AD1974_ERROR;      
    }          
    break;
  }    
  return AD1974_OK;
}

int32_t AD1974_GetResolution(AD1974_Object_t *pObj, uint32_t* BitRes)
{
  ad1974_word_width_t temp;
  if(ad1974_word_width_get(&(pObj->Ctx), &temp) != AD1974_OK)
  {
    return AD1974_ERROR;      
  }     
  
  *BitRes = (uint32_t)temp;   
  return AD1974_OK;
}

int32_t AD1974_SetProtocol(AD1974_Object_t *pObj, uint32_t params)
{
  UNUSED(params);
  UNUSED(pObj);
  return AD1974_ERROR;
}

int32_t AD1974_GetProtocol(AD1974_Object_t *pObj, uint32_t* params)
{
  UNUSED(params);
  UNUSED(pObj);
  return AD1974_ERROR;
}

int32_t AD1974_GetVolume(AD1974_Object_t *pObj, uint32_t params, uint8_t* value)
{
  /*AD1974 gain is fixed. User must change volume before or after this component*/
  
  UNUSED(value);
  UNUSED(params);
  UNUSED(pObj);
  return AD1974_NOT_IMPLEMENTED;
}

int32_t AD1974_GetFrequency(AD1974_Object_t *pObj)
{
  UNUSED(pObj);
  return AD1974_ERROR;
}


/**
* @brief  Wrap Read register component function to Bus IO function
* @param  Handle the device handler
* @param  Reg the register address
* @param  pData the stored data pointer
* @param  Length the length
* @retval 0 in case of success, an error code otherwise
*/
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  AD1974_Object_t *pObj = (AD1974_Object_t *)Handle;
  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
  
}

/**
* @brief  Wrap Write register component function to Bus IO function
* @param  Handle the device handler
* @param  Reg the register address
* @param  pData the stored data pointer
* @param  Length the length
* @retval 0 in case of success, an error code otherwise
*/
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  AD1974_Object_t *pObj = (AD1974_Object_t *)Handle;
  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
