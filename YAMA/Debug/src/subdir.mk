################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ComunicacionConFileSystem.c \
../src/ComunicacionConMaster.c \
../src/YAMA.c \
../src/configuracion.c \
../src/logYAMA.c \
../src/socketsYAMA.c 

OBJS += \
./src/ComunicacionConFileSystem.o \
./src/ComunicacionConMaster.o \
./src/YAMA.o \
./src/configuracion.o \
./src/logYAMA.o \
./src/socketsYAMA.o 

C_DEPS += \
./src/ComunicacionConFileSystem.d \
./src/ComunicacionConMaster.d \
./src/YAMA.d \
./src/configuracion.d \
./src/logYAMA.d \
./src/socketsYAMA.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


