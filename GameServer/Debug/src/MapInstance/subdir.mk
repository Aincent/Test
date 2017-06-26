################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MapInstance/BeyondBattleMgr.cpp \
../src/MapInstance/ChallengeMgr.cpp \
../src/MapInstance/HeroInfo.cpp \
../src/MapInstance/HeroLoader.cpp \
../src/MapInstance/InstanceMapLoader.cpp \
../src/MapInstance/InstanceMgr.cpp \
../src/MapInstance/MapInstanceMessage.cpp \
../src/MapInstance/MoneyInstanceMgr.cpp \
../src/MapInstance/PlayerChallenge.cpp 

OBJS += \
./src/MapInstance/BeyondBattleMgr.o \
./src/MapInstance/ChallengeMgr.o \
./src/MapInstance/HeroInfo.o \
./src/MapInstance/HeroLoader.o \
./src/MapInstance/InstanceMapLoader.o \
./src/MapInstance/InstanceMgr.o \
./src/MapInstance/MapInstanceMessage.o \
./src/MapInstance/MoneyInstanceMgr.o \
./src/MapInstance/PlayerChallenge.o 

CPP_DEPS += \
./src/MapInstance/BeyondBattleMgr.d \
./src/MapInstance/ChallengeMgr.d \
./src/MapInstance/HeroInfo.d \
./src/MapInstance/HeroLoader.d \
./src/MapInstance/InstanceMapLoader.d \
./src/MapInstance/InstanceMgr.d \
./src/MapInstance/MapInstanceMessage.d \
./src/MapInstance/MoneyInstanceMgr.d \
./src/MapInstance/PlayerChallenge.d 


# Each subdirectory must supply rules for building sources it contributes
src/MapInstance/%.o: ../src/MapInstance/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


