################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Worker.c \
../src/configuracionW.c \
../src/socketsWorker.c 

OBJS += \
./src/Worker.o \
./src/configuracionW.o \
./src/socketsWorker.o 

C_DEPS += \
./src/Worker.d \
./src/configuracionW.d \
./src/socketsWorker.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


