/**
******************************************************************************
* @file    AMic_Array_audio.h
* @author  SRA
* @brief   This file contains the common defines and functions prototypes for
*          AMic_Array_audio.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AMIC_ARRAY_AUDIO_H
#define AMIC_ARRAY_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "STWIN_conf.h"
#include "adau1978.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup AMIC_ARRAY
  * @{
  */
    
/** @addtogroup AMIC_ARRAY_AUDIO 
  * @{
  */
   
/** @defgroup AMIC_ARRAY_AUDIO_Exported_Variables
 * @{
 */
/* AMic_Array */ 
extern SAI_HandleTypeDef AMic_Array_SAI; 

#ifndef AUDIO_SHARED_TYPES
#define AUDIO_SHARED_TYPES

#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)

#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                   (((uint16_t)(A) & 0x00ff) << 8))

#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/**
 * @}
 */   

/** @defgroup AMIC_ARRAY_AUDIO_Exported_Types AMIC_ARRAY_AUDIO Exported Types
  * @{
  */

typedef struct {
	int32_t Z; 
	int32_t oldOut; 
	int32_t oldIn; 
}HP_FilterState_TypeDef;

typedef struct
{                                   
  uint32_t                    Device;                                           
  uint32_t                    SampleRate;                                         
  uint32_t                    BitsPerSample;                                          
  uint32_t                    ChannelsNbr;                                         
  uint32_t                    Volume;
}BSP_AUDIO_Init_t;

typedef struct
{
  uint32_t                    Instance;            /* Audio IN instance              */  
  uint32_t                    Device;              /* Audio IN device to be used     */ 
  uint32_t                    SampleRate;          /* Audio IN Sample rate           */
  uint32_t                    BitsPerSample;       /* Audio IN Sample resolution     */
  uint32_t                    ChannelsNbr;         /* Audio IN number of channel     */
  uint16_t                    *pBuff;              /* Audio IN record buffer         */
  uint8_t                     **pMultiBuff;        /* Audio IN multi-buffer          */
  uint32_t                    Size;                /* Audio IN record buffer size    */
  uint32_t                    Volume;              /* Audio IN volume                */
  uint32_t                    State;               /* Audio IN State                 */
  uint32_t                    IsMultiBuff;         /* Audio IN multi-buffer usage    */
  uint32_t                    IsMspCallbacksValid; /* Is Msp Callbacks registred     */
  HP_FilterState_TypeDef 	  HP_Filters[4];       /*!< HP filter state for each channel*/
  uint32_t DecimationFactor;
}AUDIO_IN_Ctx_t;

typedef struct
{
  /* Filter parameters */
  DFSDM_Filter_TypeDef   *FilterInstance;
  uint32_t               RegularTrigger;
  uint32_t               SincOrder;   
  uint32_t               Oversampling;
  /* Channel parameters */
  DFSDM_Channel_TypeDef *ChannelInstance;
  uint32_t              DigitalMicPins;
  uint32_t              DigitalMicType;
  uint32_t              Channel4Filter;
  uint32_t              ClockDivider;
  uint32_t              RightBitShift; 
  uint32_t              DmaMode;
  uint32_t              Multiplexer;
  uint32_t              Activation;
}MX_DFSDM_Config;  


/* AUDIO FREQUENCY */
#ifndef AUDIO_FREQUENCY_192K
#define AUDIO_FREQUENCY_192K     (uint32_t)192000U
#endif
#ifndef AUDIO_FREQUENCY_176K  
#define AUDIO_FREQUENCY_176K     (uint32_t)176400U
#endif
#ifndef AUDIO_FREQUENCY_96K
#define AUDIO_FREQUENCY_96K       (uint32_t)96000U
#endif
#ifndef AUDIO_FREQUENCY_88K
#define AUDIO_FREQUENCY_88K       (uint32_t)88200U
#endif
#ifndef AUDIO_FREQUENCY_48K
#define AUDIO_FREQUENCY_48K       (uint32_t)48000U
#endif
#ifndef AUDIO_FREQUENCY_44K  
#define AUDIO_FREQUENCY_44K       (uint32_t)44100U
#endif
#ifndef AUDIO_FREQUENCY_32K
#define AUDIO_FREQUENCY_32K       (uint32_t)32000U
#endif
#ifndef AUDIO_FREQUENCY_22K
#define AUDIO_FREQUENCY_22K       (uint32_t)22050U
#endif
#ifndef AUDIO_FREQUENCY_16K
#define AUDIO_FREQUENCY_16K       (uint32_t)16000U
#endif
#ifndef AUDIO_FREQUENCY_11K
#define AUDIO_FREQUENCY_11K       (uint32_t)11025U
#endif
#ifndef AUDIO_FREQUENCY_8K
#define AUDIO_FREQUENCY_8K         (uint32_t)8000U 
#endif
   
