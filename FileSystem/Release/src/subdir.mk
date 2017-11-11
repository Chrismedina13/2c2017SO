################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FileSystem.c \
../src/comunicacionConDN.c \
../src/comunicacionConWorker.c \
../src/comunicacionConYama.c \
../src/configuracion.c \
../src/consola.c \
../src/estructurasFS.c \
../src/logFileSystem.c \
../src/manejoDeArchivos.c \
../src/nodos.c \
../src/recuperacionFS.c \
../src/socketsFileSystem.c 

OBJS += \
./src/FileSystem.o \
./src/comunicacionConDN.o \
./src/comunicacionConWorker.o \
./src/comunicacionConYama.o \
./src/configuracion.o \
./src/consola.o \
./src/estructurasFS.o \
./src/logFileSystem.o \
./src/manejoDeArchivos.o \
./src/nodos.o \
./src/recuperacionFS.o \
./src/socketsFileSystem.o 

C_DEPS += \
./src/FileSystem.d \
./src/comunicacionConDN.d \
./src/comunicacionConWorker.d \
./src/comunicacionConYama.d \
./src/configuracion.d \
./src/consola.d \
./src/estructurasFS.d \
./src/logFileSystem.d \
./src/manejoDeArchivos.d \
./src/nodos.d \
./src/recuperacionFS.d \
./src/socketsFileSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


