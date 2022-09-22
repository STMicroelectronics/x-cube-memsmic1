/**
******************************************************************************
* @file    adau1978_Driver.c
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   this file provides the ADAU1978 ADC drivers.    
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

#include "adau1978_reg.h"

/**
  * @addtogroup  adau1978
  * @brief  This file provides a set of functions needed to drive the
  *         adau1978 ADC
  * @{
  */

/**
  * @addtogroup  interfaces_functions
  * @brief  This section provide a set of functions used to read and write
  *         a generic register of the device.
  * @{
  */

/**
  * @brief  Read generic device register
  *
  * @param  adau1978_ctx_t* ctx: read / write interface definitions
  * @param  uint8_t reg: register to read
  * @param  uint8_t* data: pointer to buffer that store the data read
  * @param  uint16_t len: number of consecutive register to read
  *
  */
int32_t adau1978_read_reg(adau1978_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
  int32_t ret;
  ret = ctx->read_reg(ctx->handle, reg, data, len);
  return ret;
}

/**
* @brief  Write generic device register
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  uint8_t reg: register to write
* @param  uint8_t* data: pointer to data to write in register reg
* @param  uint16_t len: number of consecutive register to write
*
*/
int32_t adau1978_write_reg(adau1978_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
  int32_t ret;
  ret = ctx->write_reg(ctx->handle, reg, data, len);
  return ret;
}

/**
  * @}
  */

/**
  * @addtogroup  common
  * @brief   This section group device setup and run functions
  * @{
  */


/**
  * @brief  adau1978_software_reset: [set]  Start sw reset by writing the  specific bit.
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written in sw reset bit
  *
  */
int32_t adau1978_software_reset_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_m_power_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_M_POWER, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.s_rst = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_M_POWER, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  adau1978_software_reset: [get]  Read sw reset bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_software_reset_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_m_power_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_M_POWER, (uint8_t*) &reg, 1);
  *val = reg.s_rst;
  
  return ret;   
}    

