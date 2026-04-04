################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/ADC/ADC.cpp 

OBJS += \
./Core/Src/ADC/ADC.o 

CPP_DEPS += \
./Core/Src/ADC/ADC.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ADC/%.o Core/Src/ADC/%.su Core/Src/ADC/%.cyclo: ../Core/Src/ADC/%.cpp Core/Src/ADC/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-ADC

clean-Core-2f-Src-2f-ADC:
	-$(RM) ./Core/Src/ADC/ADC.cyclo ./Core/Src/ADC/ADC.d ./Core/Src/ADC/ADC.o ./Core/Src/ADC/ADC.su

.PHONY: clean-Core-2f-Src-2f-ADC

