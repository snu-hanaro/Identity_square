################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/buzzer/buzzer.cpp \
../Core/Src/buzzer/switching_buzzer.cpp 

OBJS += \
./Core/Src/buzzer/buzzer.o \
./Core/Src/buzzer/switching_buzzer.o 

CPP_DEPS += \
./Core/Src/buzzer/buzzer.d \
./Core/Src/buzzer/switching_buzzer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/buzzer/%.o Core/Src/buzzer/%.su Core/Src/buzzer/%.cyclo: ../Core/Src/buzzer/%.cpp Core/Src/buzzer/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-buzzer

clean-Core-2f-Src-2f-buzzer:
	-$(RM) ./Core/Src/buzzer/buzzer.cyclo ./Core/Src/buzzer/buzzer.d ./Core/Src/buzzer/buzzer.o ./Core/Src/buzzer/buzzer.su ./Core/Src/buzzer/switching_buzzer.cyclo ./Core/Src/buzzer/switching_buzzer.d ./Core/Src/buzzer/switching_buzzer.o ./Core/Src/buzzer/switching_buzzer.su

.PHONY: clean-Core-2f-Src-2f-buzzer

