################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DataNode.c \
../src/configuracionDN.c \
../src/logDataNode.c 

OBJS += \
./src/DataNode.o \
./src/configuracionDN.o \
./src/logDataNode.o 

C_DEPS += \
./src/DataNode.d \
./src/configuracionDN.d \
./src/logDataNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


