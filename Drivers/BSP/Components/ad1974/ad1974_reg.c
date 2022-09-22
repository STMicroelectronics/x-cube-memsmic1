/**
******************************************************************************
* @file    ad1974_Driver.c
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   this file provides the AD1974 ADC drivers.    
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

#include "ad1974_reg.h"

/**
  * @addtogroup  ad1974
  * @brief  This file provides a set of functions needed to drive the
  *         ad1974 ADC
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
  * @param  ad1974_ctx_t* ctx: read / write interface definitions
  * @param  uint8_t reg: register to read
  * @param  uint8_t* data: pointer to buffer that store the data read
  * @param  uint16_t len: number of consecutive register to read
  *
  */
int32_t ad1974_read_reg(ad1974_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
  int32_t ret;
  ret = ctx->read_reg(ctx->handle, reg, data, len);
  return ret;
}

/**
* @brief  Write generic device register
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  uint8_t reg: register to write
* @param  uint8_t* data: pointer to data to write in register reg
* @param  uint16_t len: number of consecutive register to write
*
*/
int32_t ad1974_write_reg(ad1974_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
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
  * @brief  ad1974_pll_power_down_set: [set]  Set pll power down status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_pll_power_down_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.pll_pd = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_pll_power_down_get: [get]  Read pll power down bit
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_pll_power_down_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  *val = reg.pll_pd;
  
  return ret;  
}

