OPENOCD_CPU = stm32l4x
OPENOCD_SCRIPTS = ./scripts/

TEMPLATEROOT = .

# Build directory
BUILDDIR = ./build

# object files
OBJS = main.o system_stm32l4xx.o startup_stm32l476xx.o
OBJS += AudioBV_Manager.o MotionCP_Manager.o 
OBJS += MotionGR_Manager.o HWAdvanceFeatures.o MotionAR_Manager.o 
OBJS += stm32l4xx_hal_msp.o OTA.o stm32l4xx_it.o
OBJS += TargetPlatform.o usbd_conf.o sensor_service.o
OBJS += HTS221_Driver_HL.o usbd_cdc_interface.o HTS221_Driver.o
OBJS += LPS22HB_Driver_HL.o LPS22HB_Driver.o LSM303AGR_MAG_driver_HL.o
OBJS += LSM303AGR_ACC_driver_HL.o LSM303AGR_MAG_driver.o STC3115_Driver.o
OBJS += LSM303AGR_ACC_driver.o LSM6DSM_ACC_GYRO_driver_HL.o LSM6DSM_ACC_GYRO_driver.o
OBJS += usbd_desc.o SensorTile_gg.o
OBJS += SensorTile_BlueNRG.o SensorTile.o SensorTile_audio_in.o
OBJS += SensorTile_accelero.o SensorTile_gyro.o SensorTile_humidity.o
OBJS += stm32l4xx_hal.o SensorTile_pressure.o SensorTile_magneto.o
OBJS += SensorTile_temperature.o stm32l4xx_hal_adc.o stm32l4xx_hal_cortex.o
OBJS += stm32l4xx_hal_adc_ex.o stm32l4xx_hal_crc_ex.o stm32l4xx_hal_crc.o
OBJS += stm32l4xx_hal_dfsdm.o stm32l4xx_hal_dma.o stm32l4xx_hal_flash.o
OBJS += stm32l4xx_hal_gpio.o stm32l4xx_hal_flash_ex.o stm32l4xx_hal_i2c_ex.o
OBJS += stm32l4xx_hal_pwr.o stm32l4xx_hal_i2c.o stm32l4xx_hal_pwr_ex.o
OBJS += stm32l4xx_hal_rcc.o stm32l4xx_hal_rtc.o stm32l4xx_hal_rcc_ex.o
OBJS += stm32l4xx_hal_spi_ex.o stm32l4xx_hal_rtc_ex.o stm32l4xx_hal_spi.o
OBJS += stm32l4xx_hal_tim.o bluenrg_itf_template.o stm32l4xx_hal_tim_ex.o
OBJS += bluenrg_gap_aci.o bluenrg_gatt_aci.o bluenrg_hal_aci.o
OBJS += bluenrg_l2cap_aci.o bluenrg_updater_aci.o stm32l4xx_hal_uart.o
OBJS += osal.o stm32l4xx_hal_uart_ex.o stm32l4xx_hal_pcd.o stm32l4xx_ll_usb.o
OBJS += stm32l4xx_hal_pcd_ex.o hci.o gp_timer.o bluenrg_utils.o
OBJS += ble_list.o usbd_core.o usbd_ctlreq.o usbd_ioreq.o usbd_cdc.o
OBJS += MetaDataManager.o
OBJS += MotionFX_Manager.o

V = 0

# name of executable   
OUTNAME=bluenrg_sensortile
ELF=$(notdir $(OUTNAME)).elf                    
BIN=$(notdir $(OUTNAME)).bin

# Tool path
TOOLROOT=/l/arm2/devtools/bin

# Tools
CC_0 = @echo -n "Compiling $< "; $(CC_1)
CC_1 = $(TOOLROOT)/arm-none-eabi-gcc
CC = $(CC_$(V))

GAS_0 = @echo -n "Assembling $< "; $(GAS_1)
GAS_1 = $(TOOLROOT)/arm-none-eabi-gcc
GAS = $(GAS_$(V))


DD_0 = @echo " "; $(DD_1)
DD_1 = $(TOOLROOT)/arm-none-eabi-gcc
DD = $(DD_$(V))

LD_0 = @echo "Linking $@"; $(LD_1)
LD_1=$(TOOLROOT)/arm-none-eabi-gcc
LD = $(LD_$(V))

DL_0 = @echo -n "Downloading"; $(DL_1)
DL_1=$(TOOLROOT)/st-flash 
DL = $(DL_$(V))

