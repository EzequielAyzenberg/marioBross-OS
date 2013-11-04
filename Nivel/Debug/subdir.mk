################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cargador.c \
../conexiones.c \
../enemigos.c \
../gui.c \
../nivel.c \
../nivelconfig.c 

OBJS += \
./cargador.o \
./conexiones.o \
./enemigos.o \
./gui.o \
./nivel.o \
./nivelconfig.o 

C_DEPS += \
./cargador.d \
./conexiones.d \
./enemigos.d \
./gui.d \
./nivel.d \
./nivelconfig.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/GITHUB/tp-2013-2c-the-grid/nivel-gui" -I"/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library" -I"/home/utnso/GITHUB/tp-2013-2c-the-grid/Biblioteca" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


