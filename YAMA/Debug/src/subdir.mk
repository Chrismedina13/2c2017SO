################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/YAMA.c \
../src/configuracion.c \
../src/logYAMA.c 

OBJS += \
./src/YAMA.o \
./src/configuracion.o \
./src/logYAMA.o 

C_DEPS += \
./src/YAMA.d \
./src/configuracion.d \
./src/logYAMA.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


