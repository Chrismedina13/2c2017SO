################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FileSystem.c \
../src/configuracion.c \
../src/consola.c \
../src/handler-user.c \
../src/kernel.c \
../src/logFileSystem.c \
../src/socketsFileSystem.c 

OBJS += \
./src/FileSystem.o \
./src/configuracion.o \
./src/consola.o \
./src/handler-user.o \
./src/kernel.o \
./src/logFileSystem.o \
./src/socketsFileSystem.o 

C_DEPS += \
./src/FileSystem.d \
./src/configuracion.d \
./src/consola.d \
./src/handler-user.d \
./src/kernel.d \
./src/logFileSystem.d \
./src/socketsFileSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


