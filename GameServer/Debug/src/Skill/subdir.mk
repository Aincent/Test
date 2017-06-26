################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Skill/BuffBase.cpp \
../src/Skill/BuffClass.cpp \
../src/Skill/BuffFactory.cpp \
../src/Skill/SkillBase.cpp \
../src/Skill/SkillClass.cpp \
../src/Skill/SkillFactory.cpp \
../src/Skill/SkillManager.cpp \
../src/Skill/SkillMessage.cpp 

OBJS += \
./src/Skill/BuffBase.o \
./src/Skill/BuffClass.o \
./src/Skill/BuffFactory.o \
./src/Skill/SkillBase.o \
./src/Skill/SkillClass.o \
./src/Skill/SkillFactory.o \
./src/Skill/SkillManager.o \
./src/Skill/SkillMessage.o 

CPP_DEPS += \
./src/Skill/BuffBase.d \
./src/Skill/BuffClass.d \
./src/Skill/BuffFactory.d \
./src/Skill/SkillBase.d \
./src/Skill/SkillClass.d \
./src/Skill/SkillFactory.d \
./src/Skill/SkillManager.d \
./src/Skill/SkillMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/Skill/%.o: ../src/Skill/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


