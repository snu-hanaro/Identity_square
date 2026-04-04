################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/flight_control/flight_state.cpp 

OBJS += \
./Core/Src/flight_control/flight_state.o 

CPP_DEPS += \
./Core/Src/flight_control/flight_state.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/flight_control/%.o Core/Src/flight_control/%.su Core/Src/flight_control/%.cyclo: ../Core/Src/flight_control/%.cpp Core/Src/flight_control/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-flight_control

clean-Core-2f-Src-2f-flight_control:
	-$(RM) ./Core/Src/flight_control/flight_state.cyclo ./Core/Src/flight_control/flight_state.d ./Core/Src/flight_control/flight_state.o ./Core/Src/flight_control/flight_state.su

.PHONY: clean-Core-2f-Src-2f-flight_control

