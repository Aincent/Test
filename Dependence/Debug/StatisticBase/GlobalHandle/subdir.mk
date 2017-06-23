################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../StatisticBase/GlobalHandle/GlobalHandle.cpp 

OBJS += \
./StatisticBase/GlobalHandle/GlobalHandle.o 

CPP_DEPS += \
./StatisticBase/GlobalHandle/GlobalHandle.d 


# Each subdirectory must supply rules for building sources it contributes
StatisticBase/GlobalHandle/%.o: ../StatisticBase/GlobalHandle/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


