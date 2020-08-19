################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/si4463/Src/si4463.c 

OBJS += \
./Drivers/si4463/Src/si4463.o 

C_DEPS += \
./Drivers/si4463/Src/si4463.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/si4463/Src/%.o: ../Drivers/si4463/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F405xx -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/si4463/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/Delay_DWT/Inc" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/rayan/Documents/modem_proj/test-rx_24.04.2020/Drivers/CMSIS/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


