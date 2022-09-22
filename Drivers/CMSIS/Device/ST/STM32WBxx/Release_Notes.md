---
pagetitle: Release Notes for STM32WBxx CMSIS
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for
# <mark>STM32WBxx CMSIS</mark>
Copyright &copy; 2019 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

This driver provides the CMSIS device for the stm32wbxx products. This covers 

- STM32WB55xx devices
- STM32WB5Mxx devices
- STM32WB50xx devices
- STM32WB35xx devices
- STM32WB30xx devices
- STM32WB15xx devices
- STM32WB10xx devices
- STM32WB1Mxx devices

This driver is composed of the descriptions of the registers under "Include" directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32wbxx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.
- Linker files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.

# Specific consideration for available FLASH size inside linker file

The available flash size depends on the wireless binary used inside the STM32WB device. 

The linker files templates for IAR, KEIL and GCC provide example of implementation which can be tuned. 

You can refer to the below chapters to optimize the usage of the flash on your device.

## STM32WB55xx, STM32WB50xx and STM32WB5M

The default linker file provided in "/Drivers/CMSIS/Device/ST/STM32WBxx/Source/Templates" allows the application to use 512KB of flash.

The maximum flash memory that can be used by the application is up to the Secure Flash Start Address (SFSA) that can be read from the option byte.

The \__ICFEDIT_region_ROM_end__ in the linker can be modified with a value up to : (0x08000000 + (SFSA << 12)) - 1.

Example:

- When the SFSA option byte is set to 0xA0, the maximum value to be used for \__ICFEDIT_region_ROM_end is 0x0809FFFF which is 640KB of flash.

Note:

- The SFSA option byte can only be set by the CPU2. The user cannot modify that value.

## STM32WB35xx and STM32WB30xx
The default linker file provided in "/Drivers/CMSIS/Device/ST/STM32WBxx/Source/Templates allows the application to use 120KB of flash.

The maximum flash memory that can be used by the application is up to the Secure Flash Start Address (SFSA) that can be read from the option byte.

The \__ICFEDIT_region_ROM_end__ in the linker can be modified with a value up to : (0x08000000 + (SFSA << 12)) - 1.

Example:

- When the SFSA option byte is set to 0x32, the maximum value to be used for \__ICFEDIT_region_ROM_end is 0x08031FFF – which is 200KB of flash

Note: 

- The SFSA option byte can only be set by the CPU2. The user cannot modify that value.

## STM32WB15xx, STM32WB10xx and STM32WB1Mxx
The default linker file provided in "/Drivers/CMSIS/Device/ST/STM32WBxx/Source/Templates allows the application to use 110KB of flash.

The maximum flash memory that can be used by the application is up to the Secure Flash Start Address (SFSA) that can be read from the option byte.

The \__ICFEDIT_region_ROM_end__ in the linker can be modified with a value up to : (0x08000000 + (SFSA << 11)) - 1
  considering minimum CPU secure area size is one sector (2kBytes).

Example:

- When the SFSA option byte is set to 0x37, the maximum value to be used for __ICFEDIT_region_ROM_end is 0x0801B800 – which is 110kB of flash

Note: 

- The SFSA option byte can only be set by the CPU2. The user cannot modify that value.


:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section12" checked aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V1.11.0 / 08-June-2022</label>
<div>

## Main Changes

- Add support for the upcoming STM32WB1M device

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx, STM32WB30xx, STM32WB15xx, STM32WB10xx and STM32WB1Mxx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section11" aria-hidden="true">
<label for="collapse-section11" aria-hidden="true">V1.10.1 / 25-March-2022</label>
<div>

## Main Changes

- Align ADC registers naming on Reference Manual (For STM32WB15xx & STM32WB10xx)

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx, STM32WB30xx, STM32WB15xx and STM32WB10xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section10" aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">V1.10.0 / 12-November-2021</label>
<div>

## Main Changes

- Update CMSIS devices drivers for all value lines not supporting SMPS
- All source files and templates: update disclaimer to add reference to the new license agreement
- Correct English spelling typos and remove non UTF-8 characters in comments

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx, STM32WB30xx, STM32WB15xx and STM32WB10xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V1.9.0 / 24-June-2021</label>
<div>

## Main Changes

- Add atomic register access services:
  - 32-bit register access: ATOMIC_SET_BIT(), ATOMIC_CLEAR_BIT(), ATOMIC_MODIFY_REG()
  - 16-bit register access: ATOMIC_SETH_BIT(), ATOMIC_CLEARH_BIT(), ATOMIC_MODIFYH_BIT()
- Update linker files templates for all IDE (.icf/.sct/.ld) 
  - RAM start address has been updated from 0x20000000/0x20000004 to 0x20000008 to be aligned on a 8 byte boundary, and be compatible with ARM Compiler 6 (Keil MDK-ARM)