/* AUDIO RESOLUTION */   
#ifndef AUDIO_RESOLUTION_16b
#define AUDIO_RESOLUTION_16b                16U
#endif
#ifndef AUDIO_RESOLUTION_24b
#define AUDIO_RESOLUTION_24b                24U
#endif
#ifndef AUDIO_RESOLUTION_32b
#define AUDIO_RESOLUTION_32b                32U
#endif

#ifndef AUDIO_IN_CHANNELS
#define AUDIO_IN_CHANNELS 2
#endif

#ifndef AUDIO_IN_SAMPLING_FREQUENCY
#define AUDIO_IN_SAMPLING_FREQUENCY 16000
#endif

#ifndef AUDIO_VOLUME_INPUT
#define AUDIO_VOLUME_INPUT              64U
#endif

#ifndef BSP_AUDIO_INSTANCE
#define BSP_AUDIO_INSTANCE 1U
#endif

#ifndef BSP_AUDIO_IN_IT_PRIORITY
#define BSP_AUDIO_IN_IT_PRIORITY    6U
#endif

/* Buffer size defines*/
#define CHANNEL_DEMUX_MASK                    	0x55U
    
#define MAX_CH_NUMBER           (4)
#define MAX_FS                  (192000)
#define MAX_SAMPLES_PER_CH      ((MAX_FS/1000)*2)	

#define MAX_MIC_FREQ                 	  3072  /*KHz*/
#define MAX_AUDIO_IN_CHANNEL_NBR_PER_IF   2 
#define MAX_AUDIO_IN_CHANNEL_NBR_TOTAL    4 

#ifndef PDM_FREQ_16K
#define PDM_FREQ_16K 1280
#endif

/*Number of millisecond of audio at each DMA interrupt*/
#ifndef N_MS_PER_INTERRUPT
#define N_MS_PER_INTERRUPT               (1U)
#endif

/* Default Audio IN internal buffer size */   
#define DEFAULT_AUDIO_IN_BUFFER_SIZE        (AUDIO_IN_SAMPLING_FREQUENCY/1000)*2*N_MS_PER_INTERRUPT    
    
/*BSP internal buffer size in half words (16 bits)*/  
#define PDM_INTERNAL_BUFFER_SIZE_I2S          ((MAX_MIC_FREQ / 8) * MAX_AUDIO_IN_CHANNEL_NBR_PER_IF * N_MS_PER_INTERRUPT)
#if MAX_AUDIO_IN_CHANNEL_NBR_TOTAL > 2
#define PDM_INTERNAL_BUFFER_SIZE_SPI          ((MAX_MIC_FREQ / 8) * MAX_AUDIO_IN_CHANNEL_NBR_PER_IF * N_MS_PER_INTERRUPT)
#else
#define PDM_INTERNAL_BUFFER_SIZE_SPI          1
#endif

/* Audio In states */
#define AUDIO_IN_STATE_RESET               0U
#define AUDIO_IN_STATE_RECORDING           1U
#define AUDIO_IN_STATE_STOP                2U
#define AUDIO_IN_STATE_PAUSE               3U

/* Audio In instances available in STWIN */
#define AUDIO_IN_INSTANCES_NBR 1U
/* AMic_Array Instance */
#define AMIC_ARRAY_AUDIO_IN_INSTANCE    0U 

#endif /* AUDIO_SHARED_TYPES */

typedef struct
{
  uint32_t AudioFrequency;
  uint32_t AudioMode;
  uint32_t DataSize;
  uint32_t Mckdiv;
  uint32_t MonoStereoMode;
  uint32_t ClockStrobing;
  uint32_t Synchro;
  uint32_t OutputDrive;
  uint32_t SynchroExt;
  uint32_t FrameLength;
  uint32_t ActiveFrameLength;
  uint32_t SlotActive; 
  uint32_t SlotNumber;
}MX_SAI_Config;



/**
  * @}
  */ 

/** @defgroup AMIC_ARRAY_AUDIO_Exported_Constants AMIC_ARRAY_AUDIO Exported Constants
  * @{
  */
