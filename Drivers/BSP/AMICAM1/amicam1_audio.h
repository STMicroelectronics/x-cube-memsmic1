/**
******************************************************************************
* @file    amicam1_audio.h
* @author  SRA
* 
* 
* @brief   This file contains the common defines and functions prototypes for
*          amicam1_audio.c driver.
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
#ifndef AMICAM1_AUDIO_H
#define AMICAM1_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "amicam1_conf.h"
#include "ad1974.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup AMICAM1
  * @{
  */
    
/** @addtogroup AMICAM1_AUDIO 
  * @{
  */
   
/** @defgroup AMICAM1_AUDIO_Exported_Variables
 * @{
 */
/* amicam1 */ 
extern SAI_HandleTypeDef hAudioInSai;
extern ADC_HandleTypeDef hAudioInADC;
extern DFSDM_Filter_HandleTypeDef   hAudioInDfsdmFilterHandle; 
extern DMA_HandleTypeDef ADC_DmaHandle;

#ifndef AUDIO_SHARED_TYPES
#define AUDIO_SHARED_TYPES

#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)

#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                   (((uint16_t)(A) & 0x00ff) << 8))

#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/**
 * @}
 */   

/** @defgroup AMICAM1_AUDIO_Exported_Types AMICAM1_AUDIO Exported Types
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
  FunctionalState DmaMode;   
  uint32_t Trigger;
  /* Channel parameters */
  uint32_t Multiplexer;
  uint32_t Selection;
  uint32_t Divider;  
  uint32_t Oversampling;   
}
MX_DFSDM_Config;  

typedef struct
{
uint32_t AudioMode;
  uint32_t Synchro;
  uint32_t SynchroExt;
  uint32_t OutputDrive;
  uint32_t NoDivider;
  uint32_t FIFOThreshold;
  uint32_t AudioFrequency;
  uint32_t Mckdiv;
#if defined(STM32L4R5xx) || defined(STM32L4R7xx) || defined(STM32L4R9xx) || defined(STM32L4S5xx) || defined(STM32L4S7xx) || defined(STM32L4S9xx)
  uint32_t MckOverSampling;
#endif /* STM32L4R5xx || STM32L4R7xx || STM32L4R9xx || STM32L4S5xx || STM32L4S7xx || STM32L4S9xx */
  uint32_t MonoStereoMode;
  uint32_t CompandingMode;
  uint32_t TriState;
#if defined(STM32L4R5xx) || defined(STM32L4R7xx) || defined(STM32L4R9xx) || defined(STM32L4S5xx) || defined(STM32L4S7xx) || defined(STM32L4S9xx)
  SAI_PdmInitTypeDef PdmInit;   /*!< Specifies the PDM configuration. */
#endif /* STM32L4R5xx || STM32L4R7xx || STM32L4R9xx || STM32L4S5xx || STM32L4S7xx || STM32L4S9xx */
  uint32_t Protocol;
  uint32_t DataSize;
  uint32_t FirstBit;
  uint32_t ClockStrobing;
}MX_SAI_Config;


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

/*Number of millisecond of audio at each DMA interrupt*/
#ifndef N_MS_PER_INTERRUPT
#define N_MS_PER_INTERRUPT               (1U)
#endif

/* Default Audio IN internal buffer size */   
#define DEFAULT_AUDIO_IN_BUFFER_SIZE        (AUDIO_IN_SAMPLING_FREQUENCY/1000)*2*N_MS_PER_INTERRUPT    

/* Audio In states */
#define AUDIO_IN_STATE_RESET               0U
#define AUDIO_IN_STATE_RECORDING           1U
#define AUDIO_IN_STATE_STOP                2U
#define AUDIO_IN_STATE_PAUSE               3U

/* Audio In instances available in STWIN */
#define AUDIO_IN_INSTANCES_NBR 2U
#define AUDIO_IN_INSTANCE_ADC_EXT 0U
#define AUDIO_IN_INSTANCE_ADC_INT 1U

