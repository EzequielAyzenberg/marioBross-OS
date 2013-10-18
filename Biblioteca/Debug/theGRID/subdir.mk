################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../theGRID/logger.c \
../theGRID/sockets.c 

OBJS += \
./theGRID/logger.o \
./theGRID/sockets.o 

C_DEPS += \
./theGRID/logger.d \
./theGRID/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
theGRID/%.o: ../theGRID/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


