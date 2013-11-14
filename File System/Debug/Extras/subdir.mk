################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Extras/grasa.c \
../Extras/pruebaGrasa.c 

OBJS += \
./Extras/grasa.o \
./Extras/pruebaGrasa.o 

C_DEPS += \
./Extras/grasa.d \
./Extras/pruebaGrasa.d 


# Each subdirectory must supply rules for building sources it contributes
Extras/%.o: ../Extras/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library -I/home/utnso/GITHUB/tp-2013-2c-the-grid/Biblioteca -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