- Add define LSI_STARTUP_TIME used in default IWDG timeout calculation (HAL_IWDG_DEFAULT_TIMEOUT)
- Add define FLASH_ECCR_CPUID bits for new macro __HAL_FLASH_ECC_CPUID() macro


## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx, STM32WB30xx, STM32WB15xx and STM32WB10xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V1.8.0 / 09-February-2021</label>
<div>

## Main Changes

### Add support for STM32WB15xx and STM32WB10xx

- Change how to adapt VTOR for user

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx, STM32WB30xx, STM32WB15xx and STM32WB10xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7"  aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V1.7.0 / 30-October-2020</label>
<div>

## Main Changes

### Maintenance release

Maintenance release for **STM32WBxx** devices (stm32wb55xx, stm32wb50xx, stm32wb35xx and stm32wb30xx devices)

  Fixed bugs headline
  ----------------------------------------------------------------------------
  [STM32WB35] Remove EXTI_C2IMR2_IM43 and EXTI_IMR2_IM43
  [STM32WB50] Remove RCC_CR_HSEBYP
  [STM32WB55] Remove RCC_CR_HSEBYP
  [STM32WB5M] Remove RCC_CR_HSEBYP

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx and STM32WB30xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6"  aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">v1.6.0 / 05-June-2020</label>
<div>

## Main Changes

### Maintenance release

Maintenance release for **STM32WBxx** devices (stm32wb55xx, stm32wb50xx, stm32wb35xx and stm32wb30xx devices)

  Fixed bugs headline
  ----------------------------------------------------------------------------
  [All devices] Correct DMAMUX_CxCR_DMAREQ_ID_Msk
  [All devices] Remove DMAMUX_CxCR_DMAREQ_ID_[6-7]
  [All devices] Call SystemInit first in startup/Reset_Handler, so GCC code is similar to IAR/Keil
  [STM32WB50xx and STM32WB30xx] Remove OR register from LPTIM_TypeDef
  [STM32WB50xx and STM32WB30xx] Remove DMAMUX_CSR_SOF[7-13] and DMAMUX_CFR_SOF[7-13]
  [STM32WB50xx and STM32WB30xx] Remove EXTI_RTSR1_RTxx_Pos [20, 21 and 31]
  [STM32WB50xx and STM32WB30xx] Remove TIM2_OR_TI4_RMP, TIM2_OR_ITR1_RMP and LPTIM_OR_OR
  
## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx and STM32WB30xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V1.4.0 / 12-February-2020</label>
<div>

## Main Changes

### Introduction of STM32WB35xx, STM32WB30xx and STM32WB5Mxx product

  This release introduce the support of STM32WB5Mxx, STM32WB35xx product and its value line STM32WB30xx.
  
  Added features:
  
  - Templates/system_stm32wbxx.c contains the initialization code referred as SystemInit.
  - Startup files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.
  - Linker files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.
  - The product STM32WB5Mxx is supported by enabling inside your project the define "STM32WB5Mxx".
  - The product STM32WB35xx is supported by enabling inside your project the define "STM32WB35xx".
  - The product STM32WB30xx is supported by enabling inside your project the define "STM32WB30xx".
  
## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7

## Supported Devices and boards

- STM32WB55xx, STM32WB5Mxx, STM32WB50xx, STM32WB35xx and STM32WB30xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4"  aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V1.3.0 / 11-September-2019</label>
<div>

## Main Changes

Maintenance release for **STM32WBxx** devices (stm32wb55xx and stm32wb50xx devices)

  Fixed bugs headline
  ----------------------------------------------------------------------------
  Remove IS_TIM_SYNCHRO_INSTANCE macro from CMSIS
  Move FLASH_SIZE define from hal flash. h to cmsis device file
  Correct size of .bin files generated by SW4STM32
  Remove RCC_PLLSAI_SUPPORT for STM32WB50

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7

## Supported Devices and boards

- STM32WB55xx, STM32WB50xx devices

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.2.0 / 26-June-2019</label>
<div>

## Main Changes

### Introduction of STM32WB50xx device

First release for STM32WBxx CMSIS introducing **stm32wb50xx** devices.

## Contents

CMSIS devices files for stm32wb55xx, stm32wb50xx devices.

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7

## Supported Devices and boards

- STM32WB55xx and STM32WB50xx devices

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.1.0 / 05-April-2019</label>
<div>			

## Main Changes

### Maintenance release

Maintenance release for **STM32WBxx** devices (stm32wb55xx devices)

  Headline
  --------
  Correct GCC linker file: Set available size of RAM1 to 192K - 4 instead of 191K.
  Set FLASH_ACR_LATENCY_x as uint32_t (UL instead of U).

  : Fixed bugs list

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 06-February-2019</label>
<div>			

## Main Changes

### First release

Add support of STM32WB55xx.
</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32WBxx, visit: [[www.st.com/stm32wb](http://www.st.com/stm32wb)]

*This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.*
</footer>