/*------------------------------------------------------------------------------
                        AMIC_ARRAY defines parameters
------------------------------------------------------------------------------*/ 
/* SAI peripheral configuration defines */  
#define AMIC_ARRAY_INSTANCE                  SAI1_Block_A
#define AMIC_ARRAY_CLK_ENABLE()             __HAL_RCC_SAI1_CLK_ENABLE()
#define AMIC_ARRAY_CLK_DISABLE()             __SAI1_CLK_DISABLE()  
#define AMIC_ARRAY_SCK_SD_WS_MCK_AF         GPIO_AF13_SAI1
#define AMIC_ARRAY_SD_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define AMIC_ARRAY_SCK_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define AMIC_ARRAY_MCK_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define AMIC_ARRAY_FS_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define AMIC_ARRAY_FS_PIN                   GPIO_PIN_4
#define AMIC_ARRAY_SCK_PIN                  GPIO_PIN_5
#define AMIC_ARRAY_SD_PIN                   GPIO_PIN_6
#define AMIC_ARRAY_MCLK_PIN                  GPIO_PIN_2
#define AMIC_ARRAY_MCLK_SCK_SD_FS_GPIO_PORT            GPIOE  
/* SAI DMA Stream tx definitions */
#define AMIC_ARRAY_DMAx_CLK_ENABLE()        __HAL_RCC_DMA2_CLK_ENABLE()
#define AMIC_ARRAY_DMAx_INSTANCE            DMA2_Channel3
#define AMIC_ARRAY_DMAx_REQUEST             DMA_REQUEST_SAI1_A
#define AMIC_ARRAY_DMAx_IRQ                 DMA2_Channel3_IRQn
#define AMIC_ARRAY_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define AMIC_ARRAY_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD 
#define AMIC_ARRAY_IRQHandler                   DMA2_Channel3_IRQHandler


/*************************************************************************/

/* AMic_Array definition */
#define AUDIO_IN_ANALOG_MIC1      0x01U
#define AUDIO_IN_ANALOG_MIC2      0x02U
#define AUDIO_IN_ANALOG_MIC3      0x04U
#define AUDIO_IN_ANALOG_MIC4      0x08U
#define AUDIO_IN_ANALOG_MIC_LAST  AUDIO_IN_ANALOG_MIC4
#define AUDIO_IN_ANALOG_MIC       (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3 | AUDIO_IN_ANALOG_MIC4)

/* Audio In devices */ 
#define AMIC_ARRAY AUDIO_IN_ANALOG_MIC
#define AMIC_ARRAY_AUDIO_IN_IT_PRIORITY 6

/**
  * @}
  */
   
/** @defgroup AMIC_ARRAY_AUDIO_Exported_Macros AMIC_ARRAY_AUDIO Exported Macros
  * @{
  */
#define POS_VAL(VAL)                  (POSITION_VAL(VAL) - 4)
#define VOLUME_OUT_CONVERT(Volume)    (((Volume) > 100)? 63:((uint8_t)(((Volume) * 63) / 100)))
#define VOLUME_IN_CONVERT(Volume)     (((Volume) >= 100)? 239:((uint8_t)(((Volume) * 239) / 100)))   
/**
  * @}
  */ 

/** @addtogroup AMIC_ARRAY_AUDIO_Exported_Variables
  * @{
  */

/* Recording context */
extern AUDIO_IN_Ctx_t                         MicArrayCouponCtx[];

/**
  * @}
  */

/** @defgroup AMIC_ARRAY_AUDIO_IN_Exported_Functions AMIC_ARRAY_AUDIO_IN Exported Functions
  * @{
  */
int32_t AMIC_ARRAY_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t* AudioInit);    
int32_t AMIC_ARRAY_AUDIO_IN_DeInit(uint32_t Instance);
int32_t AMIC_ARRAY_AUDIO_IN_Record(uint32_t Instance, uint8_t* pBuf, uint32_t NbrOfBytes);
int32_t AMIC_ARRAY_AUDIO_IN_Stop(uint32_t Instance);
int32_t AMIC_ARRAY_AUDIO_IN_Pause(uint32_t Instance);
int32_t AMIC_ARRAY_AUDIO_IN_Resume(uint32_t Instance);

int32_t AMIC_ARRAY_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes);
int32_t AMIC_ARRAY_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device);
int32_t AMIC_ARRAY_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device);
int32_t AMIC_ARRAY_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device);

int32_t AMIC_ARRAY_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device);
int32_t AMIC_ARRAY_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device);
int32_t AMIC_ARRAY_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate);
int32_t AMIC_ARRAY_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate);                 
int32_t AMIC_ARRAY_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample);
int32_t AMIC_ARRAY_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample);                
int32_t AMIC_ARRAY_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr);
int32_t AMIC_ARRAY_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr);
int32_t AMIC_ARRAY_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume);
int32_t AMIC_ARRAY_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume);
int32_t AMIC_ARRAY_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State);


/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void AMIC_ARRAY_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance);
void AMIC_ARRAY_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void AMIC_ARRAY_AUDIO_IN_Error_CallBack(uint32_t Instance);


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

#endif /* AMIC_ARRAY_AUDIO_H */

