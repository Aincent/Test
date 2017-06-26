################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Map/ConfigLoader/FieldInfo.cpp \
../src/Map/ConfigLoader/FieldMonster.cpp \
../src/Map/ConfigLoader/FieldSpring.cpp \
../src/Map/ConfigLoader/MapConfig.cpp \
../src/Map/ConfigLoader/RegionManage.cpp 

OBJS += \
./src/Map/ConfigLoader/FieldInfo.o \
./src/Map/ConfigLoader/FieldMonster.o \
./src/Map/ConfigLoader/FieldSpring.o \
./src/Map/ConfigLoader/MapConfig.o \
./src/Map/ConfigLoader/RegionManage.o 

CPP_DEPS += \
./src/Map/ConfigLoader/FieldInfo.d \
./src/Map/ConfigLoader/FieldMonster.d \
./src/Map/ConfigLoader/FieldSpring.d \
./src/Map/ConfigLoader/MapConfig.d \
./src/Map/ConfigLoader/RegionManage.d 


# Each subdirectory must supply rules for building sources it contributes
src/Map/ConfigLoader/%.o: ../src/Map/ConfigLoader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


