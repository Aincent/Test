################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RewardHail/FileLoader/LevelRewardLoader.cpp \
../src/RewardHail/FileLoader/MicroRewardLoader.cpp \
../src/RewardHail/FileLoader/OnlineRewardLoader.cpp \
../src/RewardHail/FileLoader/SignInLoader.cpp \
../src/RewardHail/FileLoader/TotalLoginRewardLoader.cpp 

OBJS += \
./src/RewardHail/FileLoader/LevelRewardLoader.o \
./src/RewardHail/FileLoader/MicroRewardLoader.o \
./src/RewardHail/FileLoader/OnlineRewardLoader.o \
./src/RewardHail/FileLoader/SignInLoader.o \
./src/RewardHail/FileLoader/TotalLoginRewardLoader.o 

CPP_DEPS += \
./src/RewardHail/FileLoader/LevelRewardLoader.d \
./src/RewardHail/FileLoader/MicroRewardLoader.d \
./src/RewardHail/FileLoader/OnlineRewardLoader.d \
./src/RewardHail/FileLoader/SignInLoader.d \
./src/RewardHail/FileLoader/TotalLoginRewardLoader.d 


# Each subdirectory must supply rules for building sources it contributes
src/RewardHail/FileLoader/%.o: ../src/RewardHail/FileLoader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


