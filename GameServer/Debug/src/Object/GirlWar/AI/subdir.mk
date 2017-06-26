################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Object/GirlWar/AI/PetBrain.cpp \
../src/Object/GirlWar/AI/PetStateAttack.cpp \
../src/Object/GirlWar/AI/PetStateDead.cpp \
../src/Object/GirlWar/AI/PetStateIdle.cpp 

OBJS += \
./src/Object/GirlWar/AI/PetBrain.o \
./src/Object/GirlWar/AI/PetStateAttack.o \
./src/Object/GirlWar/AI/PetStateDead.o \
./src/Object/GirlWar/AI/PetStateIdle.o 

CPP_DEPS += \
./src/Object/GirlWar/AI/PetBrain.d \
./src/Object/GirlWar/AI/PetStateAttack.d \
./src/Object/GirlWar/AI/PetStateDead.d \
./src/Object/GirlWar/AI/PetStateIdle.d 


# Each subdirectory must supply rules for building sources it contributes
src/Object/GirlWar/AI/%.o: ../src/Object/GirlWar/AI/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


