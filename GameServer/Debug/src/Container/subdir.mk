################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Container/ArticleEquipMgr.cpp \
../src/Container/ArticleStoreHouseMgr.cpp \
../src/Container/ContainerBase.cpp \
../src/Container/ContainerMessage.cpp \
../src/Container/ParcelMgr.cpp \
../src/Container/funUse.cpp 

OBJS += \
./src/Container/ArticleEquipMgr.o \
./src/Container/ArticleStoreHouseMgr.o \
./src/Container/ContainerBase.o \
./src/Container/ContainerMessage.o \
./src/Container/ParcelMgr.o \
./src/Container/funUse.o 

CPP_DEPS += \
./src/Container/ArticleEquipMgr.d \
./src/Container/ArticleStoreHouseMgr.d \
./src/Container/ContainerBase.d \
./src/Container/ContainerMessage.d \
./src/Container/ParcelMgr.d \
./src/Container/funUse.d 


# Each subdirectory must supply rules for building sources it contributes
src/Container/%.o: ../src/Container/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


