################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Skill/Effect/BackHurtEffect.cpp \
../src/Skill/Effect/CountsAddBuffEffect.cpp \
../src/Skill/Effect/CountsRemoveEffect.cpp \
../src/Skill/Effect/CritBackBloodEffect.cpp \
../src/Skill/Effect/Effect.cpp \
../src/Skill/Effect/EffectMath.cpp \
../src/Skill/Effect/FixedShield.cpp \
../src/Skill/Effect/MoveEffect.cpp 

OBJS += \
./src/Skill/Effect/BackHurtEffect.o \
./src/Skill/Effect/CountsAddBuffEffect.o \
./src/Skill/Effect/CountsRemoveEffect.o \
./src/Skill/Effect/CritBackBloodEffect.o \
./src/Skill/Effect/Effect.o \
./src/Skill/Effect/EffectMath.o \
./src/Skill/Effect/FixedShield.o \
./src/Skill/Effect/MoveEffect.o 

CPP_DEPS += \
./src/Skill/Effect/BackHurtEffect.d \
./src/Skill/Effect/CountsAddBuffEffect.d \
./src/Skill/Effect/CountsRemoveEffect.d \
./src/Skill/Effect/CritBackBloodEffect.d \
./src/Skill/Effect/Effect.d \
./src/Skill/Effect/EffectMath.d \
./src/Skill/Effect/FixedShield.d \
./src/Skill/Effect/MoveEffect.d 


# Each subdirectory must supply rules for building sources it contributes
src/Skill/Effect/%.o: ../src/Skill/Effect/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


