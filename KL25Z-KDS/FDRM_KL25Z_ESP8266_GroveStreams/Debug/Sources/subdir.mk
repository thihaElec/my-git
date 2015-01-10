################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Appliction.c \
../Sources/DTH.c \
../Sources/ESP8266.c \
../Sources/Events.c \
../Sources/GroveStreams.c \
../Sources/Shell.c \
../Sources/main.c 

OBJS += \
./Sources/Appliction.o \
./Sources/DTH.o \
./Sources/ESP8266.o \
./Sources/Events.o \
./Sources/GroveStreams.o \
./Sources/Shell.o \
./Sources/main.o 

C_DEPS += \
./Sources/Appliction.d \
./Sources/DTH.d \
./Sources/ESP8266.d \
./Sources/Events.d \
./Sources/GroveStreams.d \
./Sources/Shell.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:\Freescale\KDS_2.0.0\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:\Freescale\KDS_2.0.0\eclipse\ProcessorExpert/lib/Kinetis/iofiles" -I"C:/code/Freescale-KDS/FDRM_KL25Z_ESP8266_GroveStreams/Sources" -I"C:/code/Freescale-KDS/FDRM_KL25Z_ESP8266_GroveStreams/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


