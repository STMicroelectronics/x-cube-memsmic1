/**
******************************************************************************
* @file    ad1974_reg.h 
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   This file contains the headers of the ADC AD1974 drivers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AD1974_REG_H
#define AD1974_REG_H

#ifdef __cplusplus
extern "C" {
#endif 
  
  /* Includes ------------------------------------------------------------------*/
#include <stdint.h>
  
  
  /** @addtogroup BSP
  * @{
  */ 
  
  /** @addtogroup Components
  * @{
  */ 
  
  /** @addtogroup AD1974
  * @{
  */
  
  /** @defgroup AD1974_Exported_Constants
  * @{
  */  
  
  
  typedef int32_t (*ad1974_write_ptr)(void *, uint8_t, uint8_t *, uint16_t);
  typedef int32_t (*ad1974_read_ptr)(void *, uint8_t, uint8_t *, uint16_t);
  
  typedef struct
  {
    /** Component mandatory fields **/
    ad1974_write_ptr  write_reg;
    ad1974_read_ptr   read_reg;
    /** Customizable optional pointer **/
    void *handle;
  } ad1974_ctx_t;
  
  /** @defgroup AD1974_Registers_Mapping
  * @brief AD1974 register mapping
  * @{ 
  */  
  
  /**
  * @brief PLL and Clock control 0
  * \code
  * Read/Write
  * Default value:
  * 7 Internal MCLK enable
  * [6:5] PLL input 
  * [4:3] MCLKO/XO pin: master clock rate setting 
  * [2:1] MCLKI/XI pin: master clock rate setting
  * 0: PLL Power down
  * \endcode
  */   
#define AD1974_PLL_CLOCK_0                                 ((uint8_t)0x00)
  typedef struct
  {
    uint8_t pll_pd              : 1;
    uint8_t mclki_xi            : 2;
    uint8_t mclko_xo  	     	: 2;
    uint8_t pll_input           : 2;
    uint8_t int_mclk_en         : 1;
  } ad1974_pll_clock_0_t;
  
  /**
  * @brief PLL and Clock Control 1
  * \code
  * Read/Write (except bit 3)
  * Default value:
  * [7:4] reserved 
  * 3 PLL lock (read only)
  * 2 On-chip voltage reference
  * 1 ADC clock source select
  * 0 AUXPORT clock source select  
  * \endcode
  */   
#define AD1974_PLL_CLOCK_1                              ((uint8_t)0x01)
  typedef struct
  {
    uint8_t auxport_clk_src      : 1;
    uint8_t adc_clk_src        	 : 1;
    uint8_t onchip_vref          : 1;
    uint8_t pll_lock             : 1;
    uint8_t not_used_01          : 4;
  } ad1974_pll_clock_1_t;
  
  /**
  * @brief AUXPORT Control 0
  * \code
  * Read/Write
  * Default value:
  * [7:6] Serial format  
  * [5:3] AUXDATA delay (AUXBCLK periods) 
  * [2:1] Sample rate
  * 0: Reserved
  * \endcode
  */   
#define AD1974_AUX_CONTROL_0                          ((uint8_t)0x02)
  typedef struct
  {
    uint8_t not_used_01              : 1;
    uint8_t sample_rate              : 2;
    uint8_t auxdata_delay            : 3;
    uint8_t serial_format            : 2;
  } ad1974_aux_control_0_t;
  
  /**
  * @brief AUXPORT Control 1
  * \code
  * Read/Write
  * Default value:
  * 7 AUXBCLK polarity
  * 6 AUXBCLK source
  * 5 AUXBCLK master/slave
  * 4 AUXLRCLK master/slave
  * AUXLRCLK polarity
  * [2:1] AUXBCLKs per frame
  * 0 Reserved
  * \endcode
  */   
#define AD1974_AUX_CONTROL_1                          ((uint8_t)0x03)
  typedef struct
  {
    uint8_t not_used_01           : 1;
    uint8_t auxbclks_per_frame    : 2;
    uint8_t auxlrclk_polarity     : 1;
    uint8_t auxlrclk_m_s          : 1;
    uint8_t auxbclk_m_s           : 1;
    uint8_t auxbclk_source        : 1;
    uint8_t auxbclk_polarity      : 1;
  } ad1974_aux_control_1_t;
  
  /**
  * @brief AUXPORT Control 2
  * \code
  * Read/Write
  * Default value:
  * [5:7] Reserved  
  * [4:3] Word width
  * [2:0] Reserved
  * \endcode
  */   
