V = 0

# name of executable

ELF=$(notdir $(CURDIR)).elf                    
BIN=$(notdir $(CURDIR)).bin

# Tool path
TOOLROOT=/l/arm2/devtools/bin/

# Library path
LIBROOT= ../../../../../Drivers
MIDDLEWARES = ../../../../../Middlewares
# /l/arm2/STM32F3-Discovery_FW_V1.1.0

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

# Code Paths

DEVICE=$(LIBROOT)/CMSIS/Device/ST/STM32L4xx/

CORE=$(LIBROOT)/CMSIS/Include
PERIPH=$(LIBROOT)/STM32L4xx_HAL_Driver
SYSTEM_FILE= ./
BLUENRG=$(MIDDLWARES)/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/
USBCDC=$(MIDDLEWARES)/ST/STM32_USB_Device_Library/Class/CDC
USBCORE=$(MIDDLEWARES)/ST/STM32_USB_Device_Library/Core/
# STARTUP_FILE= ../MDK-ARM/STM32L476RG-SensorTile/
STARTUP_FILE= ../SW4STM32/STM32L476RG-SensorTile
LOCAL_DRIVERS=$(LIBROOT)/BSP/SensorTile
COMPONENTS=$(LIBROOT)/BSP/Components/Common
LSM6=$(LIBROOT)/BSP/Components/lsm6dsm
HTS=$(LIBROOT)/BSP/Components/hts221
LPS=$(LIBROOT)/BSP/Components/lps22hb
LSM3=$(LIBROOT)/BSP/Components/lsm303agr
STC=$(LIBROOT)/BSP/Components/stc3115
METADATA=$(MIDDLEWARES)/ST/STM32_MetaDataManager/
FX=$(MIDDLEWARES)/ST/STM32_OSX_MotionFX_Library/
AR=$(MIDDLEWARES)/ST/STM32_OSX_MotionAR_Library/
CP=$(MIDDLEWARES)/ST/STM32_OSX_MotionCP_Library/
GR=$(MIDDLEWARES)/ST/STM32_OSX_MotionGR_Library/
BLUEVOICE=$(MIDDLEWARES)/ST/STM32_OSX_BlueVoice_Library/

LOCAL_INC=../Inc



# Search path for standard files

vpath %.c $(TEMPLATEROOT)

vpath %.h $(USBCDC)/Inc
vpath %.h $(USBCORE)/Inc

# Search path for perpheral library
vpath %.h $(COMPONENTS)
vpath %.h $(BLUENRG)/includes

vpath %.h $(METADATA)
#vpath %.c $(METADATA)
vpath %.h $(LOCAL_INC)
# good ^

vpath %.h $(LSM6)
vpath %.h $(HTS)
vpath %.h $(LPS)
vpath %.h $(LSM3)
vpath %.h $(STC)

vpath %.h $(FX)/Inc
vpath %.h $(AR)/Inc
vpath %.h $(CP)/Inc
vpath %.h $(GR)/Inc

vpath %.h $(BLUEVOICE)/Inc

vpath %.c $(BLUENRG)/hci
vpath %.c $(BLUENRG)/hci/controller
# good ^ 		
# good ^
vpath %.c $(BLUENRG)/utils
# good ^


vpath %.c $(CORE)
# good ^
vpath %.c $(PERIPH)/Src
# good ^

vpath %.h $(DEVICE)/Include/
vpath %.c $(DEVICE)/Source

# good ^
vpath %.c $(SYSTEM_FILE)
# good ^
vpath %.s $(STARTUP_FILE)    
# good ^


# vpath %.c $(ACCEL_GYRO)
vpath %.c $(LOCAL_DRIVERS)/src
# good ^ ! sensortile drivers !

#  Processor specific

LDSCRIPT = ../SW4STM32/STM32L4xx-SensorTile/STM32L4xx-SensorTile
STARTUP =  startup_stm32l476xx.o system_stm32l4xx.o
# startup_stm32f30x.o was up in startup 
# Compilation Flags

FULLASSERT = -DUSE_FULL_ASSERT 

# LDLIBS = --specs=nosys.specs -lm 
LDLIBS = -lm 
LDFLAGS+= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4
CFLAGS+= -mcpu=cortex-m4 -mthumb 
CFLAGS+= -I$(TEMPLATEROOT) -I$(DEVICE)/Include -I$(CORE) -I$(PERIPH)/inc -I.
CFLAGS+= -DUSE_STDPERIPH_DRIVER $(FULLASSERT) 
CFLAGS+= -I$(BLUENRG)/includes
CFLAGS+= -I$(METADATA) -I$(FX)/Inc -I$(COMPONENTS) -I$(AR)/Inc -I$(CP)/Inc
CFLAGS+= -I$(GR)/Inc -I$(BLUEVOICE)/Inc 
CFLAGS+= -I$(LSM6) -I$(HTS) -I$(LPS) -I$(LSM3) -I$(STC)
CFLAGS+= -I$(LOCAL_INC)
CFLAGS+= -I$(DEVICE)/Include -I$(CORE)
# CFLAGS+= -I$(LIBROOT)/Project/Peripheral_Examples/GPIO_IOToggle
# CFLAGS+= -I$(ACCEL_GYRO)
CFLAGS+= -I$(COMPONENTS)
CFLAGS+= -I$(LOCAL_DRIVERS)/inc
# CFLAGS+= -I$(FATFS)/src
CFLAGS+= -Wno-multichar

# Build executable 

$(ELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# compile and generate dependency info

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(DD) -MM $(CFLAGS) $< > $*.d

%.o: %.s
	$(GAS) -c $(CFLAGS) $< -o $@
	$(DD) -MM $(CFLAGS) $< > $*.d

%.bin: %.elf
	$(OC) -O binary $< $@

clean:
	$(RM) -f $(OBJS) $(OBJS:.o=.d) $(ELF) startup_stm32f* $(CLEANOTHER) $(BIN) *.d

debug: $(ELF)
	arm-none-eabi-gdb $(ELF)

download: $(BIN)
	$(DL) write $(BIN) 0x8000000 > st-flash.log 2>&1
	$(GP) -o "jolly" st-flash.log | sed 's/jolly/Success/'
	$(GP) -o "Couldn" st-flash.log | sed 's/Couldn/Fail/'

etags:
	find $(PERIPH) -type f -iname "*.[ch]" | xargs etags --append
	find $(DEVICE) -type f -iname "*.[ch]" | xargs etags --append
	find $(CORE) -type f -iname "*.[ch]" | xargs etags --append
	#find $(ACCEL_GYRO) -type f -iname "*.[ch]" | xargs etags --append
	find $(LOCAL_DRIVERS) -type f -iname "*.[ch]" | xargs etags --append
	#find $(LIBROOT)/Project/Peripheral_Examples/GPIO_IOToggle -type f -iname "*.[ch]" | xargs etags --append
	find . -type f -iname "*.[ch]" | xargs etags --append

all: $(ELF)

# pull in dependencies

-include $(OBJS:.o=.d)




