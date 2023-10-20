RTOSLIB = ../CMSIS_5

RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/GCC/irq_cm3.S

RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_system.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_kernel.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_thread.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_lib.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_evr.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_delay.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_memory.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_mempool.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_mutex.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_timer.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_msgqueue.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_semaphore.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_evflags.c
RTOSSRC += $(RTOSLIB)/CMSIS/RTOS2/RTX/Source/rtx_gic.c

SRCDIR   := src
INCDIR   := inc
ASMDIR   := startup
BUILDDIR := build

TARGET = rtos

# Get all C- and ASM-source files
SRCS := $(wildcard $(SRCDIR)/*.c)
SRCS += $(wildcard $(ASMDIR)/*.S)
SRCS += $(RTOSSRC)

OBJS  =  $(addsuffix .o, $(basename $(SRCS)))
SOBJS =  $(addsuffix .o, $(notdir $(basename $(SRCS))))

INCDIRS += $(INCDIR)
INCDIRS += $(RTOSLIB)/Device/ARM/ARMCM3/Include
INCDIRS += $(RTOSLIB)/CMSIS/Core/Include
INCDIRS += $(RTOSLIB)/CMSIS/RTOS2/Include
INCDIRS += $(RTOSLIB)/CMSIS/RTOS2/RTX/Include
INCLUDES = $(foreach d, $(INCDIRS), -I$d)

LINKER_SCRIPT = scripts/STM32F103X6_FLASH.ld

CFLAGS += -mcpu=cortex-m3 -mthumb # Processor setup
CFLAGS += -O0 # Optimization is off
CFLAGS += -g2 # Generate debug info
CFLAGS += -fno-common
CFLAGS += -Wall # Turn on warnings
#CFLAGS += -pedantic # more warnings
CFLAGS += -Wsign-compare
CFLAGS += -mfloat-abi=soft -fsingle-precision-constant
CFLAGS += -Wcast-align
#CFLAGS += -Wconversion # neg int const implicitly converted to uint
CFLAGS += -fomit-frame-pointer # Do not use fp if not needed
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -D__NO_SYSTEM_INIT

LDFLAGS += -march=armv7-m
LDFLAGS += --specs=nosys.specs
LDFLAGS += -lnosys

# Use the precompiled library instead of compiling your own
# Uncomment all the RTOSSRC's up top if you enable these two
#LDFLAGS += -L$(RTOSLIB)/CMSIS/RTOS2/RTX/Library/GCC
#LDFLAGS += -lRTX_CM3

LDFLAGS += -Wl,--gc-sections # Linker garbage collector
LDFLAGS += -T$(LINKER_SCRIPT)

CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size
DBG = $(CROSS_COMPILE)gdb

all: clean $(SRCS) build size cleanobjs
	@echo "Successfully finished..."

build: $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET).bin $(BUILDDIR)/$(TARGET).lst

$(BUILDDIR)/$(TARGET).elf: $(OBJS)
	@$(CC) $(SOBJS) $(LDFLAGS) -o $@

%.o: %.c
	@echo "Building" $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(notdir $@)

%.o: %.S
	@echo "Building" $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(notdir $@)

$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	@$(OBJCOPY) -O ihex $< $@

$(BUILDDIR)/%.bin: $(BUILDDIR)/%.elf
	@$(OBJCOPY) -O binary $< $@

$(BUILDDIR)/%.lst: $(BUILDDIR)/%.elf
	@$(OBJDUMP) -x -S $(BUILDDIR)/$(TARGET).elf > $@

size: $(BUILDDIR)/$(TARGET).elf
	@$(SIZE) $(BUILDDIR)/$(TARGET).elf

flash:
	@st-flash --reset write $(BUILDDIR)/$(TARGET).bin 0x8000000

debug:
	@$(DBG) -tui --eval-command="target extended-remote :4242" \
	--eval-command="layout asm" \
	--eval-command="layout regs" \
	 $(BUILDDIR)/$(TARGET).elf

clean:
	@echo "Cleaning..."
	@rm -f $(BUILDDIR)/$(TARGET).elf
	@rm -f $(BUILDDIR)/$(TARGET).bin
	@rm -f $(BUILDDIR)/$(TARGET).map
	@rm -f $(BUILDDIR)/$(TARGET).hex
	@rm -f $(BUILDDIR)/$(TARGET).lst
	@rm -f $(TARGET).o
	@rm -f *.o

cleanobjs:
	@echo "Cleaning objects..."
	@rm -f *.o

.PHONY: all build size clean cleanobjs flash debug
