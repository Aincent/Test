################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RewardHail/LevelReward.cpp \
../src/RewardHail/OnceReward.cpp \
../src/RewardHail/OnlineReward.cpp \
../src/RewardHail/RewardMgr.cpp \
../src/RewardHail/SignIn.cpp \
../src/RewardHail/TreasonKingReward.cpp 

OBJS += \
./src/RewardHail/LevelReward.o \
./src/RewardHail/OnceReward.o \
./src/RewardHail/OnlineReward.o \
./src/RewardHail/RewardMgr.o \
./src/RewardHail/SignIn.o \
./src/RewardHail/TreasonKingReward.o 

CPP_DEPS += \
./src/RewardHail/LevelReward.d \
./src/RewardHail/OnceReward.d \
./src/RewardHail/OnlineReward.d \
./src/RewardHail/RewardMgr.d \
./src/RewardHail/SignIn.d \
./src/RewardHail/TreasonKingReward.d 


# Each subdirectory must supply rules for building sources it contributes
src/RewardHail/%.o: ../src/RewardHail/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


