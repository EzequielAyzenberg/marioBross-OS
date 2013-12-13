################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include GUI/subdir.mk
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
all: Plataforma

dependents:
	-cd /home/utnso/GITHUB/tp-2013-2c-the-grid/nivel-gui/Debug && $(MAKE) clean all

# Tool invocations
Plataforma: $(OBJS) $(USER_OBJS) /home/utnso/GITHUB/tp-2013-2c-the-grid/nivel-gui/Debug/libnivel-gui.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library/Debug -L/home/utnso/GITHUB/tp-2013-2c-the-grid/Biblioteca/Debug -L/home/utnso/GITHUB/tp-2013-2c-the-grid/nivel-gui/Debug -o "Plataforma" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Plataforma
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/GITHUB/tp-2013-2c-the-grid/nivel-gui/Debug/libnivel-gui.so:

-include ../makefile.targets
