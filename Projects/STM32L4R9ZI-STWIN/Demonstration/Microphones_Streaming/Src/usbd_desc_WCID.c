/**
  ******************************************************************************
  * @file    usbd_desc_WCID.c
  * @author  SRA
  * @version v1.3.1
  * @date    17-Jul-2020
  * @brief   This file provides the USBD descriptors and string formatting method.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "SensorStreaming_WCID.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBD_VID                      0x0483
#define USBD_PID                      0x5743
#define USBD_LANGID_STRING            0x409
#define USBD_MANUFACTURER_STRING      "STMicroelectronics"
#define USBD_PRODUCT_FS_STRING        "STM Multi-Sensor Streaming Interface"
#define USBD_CONFIGURATION_FS_STRING  "STM Multi-Sensor Config"
#define USBD_INTERFACE_FS_STRING      "STM Multi-Sensor Interface"
#define USBD_OS_STRING      "STM Multi-Sensor Interface"

#define EP_IN_1_DESC_STRING "ADC_LOG"       /*0x81*/
#define EP_IN_2_DESC_STRING "FFT"      /*0x82*/
//#define EP_IN_3_DESC_STRING "LSM6DSM_Acc_FFT"   /*0x83*/
//#define EP_IN_4_DESC_STRING "Env"      /*0x84*/
//#define EP_IN_5_DESC_STRING "LSM303AGR_Mag"     /*0x85*/

const char *ep_in_desc_strings[5] = {EP_IN_1_DESC_STRING, EP_IN_2_DESC_STRING}; // EP_IN_3_DESC_STRING, EP_IN_4_DESC_STRING, EP_IN_5_DESC_STRING};


/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t *USBD_VCP_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_OSStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_OSCompIDDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_OSPropertiesFeatureDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_CustomInEpDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);


#ifdef USB_SUPPORT_USER_STRING_DESC
uint8_t *USBD_VCP_USRStringDesc(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t *length);
#endif /* USB_SUPPORT_USER_STRING_DESC */

/* Private variables ---------------------------------------------------------*/
USBD_DescriptorsTypeDef VCP_Desc =
{
  USBD_VCP_DeviceDescriptor,
  USBD_VCP_LangIDStrDescriptor,
  USBD_VCP_ManufacturerStrDescriptor,
  USBD_VCP_ProductStrDescriptor,
  USBD_VCP_SerialStrDescriptor,
  USBD_VCP_ConfigStrDescriptor,
  USBD_VCP_InterfaceStrDescriptor,
  USBD_VCP_OSStrDescriptor,
  USBD_VCP_OSCompIDDescriptor,
  USBD_VCP_OSPropertiesFeatureDescriptor,
  USBD_VCP_CustomInEpDescriptor
};

/* USB Standard Device Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
{
  0x12,                       /* bLength */
  USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
  0x00,                       /* bcdUSB */
  0x02,
  0x00,                       /* bDeviceClass */
  0x00,                       /* bDeviceSubClass */
  0x00,                       /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
  LOBYTE(USBD_VID),           /* idVendor */
  HIBYTE(USBD_VID),           /* idVendor */
  LOBYTE(USBD_PID),           /* idVendor */
  HIBYTE(USBD_PID),           /* idVendor */
  0x00,                       /* bcdDevice rel. 2.00 */
  0x02,
  USBD_IDX_MFC_STR,           /* Index of manufacturer string */
  USBD_IDX_PRODUCT_STR,       /* Index of product string */
  USBD_IDX_SERIAL_STR,        /* Index of serial number string */
  USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

//typedef struct
//{
//// Header
//uint8_t   bLength;
//uint8_t   bDescriptorType;
//uint16_t  bcdUSB;
//uint8_t   bDeviceClass;
//uint8_t   bDeviceSubClass;
//uint8_t   bDeviceProtocol;
//uint8_t   bMaxPacketSize0;
//uint16_t  idVendor;
//uint16_t  idProduct;
//uint16_t  bcdDevice;
//uint8_t   iManufacturer;
//uint8_t   iProduct;
//uint8_t   iSerialNumber;
//uint8_t   bNumConfigurations;
//} USBD_DeviceDesc_t;
//
//USBD_DeviceDesc_t USBD_DeviceDesc = { sizeof(USBD_DeviceDesc_t), USB_DESC_TYPE_DEVICE, 0x0002, 0x00 , 0x00, 0x00,\
//USB_MAX_EP0_SIZE, USBD_VID,USBD_PID, 0x0002, USBD_IDX_MFC_STR, USBD_IDX_PRODUCT_STR,\
//  USBD_IDX_SERIAL_STR, USBD_MAX_NUM_CONFIGURATION};

/* Microsoft OS String Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_OSStringDesc[] __ALIGN_END =
{
  0x12,                       /* bLength */
  USB_DESC_TYPE_STRING,       /* bDescriptorType */
  0x4D,
  0x00,
  0x53,
  0x00,
  /*Signature MSFT100*/
  0x46,
  0x00,
  0x54,
  0x00,
  0x31,
  0x00,
  0x30,
  0x00,
  0x30,
  0x00,
  /******end sig*****/
  0x07,                       /* Vendor Code */
  0x00,                       /*Padding*/
}; /* Microsoft OS String Descriptor */