#define AD1974_AUX_CONTROL_2                          ((uint8_t)0x04)
  typedef struct
  {
    uint8_t not_used_01              : 3;
    uint8_t word_width               : 2;
    uint8_t not_used_02              : 3;
  } ad1974_aux_control_2_t;
  
  /**
  * @brief ADC Control 0
  * \code
  * Read/Write
  * Default value:
  * [7:6] Output sample rate
  * 5 ADC2R mute
  * 4 ADC2L mute
  * 3 ADC1R mute
  * 2 ADC1L mute
  * 1 High-pass filter
  * 0 Power-down
  * \endcode
  */   
#define AD1974_ADC_CTRL_0                                ((uint8_t)0x0E)
  typedef struct
  {
    uint8_t pd                   : 1;
    uint8_t hpf                  : 1;
    uint8_t adc1l_mute           : 1;  
    uint8_t adc1r_mute           : 1;
    uint8_t adc2l_mute           : 1;
    uint8_t adc2r_mute           : 1;  
    uint8_t out_sample_rate      : 2;  
  } ad1974_ADC_ctrl_0_t;
  
  /**
  * @brief ADC Control 1
  * \code
  * Read/Write
  * Default value:
  * 7 BCLK active edge (TDM_IN) 
  * [6:5] Serial format  
  * [4:2] SDATA delay (BCLK periods)
  * [1:0] Word width
  * \endcode
  */   
#define AD1974_ADC_CTRL_1                                ((uint8_t)0x0F)
  typedef struct
  {
    uint8_t word_width           : 2;
    uint8_t sdata_delay          : 3;
    uint8_t serial_format        : 2;  
    uint8_t bclk_active_edge     : 1;  
    
  } ad1974_ADC_ctrl_1_t;
  
  /**
  * @brief ADC Control 2
  * \code
  * Read/Write
  * Default value:
  * 7 BCLK source
  * 6 BCLK master/slave 
  * [5:4] BCLKs per frame 
  * 3 LRCLK master/slave 
  * 2 LRCLK polarity
  * 1 BCLK polarity 
  * 0 LRCLK format 
  * \endcode
  */   
#define AD1974_ADC_CTRL_2                                ((uint8_t)0x10)
  typedef struct
  {
    uint8_t lrclk_format       : 1;
    uint8_t bclk_polarity       : 1;
    uint8_t lrclk_polarity     : 1;  
    uint8_t lrclk_m_s          : 1;
    uint8_t bclk_per_frame     : 2;
    uint8_t bclk_m_s           : 1;  
    uint8_t bclk_source        : 1;  
  } ad1974_ADC_ctrl_2_t;
  
