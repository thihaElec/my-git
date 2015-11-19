################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/Cpu.c \
../Generated_Code/LCD.c \
../Generated_Code/LED.c \
../Generated_Code/WAIT1.c \
../Generated_Code/clockMan1.c \
../Generated_Code/hardware_init.c \
../Generated_Code/osa1.c \
../Generated_Code/pin_mux.c 

OBJS += \
./Generated_Code/Cpu.o \
./Generated_Code/LCD.o \
./Generated_Code/LED.o \
./Generated_Code/WAIT1.o \
./Generated_Code/clockMan1.o \
./Generated_Code/hardware_init.o \
./Generated_Code/osa1.o \
./Generated_Code/pin_mux.o 

C_DEPS += \
./Generated_Code/Cpu.d \
./Generated_Code/LCD.d \
./Generated_Code/LED.d \
./Generated_Code/WAIT1.d \
./Generated_Code/clockMan1.d \
./Generated_Code/hardware_init.d \
./Generated_Code/osa1.d \
./Generated_Code/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"FSL_OSA_BM_TIMER_CONFIG=2" -D"CPU_MKL25Z128VLK4" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/hal/inc" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/hal/src/sim/MKL25Z4" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/system/src/clock/MKL25Z4" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/system/inc" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/osa/inc" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/CMSIS/Include" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/devices" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/devices/MKL25Z4/include" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/devices/MKL25Z4/startup" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/Generated_Code/SDK/platform/devices/MKL25Z4/startup" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/Sources" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/Generated_Code" -I"C:/code/Freescale-KDS/FDRM_KL25Z_LCD1/SDK/platform/drivers/inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


