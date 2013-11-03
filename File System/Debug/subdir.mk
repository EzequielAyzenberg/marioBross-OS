################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fuseGrid.c \
../grasa.c 

OBJS += \
./fuseGrid.o \
./grasa.o 

C_DEPS += \
./fuseGrid.d \
./grasa.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library" -I"/home/utnso/GITHUB/tp-2013-2c-the-grid/Biblioteca" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


