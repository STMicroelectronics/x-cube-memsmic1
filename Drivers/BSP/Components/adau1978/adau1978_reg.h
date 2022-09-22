/**
******************************************************************************
* @file    adau1978_Driver.h 
* @author  SRA - Central Labs
* @version v1.0.1
* @date    03-Apr-2020
* @brief   This file contains the headers of the ADC ADAU1978 drivers.
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
#ifndef ADAU1978_REG_H
#define ADAU1978_REG_H

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
  
  /** @addtogroup ADAU1978
  * @{
  */
  
  /** @defgroup ADAU1978_Exported_Constants
  * @{
  */  
  
  
  typedef int32_t (*adau1978_write_ptr)(void *, uint8_t, uint8_t *, uint16_t);
  typedef int32_t (*adau1978_read_ptr)(void *, uint8_t, uint8_t *, uint16_t);
  
  typedef struct
  {
    /** Component mandatory fields **/
    adau1978_write_ptr  write_reg;
    adau1978_read_ptr   read_reg;
    /** Customizable optional pointer **/
    void *handle;
  } adau1978_ctx_t;
  
  /** I2C Device Address 8 bit format  **/
#define ADAU1978_I2C_ADDRESS_01   ((uint8_t)0x71<<1)
#define ADAU1978_I2C_ADDRESS_02   ((uint8_t)0x51<<1)
#define ADAU1978_I2C_ADDRESS_03   ((uint8_t)0x31<<1)
#define ADAU1978_I2C_ADDRESS_04   ((uint8_t)0x11<<1)
  
  
  /** @defgroup ADAU1978_Registers_Mapping
  * @brief ADAU1978 register mapping
  * @{ 
  */
  
  
  /**
  * @brief Master Power and Reset
  * \code
  * Read/Write
  * Default value:
  * 7 Software reset
  * [6:1] Reserved 
  * 0 Master Power-Up Control
  * \endcode
  */   
#define ADAU1978_M_POWER                                  ((uint8_t)0x00)
  typedef struct
  {
    uint8_t pwup                 : 1;
    uint8_t not_used_01          : 6;
    uint8_t s_rst                : 1;
  } adau1978_m_power_t;
  
  /**
  * @brief PLL Control
  * \code
  * Read/Write( except bit7 ) 
  * Default value:
  * 7 PLL Lock status (read only)
  * 6 PLL Unlock automute 
  * 5 Reserved
  * 4 PLL Clock select
  * 3 Reserved
  * [2:0] Master Clock Select
  * \endcode
  */   
#define ADAU1978_PLL_CONTROL                              ((uint8_t)0x01)
  typedef struct
  {
    uint8_t mcs                  : 3;
    uint8_t not_used_01          : 1;
    uint8_t clk_s                : 1;
    uint8_t not_used_02          : 1;
    uint8_t pll_mute             : 1;
    uint8_t pll_lock             : 1;
  } adau1978_pll_control_t;
  
  /**
  * @brief Block power control and Serial port control register
  * \code
  * Read/Write
  * Default value:
  * 7 LRCLK Polarity
  * 6 Bit clock edge (Falling vs Rising)
  * 5 LDO enable
  * 4 Voltage reference enable
  * 3 ADC channel 4 enable
  * 2 ADC channel 3 enable
  * 1 ADC channel 2 enable
  * 0 ADC channel 1 enable
  * \endcode
  */   
#define ADAU1978_BLOCK_POWER_SAI                          ((uint8_t)0x04)
  typedef struct
  {
    uint8_t adc_en1              : 1;
    uint8_t adc_en2              : 1;
    uint8_t adc_en3              : 1;
    uint8_t adc_en4              : 1;
    uint8_t vref_en              : 1;
    uint8_t ldo_en               : 1;
    uint8_t bclkedge             : 1;
    uint8_t lr_pol               : 1;
  } adau1978_block_power_sai_t;
  
  /**
  * @brief Serial port control register 1
  * \code
  * Read/Write
  * Default value:
  * [7:6] Serial data format 
  * [5:3] Serial port mode
  * [2:0] Sampling rate
  * \endcode
  */   
