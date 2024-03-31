################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/usb.c 

OBJS += \
./src/main.o \
./src/usb.o 

C_DEPS += \
./src/main.d \
./src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/jpnib/Insync/Google_Drive/Documents/Developments/STM32CubeIDE_workspace/STM32F103_usb_test/CMSIS/core" -I"C:/Users/jpnib/Insync/Google_Drive/Documents/Developments/STM32CubeIDE_workspace/STM32F103_usb_test/CMSIS/device" -O1 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