#endif /* AUDIO_SHARED_TYPES */





/**
  * @}
  */ 

/** @defgroup AMICAM1_AUDIO_Exported_Constants AMICAM1_AUDIO Exported Constants
  * @{
  */
/*------------------------------------------------------------------------------
                        AMICAM1 defines parameters
------------------------------------------------------------------------------*/ 
/* SAI peripheral configuration defines */  
#define AMICAM1_SAI_INSTANCE                  SAI2_Block_B
#define AMICAM1_SAI_CLK_ENABLE()             __HAL_RCC_SAI2_CLK_ENABLE()
#define AMICAM1_SAI_CLK_DISABLE()             __SAI2_CLK_DISABLE()  

#define AMICAM1_SD_AF                     GPIO_AF13_SAI2
#define AMICAM1_SCK_AF                    GPIO_AF13_SAI2
#define AMICAM1_MCLK_AF                   GPIO_AF13_SAI2
#define AMICAM1_FS_AF                     GPIO_AF13_SAI2

#define AMICAM1_SD_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define AMICAM1_SCK_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define AMICAM1_MCLK_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define AMICAM1_FS_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()

#define AMICAM1_SD_PIN                   GPIO_PIN_12
#define AMICAM1_SCK_PIN                  GPIO_PIN_10
#define AMICAM1_MCLK_PIN                 GPIO_PIN_7
#define AMICAM1_FS_PIN                   GPIO_PIN_15

#define AMICAM1_SD_GPIO_PORT             GPIOC  
#define AMICAM1_SCK_GPIO_PORT            GPIOC  
#define AMICAM1_MCLK_GPIO_PORT           GPIOC  
#define AMICAM1_FS_GPIO_PORT             GPIOA  
/* SAI DMA Stream tx definitions */
#define AMICAM1_DMAx_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define AMICAM1_DMAx_INSTANCE            DMA1_Channel7
#ifdef STM32L4R5xx
#define AMICAM1_DMAx_REQUEST             DMA_REQUEST_SAI2_B
#else
#define AMICAM1_DMAx_REQUEST             DMA_REQUEST_1
#endif
#define AMICAM1_DMAx_IRQ                 DMA1_Channel7_IRQn
#define AMICAM1_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define AMICAM1_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD 
#define AMICAM1_IRQHandler               DMA1_Channel7_IRQHandler


/* SAI peripheral configuration defines */  
#define AMICAM1_ADC_INSTANCE                  ADC1
#define AMICAM1_ADC_CLK_ENABLE()             __HAL_RCC_ADC_CLK_ENABLE()
#define AMICAM1_ADC_CLK_DISABLE()            __HAL_RCC_ADC_CLK_DISABLE() 

#define AMICAM1_ADC_PIN_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define AMICAM1_ADC_PIN_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISBLE()
#define AMICAM1_ADC_PIN                   	GPIO_PIN_0
#define AMICAM1_ADC_GPIO_PORT               GPIOA

#define AMICAM1_ADC_DMAx_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define AMICAM1_ADC_DMAx_INSTANCE            DMA1_Channel1
#ifdef STM32L4R5xx
#define AMICAM1_ADC_DMAx_REQUEST             DMA_REQUEST_ADC1
#define AMICAM1_ADC_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD
#else
#define AMICAM1_ADC_DMAx_REQUEST             DMA_REQUEST_0
#define AMICAM1_ADC_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_WORD
#endif
#define AMICAM1_ADC_DMAx_IRQ                 DMA1_Channel1_IRQn
#define AMICAM1_ADC_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define AMICAM1_ADC_IRQHandler               DMA1_Channel1_IRQHandler

#define AMICAM1_DFSDM_FILTER_INSTANCE        DFSDM1_Filter0
#define AMICAM1_DFSDM_CLK_ENABLE()             __HAL_RCC_DFSDM_CLK_ENABLE()
#define AMICAM1_DFSDM_CLK_DISABLE()            __HAL_RCC_DFSDM_CLK_DISABLE()