#define ADAU1978_SAI_CTRL0                                ((uint8_t)0x05)
  typedef struct
  {
    uint8_t fs                   : 3;
    uint8_t sai                  : 3;
    uint8_t sdata_fmt            : 2;  
  } adau1978_sai_ctrl0_t;
  
  /**
  * @brief Serial port control register 2
  * \code
  * Read/Write
  * Default value:
  * 7 SDATAOUT Pin selection in TDM4 or greater mode
  * [6:5] BCLK per slot in TDM mode
  * 4 Output data bit width
  * 3 LRCLK mode
  * 2 MSB or LSB first
  * 1 BCLK per channel
  * 0 Master/slave
  * \endcode
  */ 
#define ADAU1978_SAI_CTRL1                                ((uint8_t)0x06)
  typedef struct
  {
    uint8_t sai_ms               : 1;
    uint8_t bclkrate             : 1;
    uint8_t sai_msb              : 1;
    uint8_t lr_mode              : 1;
    uint8_t data_width           : 1;
    uint8_t slot_width           : 2;
    uint8_t sdata_sel            : 1;
  } adau1978_sai_ctrl1_t;
  
  /**
  * @brief CHANNEL 1 AND CHANNEL 2 MAPPING FOR OUTPUT SERIAL PORTS REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:4] ADC Channel 2 Output Mapping
  * [3:0] ADC Channel 1 Output Mapping
  * \endcode
  */   
#define ADAU1978_SAI_CMAP12                               ((uint8_t)0x07)
  typedef struct
  {
    uint8_t cmap_c1               : 4;
    uint8_t cmap_c2               : 4;  
  } adau1978_sai_cmap12_t;
  
  /**
  * @brief CHANNEL 3 AND CHANNEL 4 MAPPING FOR OUTPUT SERIAL PORTS REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:4] ADC Channel 3 Output Mapping
  * [3:0] ADC Channel 4 Output Mapping
  * \endcode
  */   
#define ADAU1978_SAI_CMAP34                               ((uint8_t)0x08)
  typedef struct
  {
    uint8_t cmap_c3               : 4;
    uint8_t cmap_c4               : 4;  
  } adau1978_sai_cmap34_t;
  
  /**
  * @brief SERIAL OUTPUT DRIVE CONTROL AND OVERTEMPERATURE PROTECTION STATUS REGISTER
  * \code
  * Read/Write (except [2:0])
  * Default value:
  * 7 Channel 4 Serial Output Drive Enable.
  * 6 Channel 3 Serial Output Drive Enable.
  * 5 Channel 2 Serial Output Drive Enable.
  * 4 Channel 1 Serial Output Drive Enable.
  * 3 Select whether to tristate unused SAI channels or actively drive these data slots.
  * [2:1] Reserved
  * 0 Overtemperature Status
  * \endcode
  */   
#define ADAU1978_SAI_OVERTEMP                             ((uint8_t)0x09)
  typedef struct
  {
    uint8_t ot                   : 1;
    uint8_t not_used_01          : 2;
    uint8_t drv_hiz              : 1;
    uint8_t sai_drv_c1           : 1;
    uint8_t sai_drv_c2           : 1;
    uint8_t sai_drv_c3           : 1;
    uint8_t sai_drv_c4           : 1;
  } adau1978_sai_overtem_t;
  
  /**
  * @brief POST ADC GAIN CHANNEL 1 CONTROL REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:0] Channel 1 Post ADC Gain
  * \endcode
  */   
#define ADAU1978_POSTADC_GAIN1                            ((uint8_t)0x0A)
  
  /**
  * @brief POST ADC GAIN CHANNEL 2 CONTROL REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:0] Channel 2 Post ADC Gain
  * \endcode
  */   
#define ADAU1978_POSTADC_GAIN2                            ((uint8_t)0x0B)
  
  /**
  * @brief POST ADC GAIN CHANNEL 3 CONTROL REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:0] Channel 3 Post ADC Gain
  * \endcode
  */   
