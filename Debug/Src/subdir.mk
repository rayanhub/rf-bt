################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/main.c \
../Src/rng.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_tim.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c \
../Src/usart.c 

OBJS += \
./Src/adc.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/main.o \
./Src/rng.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_tim.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o \
./Src/usart.o 

C_DEPS += \
./Src/adc.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/main.d \
./Src/rng.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_tim.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F405xx -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/si4463/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/Delay_DWT/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/CMSIS/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


