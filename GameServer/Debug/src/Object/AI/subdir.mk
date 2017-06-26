################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Object/AI/AIBrain.cpp \
../src/Object/AI/AIStateArena.cpp \
../src/Object/AI/AIStateAttack.cpp \
../src/Object/AI/AIStateDead.cpp \
../src/Object/AI/AIStateIdle.cpp \
../src/Object/AI/AIStateReturn.cpp 

OBJS += \
./src/Object/AI/AIBrain.o \
./src/Object/AI/AIStateArena.o \
./src/Object/AI/AIStateAttack.o \
./src/Object/AI/AIStateDead.o \
./src/Object/AI/AIStateIdle.o \
./src/Object/AI/AIStateReturn.o 

CPP_DEPS += \
./src/Object/AI/AIBrain.d \
./src/Object/AI/AIStateArena.d \
./src/Object/AI/AIStateAttack.d \
./src/Object/AI/AIStateDead.d \
./src/Object/AI/AIStateIdle.d \
./src/Object/AI/AIStateReturn.d 


# Each subdirectory must supply rules for building sources it contributes
src/Object/AI/%.o: ../src/Object/AI/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


