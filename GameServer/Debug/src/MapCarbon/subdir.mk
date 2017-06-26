################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MapCarbon/AdvancedCarbon.cpp \
../src/MapCarbon/BeyondCarbon.cpp \
../src/MapCarbon/ChallengeCarbon.cpp \
../src/MapCarbon/ClanCarbon.cpp \
../src/MapCarbon/ClanIntegralCarbon.cpp \
../src/MapCarbon/CountryBossCarbon.cpp \
../src/MapCarbon/CountryOverlordCarbon.cpp \
../src/MapCarbon/CountryWarCarbon.cpp \
../src/MapCarbon/GroupCarbon.cpp \
../src/MapCarbon/GuideCarbon.cpp \
../src/MapCarbon/HeroCarbon.cpp \
../src/MapCarbon/MarryCarbon.cpp \
../src/MapCarbon/MoneyCarbon.cpp \
../src/MapCarbon/PVPCarbon.cpp \
../src/MapCarbon/ProtectKingCarBon.cpp \
../src/MapCarbon/RobberCarbon.cpp \
../src/MapCarbon/SeabeachCarbon.cpp \
../src/MapCarbon/TreasonKingCarbon.cpp \
../src/MapCarbon/WorldBossCarbon.cpp 

OBJS += \
./src/MapCarbon/AdvancedCarbon.o \
./src/MapCarbon/BeyondCarbon.o \
./src/MapCarbon/ChallengeCarbon.o \
./src/MapCarbon/ClanCarbon.o \
./src/MapCarbon/ClanIntegralCarbon.o \
./src/MapCarbon/CountryBossCarbon.o \
./src/MapCarbon/CountryOverlordCarbon.o \
./src/MapCarbon/CountryWarCarbon.o \
./src/MapCarbon/GroupCarbon.o \
./src/MapCarbon/GuideCarbon.o \
./src/MapCarbon/HeroCarbon.o \
./src/MapCarbon/MarryCarbon.o \
./src/MapCarbon/MoneyCarbon.o \
./src/MapCarbon/PVPCarbon.o \
./src/MapCarbon/ProtectKingCarBon.o \
./src/MapCarbon/RobberCarbon.o \
./src/MapCarbon/SeabeachCarbon.o \
./src/MapCarbon/TreasonKingCarbon.o \
./src/MapCarbon/WorldBossCarbon.o 

CPP_DEPS += \
./src/MapCarbon/AdvancedCarbon.d \
./src/MapCarbon/BeyondCarbon.d \
./src/MapCarbon/ChallengeCarbon.d \
./src/MapCarbon/ClanCarbon.d \
./src/MapCarbon/ClanIntegralCarbon.d \
./src/MapCarbon/CountryBossCarbon.d \
./src/MapCarbon/CountryOverlordCarbon.d \
./src/MapCarbon/CountryWarCarbon.d \
./src/MapCarbon/GroupCarbon.d \
./src/MapCarbon/GuideCarbon.d \
./src/MapCarbon/HeroCarbon.d \
./src/MapCarbon/MarryCarbon.d \
./src/MapCarbon/MoneyCarbon.d \
./src/MapCarbon/PVPCarbon.d \
./src/MapCarbon/ProtectKingCarBon.d \
./src/MapCarbon/RobberCarbon.d \
./src/MapCarbon/SeabeachCarbon.d \
./src/MapCarbon/TreasonKingCarbon.d \
./src/MapCarbon/WorldBossCarbon.d 


# Each subdirectory must supply rules for building sources it contributes
src/MapCarbon/%.o: ../src/MapCarbon/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