/**
* @brief  ad1974_pll_mclk_in_rate: [set]  Write pll mclki/xi status
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_pll_mclk_in_rate_set(ad1974_ctx_t *ctx, ad1974_pll_mclk_in_rate_t val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.mclki_xi = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_pll_mclk_in_rate: [get]  Read pll mclki/xi status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_pll_mclk_in_rate_get(ad1974_ctx_t *ctx, ad1974_pll_mclk_in_rate_t *val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  switch (reg.mclki_xi)
  {
  case AD1974_PLL_MCLK_IN_RATE_INX256:
    *val = AD1974_PLL_MCLK_IN_RATE_INX256;
    break;
  case AD1974_PLL_MCLK_IN_RATE_INX384:
    *val = AD1974_PLL_MCLK_IN_RATE_INX384;
    break;   
  case AD1974_PLL_MCLK_IN_RATE_INX512:
    *val = AD1974_PLL_MCLK_IN_RATE_INX512;
    break; 
  case AD1974_PLL_MCLK_IN_RATE_INX768:
    *val = AD1974_PLL_MCLK_IN_RATE_INX768;
    break; 	
  default:
    *val = AD1974_PLL_MCLK_IN_RATE_ND;
    break;
  }
  
  return ret;  
}


/**
* @brief  ad1974_pll_mclk_out_rate: [set]  Write pll mclko/xo status
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_pll_mclk_out_rate_set(ad1974_ctx_t *ctx, ad1974_pll_mclk_out_rate_t val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.mclko_xo = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_pll_mclk_out_rate: [get]  Read pll mclko/xo status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_pll_mclk_out_rate_get(ad1974_ctx_t *ctx, ad1974_pll_mclk_out_rate_t *val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  switch (reg.mclko_xo)
  {
  case AD1974_PLL_MCLK_OUT_XTAL_EN:
    *val = AD1974_PLL_MCLK_OUT_XTAL_EN;
    break;
  case AD1974_PLL_MCLK_OUT_RATE_256XFS:
    *val = AD1974_PLL_MCLK_OUT_RATE_256XFS;
    break;   
  case AD1974_PLL_MCLK_OUT_RATE_512XFS:
    *val = AD1974_PLL_MCLK_OUT_RATE_512XFS;
    break; 
  case AD1974_PLL_MCLK_OUT_RATE_OFF:
    *val = AD1974_PLL_MCLK_OUT_RATE_OFF;
    break; 	
  default:
    *val = AD1974_PLL_MCLK_OUT_RATE_ND;
    break;
  }
  
  return ret;  
}


/**
* @brief  ad1974_pll_input: [set]  Write pll input config
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_pll_input_set(ad1974_ctx_t *ctx, ad1974_pll_input_t val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.pll_input = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_pll_input: [get]  Read pll input config
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_pll_input_get(ad1974_ctx_t *ctx, ad1974_pll_input_t *val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  switch (reg.pll_input)
  {
  case AD1974_PLL_IN_MCLKI_XI:
    *val = AD1974_PLL_IN_MCLKI_XI;
    break;
  case AD1974_PLL_IN_AUXLRCLK:
    *val = AD1974_PLL_IN_AUXLRCLK;
    break;   
  case AD1974_PLL_IN_ALRCLK:
    *val = AD1974_PLL_IN_ALRCLK;
    break; 
  case AD1974_PLL_IN_RESERVED:
    *val = AD1974_PLL_IN_RESERVED;
    break; 	
  default:
    *val = AD1974_PLL_IN_ND;
    break;
  }
  
  return ret;  
}

/**
  * @brief  ad1974_internal_mclk_enable: [set]  Set mclk enable bit
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_internal_mclk_enable_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.int_mclk_en = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_internal_mclk_enable: [get]  Read mclk enable bit
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_internal_mclk_enable_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_pll_clock_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_0, (uint8_t*) &reg, 1);
  *val = reg.int_mclk_en;
  
  return ret;  
}

/**
* @brief  ad1974_auxport_clk_sel: [set]  Set AUX clock
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_auxport_clk_sel_set(ad1974_ctx_t *ctx, ad1974_auxport_clk_sel_t val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.auxport_clk_src = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_auxport_clk_sel: [get]  Get AUX clock
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_auxport_clk_sel_get(ad1974_ctx_t *ctx, ad1974_auxport_clk_sel_t *val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  switch (reg.auxport_clk_src)
  {
  case AD1974_AUX_CLOCK_PLL:
    *val = AD1974_AUX_CLOCK_PLL;
    break;
  case AD1974_AUX_CLOCK_MCLK:
    *val = AD1974_AUX_CLOCK_MCLK;
    break;   	
  default:
    *val = AD1974_AUX_CLOCK_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_adc_clk_sel: [set] Write ADC clock selection
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_adc_clk_sel_set(ad1974_ctx_t *ctx, ad1974_adc_clk_sel_t_t val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc_clk_src = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_adc_clk_sel: [get]  Read ADC clock selection
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_adc_clk_sel_get(ad1974_ctx_t *ctx, ad1974_adc_clk_sel_t_t *val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  switch (reg.adc_clk_src)
  {
  case AD1974_ADC_CLOCK_PLL:
    *val = AD1974_ADC_CLOCK_PLL;
    break;
  case AD1974_ADC_CLOCK_MCLK:
    *val = AD1974_ADC_CLOCK_MCLK;
    break;   	
  default:
    *val = AD1974_ADC_CLOCK_ND;
    break;
  }  
  return ret;  
}

/**
  * @brief  ad1974_internal_onchip_vref_enable: [set]  Write int vref enable bit
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_internal_onchip_vref_enable_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.onchip_vref = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_internal_onchip_vref_enable: [get]  Read int vref enable bit
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_internal_onchip_vref_enable_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  *val = reg.onchip_vref;
  
  return ret;  
}

/**
  * @brief  ad1974_pll_lock: [get]  Read pll lock status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_pll_lock_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_pll_clock_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_PLL_CLOCK_1, (uint8_t*) &reg, 1);
  *val = reg.pll_lock;
  
  return ret;  
}

/**
  * @brief  ad1974_power_down: [set]  Write power down status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_power_down_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.pd = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_power_down: [get]  Read power down status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_power_down_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  *val = reg.pd;
  
  return ret;  
}

/**
  * @brief  ad1974_hpf_enable: [set]  Write hpf status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_hpf_enable_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.hpf = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_hpf_enable: [get]  Read hpf status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_hpf_enable_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  *val = reg.hpf;
  
  return ret;  
}

/**
  * @brief  ad1974_adc1l_mute: [set]  Write adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_adc1l_mute_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc1l_mute = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_adc1l_mute: [get]  Read adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_adc1l_mute_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  *val = reg.adc1l_mute;
  
  return ret;  
}

/**
  * @brief  ad1974_adc1r_mute: [set]  Write adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_adc1r_mute_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc1r_mute = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_adc1r_mute: [get]  Read adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_adc1r_mute_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  *val = reg.adc1r_mute;
  
  return ret;  
}

/**
  * @brief  ad1974_adc2l_mute: [set]  Write adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_adc2l_mute_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc2l_mute = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_adc2l_mute: [get]  Read adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_adc2l_mute_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  *val = reg.adc2l_mute;
  
  return ret;  
}

/**
  * @brief  ad1974_adc2r_mute: [set]  Write adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: value to be written
  *
  */