/**
  * @defgroup AD1974_Register_Union
  * @brief    This union group all the registers that has a bitfield
  *           description.
  *           This union is useful but not need by the driver.
  *
  *           REMOVING this union you are compliant with:
  *           MISRA-C 2012 [Rule 19.2] -> " Union are not allowed "
  *
  * @{
  *
  */
  
  typedef union
  {  
    ad1974_pll_clock_0_t           	ad1974_pll_clock_0;
    ad1974_pll_clock_1_t           	ad1974_pll_clock_1;
    ad1974_aux_control_0_t          ad1974_aux_control_0;
    ad1974_aux_control_1_t          ad1974_aux_control_1;
    ad1974_aux_control_2_t          ad1974_aux_control_2;
    ad1974_ADC_ctrl_0_t           	ad1974_ADC_ctrl_0;
    ad1974_ADC_ctrl_1_t           	ad1974_ADC_ctrl_1;
    ad1974_ADC_ctrl_2_t           	ad1974_ADC_ctrl_2;
    uint8_t                         byte;
  } ad1974_reg_t; 
  
  /** @defgroup AD1974_address_define AD1974 address define
  * @brief AD1974 address definitions
  * @{
  */
  /**
  * @}
  */
  
  
  /** @defgroup AD1974_Exported_Functions
  * @{
  */  
  int32_t ad1974_read_reg(ad1974_ctx_t *ctx, uint8_t reg, uint8_t *data,
                          uint16_t len);
  int32_t ad1974_write_reg(ad1974_ctx_t *ctx, uint8_t reg, uint8_t *data,
                           uint16_t len);  
  
  int32_t ad1974_pll_power_down_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_pll_power_down_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  typedef enum
  {
    AD1974_PLL_MCLK_IN_RATE_INX256  	= 0,
    AD1974_PLL_MCLK_IN_RATE_INX384  	= 1,
    AD1974_PLL_MCLK_IN_RATE_INX512  	= 2,
    AD1974_PLL_MCLK_IN_RATE_INX768  	= 3,
    AD1974_PLL_MCLK_IN_RATE_ND  		= 4,    
  }
  ad1974_pll_mclk_in_rate_t;
  
  int32_t ad1974_pll_mclk_in_rate_set(ad1974_ctx_t *ctx, ad1974_pll_mclk_in_rate_t val);
  int32_t ad1974_pll_mclk_in_rate_get(ad1974_ctx_t *ctx, ad1974_pll_mclk_in_rate_t *val);
  
  typedef enum
  {
    AD1974_PLL_MCLK_OUT_XTAL_EN  		= 0,
    AD1974_PLL_MCLK_OUT_RATE_256XFS  	= 1,
    AD1974_PLL_MCLK_OUT_RATE_512XFS  	= 2,
    AD1974_PLL_MCLK_OUT_RATE_OFF  		= 3,
    AD1974_PLL_MCLK_OUT_RATE_ND  		= 4,    
  }
  ad1974_pll_mclk_out_rate_t;
  
  int32_t ad1974_pll_mclk_out_rate_set(ad1974_ctx_t *ctx, ad1974_pll_mclk_out_rate_t val);
  int32_t ad1974_pll_mclk_out_rate_get(ad1974_ctx_t *ctx, ad1974_pll_mclk_out_rate_t *val);
  
  
  
  typedef enum
  {
    AD1974_PLL_IN_MCLKI_XI  			= 0,
    AD1974_PLL_IN_AUXLRCLK			    = 1,
    AD1974_PLL_IN_ALRCLK			    = 2,
    AD1974_PLL_IN_RESERVED				= 3,   
    AD1974_PLL_IN_ND  					= 4,           
  } 
  ad1974_pll_input_t;
  
  int32_t ad1974_pll_input_set(ad1974_ctx_t *ctx, ad1974_pll_input_t val);
  int32_t ad1974_pll_input_get(ad1974_ctx_t *ctx, ad1974_pll_input_t *val);
  
  int32_t ad1974_internal_mclk_enable_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_internal_mclk_enable_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  typedef enum
  {
    AD1974_AUX_CLOCK_PLL  				= 0,
    AD1974_AUX_CLOCK_MCLK   			= 1, 
    AD1974_AUX_CLOCK_ND			   		= 2,
  } 
  ad1974_auxport_clk_sel_t;
  
  int32_t ad1974_auxport_clk_sel_set(ad1974_ctx_t *ctx, ad1974_auxport_clk_sel_t val);
  int32_t ad1974_auxport_clk_sel_get(ad1974_ctx_t *ctx, ad1974_auxport_clk_sel_t *val);
  
  typedef enum
  {
    AD1974_ADC_CLOCK_PLL  				= 0,
    AD1974_ADC_CLOCK_MCLK   			= 1, 
    AD1974_ADC_CLOCK_ND			   		= 2,
  } 
  ad1974_adc_clk_sel_t_t;
  
  int32_t ad1974_adc_clk_sel_set(ad1974_ctx_t *ctx, ad1974_adc_clk_sel_t_t val);
  int32_t ad1974_adc_clk_sel_get(ad1974_ctx_t *ctx, ad1974_adc_clk_sel_t_t *val);
  
  int32_t ad1974_internal_onchip_vref_enable_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_internal_onchip_vref_enable_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  int32_t ad1974_pll_lock_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  /*AUX PORT registers not used*/
  
  int32_t ad1974_power_down_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_power_down_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  int32_t ad1974_hpf_enable_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_hpf_enable_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  int32_t ad1974_adc1l_mute_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_adc1l_mute_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  int32_t ad1974_adc1r_mute_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_adc1r_mute_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  int32_t ad1974_adc2l_mute_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_adc2l_mute_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  int32_t ad1974_adc2r_mute_set(ad1974_ctx_t *ctx, uint8_t val);
  int32_t ad1974_adc2r_mute_get(ad1974_ctx_t *ctx, uint8_t *val);
  
  typedef enum
  {
    AD1974_OUT_RATE_32_441_48   		= 0,
    AD1974_OUT_RATE_64_882_96   		= 1, 
    AD1974_OUT_RATE_128_1764_192   		= 2,
    AD1974_OUT_RATE_RESERVED		    = 3,
    AD1974_OUT_RATE_ND   				= 4,
  } 
  ad1974_out_rate_t;
  
  int32_t ad1974_out_rate_set(ad1974_ctx_t *ctx, ad1974_out_rate_t val);
  int32_t ad1974_out_rate_get(ad1974_ctx_t *ctx, ad1974_out_rate_t *val);
  
  typedef enum
  {
    AD1974_WORD_WIDTH_24 				= 0,
    AD1974_WORD_WIDTH_20 				= 1, 
    AD1974_WORD_WIDTH_RESERVED 			= 2, 
    AD1974_WORD_WIDTH_16 				= 3,
    AD1974_WORD_WIDTH_ND 				= 4, 
    
  } 
  ad1974_word_width_t;
  int32_t ad1974_word_width_set(ad1974_ctx_t *ctx, ad1974_word_width_t val);
  int32_t ad1974_word_width_get(ad1974_ctx_t *ctx, ad1974_word_width_t *val);
  
  
  typedef enum
  {
    AD1974_SDATA_DELAY_1 				= 0,
    AD1974_SDATA_DELAY_0 				= 1, 
    AD1974_SDATA_DELAY_8 				= 2, 
    AD1974_SDATA_DELAY_12 				= 3,
    AD1974_SDATA_DELAY_16 				= 4,
    AD1974_SDATA_DELAY_RESERVED1 		= 5,
    AD1974_SDATA_DELAY_RESERVED2 		= 6, 
    AD1974_SDATA_DELAY_RESERVED3 		= 7, 
    AD1974_SDATA_DELAY_ND 				= 8,    
  } 
  ad1974_sdata_delay_t;
  int32_t ad1974_sdata_delay_set(ad1974_ctx_t *ctx, ad1974_sdata_delay_t val);
  int32_t ad1974_sdata_delay_get(ad1974_ctx_t *ctx, ad1974_sdata_delay_t *val);
  
  typedef enum
  {
    AD1974_SERIAL_FORMAT_STEREO   		= 0,
    AD1974_SERIAL_FORMAT_TDM   			= 1, 
    AD1974_SERIAL_FORMAT_ADC_AUX   		= 2,
    AD1974_SERIAL_FORMAT_RESERVED	    = 3,
    AD1974_SERIAL_FORMAT_ND   			= 4,
  } 
  ad1974_serial_format_t;
  
  int32_t ad1974_serial_format_set(ad1974_ctx_t *ctx, ad1974_serial_format_t val);
  int32_t ad1974_serial_format_get(ad1974_ctx_t *ctx, ad1974_serial_format_t *val);
  
  typedef enum
  {
    AD1974_BCLK_ACTIVE_EDGE_MIDCYCLE	= 0,
    AD1974_BCLK_ACTIVE_EDGE_ENDCYCLE  	= 1, 
    AD1974_BCLK_ACTIVE_EDGE_ND   		= 2,
  } 
  ad1974_bclk_active_edge_t;
  
  int32_t ad1974_bclk_active_edge_set(ad1974_ctx_t *ctx, ad1974_bclk_active_edge_t val);
  int32_t ad1974_bclk_active_edge_get(ad1974_ctx_t *ctx, ad1974_bclk_active_edge_t *val);
  
  typedef enum
  {
    AD1974_LRCLK_FORMAT_50_50		= 0,
    AD1974_LRCLK_FORMAT_PULSE  		= 1, 
    AD1974_LRCLK_FORMAT_ND   		= 2,
  } 
  ad1974_lrclk_format_t;
  
  int32_t ad1974_lrclk_format_set(ad1974_ctx_t *ctx, ad1974_lrclk_format_t val);
  int32_t ad1974_lrclk_format_get(ad1974_ctx_t *ctx, ad1974_lrclk_format_t *val);
  
  typedef enum
  {
    AD1974_BCLK_POLARITY_FALLING		= 0,
    AD1974_BCLK_POLARITY_RISING  		= 1, 
    AD1974_BCLK_POLARITY_ND   			= 2,
  } 
  ad1974_bclk_polarity_t;
  
  int32_t ad1974_bclk_polarity_set(ad1974_ctx_t *ctx, ad1974_bclk_polarity_t val);
  int32_t ad1974_bclk_polarity_get(ad1974_ctx_t *ctx, ad1974_bclk_polarity_t *val);
  
  typedef enum
  {
    AD1974_LRCLK_POLARITY_LEFT_LOW		= 0,
    AD1974_LRCLK_POLARITY_LEFT_HIGH  	= 1, 
    AD1974_LRCLK_POLARITY_ND   			= 2,
  } 
  ad1974_lrclk_polarity_t;
  
  int32_t ad1974_lrclk_polarity_set(ad1974_ctx_t *ctx, ad1974_lrclk_polarity_t val);
  int32_t ad1974_lrclk_polarity_get(ad1974_ctx_t *ctx, ad1974_lrclk_polarity_t *val);
  
  typedef enum
  {
    AD1974_LRCLK_M_S_SLAVE			= 0,
    AD1974_LRCLK_M_S_MASTER  		= 1, 
    AD1974_LRCLK_M_S_ND   			= 2,
  } 
  ad1974_lrclk_master_slave_t;
  
  int32_t ad1974_lrclk_master_slave_set(ad1974_ctx_t *ctx, ad1974_lrclk_master_slave_t val);
  int32_t ad1974_lrclk_master_slave_get(ad1974_ctx_t *ctx, ad1974_lrclk_master_slave_t *val);
  
  typedef enum
  {
    AD1974_BCLK_PER_FRAME_64   		= 0,
    AD1974_BCLK_PER_FRAME_128   	= 1, 
    AD1974_BCLK_PER_FRAME_256   	= 2,
    AD1974_BCLK_PER_FRAME_512	    = 3,
    AD1974_BCLK_PER_FRAME_ND   		= 4,
  } 
  ad1974_bclk_per_frame_t;
  
  int32_t ad1974_bclk_per_frame_set(ad1974_ctx_t *ctx, ad1974_bclk_per_frame_t val);
  int32_t ad1974_bclk_per_frame_get(ad1974_ctx_t *ctx, ad1974_bclk_per_frame_t *val);
  
  typedef enum
  {
    AD1974_BCLK_M_S_SLAVE			= 0,
    AD1974_BCLK_M_S_MASTER  		= 1, 
    AD1974_BCLK_M_S_ND   			= 2,
  } 
  ad1974_bclk_master_slave_t;
  
  int32_t ad1974_bclk_master_slave_set(ad1974_ctx_t *ctx, ad1974_bclk_master_slave_t val);
  int32_t ad1974_bclk_master_slave_get(ad1974_ctx_t *ctx, ad1974_bclk_master_slave_t *val);
  
  typedef enum
  {
    AD1974_BCLK_SOURCE_ABCLK			= 0,
    AD1974_BCLK_SOURCE_INTERNAL  		= 1, 
    AD1974_BCLK_SOURCE_ND   			= 2,
  } 
  ad1974_bclk_source_t;
  
  int32_t ad1974_bclk_source_set(ad1974_ctx_t *ctx, ad1974_bclk_source_t val);
  int32_t ad1974_bclk_source_get(ad1974_ctx_t *ctx, ad1974_bclk_source_t *val);
  
  
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
#ifdef __cplusplus
}
#endif  

#endif /*AD1974_REG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
