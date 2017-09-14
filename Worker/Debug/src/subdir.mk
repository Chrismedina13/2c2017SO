################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Worker.c \
../src/configuracionW.c \
../src/logWorker.c \
../src/socketsWorker.c 

OBJS += \
./src/Worker.o \
./src/configuracionW.o \
./src/logWorker.o \
./src/socketsWorker.o 

C_DEPS += \
./src/Worker.d \
./src/configuracionW.d \
./src/logWorker.d \
./src/socketsWorker.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/SO_lib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