/**
  * @brief  adau1978_software_reset: [set]  Write sw reset bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t adau1978_power_up_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_m_power_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_M_POWER, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.pwup = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_M_POWER, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  adau1978_power_up: [get]  Read power up bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_power_up_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_m_power_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_M_POWER, (uint8_t*) &reg, 1);
  *val = reg.pwup;
  
  return ret;  
}

/**
* @brief  adau1978_pll_source: [set]  Write PLL source configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_pll_source_set(adau1978_ctx_t *ctx, adau1978_pll_source_t val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.clk_s = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  adau1978_pll_source: [get]  Read pll source
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_pll_source_get(adau1978_ctx_t *ctx, adau1978_pll_source_t *val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  switch (reg.clk_s)
  {
  case ADAU1978_PLL_SOURCE_MCLK:
    *val = ADAU1978_PLL_SOURCE_MCLK;
    break;
  case ADAU1978_PLL_SOURCE_LRCLK:
    *val = ADAU1978_PLL_SOURCE_LRCLK;
    break;    
  default:
    *val = ADAU1978_PLL_SOURCE_ND;
    break;
  }
  
  return ret;  
}

/**
* @brief  adau1978_mclk_ratio: [set]  Write MCLK ratio configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_mclk_ratio_set(adau1978_ctx_t *ctx, adau1978_mclk_ratio_t val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.mcs = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  adau1978_mclk_ratio: [get]  Read master clock ratio
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_mclk_ratio_get(adau1978_ctx_t *ctx, adau1978_mclk_ratio_t *val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  switch (reg.mcs)
  {
  case ADAU1978_MCLK_128xFs:
    *val = ADAU1978_MCLK_128xFs;
    break;
  case ADAU1978_MCLK_256xFs:
    *val = ADAU1978_MCLK_256xFs;
    break;    
  case ADAU1978_MCLK_384xFs:
    *val = ADAU1978_MCLK_384xFs;
    break;
  case ADAU1978_MCLK_512xFs:
    *val = ADAU1978_MCLK_512xFs;
    break;
  case ADAU1978_MCLK_768xFs:
    *val = ADAU1978_MCLK_768xFs;
    break;
  default:
    *val = ADAU1978_MCLK_ND;
    break;
  }
  
  return ret;  
  
}

/**
* @brief  adau1978_pll_automute: [set]  Write PLL automute configuration pin
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_pll_automute_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.pll_mute = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  }
  
  return ret;
  
}
  
/**
  * @brief  adau1978_pll_automute: [get]  Read pll automute configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_pll_automute_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  *val = reg.pll_mute;
  
  return ret;  
}

/**
  * @brief  adau1978_pll_lock: [get]  Read PLL lock status
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_pll_lock_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_pll_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_PLL_CONTROL, (uint8_t*) &reg, 1);
  *val = reg.pll_lock;
  
  return ret;  
  
}

/**
* @brief  adau1978_lrclk_polarity: [set]  Write L/R polarity configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_lrclk_polarity_set(adau1978_ctx_t *ctx, adau1978_lrclk_polarity_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.lr_pol = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_lrclk_polarity: [get]  Read L/R polarity configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_lrclk_polarity_get(adau1978_ctx_t *ctx, adau1978_lrclk_polarity_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  switch (reg.lr_pol)
  {
  case ADAU1978_LRCLK_LH:
    *val = ADAU1978_LRCLK_LH;
    break;
  case ADAU1978_LRCLK_HL:
    *val = ADAU1978_LRCLK_HL;
    break;    
  default:
    *val = ADAU1978_LRCLK_ND;
    break;
  }
  
  return ret;  
}

/**
* @brief  adau1978_bclk_edge: [set]  Write bitclock edge configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_bclk_edge_set(adau1978_ctx_t *ctx, adau1978_bclk_edge_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclkedge = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_bclk_edge: [get]  Read bit clock edge configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_bclk_edge_get(adau1978_ctx_t *ctx, adau1978_bclk_edge_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  switch (reg.bclkedge)
  {
  case ADAU1978_BCLK_FALLING:
    *val = ADAU1978_BCLK_FALLING;
    break;
  case ADAU1978_BCLK_RISING:
    *val = ADAU1978_BCLK_RISING;
    break;    
  default:
    *val = ADAU1978_BCLK_ND;
    break;
  }
  
  return ret;  
}

/**
* @brief  adau1978_ldo_en: [set]  Write LDO enable bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_ldo_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.ldo_en = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_ldo_en: [get]  Read LDO enable configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_ldo_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  *val = reg.ldo_en;
  
  return ret;  
}

/**
* @brief  adau1978_vref_en: [set]  Write VREF enable bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_vref_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.vref_en = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_vref_en: [get]  Read VREF enable configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_vref_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  *val = reg.vref_en;
  
  return ret;  
}

/**
* @brief  adau1978_adc_ch1_en: [set]  Write adc ch1 enable bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_adc_ch1_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc_en1 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_adc_ch1_en: [get]  Read ch1 enable bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_adc_ch1_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  *val = reg.adc_en1;
  
  return ret;  
}

/**
* @brief  adau1978_adc_ch2_en: [set]  Write adc ch2 enable bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_adc_ch2_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc_en2 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_adc_ch2_en: [get]  Read ch2 enable bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_adc_ch2_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  *val = reg.adc_en2;
  
  return ret;  
}

/**
* @brief  adau1978_adc_ch3_en: [set]  Write adc ch3 enable bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_adc_ch3_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc_en3 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_adc_ch3_en: [get]  Read ch3 enable bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_adc_ch3_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
     adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  *val = reg.adc_en3;
  
  return ret;  
}

/**
* @brief  adau1978_adc_ch4_en: [set]  Write adc ch4 enable bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_adc_ch4_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc_en4 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_adc_ch4_en: [get]  Read ch4 enable bit
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_adc_ch4_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_block_power_sai_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_BLOCK_POWER_SAI, (uint8_t*) &reg, 1);
  *val = reg.adc_en4;
  
  return ret;  
}

/**
* @brief  adau1978_data_format: [set]  Write data format configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_data_format_set(adau1978_ctx_t *ctx, adau1978_data_format_t val)
{
  adau1978_sai_ctrl0_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sdata_fmt = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_data_format: [get]  Read data format configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_data_format_get(adau1978_ctx_t *ctx, adau1978_data_format_t *val)
{
  adau1978_sai_ctrl0_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  switch (reg.sdata_fmt)
  {
  case ADAU1978_DFORMAT_I2S:
    *val = ADAU1978_DFORMAT_I2S;
    break;
  case ADAU1978_DFORMAT_LJ:
    *val = ADAU1978_DFORMAT_LJ;
    break;    
  case ADAU1978_DFORMAT_RJ24:
    *val = ADAU1978_DFORMAT_RJ24;
    break; 
  case ADAU1978_DFORMAT_RJ16:
    *val = ADAU1978_DFORMAT_RJ16;
    break; 
  default:
    *val = ADAU1978_DFORMAT_ND;
    break;
  }
  
  return ret;  
}

/**
* @brief  adau1978_serial_port_mode: [set]  Write serial port mode configuratin
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_serial_port_mode_set(adau1978_ctx_t *ctx, adau1978_serial_port_mode_t val)
{
  adau1978_sai_ctrl0_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_serial_port_mode: [get]  Read serial port mode configuratin
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_serial_port_mode_get(adau1978_ctx_t *ctx, adau1978_serial_port_mode_t *val)
{
  adau1978_sai_ctrl0_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  switch (reg.sai)
  {
  case ADAU1978_SMODE_STEREO:
    *val = ADAU1978_SMODE_STEREO;
    break;
  case ADAU1978_SMODE_TDM2:
    *val = ADAU1978_SMODE_TDM2;
    break;    
  case ADAU1978_SMODE_TDM4:
    *val = ADAU1978_SMODE_TDM4;
    break; 
  case ADAU1978_SMODE_TDM8:
    *val = ADAU1978_SMODE_TDM8;
    break; 
      case ADAU1978_SMODE_TDM16:
    *val = ADAU1978_SMODE_TDM16;
    break; 
  default:
    *val = ADAU1978_SMODE_ND;
    break;
  }
  
  return ret;  
  
}

/**
* @brief  adau1978_sampling_rate: [set]  Write sampling rate configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_sampling_rate_set(adau1978_ctx_t *ctx, adau1978_sampling_rate_t val)
{
  adau1978_sai_ctrl0_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.fs = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_sampling_rate: [get]  Read sampling rate configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_sampling_rate_get(adau1978_ctx_t *ctx, adau1978_sampling_rate_t *val)
{
  adau1978_sai_ctrl0_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL0, (uint8_t*) &reg, 1);
  switch (reg.fs)
  {
  case ADAU1978_SRATE_8_12:
    *val = ADAU1978_SRATE_8_12;
    break;
  case ADAU1978_SRATE_16_24:
    *val = ADAU1978_SRATE_16_24;
    break;    
  case ADAU1978_SRATE_32_48:
    *val = ADAU1978_SRATE_32_48;
    break; 
  case ADAU1978_SRATE_64_96:
    *val = ADAU1978_SRATE_64_96;
    break; 
      case ADAU1978_SRATE_128_192:
    *val = ADAU1978_SRATE_128_192;
    break; 
  default:
    *val = ADAU1978_SRATE_ND;
    break;
  }
  
  return ret;  
  
}

/**
* @brief  adau1978_sai_ms: [set]  Write interface master/slave configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_sai_ms_set(adau1978_ctx_t *ctx, adau1978_sai_ms_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai_ms = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_sai_ms: [get]  Read interface master/slave configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_sai_ms_get(adau1978_ctx_t *ctx, adau1978_sai_ms_t *val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.sai_ms)
  {
  case ADAU1978_SAI_M:
    *val = ADAU1978_SAI_M;
    break;
  case ADAU1978_SAI_S:
    *val = ADAU1978_SAI_S;
    break;    
  default:
    *val = ADAU1978_SAI_MS_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_bclkrate: [set]  Write bitclocks/channel configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_bclkrate_set(adau1978_ctx_t *ctx, adau1978_sai_bclkrate_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclkrate = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_bclkrate: [get]  Read bitclocks/channel configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_bclkrate_get(adau1978_ctx_t *ctx, adau1978_sai_bclkrate_t *val)
{
   adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.bclkrate)
  {
  case ADAU1978_BCLKRATE_32:
    *val = ADAU1978_BCLKRATE_32;
    break;
  case ADAU1978_BCLKRATE_16:
    *val = ADAU1978_BCLKRATE_16;
    break;    
  default:
    *val = ADAU1978_BCLKRATE_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_sai_msb: [set]  Write MSB/LSB order configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_sai_msb_set(adau1978_ctx_t *ctx, adau1978_sai_msblsb_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai_msb = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_sai_msb: [get]  Read MSB/LSB order configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_sai_msb_get(adau1978_ctx_t *ctx, adau1978_sai_msblsb_t  *val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.sai_msb)
  {
  case ADAU1978_SAI_MSB:
    *val = ADAU1978_SAI_MSB;
    break;
  case ADAU1978_SAI_LSB:
    *val = ADAU1978_SAI_LSB;
    break;    
  default:
    *val = ADAU1978_SAI_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_lr_mode: [set]  Write L/R mode configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_lr_mode_set(adau1978_ctx_t *ctx, adau1978_lr_mode_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.lr_mode = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_lr_mode: [get]  Read L/R signal duty cicle configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_lr_mode_get(adau1978_ctx_t *ctx, adau1978_lr_mode_t  *val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.lr_mode)
  {
  case ADAU1978_LR_MODE_50DUTY:
    *val = ADAU1978_LR_MODE_50DUTY;
    break;
  case ADAU1978_LR_MODE_SINGLE:
    *val = ADAU1978_LR_MODE_SINGLE;
    break;    
  default:
    *val = ADAU1978_LR_MODE_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_data_width: [set]  Write data width configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_data_width_set(adau1978_ctx_t *ctx, adau1978_data_width_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.data_width = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_data_width: [get]  Read output data width configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_data_width_get(adau1978_ctx_t *ctx, adau1978_data_width_t  *val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.data_width)
  {
  case ADAU1978_DATA_WIDTH_24:
    *val = ADAU1978_DATA_WIDTH_24;
    break;
  case ADAU1978_DATA_WIDTH_16:
    *val = ADAU1978_DATA_WIDTH_16;
    break;    
  default:
    *val = ADAU1978_DATA_WIDTH_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_slot_width: [set]  Write slot width configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_slot_width_set(adau1978_ctx_t *ctx, adau1978_slot_width_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.slot_width = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_slot_width: [get]  Read slot data width configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_slot_width_get(adau1978_ctx_t *ctx, adau1978_slot_width_t  *val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.slot_width)
  {
  case ADAU1978_SLOT_WIDTH_32:
    *val = ADAU1978_SLOT_WIDTH_32;
    break;
  case ADAU1978_SLOT_WIDTH_24:
    *val = ADAU1978_SLOT_WIDTH_24;
    break;    
      case ADAU1978_SLOT_WIDTH_16:
    *val = ADAU1978_SLOT_WIDTH_16;
    break;  
  default:
    *val = ADAU1978_SLOT_WIDTH_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_sdata_sel: [set]  Write serial data out configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_sdata_sel_set(adau1978_ctx_t *ctx, adau1978_sdata_sel_t val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sdata_sel = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_sdata_sel: [get]  Read serial data out pin configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_sdata_sel_get(adau1978_ctx_t *ctx, adau1978_sdata_sel_t  *val)
{
  adau1978_sai_ctrl1_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CTRL1, (uint8_t*) &reg, 1);
  switch (reg.sdata_sel)
  {
  case ADAU1978_SDATAOUT_1:
    *val = ADAU1978_SDATAOUT_1;
    break;
  case ADAU1978_SDATAOUT_2:
    *val = ADAU1978_SDATAOUT_2;
    break;    
  default:
    *val = ADAU1978_SDATAOUT_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_cmap1: [set]  Write ADC ch1 TDM mapping configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_cmap1_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_cmap12_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP12, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.cmap_c1 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CMAP12, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_cmap1: [get]  Read channel 1 mapping configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_cmap1_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_cmap12_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP12, (uint8_t*) &reg, 1);
  *val = reg.cmap_c1;
  
  return ret;  
}

/**
* @brief  adau1978_cmap2: [set]  Write ADC ch2 TDM mapping configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_cmap2_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_cmap12_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP12, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.cmap_c2 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CMAP12, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_cmap2: [get]  Read channel 2 mapping configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_cmap2_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_cmap12_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP12, (uint8_t*) &reg, 1);
  *val = reg.cmap_c2;
  
  return ret;  
}

/**
* @brief  adau1978_cmap3: [set]  Write ADC ch3 TDM mapping configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_cmap3_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_cmap34_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP34, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.cmap_c3 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CMAP34, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_cmap3: [get]  Read channel 3 mapping configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_cmap3_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_cmap34_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP34, (uint8_t*) &reg, 1);
  *val = reg.cmap_c3;
  
  return ret;  
}

/**
* @brief  adau1978_cmap4: [set]  Write ADC ch4 TDM mapping configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_cmap4_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_cmap34_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP34, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.cmap_c4 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_CMAP34, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_cmap4: [get]  Read channel 4 mapping configuration
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_cmap4_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_cmap34_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_CMAP34, (uint8_t*) &reg, 1);
  *val = reg.cmap_c4;
  
  return ret;  
}

/**
  * @brief  adau1978_over_temp: [get]  Read over temperature status
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_over_temp_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  *val = reg.ot;
  
  return ret;  
}

/**
* @brief  adau1978_drv_hiz: [set]  Write hiz configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_drv_hiz_set(adau1978_ctx_t *ctx, adau1978_drv_hiz_t val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.drv_hiz = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_drv_hiz: [get]  Read hiz status
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_drv_hiz_get(adau1978_ctx_t *ctx, adau1978_drv_hiz_t *val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  switch (reg.drv_hiz)
  {
  case ADAU1978_UNUSD_OUT_LOW:
    *val = ADAU1978_UNUSD_OUT_LOW;
    break;
  case ADAU1978_UNUSD_OUT_HIZ:
    *val = ADAU1978_UNUSD_OUT_HIZ;
    break;    
  default:
    *val = ADAU1978_UNUSD_OUT_ND;
    break;
  }  
  return ret;    
}

/**
* @brief  adau1978_ch1_out_drv_en: [set]  Write output drive enable bit for ch1
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_ch1_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai_drv_c1 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_ch1_out_drv_en: [get]  Read channel 1 serial out enable
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_ch1_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  *val = reg.sai_drv_c1;
  
  return ret;  
}

/**
* @brief  adau1978_ch2_out_drv_en: [set]  Write output drive enable bit for ch2
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_ch2_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai_drv_c2 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_ch2_out_drv_en: [get]  Read channel 2 serial out enable
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_ch2_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  *val = reg.sai_drv_c2;
  
  return ret;  
}

/**
* @brief  adau1978_ch3_out_drv_en: [set]  Write output drive enable bit for ch3
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_ch3_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai_drv_c3 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_ch3_out_drv_en: [get]  Read channel 3 serial out enable
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_ch3_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  *val = reg.sai_drv_c3;
  
  return ret;  
}

/**
* @brief  adau1978_ch4_out_drv_en: [set]  Write output drive enable bit for ch4
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_ch4_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sai_drv_c4 = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_ch4_out_drv_en: [get]  Read channel 4 serial out enable
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_ch4_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_sai_overtem_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_SAI_OVERTEMP, (uint8_t*) &reg, 1);
  *val = reg.sai_drv_c4;
  
  return ret;  
}

/**
* @brief  adau1978_padc_gain1: [set]  Write post adc gain value for ch1
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_padc_gain1_set(adau1978_ctx_t *ctx, uint8_t val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_POSTADC_GAIN1, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_padc_gain1: [get]  Read current post adc gain for channel 1
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_padc_gain1_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN1, (uint8_t*) &reg, 1);
  *val = reg;
  
  return ret;  
}

/**
* @brief  adau1978_padc_gain2: [set]  Write post adc gain value for ch2
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_padc_gain2_set(adau1978_ctx_t *ctx, uint8_t val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_POSTADC_GAIN2, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_padc_gain2: [get]  Read current post adc gain for channel 2
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_padc_gain2_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN2, (uint8_t*) &reg, 1);
  *val = reg;
  
  return ret;  
}

/**
* @brief  adau1978_padc_gain3: [set]  Write post adc gain value for ch3
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_padc_gain3_set(adau1978_ctx_t *ctx, uint8_t val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN3, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_POSTADC_GAIN3, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_padc_gain3: [get]  Read current post adc gain for channel 3
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_padc_gain3_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN3, (uint8_t*) &reg, 1);
  *val = reg;
  
  return ret;  
}

/**
* @brief  adau1978_padc_gain4: [set]  Write post adc gain value for ch4
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_padc_gain4_set(adau1978_ctx_t *ctx, uint8_t val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN4, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_POSTADC_GAIN4, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_padc_gain4: [get]  Read current post adc gain for channel 4
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_padc_gain4_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  uint8_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_POSTADC_GAIN4, (uint8_t*) &reg, 1);
  *val = reg;
  
  return ret;  
}

/**
* @brief  adau1978_dc_cal: [set]  Write dc calibration bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_dc_cal_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_misc_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_cal = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_dc_cal: [get]  Read current dc calibration status
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_dc_cal_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_misc_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  *val = reg.dc_cal;
  
  return ret;  
}

/**
* @brief  adau1978_master_mute: [set]  Write mute bit
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_master_mute_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_misc_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.mmute = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_master_mute: [get]  Read master mute status
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_master_mute_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_misc_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  *val = reg.mmute;
  
  return ret;  
}

/**
* @brief  adau1978_sum_mode: [set]  Write sum mode configuration
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_sum_mode_set(adau1978_ctx_t *ctx, adau1978_sum_mode_t val)
{
  adau1978_misc_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sum_mode = (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_sum_mode: [get]  Read sum mode status
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_sum_mode_get(adau1978_ctx_t *ctx, adau1978_sum_mode_t  *val)
{
  adau1978_misc_control_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_MISC_CONTROL, (uint8_t*) &reg, 1);
  switch (reg.sum_mode)
  {
  case SUM_MODE_NORMAL_4_CH:
    *val = SUM_MODE_NORMAL_4_CH;
    break;
  case SUM_MODE_TWO_CH_SUMMING:
    *val = SUM_MODE_TWO_CH_SUMMING;
    break;    
      case SUM_MODE_ONE_CH_SUMMING:
    *val = SUM_MODE_ONE_CH_SUMMING;
    break;
  default:
    *val = SUM_MODE_ND;
    break;
  }  
  return ret;    
}

/**
  * @brief  adau1978_clip_status_ch1: [get] get clipping flag for ch1
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_clip_status_ch1_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_asdc_clip_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_ASDC_CLIP, (uint8_t*) &reg, 1);
  *val = reg.adc_clip1;
  
  return ret;  
}

/**
  * @brief  adau1978_clip_status_ch2: [get] get clipping flag for ch2
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_clip_status_ch2_get(adau1978_ctx_t *ctx, uint8_t  *val)
{
  adau1978_asdc_clip_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_ASDC_CLIP, (uint8_t*) &reg, 1);
  *val = reg.adc_clip2;
  
  return ret;  
}

/**
  * @brief  adau1978_clip_status_ch3: [get] get clipping flag for ch3
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_clip_status_ch3_get(adau1978_ctx_t *ctx, uint8_t  *val)
{
  adau1978_asdc_clip_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_ASDC_CLIP, (uint8_t*) &reg, 1);
  *val = reg.adc_clip3;
  
  return ret;  
}

/**
  * @brief  adau1978_clip_status_ch3: [get] get clipping flag for ch3
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_clip_status_ch4_get(adau1978_ctx_t *ctx, uint8_t  *val)
{
  adau1978_asdc_clip_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_ASDC_CLIP, (uint8_t*) &reg, 1);
  *val = reg.adc_clip4;
  
  return ret;  
}

/**
* @brief  adau1978_dc_sub_ch1: [set]  Write dc subtraction bit for ch1
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_dc_sub_ch1_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_sub_c1= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_dc_sub_ch1: [get]  Read DC subtraction bit for ch1
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_dc_sub_ch1_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_sub_c1;
  
  return ret;  
}

/**
* @brief  adau1978_dc_sub_ch2: [set]  Write dc subtraction bit for ch2
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_dc_sub_ch2_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_sub_c2= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_dc_sub_ch2: [get]  Read DC subtraction bit for ch2
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_dc_sub_ch2_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_sub_c2;
  
  return ret;  
}

/**
* @brief  adau1978_dc_sub_ch3: [set]  Write dc subtraction bit for ch3
*
* @param  adau1978_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t adau1978_dc_sub_ch3_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_sub_c3= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_dc_sub_ch3: [get]  Read DC subtraction bit for ch3
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_dc_sub_ch3_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_sub_c3;
  
  return ret;  
}

/**
  * @brief  adau1978_dc_sub_ch4: [set]  Write dc subtraction bit for ch4
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t adau1978_dc_sub_ch4_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_sub_c4= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_dc_sub_ch4: [get]  Read DC subtraction bit for ch4
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_dc_sub_ch4_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_sub_c4;
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch1: [set]  Write hpf enable bit for ch1
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t adau1978_hpf_en_ch1_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_hpf_c1= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch1: [get]  Read hpf enable bit for ch1
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_hpf_en_ch1_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_hpf_c1;
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch2: [set]  Write hpf enable bit for ch2
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t adau1978_hpf_en_ch2_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_hpf_c2= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch2: [get]  Read hpf enable bit for ch2
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_hpf_en_ch2_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_hpf_c2;
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch3: [set]  Write hpf enable bit for ch3
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t adau1978_hpf_en_ch3_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_hpf_c3= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch3: [get]  Read hpf enable bit for ch3
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_hpf_en_ch3_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_hpf_c3;
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch4: [set]  Write hpf enable bit for ch4
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t adau1978_hpf_en_ch4_set(adau1978_ctx_t *ctx, uint8_t val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.dc_hpf_c4= (uint8_t)val;
    ret = adau1978_write_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  }
  
  return ret;  
}

/**
  * @brief  adau1978_hpf_en_ch4: [get]  Read hpf enable bit for ch4
  *
  * @param  adau1978_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t adau1978_hpf_en_ch4_get(adau1978_ctx_t *ctx, uint8_t *val)
{
  adau1978_dc_hpf_cal_t reg;
  int32_t ret;
  
  ret = adau1978_read_reg(ctx, ADAU1978_DC_HPF_CAL, (uint8_t*) &reg, 1);
  *val = reg.dc_hpf_c4;
  
  return ret;  
}

/**
* @}
*/

/**
* @}
*/



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


