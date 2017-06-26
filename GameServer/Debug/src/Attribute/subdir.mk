################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Attribute/AttributeCreature.cpp \
../src/Attribute/AttributeGirlWar.cpp \
../src/Attribute/AttributeManage.cpp \
../src/Attribute/AttributeMonster.cpp \
../src/Attribute/AttributePlayer.cpp 

OBJS += \
./src/Attribute/AttributeCreature.o \
./src/Attribute/AttributeGirlWar.o \
./src/Attribute/AttributeManage.o \
./src/Attribute/AttributeMonster.o \
./src/Attribute/AttributePlayer.o 

CPP_DEPS += \
./src/Attribute/AttributeCreature.d \
./src/Attribute/AttributeGirlWar.d \
./src/Attribute/AttributeManage.d \
./src/Attribute/AttributeMonster.d \
./src/Attribute/AttributePlayer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Attribute/%.o: ../src/Attribute/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


