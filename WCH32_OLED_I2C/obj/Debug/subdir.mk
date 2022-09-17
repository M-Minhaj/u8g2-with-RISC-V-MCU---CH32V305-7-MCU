################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/RISC_V_RT_THREAD/ch32v307-main/EVT/EXAM/SRC/Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/debug.o: D:/RISC_V_RT_THREAD/ch32v307-main/EVT/EXAM/SRC/Debug/debug.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\RISC_V_RT_THREAD\ch32v307-main\EVT\EXAM\SRC\Debug" -I"D:\RISC_V_RT_THREAD\ch32v307-main\EVT\EXAM\SRC\Core" -I"D:\RISC_V_RT_THREAD\WCH32_OLED_I2C\User" -I"D:\RISC_V_RT_THREAD\ch32v307-main\EVT\EXAM\SRC\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


