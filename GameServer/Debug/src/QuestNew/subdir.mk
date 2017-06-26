################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/QuestNew/AcceptableQuests.cpp \
../src/QuestNew/AcceptedQuest.cpp \
../src/QuestNew/CampQuest.cpp \
../src/QuestNew/Quest.cpp \
../src/QuestNew/QuestClan.cpp \
../src/QuestNew/QuestCondition.cpp \
../src/QuestNew/QuestDaily.cpp \
../src/QuestNew/QuestGuide.cpp \
../src/QuestNew/QuestLoader.cpp \
../src/QuestNew/QuestMessage.cpp \
../src/QuestNew/QuestPart.cpp \
../src/QuestNew/QuestWeek.cpp 

OBJS += \
./src/QuestNew/AcceptableQuests.o \
./src/QuestNew/AcceptedQuest.o \
./src/QuestNew/CampQuest.o \
./src/QuestNew/Quest.o \
./src/QuestNew/QuestClan.o \
./src/QuestNew/QuestCondition.o \
./src/QuestNew/QuestDaily.o \
./src/QuestNew/QuestGuide.o \
./src/QuestNew/QuestLoader.o \
./src/QuestNew/QuestMessage.o \
./src/QuestNew/QuestPart.o \
./src/QuestNew/QuestWeek.o 

CPP_DEPS += \
./src/QuestNew/AcceptableQuests.d \
./src/QuestNew/AcceptedQuest.d \
./src/QuestNew/CampQuest.d \
./src/QuestNew/Quest.d \
./src/QuestNew/QuestClan.d \
./src/QuestNew/QuestCondition.d \
./src/QuestNew/QuestDaily.d \
./src/QuestNew/QuestGuide.d \
./src/QuestNew/QuestLoader.d \
./src/QuestNew/QuestMessage.d \
./src/QuestNew/QuestPart.d \
./src/QuestNew/QuestWeek.d 


# Each subdirectory must supply rules for building sources it contributes
src/QuestNew/%.o: ../src/QuestNew/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


