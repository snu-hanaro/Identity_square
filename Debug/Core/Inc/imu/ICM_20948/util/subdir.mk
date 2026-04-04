################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/imu/ICM_20948/util/ICM_20948_C.c 

C_DEPS += \
./Core/Inc/imu/ICM_20948/util/ICM_20948_C.d 

OBJS += \
./Core/Inc/imu/ICM_20948/util/ICM_20948_C.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/imu/ICM_20948/util/%.o Core/Inc/imu/ICM_20948/util/%.su Core/Inc/imu/ICM_20948/util/%.cyclo: ../Core/Inc/imu/ICM_20948/util/%.c Core/Inc/imu/ICM_20948/util/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-imu-2f-ICM_20948-2f-util

clean-Core-2f-Inc-2f-imu-2f-ICM_20948-2f-util:
	-$(RM) ./Core/Inc/imu/ICM_20948/util/ICM_20948_C.cyclo ./Core/Inc/imu/ICM_20948/util/ICM_20948_C.d ./Core/Inc/imu/ICM_20948/util/ICM_20948_C.o ./Core/Inc/imu/ICM_20948/util/ICM_20948_C.su

.PHONY: clean-Core-2f-Inc-2f-imu-2f-ICM_20948-2f-util

