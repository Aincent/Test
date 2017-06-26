################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Object/FileLoader/AILoader.cpp \
../src/Object/FileLoader/BattleInfoLoader.cpp \
../src/Object/FileLoader/DropItemLoader.cpp \
../src/Object/FileLoader/GirlWarLoader.cpp \
../src/Object/FileLoader/MakeTypeLoader.cpp \
../src/Object/FileLoader/MonsterLoader.cpp \
../src/Object/FileLoader/NPCLoader.cpp \
../src/Object/FileLoader/NpcShop.cpp \
../src/Object/FileLoader/ScortpacksTypeDataLoader.cpp 

OBJS += \
./src/Object/FileLoader/AILoader.o \
./src/Object/FileLoader/BattleInfoLoader.o \
./src/Object/FileLoader/DropItemLoader.o \
./src/Object/FileLoader/GirlWarLoader.o \
./src/Object/FileLoader/MakeTypeLoader.o \
./src/Object/FileLoader/MonsterLoader.o \
./src/Object/FileLoader/NPCLoader.o \
./src/Object/FileLoader/NpcShop.o \
./src/Object/FileLoader/ScortpacksTypeDataLoader.o 

CPP_DEPS += \
./src/Object/FileLoader/AILoader.d \
./src/Object/FileLoader/BattleInfoLoader.d \
./src/Object/FileLoader/DropItemLoader.d \
./src/Object/FileLoader/GirlWarLoader.d \
./src/Object/FileLoader/MakeTypeLoader.d \
./src/Object/FileLoader/MonsterLoader.d \
./src/Object/FileLoader/NPCLoader.d \
./src/Object/FileLoader/NpcShop.d \
./src/Object/FileLoader/ScortpacksTypeDataLoader.d 


# Each subdirectory must supply rules for building sources it contributes
src/Object/FileLoader/%.o: ../src/Object/FileLoader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