OC_0 = @echo "Creating Bin Downloadable File"; $(OC_1)
OC_1=$(TOOLROOT)/arm-none-eabi-objcopy
OC = $(OC_$(V))

GP_0 = @echo " "; $(GP_1)
GP_1=grep
GP = $(GP_$(V))

RM_0 = @echo "Cleaning Directory"; $(RM_1)
RM_1=rm
RM = $(RM_$(V))

AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as
OBJCOPY=$(TOOLROOT)/arm-none-eabi-objcopy

# Library path
LIBROOT=../../../../../Drivers

# Code Paths
PERIPH=$(LIBROOT)/STM32L4xx_HAL_Driver
SYSTEM_FILE=$(LIBROOT)/CMSIS/Device/ST/STM32L4xx/Source/Templates
STARTUP_FILE=$(LIBROOT)/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc

# Include Paths
PROJECT_INCLUDE=../Inc
MIDDLEWARES = $(LIBROOT)/../Middlewares/ST
METADATAMANAGER = $(MIDDLEWARES)/STM32_MetaDataManager
MOTIONFX=$(MIDDLEWARES)/STM32_OSX_MotionFX_Library
BSP=$(LIBROOT)/BSP/Components/Common

# Search path for standard files

vpath %.c $(TEMPLATEROOT)
# Search path for perpheral library

vpath %.c ../Src
vpath %.c $(PERIPH)/Src
vpath %.c $(SYSTEM_FILE)
vpath %.s $(STARTUP_FILE)
vpath %.c $(LIBROOT)/BSP/Components/hts221
vpath %.c $(LIBROOT)/BSP/Components/
vpath %.c $(LIBROOT)/BSP/Components/Common
vpath %.c $(LIBROOT)/BSP/Components/hts221
vpath %.c $(LIBROOT)/BSP/Components/lis3mdl
vpath %.c $(LIBROOT)/BSP/Components/lps22hb
vpath %.c $(LIBROOT)/BSP/Components/lps25hb
vpath %.c $(LIBROOT)/BSP/Components/lsm303agr
vpath %.c $(LIBROOT)/BSP/Components/lsm6ds0
vpath %.c $(LIBROOT)/BSP/Components/lsm6ds3
vpath %.c $(LIBROOT)/BSP/Components/lsm6dsl
vpath %.c $(LIBROOT)/BSP/Components/lsm6dsm
vpath %.c $(LIBROOT)/BSP/Components/stc3115
vpath %.c $(LIBROOT)/BSP/SensorTile
vpath %.c $(MIDDLEWARES)/STM32_BlueNRG/Interface
vpath %.c $(MIDDLEWARES)/STM32_BlueNRG/SimpleBlueNRG_HCI/hci
vpath %.c $(MIDDLEWARES)/STM32_BlueNRG/SimpleBlueNRG_HCI/utils
vpath %.c $(MIDDLEWARES)/STM32_BlueNRG/Interface
vpath %.c $(MIDDLEWARES)/STM32_BlueNRG/SimpleBlueNRG_HCI/hci/controller
vpath %.c $(MIDDLEWARES)/STM32_USB_Device_Library/Core/Src
vpath %.c $(MIDDLEWARES)/STM32_USB_Device_Library/Class/CDC/Src
vpath %.c $(MIDDLEWARES)/STM32_MetaDataManager

#  Processor specific
LDSCRIPT = ./sensortile.ld
STARTUP = system_stm32l4xx.o startup_stm32l432xx.o

# Compilation Flags

FULLASSERT = -DUSE_FULL_ASSERT 

LDLIBS = -lm -lc
LDFLAGS+= -T$(LDSCRIPT) --specs=nosys.specs -mthumb -mcpu=cortex-m4 

# Compiler Switches
ASFLAGS = -ggdb 
CFLAGS  = -O3 -ggdb
CFLAGS+= -std=c99 -mthumb -mcpu=cortex-m4 $(FULLASSERT) 
CFLAGS+= -Wno-multichar 
# Compiler Include paths
CFLAGS+= -I ../Inc 
CFLAGS+= -I ../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include 
CFLAGS+= -I ../../../../../Drivers/STM32L4xx_HAL_Driver/Inc 
CFLAGS+= -I ../../../../../Drivers/BSP/SensorTile 
CFLAGS+= -I ../../../../../Drivers/BSP/Components/Common 
CFLAGS+= -I ../../../../../Drivers/CMSIS/Include 
CFLAGS+= -I ../../../../../Drivers/BSP/Components/lsm6dsm 
CFLAGS+= -I ../../../../../Drivers/BSP/Components/hts221 
CFLAGS+= -I ../../../../../Drivers/BSP/Components/lps22hb 
CFLAGS+= -I ../../../../../Drivers/BSP/Components/lsm303agr 
CFLAGS+= -I ../../../../../Drivers/BSP/Components/stc3115 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_MetaDataManager 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_OSX_MotionFX_Library/Inc 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_OSX_MotionAR_Library/Inc 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_OSX_MotionCP_Library/Inc 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_OSX_MotionGR_Library/Inc 
CFLAGS+= -I ../../../../../Middlewares/ST/STM32_OSX_BlueVoice_Library/Inc 

