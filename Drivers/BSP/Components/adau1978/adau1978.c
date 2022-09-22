/**
******************************************************************************
* @file    adau1978.c
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   ADAU1978 driver file
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
#include "adau1978.h"

/** @addtogroup BSP BSP
* @{
*/

/** @addtogroup Component Component
* @{
*/

/** @defgroup ADAU1978 ADAU1978
* @{
*/

/** @defgroup ADAU1978_Exported_Variables ADAU1978 Exported Variables
* @{
*/

ADAU1978_AUDIO_Drv_t ADAU1978_AUDIO_Driver =
{
  ADAU1978_Init,
  ADAU1978_DeInit,
  ADAU1978_ReadID,
  ADAU1978_Play,
  ADAU1978_Pause,
  ADAU1978_Resume,
  ADAU1978_Stop,
  ADAU1978_SetFrequency,
  ADAU1978_GetFrequency,
  ADAU1978_SetVolume,
  ADAU1978_GetVolume,
  ADAU1978_SetMute,  
  ADAU1978_SetOutputMode,
  ADAU1978_SetResolution, 
  ADAU1978_GetResolution,   
  ADAU1978_SetProtocol,   
  ADAU1978_GetProtocol,   
  ADAU1978_Reset         
};


/**
* @}
*/

/** @defgroup ADAU1978_Private_Function_Prototypes ADAU1978 Private Function Prototypes
* @{
*/

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
* @}
*/

/** @defgroup ADAU1978_Exported_Functions ADAU1978 Exported Functions
* @{
*/

/**
* @brief  Register Component Bus IO operations
* @param  pObj the device pObj
* @retval 0 in case of success, an error code otherwise
*/
int32_t ADAU1978_RegisterBusIO(ADAU1978_Object_t *pObj, ADAU1978_IO_t *pIO)
{
  int32_t ret;
  
  if (pObj == NULL)
  {
    ret = ADAU1978_ERROR;
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
      ret = ADAU1978_ERROR;
    }
  }
  
  return ret;
}

