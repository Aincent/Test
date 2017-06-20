################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GT_MSOpreation/MSOpreationMessage.cpp 

OBJS += \
./src/GT_MSOpreation/MSOpreationMessage.o 

CPP_DEPS += \
./src/GT_MSOpreation/MSOpreationMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/GT_MSOpreation/%.o: ../src/GT_MSOpreation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I../../Dependence -I/usr/local/lua/include -I/usr/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