/* USB Standard Device Descriptor */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
  USB_LEN_LANGID_STR_DESC,
  USB_DESC_TYPE_STRING,
  LOBYTE(USBD_LANGID_STRING),
  HIBYTE(USBD_LANGID_STRING),
};

uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] =
{
  USB_SIZ_STRING_SERIAL,
  USB_DESC_TYPE_STRING,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;


/**
  * @brief  USBD_StdDevReq
  *         Handle standard usb device requests
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
uint8_t OS_IDFeatureDescriptor[] =
{
  0x28,  /*LENGTH*/
  0x00,  /* "    */
  0x00,  /* "    */
  0x00,  /* "    */
  0x00,  /*Version 0.1*/
  0x01,
  0x04,  /*Compatibility ID Descriptor index (0x0004) */
  0x00,
  0x01,  /*N of sections*/
  0x00,  /* 7 bytes reserved */
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00, /*Interface number (0) */
  0x01,  /*reserved*/
  /* Compatible ID: "WINUSB\0\"*/
  0x57,
  0x49,
  0x4E,
  0x55,
  0x53,
  0x42,
  0x00,
  0x00,
  /* Sub Compatible ID: Unused*/
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  /*reserved*/
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};


typedef struct
{
// Header
  uint32_t  dwLength;
  uint16_t  bcdVersion;
  uint16_t  wIndex;
  uint16_t  wCount;
// Custom Property Section 1
  uint32_t  dwSize;
  uint32_t  dwPropertyDataType;
  uint16_t  wPropertyNameLength;
  uint16_t  bPropertyName[20];
  uint32_t  dwPropertyDataLength;
  uint16_t  bPropertyData[39];
} USBD_ExtPropertiesDescStruct;

USBD_ExtPropertiesDescStruct USBD_ExtPropertiesDesc = { sizeof(USBD_ExtPropertiesDesc), 0x0100, 0x0005, 0x0001, \
                                                        0x00000084, 0x00000001, \
                                                        0x0028,     {'D', 'e', 'v', 'i', 'c', 'e', 'I', 'n', 't', 'e', 'r', 'f', 'a', 'c', 'e', 'G', 'U', 'I', 'D', 0}, \
                                                        0x0000004E, {'{', 'F', '7', '0', '2', '4', '2', 'C', '7', '-', 'F', 'B', '2', '5', '-', '4', '4', '3', 'B', \
                                                                     '-', '9', 'E', '7', 'E', '-', 'A', '4', '2', '6', '0', 'F', '3', '7', '3', '9', '8', '2', '}', 0
                                                                    }
                                                      };



/* Private functions ---------------------------------------------------------*/
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);
static void Get_SerialNum(void);

/**
  * @brief  Returns the device descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_DeviceDesc);
  return (uint8_t *)USBD_DeviceDesc;
}

/**
  * @brief  Returns the LangID string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_LangIDDesc);
  return (uint8_t *)USBD_LangIDDesc;
}

/**
  * @brief  Returns the product string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  USBD_GetString((uint8_t *)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
  * @brief  Returns the manufacturer string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
  * @brief  Returns the serial number string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = USB_SIZ_STRING_SERIAL;

  /* Update the serial number string descriptor with the data from the unique ID*/
  Get_SerialNum();

  return (uint8_t *)USBD_StringSerial;
}

/**
  * @brief  Returns the configuration string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  USBD_GetString((uint8_t *)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  USBD_GetString((uint8_t *)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}


/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_OSStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_OSStringDesc);
  return (uint8_t *)USBD_OSStringDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_OSCompIDDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(OS_IDFeatureDescriptor);
  return (uint8_t *)OS_IDFeatureDescriptor;
}



/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_OSPropertiesFeatureDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  *length = sizeof(USBD_ExtPropertiesDescStruct);
  return (uint8_t *)&USBD_ExtPropertiesDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */

uint8_t *USBD_VCP_CustomInEpDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{


  uint8_t i = 0;
  uint8_t buff[256];
  uint16_t idx = 0;


  buff[0] = N_IN_ENDPOINTS;
  idx ++;

  for (i = 0; i < N_IN_ENDPOINTS; i++)
  {
    buff[idx] = strlen(ep_in_desc_strings[i]);
    idx ++;
  }

  for (i = 0; i < N_IN_ENDPOINTS; i++)
  {
    memcpy(&buff[idx], ep_in_desc_strings[i], strlen(ep_in_desc_strings[i]));
    idx += strlen(ep_in_desc_strings[i]);
  }


  *length = idx;
  return (uint8_t *)buff;
}





/**
  * @brief  Create the serial number string descriptor
  * @param  None
  * @retval None
  */
static void Get_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;

  deviceserial0 = (uint32_t)DEVICE_ID1;
  deviceserial1 = (uint32_t)DEVICE_ID2;
  deviceserial2 = (uint32_t)DEVICE_ID3;

  deviceserial0 += deviceserial2;

  if (deviceserial0 != 0)
  {
    IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8);
    IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4);
  }
}

/**
  * @brief  Convert Hex 32Bits value into char
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
  uint8_t idx = 0;

  for (idx = 0 ; idx < len ; idx ++)
  {
    if (((value >> 28)) < 0xA)
    {
      pbuf[ 2 * idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2 * idx] = (value >> 28) + 'A' - 10;
    }

    value = value << 4;

    pbuf[ 2 * idx + 1] = 0;
  }
}


