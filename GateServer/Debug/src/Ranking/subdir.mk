################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Ranking/RankingManager.cpp 

OBJS += \
./src/Ranking/RankingManager.o 

CPP_DEPS += \
./src/Ranking/RankingManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/Ranking/%.o: ../src/Ranking/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I../../Dependence -I/usr/local/lua/include -I/usr/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


