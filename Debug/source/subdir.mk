################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Command_Processor.c \
../source/Hexdump.c \
../source/UART.c \
../source/cbfifo.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/test_cbfifo.c 

C_DEPS += \
./source/Command_Processor.d \
./source/Hexdump.d \
./source/UART.d \
./source/cbfifo.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/test_cbfifo.d 

OBJS += \
./source/Command_Processor.o \
./source/Hexdump.o \
./source/UART.o \
./source/cbfifo.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/test_cbfifo.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -DSDK_DEBUGCONSOLE=0 -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BreakfastSerial\source" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BreakfastSerial" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BreakfastSerial\startup" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BreakfastSerial\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Command_Processor.d ./source/Command_Processor.o ./source/Hexdump.d ./source/Hexdump.o ./source/UART.d ./source/UART.o ./source/cbfifo.d ./source/cbfifo.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sysclock.d ./source/sysclock.o ./source/test_cbfifo.d ./source/test_cbfifo.o

.PHONY: clean-source

