################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/logger/errno.c \
../source/logger/logger.c \
../source/logger/timestamp.c 

OBJS += \
./source/logger/errno.o \
./source/logger/logger.o \
./source/logger/timestamp.o 

C_DEPS += \
./source/logger/errno.d \
./source/logger/logger.d \
./source/logger/timestamp.d 


# Each subdirectory must supply rules for building sources it contributes
source/logger/%.o: ../source/logger/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DDEBUG_CODE -DARM_MATH_CM0PLUS -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_OS_FREE_RTOS -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I../board -I../source/dac -I../source/dma -I../source/lookup_generator -I../source/adc -I../source -I../ -I../source/logger -I../source/circular_buffer -I../source/led_control -I../freertos -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


