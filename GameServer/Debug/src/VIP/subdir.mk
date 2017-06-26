################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/VIP/PlayerVipPart.cpp \
../src/VIP/VIPMessage.cpp \
../src/VIP/VipLoader.cpp 

OBJS += \
./src/VIP/PlayerVipPart.o \
./src/VIP/VIPMessage.o \
./src/VIP/VipLoader.o 

CPP_DEPS += \
./src/VIP/PlayerVipPart.d \
./src/VIP/VIPMessage.d \
./src/VIP/VipLoader.d 


# Each subdirectory must supply rules for building sources it contributes
src/VIP/%.o: ../src/VIP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


