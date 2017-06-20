################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CharactorMgr/CharSynch.cpp \
../src/CharactorMgr/CharactorInfo.cpp \
../src/CharactorMgr/CharactorLogin.cpp 

OBJS += \
./src/CharactorMgr/CharSynch.o \
./src/CharactorMgr/CharactorInfo.o \
./src/CharactorMgr/CharactorLogin.o 

CPP_DEPS += \
./src/CharactorMgr/CharSynch.d \
./src/CharactorMgr/CharactorInfo.d \
./src/CharactorMgr/CharactorLogin.d 


# Each subdirectory must supply rules for building sources it contributes
src/CharactorMgr/%.o: ../src/CharactorMgr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I../../Dependence -I/usr/local/lua/include -I/usr/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


