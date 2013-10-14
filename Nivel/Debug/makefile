################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Nivel

dependents:
	-cd /home/utnso/workspace/Commons-Library/Debug && $(MAKE) all
	-cd /home/utnso/GITHUB/tp-2013-2c-the-grid/Nivel/Release && $(MAKE) all

# Tool invocations
Nivel: $(OBJS) $(USER_OBJS) /home/utnso/workspace/Commons-Library/Debug/libCommons-Library.so /home/utnso/GITHUB/tp-2013-2c-the-grid/Nivel/Release/Nivel
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/Commons-Library/Debug" -o "Nivel" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Nivel
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/workspace/Commons-Library/Debug/libCommons-Library.so:
/home/utnso/GITHUB/tp-2013-2c-the-grid/Nivel/Release/Nivel:

-include ../makefile.targets