int32_t ad1974_adc2r_mute_set(ad1974_ctx_t *ctx, uint8_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.adc2r_mute = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;   
}

/**
  * @brief  ad1974_adc2r_mute: [get]  Read adc1l mute status
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_adc2r_mute_get(ad1974_ctx_t *ctx, uint8_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  *val = reg.adc2r_mute;
  
  return ret;  
}

/**
* @brief  ad1974_out_rate: [set] Write output sample rate
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_out_rate_set(ad1974_ctx_t *ctx, ad1974_out_rate_t val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.out_sample_rate = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_out_rate: [get]  Read output sample rate
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_out_rate_get(ad1974_ctx_t *ctx, ad1974_out_rate_t *val)
{
  ad1974_ADC_ctrl_0_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_0, (uint8_t*) &reg, 1);
  switch (reg.out_sample_rate)
  {
  case AD1974_OUT_RATE_32_441_48:
    *val = AD1974_OUT_RATE_32_441_48;
    break;
  case AD1974_OUT_RATE_64_882_96:
    *val = AD1974_OUT_RATE_64_882_96;
    break;   	
  case AD1974_OUT_RATE_128_1764_192:
    *val = AD1974_OUT_RATE_128_1764_192;
    break; 
  case AD1974_OUT_RATE_RESERVED:
    *val = AD1974_OUT_RATE_RESERVED;
    break; 
  default:
    *val = AD1974_OUT_RATE_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_word_width: [set] Write word width
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_word_width_set(ad1974_ctx_t *ctx, ad1974_word_width_t val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.word_width = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_word_width: [get]  Read word width
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_word_width_get(ad1974_ctx_t *ctx, ad1974_word_width_t *val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  switch (reg.word_width)
  {
  case AD1974_WORD_WIDTH_24:
    *val = AD1974_WORD_WIDTH_24;
    break;
  case AD1974_WORD_WIDTH_20:
    *val = AD1974_WORD_WIDTH_20;
    break;   	
  case AD1974_WORD_WIDTH_RESERVED:
    *val = AD1974_WORD_WIDTH_RESERVED;
    break; 
  case AD1974_WORD_WIDTH_16:
    *val = AD1974_WORD_WIDTH_16;
    break; 
  default:
    *val = AD1974_WORD_WIDTH_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_sdata_delay: [set] Write sdata delay
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_sdata_delay_set(ad1974_ctx_t *ctx, ad1974_sdata_delay_t val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.sdata_delay = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_sdata_delay: [get]  Read sdata delay
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_sdata_delay_get(ad1974_ctx_t *ctx, ad1974_sdata_delay_t *val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  switch (reg.sdata_delay)
  {
  case AD1974_SDATA_DELAY_1:
    *val = AD1974_SDATA_DELAY_1;
    break;
  case AD1974_SDATA_DELAY_0:
    *val = AD1974_SDATA_DELAY_0;
    break;   	
  case AD1974_SDATA_DELAY_8:
    *val = AD1974_SDATA_DELAY_8;
    break; 
  case AD1974_SDATA_DELAY_12:
    *val = AD1974_SDATA_DELAY_12;
    break; 
  case AD1974_SDATA_DELAY_16:
    *val = AD1974_SDATA_DELAY_16;
    break;
  case AD1974_SDATA_DELAY_RESERVED1:
    *val = AD1974_SDATA_DELAY_RESERVED1;
    break;   	
  case AD1974_SDATA_DELAY_RESERVED2:
    *val = AD1974_SDATA_DELAY_RESERVED2;
    break; 
  case AD1974_SDATA_DELAY_RESERVED3:
    *val = AD1974_SDATA_DELAY_RESERVED3;
    break; 
  default:
    *val = AD1974_SDATA_DELAY_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_serial_format: [set] Write serial format
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_serial_format_set(ad1974_ctx_t *ctx, ad1974_serial_format_t val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.serial_format = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_serial_format: [get]  Read serial format
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_serial_format_get(ad1974_ctx_t *ctx, ad1974_serial_format_t *val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  switch (reg.serial_format)
  {
  case AD1974_SERIAL_FORMAT_STEREO:
    *val = AD1974_SERIAL_FORMAT_STEREO;
    break;
  case AD1974_SERIAL_FORMAT_TDM:
    *val = AD1974_SERIAL_FORMAT_TDM;
    break;   	
	  case AD1974_SERIAL_FORMAT_ADC_AUX:
    *val = AD1974_SERIAL_FORMAT_ADC_AUX;
    break; 
	  case AD1974_SERIAL_FORMAT_RESERVED:
    *val = AD1974_SERIAL_FORMAT_RESERVED;
    break; 
  default:
    *val = AD1974_SERIAL_FORMAT_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_bclk_active_edge: [set] Write bclk active edge
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_bclk_active_edge_set(ad1974_ctx_t *ctx, ad1974_bclk_active_edge_t val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclk_active_edge = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_bclk_active_edge: [get]  bclk active edge
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_bclk_active_edge_get(ad1974_ctx_t *ctx, ad1974_bclk_active_edge_t *val)
{
  ad1974_ADC_ctrl_1_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_1, (uint8_t*) &reg, 1);
  switch (reg.bclk_active_edge)
  {
  case AD1974_BCLK_ACTIVE_EDGE_MIDCYCLE:
    *val = AD1974_BCLK_ACTIVE_EDGE_ENDCYCLE;
    break;
  case AD1974_BCLK_ACTIVE_EDGE_ENDCYCLE:
    *val = AD1974_BCLK_ACTIVE_EDGE_ENDCYCLE;
    break;   	
  default:
    *val = AD1974_BCLK_ACTIVE_EDGE_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_lrclk_format: [set] Write ltclk format
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_lrclk_format_set(ad1974_ctx_t *ctx, ad1974_lrclk_format_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.lrclk_format = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_lrclk_format: [get] Read  ltclk format
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_lrclk_format_get(ad1974_ctx_t *ctx, ad1974_lrclk_format_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.lrclk_format)
  {
  case AD1974_LRCLK_FORMAT_50_50:
    *val = AD1974_LRCLK_FORMAT_50_50;
    break;
  case AD1974_LRCLK_FORMAT_PULSE:
    *val = AD1974_LRCLK_FORMAT_PULSE;
    break;   	
  default:
    *val = AD1974_LRCLK_FORMAT_ND;
    break;
  }  
  return ret;  
}


/**
* @brief  ad1974_bclk_polarity: [set] Write bclk polarity
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_bclk_polarity_set(ad1974_ctx_t *ctx, ad1974_bclk_polarity_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclk_polarity = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_bclk_polarity: [get] Read bclk polarity
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_bclk_polarity_get(ad1974_ctx_t *ctx, ad1974_bclk_polarity_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.bclk_polarity)
  {
  case AD1974_BCLK_POLARITY_FALLING:
    *val = AD1974_BCLK_POLARITY_FALLING;
    break;
  case AD1974_BCLK_POLARITY_RISING:
    *val = AD1974_BCLK_POLARITY_RISING;
    break;   	
  default:
    *val = AD1974_BCLK_POLARITY_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_lrclk_polarity: [set] Write lrclk polarity
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_lrclk_polarity_set(ad1974_ctx_t *ctx, ad1974_lrclk_polarity_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.lrclk_polarity = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_lrclk_polarity: [get] Read lrclk polarity
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_lrclk_polarity_get(ad1974_ctx_t *ctx, ad1974_lrclk_polarity_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.lrclk_polarity)
  {
  case AD1974_LRCLK_POLARITY_LEFT_LOW:
    *val = AD1974_LRCLK_POLARITY_LEFT_LOW;
    break;
  case AD1974_LRCLK_POLARITY_LEFT_HIGH:
    *val = AD1974_LRCLK_POLARITY_LEFT_HIGH;
    break;   	
  default:
    *val = AD1974_LRCLK_POLARITY_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_lrclk_master_slave: [set] Write lrclk master slave
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_lrclk_master_slave_set(ad1974_ctx_t *ctx, ad1974_lrclk_master_slave_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.lrclk_m_s = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_lrclk_master_slave: [get] Read lrclk master slave
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_lrclk_master_slave_get(ad1974_ctx_t *ctx, ad1974_lrclk_master_slave_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.lrclk_m_s)
  {
  case AD1974_LRCLK_M_S_SLAVE:
    *val = AD1974_LRCLK_M_S_SLAVE;
    break;
  case AD1974_LRCLK_M_S_MASTER:
    *val = AD1974_LRCLK_M_S_MASTER;
    break;   	
  default:
    *val = AD1974_LRCLK_M_S_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_bclk_per_frame: [set] Write bclk per frame
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_bclk_per_frame_set(ad1974_ctx_t *ctx, ad1974_bclk_per_frame_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclk_per_frame = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_bclk_per_frame: [get] Read bclk per frame
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_bclk_per_frame_get(ad1974_ctx_t *ctx, ad1974_bclk_per_frame_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.bclk_per_frame)
  {
  case AD1974_BCLK_PER_FRAME_64:
    *val = AD1974_BCLK_PER_FRAME_64;
    break;
  case AD1974_BCLK_PER_FRAME_128:
    *val = AD1974_BCLK_PER_FRAME_128;
    break;   	
  case AD1974_BCLK_PER_FRAME_256:
    *val = AD1974_BCLK_PER_FRAME_256;
    break;
  case AD1974_BCLK_PER_FRAME_512:
    *val = AD1974_BCLK_PER_FRAME_512;
    break;   	
  default:
    *val = AD1974_BCLK_PER_FRAME_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_bclk_master_slave: [set] Write bclk master slave
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_bclk_master_slave_set(ad1974_ctx_t *ctx, ad1974_bclk_master_slave_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclk_m_s = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_bclk_master_slave: [get] Read bclk master slave
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_bclk_master_slave_get(ad1974_ctx_t *ctx, ad1974_bclk_master_slave_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.bclk_m_s)
  {
  case AD1974_BCLK_M_S_SLAVE:
    *val = AD1974_BCLK_M_S_SLAVE;
    break;
  case AD1974_BCLK_M_S_MASTER:
    *val = AD1974_BCLK_M_S_MASTER;
    break;   	   	
  default:
    *val = AD1974_BCLK_M_S_ND;
    break;
  }  
  return ret;  
}

/**
* @brief  ad1974_bclk_source: [set] Write bclk source
*
* @param  ad1974_ctx_t *ctx: read / write interface definitions
* @param  val: value to be written
*
*/
int32_t ad1974_bclk_source_set(ad1974_ctx_t *ctx, ad1974_bclk_source_t val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  
  if (ret == 0)
  {
    reg.bclk_source = (uint8_t)val;
    ret = ad1974_write_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  }
  
  return ret;
}

/**
  * @brief  ad1974_bclk_source: [get] Read bclk source
  *
  * @param  ad1974_ctx_t *ctx: read / write interface definitions
  * @param  val: destination pointer
  *
  */
int32_t ad1974_bclk_source_get(ad1974_ctx_t *ctx, ad1974_bclk_source_t *val)
{
  ad1974_ADC_ctrl_2_t reg;
  int32_t ret;
  
  ret = ad1974_read_reg(ctx, AD1974_ADC_CTRL_2, (uint8_t*) &reg, 1);
  switch (reg.bclk_source)
  {
  case AD1974_BCLK_SOURCE_ABCLK:
    *val = AD1974_BCLK_SOURCE_ABCLK;
    break;
  case AD1974_BCLK_SOURCE_INTERNAL:
    *val = AD1974_BCLK_SOURCE_INTERNAL;
    break;   	   	
  default:
    *val = AD1974_BCLK_SOURCE_ND;
    break;
  }  
  return ret;  
}

/**
* @}
*/

/**
* @}
*/



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


