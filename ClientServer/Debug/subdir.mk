################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ClientAgent.c \
../main.c \
../net_protocol.c \
../server.c \
../server_config.c 

OBJS += \
./ClientAgent.o \
./main.o \
./net_protocol.o \
./server.o \
./server_config.o 

C_DEPS += \
./ClientAgent.d \
./main.d \
./net_protocol.d \
./server.d \
./server_config.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../../ -I"/data/ChatShow/antlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


