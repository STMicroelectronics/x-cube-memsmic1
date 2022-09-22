@color 0B
@echo off

REM Generate all CMSIS files 
REM Active perl must be installed

set scriptPATH=%CD%\..\..\..\..\..\..\..\__INTERNAL__tools\tools\_CmsisDeviceGenerator\

if not exist "%scriptPATH%"  (
	echo Input directory does not exist!
	pause
	exit
)

cd %scriptPATH%

DeviceGeneration.pl  --target STM32WBxx -replace

REM Suppress STM32WB25xx and STM32WB20xx  Devices as not used for the moment.
REM STM32WB25xx and STM32WB20xx corresponding to old devices STM32WB35xx and STM32WB30xx abandoned on W25 (2020)
del ..\..\..\Firmware\Drivers\CMSIS\Device\ST\STM32WBxx\Include\stm32wb20xx.h
del ..\..\..\Firmware\Drivers\CMSIS\Device\ST\STM32WBxx\Include\stm32wb25xx.h
pause
:EOF
