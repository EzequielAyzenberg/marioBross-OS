################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cargador.c \
../conexiones.c \
../enemigos.c \
../gui.c \
../interbloqueo.c \
../nivel.c \
../nivelconfig.c \
../personajes.c 

OBJS += \
./cargador.o \
./conexiones.o \
./enemigos.o \
./gui.o \
./interbloqueo.o \
./nivel.o \
./nivelconfig.o \
./personajes.o 

C_DEPS += \
./cargador.d \
./conexiones.d \
./enemigos.d \
./gui.d \
./interbloqueo.d \
./nivel.d \
./nivelconfig.d \
./personajes.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


