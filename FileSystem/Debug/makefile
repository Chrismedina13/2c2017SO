################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
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
all: FileSystem

dependents:
	-cd /home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/SO_lib/Debug && $(MAKE) all

# Tool invocations
FileSystem: $(OBJS) $(USER_OBJS) /home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/SO_lib/Debug/libSO_lib.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/SO_lib/Debug" -o "FileSystem" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) FileSystem
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/SO_lib/Debug/libSO_lib.so:

-include ../makefile.targets