#define ADAU1978_POSTADC_GAIN3                            ((uint8_t)0x0C)
  
  /**
  * @brief POST ADC GAIN CHANNEL 4 CONTROL REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:0] Channel 4 Post ADC Gain
  * \endcode
  */   
#define ADAU1978_POSTADC_GAIN4                            ((uint8_t)0x0D)
  
  /**
  * @brief HIGH-PASS FILTER AND DC OFFSET CONTROL REGISTER AND MASTER MUTE REGISTER
  * \code
  * Read/Write
  * [7:6] Channel Summing Mode Control for Higher SNR
  * 5 Reserved
  * 4 Master Mute
  * [3:1] Reserved
  * 0 DC Calibration Enable
  * \endcode
  */   
#define ADAU1978_MISC_CONTROL                             ((uint8_t)0x0E)
  typedef struct
  {
    uint8_t dc_cal               : 1;
    uint8_t not_used_01          : 3;
    uint8_t mmute                : 1;
    uint8_t not_used_02          : 1;
    uint8_t sum_mode             : 2;  
  } adau1978_misc_control_t;
  
  /**
  * @brief ADC CLIPPING STATUS REGISTER
  * \code
  * Read/Write
  * Default value:
  * [7:4] Reserved
  * 3 ADC Channel 4 Clip Status
  * 2 ADC Channel 3 Clip Status
  * 1 ADC Channel 2 Clip Status
  * 0 ADC Channel 1 Clip Status
  * \endcode
  */   
#define ADAU1978_ASDC_CLIP                                ((uint8_t)0x19)
  typedef struct
  {
    uint8_t adc_clip1            : 1;
    uint8_t adc_clip2            : 1;
    uint8_t adc_clip3            : 1;
    uint8_t adc_clip4            : 1;
    uint8_t not_used_01          : 4;  
  } adau1978_asdc_clip_t;
  
  /**
  * @brief DIGITAL DC HIGH-PASS FILTER AND CALIBRATION REGISTER
  * \code
  * Read/Write
  * Default value:
  * 7 Channel 4 DC Subtraction from Calibration
  * 6 Channel 3 DC Subtraction from Calibration
  * 5 Channel 2 DC Subtraction from Calibration
  * 4 Channel 1 DC Subtraction from Calibration
  * 3 Channel 4 DC High-Pass Filter Enable
  * 2 Channel 3 DC High-Pass Filter Enable
  * 1 Channel 2 DC High-Pass Filter Enable
  * 0 Channel 1 DC High-Pass Filter Enable
  * \endcode
  */   
#define ADAU1978_DC_HPF_CAL                               ((uint8_t)0x1A)
  typedef struct
  {
    uint8_t dc_hpf_c1            : 1;
    uint8_t dc_hpf_c2            : 1;
    uint8_t dc_hpf_c3            : 1;
    uint8_t dc_hpf_c4            : 1;
    uint8_t dc_sub_c1            : 1;
    uint8_t dc_sub_c2            : 1;
    uint8_t dc_sub_c3            : 1;
    uint8_t dc_sub_c4            : 1;
  } adau1978_dc_hpf_cal_t;
  
