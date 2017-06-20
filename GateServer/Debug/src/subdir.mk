################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GateServer.cpp \
../src/MessageBuild.cpp \
../src/ServerManager.cpp \
../src/ServiceMain.cpp 

OBJS += \
./src/GateServer.o \
./src/MessageBuild.o \
./src/ServerManager.o \
./src/ServiceMain.o 

CPP_DEPS += \
./src/GateServer.d \
./src/MessageBuild.d \
./src/ServerManager.d \
./src/ServiceMain.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I../../Dependence -I/usr/local/lua/include -I/usr/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


