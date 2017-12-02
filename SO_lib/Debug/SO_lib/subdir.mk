################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SO_lib/FuncionesAuxiliares.c \
../SO_lib/bloques.c \
../SO_lib/estructuras.c \
../SO_lib/sem.c \
../SO_lib/sendAndReceive.c \
../SO_lib/serializacion.c \
../SO_lib/sockets.c 

OBJS += \
./SO_lib/FuncionesAuxiliares.o \
./SO_lib/bloques.o \
./SO_lib/estructuras.o \
./SO_lib/sem.o \
./SO_lib/sendAndReceive.o \
./SO_lib/serializacion.o \
./SO_lib/sockets.o 

C_DEPS += \
./SO_lib/FuncionesAuxiliares.d \
./SO_lib/bloques.d \
./SO_lib/estructuras.d \
./SO_lib/sem.d \
./SO_lib/sendAndReceive.d \
./SO_lib/serializacion.d \
./SO_lib/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
SO_lib/%.o: ../SO_lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


