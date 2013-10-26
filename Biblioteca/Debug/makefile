################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include theGRID/subdir.mk
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
all: libBiblioteca.so

dependents:
	-cd /home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library/Debug && $(MAKE) all

# Tool invocations
libBiblioteca.so: $(OBJS) $(USER_OBJS) /home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library/Debug/libCommons-Library.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library/Debug -shared -Wl,-soname=Biblioteca -o "libBiblioteca.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LIBRARIES) libBiblioteca.so
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/GITHUB/tp-2013-2c-the-grid/Commons-Library/Debug/libCommons-Library.so:

-include ../makefile.targets