#define AMICAM1_DFSDM_DMAx_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define AMICAM1_DFSDM_DMAx_INSTANCE            DMA1_Channel4
#ifdef STM32L4R5xx
#define AMICAM1_DFSDM_DMAx_REQUEST             DMA_REQUEST_DFSDM1_FLT0
#else
#define AMICAM1_DFSDM_DMAx_REQUEST             DMA_REQUEST_0
#endif
#define AMICAM1_DFSDM_DMAx_IRQ                 DMA1_Channel4_IRQn
#define AMICAM1_DFSDM_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_WORD
#define AMICAM1_DFSDM_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_WORD
#define AMICAM1_DFSDM_IRQHandler               DMA1_Channel4_IRQHandler


/*************************************************************************/

/* amicam1 definition */
#define AUDIO_IN_ANALOG_MIC1      0x01U
#define AUDIO_IN_ANALOG_MIC2      0x02U
#define AUDIO_IN_ANALOG_MIC3      0x04U
#define AUDIO_IN_ANALOG_MIC4      0x08U
#define AUDIO_IN_ANALOG_MIC5      0x10U

#define AUDIO_IN_ANALOG_MIC       (AUDIO_IN_ANALOG_MIC1 | AUDIO_IN_ANALOG_MIC2 | AUDIO_IN_ANALOG_MIC3 | AUDIO_IN_ANALOG_MIC4)

/* Audio In devices */ 
#ifndef AMICAM1_AUDIO_IN_IT_PRIORITY
#define AMICAM1_AUDIO_IN_IT_PRIORITY 6
#endif


/**
  * @}
  */
   
/** @defgroup AMICAM1_AUDIO_Exported_Macros AMICAM1_AUDIO Exported Macros
  * @{
  */
#define POS_VAL(VAL)                  (POSITION_VAL(VAL) - 4)
#define VOLUME_OUT_CONVERT(Volume)    (((Volume) > 100)? 63:((uint8_t)(((Volume) * 63) / 100)))
#define VOLUME_IN_CONVERT(Volume)     (((Volume) >= 100)? 239:((uint8_t)(((Volume) * 239) / 100)))   
/**
  * @}
  */ 

/** @addtogroup AMICAM1_AUDIO_Exported_Variables
  * @{
  */

/* Recording context */
extern AUDIO_IN_Ctx_t                         amicam1_ctx[AUDIO_IN_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup AMICAM1_AUDIO_IN_Exported_Functions AMICAM1_AUDIO_IN Exported Functions
  * @{
  */
int32_t AMICAM1_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t* AudioInit);    
int32_t AMICAM1_AUDIO_IN_DeInit(uint32_t Instance);
int32_t AMICAM1_AUDIO_IN_Record(uint32_t Instance, uint8_t* pBuf, uint32_t NbrOfBytes);
int32_t AMICAM1_AUDIO_IN_Stop(uint32_t Instance);
int32_t AMICAM1_AUDIO_IN_Pause(uint32_t Instance);
int32_t AMICAM1_AUDIO_IN_Resume(uint32_t Instance);

int32_t AMICAM1_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes);
int32_t AMICAM1_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device);
int32_t AMICAM1_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device);
int32_t AMICAM1_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device);

int32_t AMICAM1_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device);
int32_t AMICAM1_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device);
int32_t AMICAM1_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate);
int32_t AMICAM1_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate);                 
int32_t AMICAM1_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample);
int32_t AMICAM1_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample);                
int32_t AMICAM1_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr);
int32_t AMICAM1_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr);
int32_t AMICAM1_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume);
int32_t AMICAM1_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume);
int32_t AMICAM1_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State);


/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void AMICAM1_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance);
void AMICAM1_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void AMICAM1_AUDIO_IN_Error_CallBack(uint32_t Instance);


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

#endif /* AMICAM1_AUDIO_H */


