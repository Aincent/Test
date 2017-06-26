################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Map/CreatMapTask.cpp \
../src/Map/DuplicateMgr.cpp \
../src/Map/FindPathMgr.cpp \
../src/Map/GameDuplicate.cpp \
../src/Map/GameMarry.cpp \
../src/Map/GameVillage.cpp \
../src/Map/GameZone.cpp \
../src/Map/SceneManager.cpp \
../src/Map/Sector.cpp 

OBJS += \
./src/Map/CreatMapTask.o \
./src/Map/DuplicateMgr.o \
./src/Map/FindPathMgr.o \
./src/Map/GameDuplicate.o \
./src/Map/GameMarry.o \
./src/Map/GameVillage.o \
./src/Map/GameZone.o \
./src/Map/SceneManager.o \
./src/Map/Sector.o 

CPP_DEPS += \
./src/Map/CreatMapTask.d \
./src/Map/DuplicateMgr.d \
./src/Map/FindPathMgr.d \
./src/Map/GameDuplicate.d \
./src/Map/GameMarry.d \
./src/Map/GameVillage.d \
./src/Map/GameZone.d \
./src/Map/SceneManager.d \
./src/Map/Sector.d 


# Each subdirectory must supply rules for building sources it contributes
src/Map/%.o: ../src/Map/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


