################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../theGRID/cadenas.c \
../theGRID/general.c \
../theGRID/logger.c \
../theGRID/sockets.c 

OBJS += \
./theGRID/cadenas.o \
./theGRID/general.o \
./theGRID/logger.o \
./theGRID/sockets.o 

C_DEPS += \
./theGRID/cadenas.d \
./theGRID/general.d \
./theGRID/logger.d \
./theGRID/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
theGRID/%.o: ../theGRID/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library" -I/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