/**
  * @defgroup adau1978_Register_Union
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
    adau1978_m_power_t                    m_power;
    adau1978_pll_control_t                pll_control;
    adau1978_block_power_sai_t            block_power_sai;
    adau1978_sai_ctrl0_t                  sai_ctrl0;
    adau1978_sai_ctrl1_t                  sai_ctrl1;
    adau1978_sai_cmap12_t                 sai_cmap12;
    adau1978_sai_cmap34_t                 sai_cmap34;
    adau1978_sai_overtem_t                sai_overtem;
    adau1978_misc_control_t               misc_control;
    adau1978_asdc_clip_t                  asdc_clip;
    adau1978_dc_hpf_cal_t                 dc_hpf_cal;
    uint8_t                               byte;
  } adau1978_reg_t;
  
  
  /** @defgroup ADAU1978_adsress_define ADAU1978 address define
  * @brief ADAU1978 address definitions
  * @{
  */
  /**
  * @}
  */
  
  
  /** @defgroup ADAU1978_Exported_Functions
  * @{
  */  
  int32_t adau1978_read_reg(adau1978_ctx_t *ctx, uint8_t reg, uint8_t *data,
                            uint16_t len);
  int32_t adau1978_write_reg(adau1978_ctx_t *ctx, uint8_t reg, uint8_t *data,
                             uint16_t len);
  
  int32_t adau1978_software_reset_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_software_reset_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_power_up_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_power_up_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  typedef enum
  {
    ADAU1978_PLL_SOURCE_MCLK  = 0,
    ADAU1978_PLL_SOURCE_LRCLK   = 1,  
    ADAU1978_PLL_SOURCE_ND   = 2,  
    
  }
  adau1978_pll_source_t;
  
  int32_t adau1978_pll_source_set(adau1978_ctx_t *ctx, adau1978_pll_source_t val);
  int32_t adau1978_pll_source_get(adau1978_ctx_t *ctx, adau1978_pll_source_t *val);
  
  typedef enum
  {
    ADAU1978_MCLK_128xFs  = 0,
    ADAU1978_MCLK_256xFs  = 1,
    ADAU1978_MCLK_384xFs  = 2,
    ADAU1978_MCLK_512xFs  = 3,
    ADAU1978_MCLK_768xFs  = 4,     
    ADAU1978_MCLK_ND  = 5,       
    
  } 
  adau1978_mclk_ratio_t;
  
  int32_t adau1978_mclk_ratio_set(adau1978_ctx_t *ctx, adau1978_mclk_ratio_t val);
  int32_t adau1978_mclk_ratio_get(adau1978_ctx_t *ctx, adau1978_mclk_ratio_t *val);
  int32_t adau1978_pll_automute_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_pll_automute_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  int32_t adau1978_pll_lock_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  
  typedef enum
  {
    ADAU1978_LRCLK_LH  = 0,
    ADAU1978_LRCLK_HL   = 1, 
    ADAU1978_LRCLK_ND   = 2,
  } 
  adau1978_lrclk_polarity_t;
  
  int32_t adau1978_lrclk_polarity_set(adau1978_ctx_t *ctx, adau1978_lrclk_polarity_t val);
  int32_t adau1978_lrclk_polarity_get(adau1978_ctx_t *ctx, adau1978_lrclk_polarity_t *val);
  
  typedef enum
  {
    ADAU1978_BCLK_FALLING  = 0,
    ADAU1978_BCLK_RISING   = 1, 
    ADAU1978_BCLK_ND   = 2,
  } 
  adau1978_bclk_edge_t;
  int32_t adau1978_bclk_edge_set(adau1978_ctx_t *ctx, adau1978_bclk_edge_t val);
  int32_t adau1978_bclk_edge_get(adau1978_ctx_t *ctx, adau1978_bclk_edge_t *val);
  
  int32_t adau1978_ldo_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_ldo_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_vref_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_vref_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_adc_ch1_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_adc_ch1_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_adc_ch2_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_adc_ch2_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_adc_ch3_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_adc_ch3_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_adc_ch4_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_adc_ch4_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  typedef enum
  {
    ADAU1978_DFORMAT_I2S  = 0,
    ADAU1978_DFORMAT_LJ   = 1, 
    ADAU1978_DFORMAT_RJ24   = 2, 
    ADAU1978_DFORMAT_RJ16   = 3, 
    ADAU1978_DFORMAT_ND   = 4, 
    
  } 
  adau1978_data_format_t;
  int32_t adau1978_data_format_set(adau1978_ctx_t *ctx, adau1978_data_format_t val);
  int32_t adau1978_data_format_get(adau1978_ctx_t *ctx, adau1978_data_format_t *val);
  
  typedef enum
  {
    ADAU1978_SMODE_STEREO  = 0,
    ADAU1978_SMODE_TDM2   = 1, 
    ADAU1978_SMODE_TDM4   = 2, 
    ADAU1978_SMODE_TDM8   = 3, 
    ADAU1978_SMODE_TDM16   = 4, 
    ADAU1978_SMODE_ND   = 5, 
    
  } 
  adau1978_serial_port_mode_t;
  int32_t adau1978_serial_port_mode_set(adau1978_ctx_t *ctx, adau1978_serial_port_mode_t val);
  int32_t adau1978_serial_port_mode_get(adau1978_ctx_t *ctx, adau1978_serial_port_mode_t *val);
  
  typedef enum
  {
    ADAU1978_SRATE_8_12  = 0,
    ADAU1978_SRATE_16_24 = 1, 
    ADAU1978_SRATE_32_48 = 2, 
    ADAU1978_SRATE_64_96 = 3, 
    ADAU1978_SRATE_128_192 = 4, 
    ADAU1978_SRATE_ND = 5, 
    
  } 
  adau1978_sampling_rate_t;
  int32_t adau1978_sampling_rate_set(adau1978_ctx_t *ctx, adau1978_sampling_rate_t val);
  int32_t adau1978_sampling_rate_get(adau1978_ctx_t *ctx, adau1978_sampling_rate_t *val);
  
  typedef enum
  {
    ADAU1978_SAI_S = 0,
    ADAU1978_SAI_M = 1, 
    ADAU1978_SAI_MS_ND = 2, 
    
  } 
  adau1978_sai_ms_t;
  int32_t adau1978_sai_ms_set(adau1978_ctx_t *ctx, adau1978_sai_ms_t val);
  int32_t adau1978_sai_ms_get(adau1978_ctx_t *ctx, adau1978_sai_ms_t *val);
  
  typedef enum
  {
    ADAU1978_BCLKRATE_32 = 0,
    ADAU1978_BCLKRATE_16 = 1, 
    ADAU1978_BCLKRATE_ND = 2, 
    
  } 
  adau1978_sai_bclkrate_t;
  int32_t adau1978_bclkrate_set(adau1978_ctx_t *ctx, adau1978_sai_bclkrate_t val);
  int32_t adau1978_bclkrate_get(adau1978_ctx_t *ctx, adau1978_sai_bclkrate_t *val);
  
  typedef enum
  {
    ADAU1978_SAI_MSB = 0,
    ADAU1978_SAI_LSB = 1, 
    ADAU1978_SAI_ND = 2, 
    
  } 
  adau1978_sai_msblsb_t;
  int32_t adau1978_sai_msb_set(adau1978_ctx_t *ctx, adau1978_sai_msblsb_t val);
  int32_t adau1978_sai_msb_get(adau1978_ctx_t *ctx, adau1978_sai_msblsb_t  *val);
  
  typedef enum
  {
    ADAU1978_LR_MODE_50DUTY = 0,
    ADAU1978_LR_MODE_SINGLE = 1, 
    ADAU1978_LR_MODE_ND = 2, 
    
  } 
  adau1978_lr_mode_t;
  int32_t adau1978_lr_mode_set(adau1978_ctx_t *ctx, adau1978_lr_mode_t val);
  int32_t adau1978_lr_mode_get(adau1978_ctx_t *ctx, adau1978_lr_mode_t  *val);
  
  typedef enum
  {
    ADAU1978_DATA_WIDTH_24 = 0,
    ADAU1978_DATA_WIDTH_16 = 1, 
    ADAU1978_DATA_WIDTH_ND = 2, 
    
  } 
  adau1978_data_width_t;
  int32_t adau1978_data_width_set(adau1978_ctx_t *ctx, adau1978_data_width_t val);
  int32_t adau1978_data_width_get(adau1978_ctx_t *ctx, adau1978_data_width_t  *val);
  
  typedef enum
  {
    ADAU1978_SLOT_WIDTH_32 = 0,
    ADAU1978_SLOT_WIDTH_24 = 1,
    ADAU1978_SLOT_WIDTH_16 = 2,
    ADAU1978_SLOT_WIDTH_ND = 3,
    
  } 
  adau1978_slot_width_t;
  int32_t adau1978_slot_width_set(adau1978_ctx_t *ctx, adau1978_slot_width_t val);
  int32_t adau1978_slot_width_get(adau1978_ctx_t *ctx, adau1978_slot_width_t  *val);
  
  
  typedef enum
  {
    ADAU1978_SDATAOUT_1 = 0,
    ADAU1978_SDATAOUT_2 = 1,   
    ADAU1978_SDATAOUT_ND = 2,   
    
  } 
  adau1978_sdata_sel_t;
  int32_t adau1978_sdata_sel_set(adau1978_ctx_t *ctx, adau1978_sdata_sel_t val);
  int32_t adau1978_sdata_sel_get(adau1978_ctx_t *ctx, adau1978_sdata_sel_t  *val);
  
  int32_t adau1978_cmap1_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_cmap1_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_cmap2_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_cmap2_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_cmap3_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_cmap3_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_cmap4_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_cmap4_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  
  int32_t adau1978_over_temp_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  typedef enum
  {
    ADAU1978_UNUSD_OUT_LOW = 0,
    ADAU1978_UNUSD_OUT_HIZ = 1,   
    ADAU1978_UNUSD_OUT_ND = 2,   
    
  } 
  adau1978_drv_hiz_t;
  int32_t adau1978_drv_hiz_set(adau1978_ctx_t *ctx, adau1978_drv_hiz_t val);
  int32_t adau1978_drv_hiz_get(adau1978_ctx_t *ctx, adau1978_drv_hiz_t *val);
  
  int32_t adau1978_ch1_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_ch1_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_ch2_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_ch2_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val);  
  int32_t adau1978_ch3_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_ch3_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_ch4_out_drv_en_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_ch4_out_drv_en_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  int32_t adau1978_padc_gain1_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_padc_gain1_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_padc_gain2_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_padc_gain2_get(adau1978_ctx_t *ctx, uint8_t *val);  
  int32_t adau1978_padc_gain3_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_padc_gain3_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_padc_gain4_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_padc_gain4_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  int32_t adau1978_dc_cal_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_dc_cal_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  int32_t adau1978_master_mute_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_master_mute_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  
  typedef enum
  {
    SUM_MODE_NORMAL_4_CH = 0,
    SUM_MODE_TWO_CH_SUMMING = 1,
    SUM_MODE_ONE_CH_SUMMING = 2,
    SUM_MODE_ND = 3,
    
  } 
  adau1978_sum_mode_t;
  int32_t adau1978_sum_mode_set(adau1978_ctx_t *ctx, adau1978_sum_mode_t val);
  int32_t adau1978_sum_mode_get(adau1978_ctx_t *ctx, adau1978_sum_mode_t  *val);
  
  int32_t adau1978_clip_status_ch1_get(adau1978_ctx_t *ctx, uint8_t  *val);
  int32_t adau1978_clip_status_ch2_get(adau1978_ctx_t *ctx, uint8_t  *val);
  int32_t adau1978_clip_status_ch3_get(adau1978_ctx_t *ctx, uint8_t  *val);
  int32_t adau1978_clip_status_ch4_get(adau1978_ctx_t *ctx, uint8_t  *val);
  
  int32_t adau1978_dc_sub_ch1_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_dc_sub_ch1_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_dc_sub_ch2_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_dc_sub_ch2_get(adau1978_ctx_t *ctx, uint8_t *val);  
  int32_t adau1978_dc_sub_ch3_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_dc_sub_ch3_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_dc_sub_ch4_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_dc_sub_ch4_get(adau1978_ctx_t *ctx, uint8_t *val);
  
  int32_t adau1978_hpf_en_ch1_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_hpf_en_ch1_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_hpf_en_ch2_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_hpf_en_ch2_get(adau1978_ctx_t *ctx, uint8_t *val);  
  int32_t adau1978_hpf_en_ch3_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_hpf_en_ch3_get(adau1978_ctx_t *ctx, uint8_t *val);
  int32_t adau1978_hpf_en_ch4_set(adau1978_ctx_t *ctx, uint8_t val);
  int32_t adau1978_hpf_en_ch4_get(adau1978_ctx_t *ctx, uint8_t *val);
  
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

#endif /*ADAU1978_REG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