/**
* @brief  Initialize the ADAU1978 sensor
* @param  pObj the device pObj
* @retval 0 in case of success, an error code otherwise
*/
int32_t ADAU1978_Init(ADAU1978_Object_t *pObj, void *params)
{  
  UNUSED(params);
  uint32_t sampling_freq = pObj->SampleRate;
  uint32_t channel_number = pObj->ChannelNbr;
  uint32_t bit_resolution = pObj->bit_resolution;
  uint8_t tmp;
  
  if (pObj->is_initialized == 0U)
  {
    /* Setup standard channel mapping */  
    uint8_t idx;
    for (idx = 0U; idx < 4U; idx ++)
    {
      pObj->channel_map[idx] = idx; 
    } 
    
    /* Power Up / Reset */       
    if(adau1978_power_up_get(&(pObj->Ctx), &tmp) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    } 
    
    if(tmp != 1U) /*Power up if not already Powered */
    {
      if(adau1978_power_up_set(&(pObj->Ctx), 1) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }
    }
    else /*else reset*/
    {
      if(adau1978_software_reset_set(&(pObj->Ctx), 1) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }
    }
    
    if(adau1978_pll_automute_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }   
    
    if(adau1978_lrclk_polarity_set(&(pObj->Ctx), ADAU1978_LRCLK_HL) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }   
    
    if (sampling_freq >= AUDIO_FREQUENCY_32K)
    {
      /* LR clock used as PLL input */
      if(adau1978_pll_source_set(&(pObj->Ctx), ADAU1978_PLL_SOURCE_LRCLK) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }      
    }
    else
    {
      /* MCLK clock used as PLL input */
      if(adau1978_pll_source_set(&(pObj->Ctx), ADAU1978_PLL_SOURCE_MCLK) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }            
      /*MCLK ratio setup */
      if(adau1978_mclk_ratio_set(&(pObj->Ctx), ADAU1978_MCLK_128xFs) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }         
    }        
    
    (void)ADAU1978_SetResolution(pObj, bit_resolution);
    (void)ADAU1978_SetFrequency(pObj, sampling_freq);    
    
    switch (channel_number)
    {
    case 1:
      (void)adau1978_serial_port_mode_set(&(pObj->Ctx), ADAU1978_SMODE_STEREO);    
      break; 
    
    case 2:
      (void)adau1978_serial_port_mode_set(&(pObj->Ctx), ADAU1978_SMODE_STEREO);
      break; 
      
    case 4:
      (void)adau1978_serial_port_mode_set(&(pObj->Ctx), ADAU1978_SMODE_TDM4);
      break;     
    
    default:
      break;
    }
    
    /* Enable all channels by default */
    if(adau1978_adc_ch1_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }      
    if(adau1978_adc_ch2_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    if(adau1978_adc_ch3_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    if(adau1978_adc_ch4_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    if(adau1978_ch1_out_drv_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }      
    if(adau1978_ch2_out_drv_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    if(adau1978_ch3_out_drv_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    if(adau1978_ch4_out_drv_en_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }   
    if(adau1978_hpf_en_ch1_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }   
    if(adau1978_hpf_en_ch2_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }
    if(adau1978_hpf_en_ch3_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }
    if(adau1978_hpf_en_ch4_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }
    
    /* Default Volume for all the channels*/
    uint8_t Volume = 63;
    
    if(adau1978_padc_gain1_set(&(pObj->Ctx), Volume) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;
    }  
    if(adau1978_padc_gain2_set(&(pObj->Ctx), Volume) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;
    }
    if(adau1978_padc_gain3_set(&(pObj->Ctx), Volume) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;
    }
    if(adau1978_padc_gain4_set(&(pObj->Ctx), Volume) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;
    }
    
    
  }
  
  pObj->is_initialized = 1;
  
  return ADAU1978_OK;
}

/**
* @brief  Deinitialize the ADAU1978 sensor
* @param  pObj the device pObj
* @retval 0 in case of success, an error code otherwise
*/
int32_t ADAU1978_DeInit(ADAU1978_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    /* Power off */
    if(adau1978_power_up_set(&(pObj->Ctx), 0) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    pObj->is_initialized = 0;
  }
  
  return ADAU1978_OK;
}

/**
* @brief  Get WHO_AM_I value
* @param  pObj the device pObj
* @param  Id the WHO_AM_I value
* @retval 0 in case of success, an error code otherwise
*/
int32_t ADAU1978_ReadID(ADAU1978_Object_t *pObj, uint32_t *Id)
{
  UNUSED(Id);
  UNUSED(pObj);
  return ADAU1978_OK;
}


int32_t ADAU1978_Play(ADAU1978_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    if(adau1978_master_mute_set(&(pObj->Ctx), 0) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    } 
    pObj->audio_is_enabled = 1U;
    return ADAU1978_OK;
  }    
  return ADAU1978_ERROR;
}



int32_t ADAU1978_Pause(ADAU1978_Object_t *pObj)
{
  if (pObj->audio_is_enabled == 1U)
  {
    if(adau1978_master_mute_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    pObj->audio_is_enabled = 0U;
  }
  else
  {
    return ADAU1978_ERROR;  
  }    
  
  return ADAU1978_OK;
}



int32_t ADAU1978_Resume(ADAU1978_Object_t *pObj)
{
 if (pObj->audio_is_enabled == 0U)
  {
    if(adau1978_master_mute_set(&(pObj->Ctx), 0) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    pObj->audio_is_enabled = 1;
  }
 else
 {
   return ADAU1978_ERROR;  
 }    
  return ADAU1978_OK;
}



int32_t ADAU1978_Stop(ADAU1978_Object_t *pObj, uint32_t Cmd)
{
  UNUSED(Cmd);
  if (pObj->audio_is_enabled == 1U)
  {
    if(adau1978_master_mute_set(&(pObj->Ctx), 1) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }    
    pObj->audio_is_enabled = 0;
  }
  else
  {
    return ADAU1978_ERROR;  
  }      

  return ADAU1978_OK;
}



int32_t ADAU1978_SetVolume(ADAU1978_Object_t *pObj, uint32_t Cmd, uint8_t Volume)
{
  uint8_t gain = (100U - Volume) * 10U / 4U;   /* To avoid calculation in floating point, we approximated 2.56f as 10/4 */
  
  switch(pObj->channel_map[Cmd])
  {
  case 0:
    (void)adau1978_padc_gain1_set(&(pObj->Ctx), gain);
    break;
    
  case 1:
    (void)adau1978_padc_gain2_set(&(pObj->Ctx), gain);
    break;
    
  case 2:
    (void)adau1978_padc_gain3_set(&(pObj->Ctx), gain);
    break;
    
  case 3:
    (void)adau1978_padc_gain4_set(&(pObj->Ctx), gain);
    break;
    
  default:
    break;
  } 
  
  return ADAU1978_OK;
}



int32_t ADAU1978_SetMute(ADAU1978_Object_t *pObj, uint32_t Cmd)
{
  UNUSED(Cmd);
  if(adau1978_master_mute_set(&(pObj->Ctx), 1) != ADAU1978_OK)
  {
    return ADAU1978_ERROR;      
  }      
  return ADAU1978_OK;
}



int32_t ADAU1978_SetOutputMode(ADAU1978_Object_t *pObj, uint8_t Output)
{
  UNUSED(pObj);
  UNUSED(Output);
  return ADAU1978_ERROR;
}



int32_t ADAU1978_SetFrequency(ADAU1978_Object_t *pObj, uint32_t AudioFreq)
{
  adau1978_sampling_rate_t val;    
    
    switch (AudioFreq)
    {
    case AUDIO_FREQUENCY_8K:
      val = ADAU1978_SRATE_16_24;
      break;
    case AUDIO_FREQUENCY_16K:
      val = ADAU1978_SRATE_16_24;
      break;    
    case AUDIO_FREQUENCY_32K:
      val = ADAU1978_SRATE_32_48;
      break; 
    case AUDIO_FREQUENCY_48K:
      val = ADAU1978_SRATE_32_48;
      break; 
    case AUDIO_FREQUENCY_96K:
      val = ADAU1978_SRATE_64_96;
      break; 
    case AUDIO_FREQUENCY_192K:
      val = ADAU1978_SRATE_128_192;
      break; 
    default:
       val = ADAU1978_SRATE_32_48;
      break; 
    }
    
    if(adau1978_sampling_rate_set(&(pObj->Ctx), val) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }        
  return ADAU1978_OK;
}



int32_t ADAU1978_Reset(ADAU1978_Object_t *pObj)
{
  if(adau1978_software_reset_set(&(pObj->Ctx), 1) != ADAU1978_OK)
  {    
    return ADAU1978_ERROR;      
  }
  pObj->audio_is_enabled = 0;
  pObj->is_initialized = 0;
  return ADAU1978_OK;
}

int32_t ADAU1978_SetResolution(ADAU1978_Object_t *pObj, uint32_t BitRes)
{
  switch (BitRes)
    {
    case AUDIO_RESOLUTION_16b:
      if(adau1978_data_width_set(&(pObj->Ctx), ADAU1978_DATA_WIDTH_16) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }        
      if(adau1978_slot_width_set(&(pObj->Ctx), ADAU1978_SLOT_WIDTH_16) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }     
      break;
    case AUDIO_RESOLUTION_24b:
      if(adau1978_data_width_set(&(pObj->Ctx), ADAU1978_DATA_WIDTH_24) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }     
      if(adau1978_slot_width_set(&(pObj->Ctx), ADAU1978_SLOT_WIDTH_24) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }   
      break;          
    default:
     if(adau1978_data_width_set(&(pObj->Ctx), ADAU1978_DATA_WIDTH_16) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }        
      if(adau1978_slot_width_set(&(pObj->Ctx), ADAU1978_SLOT_WIDTH_16) != ADAU1978_OK)
      {
        return ADAU1978_ERROR;      
      }     
      break;
    }    
  return ADAU1978_OK;
}

int32_t ADAU1978_GetResolution(ADAU1978_Object_t *pObj, uint32_t* BitRes)
{
  adau1978_data_width_t temp;
  if(adau1978_data_width_get(&(pObj->Ctx), &temp) != ADAU1978_OK)
  {
    return ADAU1978_ERROR;      
  }     
  
  *BitRes = (uint32_t)temp;   
  return ADAU1978_OK;
}

int32_t ADAU1978_SetProtocol(ADAU1978_Object_t *pObj, uint32_t params)
{
  UNUSED(params);
  UNUSED(pObj);
  return ADAU1978_ERROR;
}

int32_t ADAU1978_GetProtocol(ADAU1978_Object_t *pObj, uint32_t* params)
{
  UNUSED(params);
  UNUSED(pObj);
  return ADAU1978_ERROR;
}

int32_t ADAU1978_GetVolume(ADAU1978_Object_t *pObj, uint32_t params, uint8_t* value)
{
  uint8_t gain = 0;
  switch (params)
  {
  case 0:
    if(adau1978_padc_gain1_get(&(pObj->Ctx), &gain) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }         
    break;
  case 1:
    if(adau1978_padc_gain2_get(&(pObj->Ctx), &gain) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }           
    break;      
  case 2:
    if(adau1978_padc_gain3_get(&(pObj->Ctx), &gain) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }        
    break; 
  case 3:
    if(adau1978_padc_gain4_get(&(pObj->Ctx), &gain) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }       
    break; 
  default:
    if(adau1978_padc_gain1_get(&(pObj->Ctx), &gain) != ADAU1978_OK)
    {
      return ADAU1978_ERROR;      
    }          
    break;
  } 
  
  *value = 100U - (gain * 4U / 10U); /* To avoid calculation in floating point, we approximated 0.390625f as 4/10 */
  return ADAU1978_OK;
}

int32_t ADAU1978_GetFrequency(ADAU1978_Object_t *pObj)
{
  UNUSED(pObj);
  return ADAU1978_ERROR;
}

int32_t ADAU1978_SetChannelMapping(ADAU1978_Object_t *pObj, uint8_t * map)
{
  uint8_t idx;
  for (idx = 0U; idx < 4U; idx ++)
  {
    pObj->channel_map[idx] = map[idx]; 
  } 
  
  (void)adau1978_cmap1_set(&(pObj->Ctx), pObj->channel_map[0]);
  (void)adau1978_cmap2_set(&(pObj->Ctx), pObj->channel_map[1]);
  (void)adau1978_cmap3_set(&(pObj->Ctx), pObj->channel_map[2]);
  (void)adau1978_cmap4_set(&(pObj->Ctx), pObj->channel_map[3]); 
  
  return ADAU1978_OK;
  
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
  ADAU1978_Object_t *pObj = (ADAU1978_Object_t *)Handle;
  
  if (pObj->IO.BusType == (uint32_t)ADAU1978_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte read */
    return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
  }
  else /* SPI 3-Wires */
  {
    /* Enable Multi-byte read */
    return ADAU1978_ERROR;
  }
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
  ADAU1978_Object_t *pObj = (ADAU1978_Object_t *)Handle;
  
  if (pObj->IO.BusType == (uint32_t)ADAU1978_I2C_BUS) /* I2C */
  {
    /* Enable Multi-byte write */
    return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
  }
  else /* SPI 3-Wires */
  {
    /* Enable Multi-byte write */
    return ADAU1978_ERROR;
  }
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
