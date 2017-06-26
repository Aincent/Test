################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Destination/Destination.cpp \
../src/Destination/DestinationLoader.cpp \
../src/Destination/DestinationMessage.cpp 

OBJS += \
./src/Destination/Destination.o \
./src/Destination/DestinationLoader.o \
./src/Destination/DestinationMessage.o 

CPP_DEPS += \
./src/Destination/Destination.d \
./src/Destination/DestinationLoader.d \
./src/Destination/DestinationMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/Destination/%.o: ../src/Destination/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