# Compiler defined variables 
CFLAGS+= -D_RTE_ -DSTM32L476xx 
CFLAGS+= -DSTM32L476xx 
CFLAGS+= -DUSE_HAL_DRIVER 
CFLAGS+= -DUSE_STM32L4XX_NUCLEO 
CFLAGS+= -DSTM32_SENSORTILE

# Build executable 

$(ELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# compile and generate dependency info

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 
	$(DD) -MM $(CFLAGS) $< > $*.d

%.o: %.s
	@echo "assembling the file"
	$(GAS) -c $(CFLAGS) $< -o $@ 
	$(DD) -MM $(CFLAGS) $< > $*.d

%.bin: %.elf
	$(OC) -O binary $< $@

clean:
	$(RM) -f $(OBJS) $(OBJS:.o=.d) $(ELF) startup_stm32f* $(CLEANOTHER) $(BIN) *.d *.log raw.bin TAGS

debug: $(ELF)
	arm-none-eabi-gdb $(ELF)

download: $(BIN)
	@echo "Merging the bootloader and the application binary images"
	cat ../Binary/STM32L476RG-SensorTile/BlueMS2_ST_BL.bin $(BIN) > raw.bin
	@echo "Downloading Bootloader + Application"
	@$(DL) write raw.bin 0x8000000 > st-flash.log 2>&1
	$(GP) -o "jolly" st-flash.log | sed 's/jolly/Success/'
	$(GP) -o "Couldn" st-flash.log | sed 's/Couldn/Application Fail/'

openocd:
	cp gdbinit .gdbinit
	openocd -s ${OPENOCD_SCRIPTS} -f board/sensortile.cfg 
etags:
	find ../Src -type f -iname "*.[chs]" | xargs etags --append
	find $(PERIPH)/Src -type f -iname "*.[chs]" | xargs etags --append
	find $(SYSTEM_FILE) -type f -iname "*.[chs]" | xargs etags --append
	find $(STARTUP_FILE) -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/hts221 -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/ -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/Common -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/hts221 -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lis3mdl -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lps22hb -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lps25hb -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lsm303agr -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lsm6ds0 -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lsm6ds3 -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lsm6dsl -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/lsm6dsm -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/Components/stc3115 -type f -iname "*.[chs]" | xargs etags --append
	find $(LIBROOT)/BSP/SensorTile -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_BlueNRG/Interface -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_BlueNRG/SimpleBlueNRG_HCI/hci -type f -iname "*.[chs]" | xargs etags --append 
	find $(MIDDLEWARES)/STM32_BlueNRG/SimpleBlueNRG_HCI/utils -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_BlueNRG/Interface -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_BlueNRG/SimpleBlueNRG_HCI/hci/controller -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_USB_Device_Library/Core/Src -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_USB_Device_Library/Class/CDC/Src -type f -iname "*.[chs]" | xargs etags --append
	find $(MIDDLEWARES)/STM32_MetaDataManager -type f -iname "*.[chs]" | xargs etags --append 
	find ../Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -type f -iname "*.[chs]" | xargs etags --append 
	find ../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/SensorTile -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/Components/Common -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/CMSIS/Include -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/Components/lsm6dsm -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/Components/hts221 -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/Components/lps22hb -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/Components/lsm303agr -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Drivers/BSP/Components/stc3115 -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_MetaDataManager -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_OSX_MotionFX_Library/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_OSX_MotionAR_Library/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_OSX_MotionCP_Library/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_OSX_MotionGR_Library/Inc -type f -iname "*.[chs]" | xargs etags --append
	find ../../../../../Middlewares/ST/STM32_OSX_BlueVoice_Library/Inc -type f -iname "*.[chs]" | xargs etags --append

all: $(ELF)

# pull in dependencies

-include $(OBJS:.o=.d)


