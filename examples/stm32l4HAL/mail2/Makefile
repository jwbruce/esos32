##########################################################################################################################
# Makefile based on auto-generated from STM32Cube tool
##########################################################################################################################

######################################
# target
######################################
TARGET = mail2_app

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
# ESOS base path
ESOS_DIR = /home/jwbruce/REPOs/esos32
# HAL base path
HAL_DIR = /home/jwbruce/STM32Tools/STM32Cube/Repository/STM32Cube_FW_L4_V1.13.0/


######################################
# source
######################################
# C sources
C_SOURCES =  \
src/mail2_app.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
$(ESOS_DIR)/src/esos.c \
$(ESOS_DIR)/src/esos_cb.c \
$(ESOS_DIR)/src/esos_utils.c \
$(ESOS_DIR)/src/esos_comm.c \
$(ESOS_DIR)/src/esos_mail.c \
$(ESOS_DIR)/src/stm32l4/esos_stm32l4_tick.c \
$(ESOS_DIR)/src/stm32l4/esos_stm32l4_utils.c \
$(ESOS_DIR)/src/stm32l4/esos_stm32l4_rs232.c \
$(ESOS_DIR)/src/stm32l4/HAL_core/system_stm32l4xx.c \
$(ESOS_DIR)/src/stm32l4/HAL_core/stm32l4xx_it.c \
$(ESOS_DIR)/src/stm32l4/HAL_core/stm32l4xx_hal_msp.c

# ASM sources
ASM_SOURCES =  \
$(ESOS_DIR)/src/stm32l4/HAL_core/startup_stm32l452xx.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32L452xx  \
-DHARDWARE_PLATFORM=NUCLEO64


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Iinclude \
-I$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Inc \
-I$(HAL_DIR)/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
-I$(HAL_DIR)/Drivers/CMSIS/Device/ST/STM32L4xx/Include \
-I$(HAL_DIR)/Drivers/CMSIS/Include \
-I$(ESOS_DIR)/include \
-I$(ESOS_DIR)/include/stm32l4 \
-I$(ESOS_DIR)/include/stm32l4/HAL_core \

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L452RETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***


