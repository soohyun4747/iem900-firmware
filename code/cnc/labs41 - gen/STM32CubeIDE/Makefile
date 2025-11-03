######################################
# Project configuration
######################################
PROJECT_NAME := labs41_gen
PROJECT_ROOT := ..
BUILD_DIR := build

######################################
# Toolchain configuration
######################################
CC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
AR := arm-none-eabi-ar
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size

######################################
# MCU configuration
######################################
CPU := -mcpu=cortex-m3
FPU :=
FLOAT_ABI :=
MCU_FLAGS := $(CPU) -mthumb $(FPU) $(FLOAT_ABI)

######################################
# Directories
######################################
LIB_ROOT := ../../../../Libraries
DRIVER_SRC_DIR := $(LIB_ROOT)/STM32F2xx_StdPeriph_Driver/src
CMSIS_TEMPLATE_DIR := $(LIB_ROOT)/CMSIS/Device/ST/STM32F2xx/Source/Templates

######################################
# Sources
######################################
C_SOURCES := \
  $(PROJECT_ROOT)/src/FILE_SYS.C \
  $(PROJECT_ROOT)/src/SPI_HW.C \
  $(PROJECT_ROOT)/src/adc_input.c \
  $(PROJECT_ROOT)/src/buzzer.c \
  $(PROJECT_ROOT)/src/chamber_work.c \
  $(PROJECT_ROOT)/src/control_uart.c \
  $(PROJECT_ROOT)/src/data_io.c \
  $(PROJECT_ROOT)/src/delay.c \
  $(PROJECT_ROOT)/src/device.c \
  $(PROJECT_ROOT)/src/door_com.c \
  $(PROJECT_ROOT)/src/error.c \
  $(PROJECT_ROOT)/src/fonts.c \
  $(PROJECT_ROOT)/src/fsmc_lcd.c \
  $(PROJECT_ROOT)/src/fsmc_nand.c \
  $(PROJECT_ROOT)/src/graphic.c \
  $(PROJECT_ROOT)/src/main.c \
  $(PROJECT_ROOT)/src/pid_control.c \
  $(PROJECT_ROOT)/src/rtc_calender.c \
  $(PROJECT_ROOT)/src/temp_ic_spi.c \
  $(PROJECT_ROOT)/src/timer_pwm.c \
  $(PROJECT_ROOT)/src/touch_i2c.c \
  $(PROJECT_ROOT)/src/usb_out.c \
  $(PROJECT_ROOT)/src/voc_filter_chrck.c \
  $(PROJECT_ROOT)/src/stm32f2xx_it.c

C_SOURCES += \
  $(DRIVER_SRC_DIR)/misc.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_adc.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_dma.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_fsmc.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_gpio.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_i2c.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_iwdg.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_pwr.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_rcc.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_rtc.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_spi.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_tim.c \
  $(DRIVER_SRC_DIR)/stm32f2xx_usart.c

C_SOURCES += $(CMSIS_TEMPLATE_DIR)/system_stm32f2xx.c

ASM_SOURCES := $(CMSIS_TEMPLATE_DIR)/gcc_ride7/startup_stm32f2xx.s

######################################
# Includes and defines
######################################
C_INCLUDES := \
  -I"$(PROJECT_ROOT)/inc" \
  -I"$(LIB_ROOT)/CMSIS/Include" \
  -I"$(LIB_ROOT)/CMSIS/Device/ST/STM32F2xx/Include" \
  -I"$(LIB_ROOT)/STM32F2xx_StdPeriph_Driver/inc"

C_DEFS := \
  -DUSE_STDPERIPH_DRIVER \
  -DSTM32F2XX \
  -DSYSCLK_FREQ_120MHz

######################################
# Flags
######################################
OPT := -O2
CFLAGS := $(MCU_FLAGS) $(OPT) -ffunction-sections -fdata-sections -fno-common -Wall -Wextra -Wundef -Wno-unused-parameter -std=gnu11
CFLAGS += $(C_DEFS) $(C_INCLUDES)
CFLAGS += -MMD -MP

ASFLAGS := $(MCU_FLAGS) -x assembler-with-cpp -MMD -MP $(C_DEFS) $(C_INCLUDES)

LDSCRIPT := STM32F205ZGTX_FLASH.ld
LDFLAGS := $(MCU_FLAGS) -specs=nano.specs -specs=nosys.specs -Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map -Wl,--gc-sections -Wl,--start-group -lc -lm -lnosys -Wl,--end-group -T$(LDSCRIPT)

######################################
# Build rules
######################################
OBJECTS := $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES)))
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(OBJECTS:.C=.o)
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

VPATH := $(sort $(dir $(C_SOURCES) $(ASM_SOURCES)))

all: $(BUILD_DIR) $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).hex $(BUILD_DIR)/$(PROJECT_NAME).bin

$(BUILD_DIR):
@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
@echo "CC $<"
@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.C | $(BUILD_DIR)
@echo "CC $<"
@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
@echo "AS $<"
@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJECTS) $(LDSCRIPT)
@echo "LD $@"
@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
@$(SIZE) $@

$(BUILD_DIR)/$(PROJECT_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME).elf
@echo "OBJCOPY $@"
@$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
@echo "OBJCOPY $@"
@$(OBJCOPY) -O binary $< $@

clean:
@echo "Cleaning..."
@rm -rf $(BUILD_DIR)

.PHONY: all clean

-include $(BUILD_DIR)/*.d
