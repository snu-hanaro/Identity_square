################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/baro/bmp/bmp581.cpp 

C_SRCS += \
../Core/Src/baro/bmp/bmp5.c 

C_DEPS += \
./Core/Src/baro/bmp/bmp5.d 

OBJS += \
./Core/Src/baro/bmp/bmp5.o \
./Core/Src/baro/bmp/bmp581.o 

CPP_DEPS += \
./Core/Src/baro/bmp/bmp581.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/baro/bmp/%.o Core/Src/baro/bmp/%.su Core/Src/baro/bmp/%.cyclo: ../Core/Src/baro/bmp/%.c Core/Src/baro/bmp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/baro/bmp/%.o Core/Src/baro/bmp/%.su Core/Src/baro/bmp/%.cyclo: ../Core/Src/baro/bmp/%.cpp Core/Src/baro/bmp/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-baro-2f-bmp

clean-Core-2f-Src-2f-baro-2f-bmp:
	-$(RM) ./Core/Src/baro/bmp/bmp5.cyclo ./Core/Src/baro/bmp/bmp5.d ./Core/Src/baro/bmp/bmp5.o ./Core/Src/baro/bmp/bmp5.su ./Core/Src/baro/bmp/bmp581.cyclo ./Core/Src/baro/bmp/bmp581.d ./Core/Src/baro/bmp/bmp581.o ./Core/Src/baro/bmp/bmp581.su

.PHONY: clean-Core-2f-Src-2f-baro-2f-bmp

