################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Activity/ActivityBox.cpp \
../src/Activity/ActivityMessage.cpp \
../src/Activity/ActivityMgr.cpp \
../src/Activity/AnswerMgr.cpp \
../src/Activity/OnLineAtTimeMgr.cpp \
../src/Activity/TreasonKingMessage.cpp 

OBJS += \
./src/Activity/ActivityBox.o \
./src/Activity/ActivityMessage.o \
./src/Activity/ActivityMgr.o \
./src/Activity/AnswerMgr.o \
./src/Activity/OnLineAtTimeMgr.o \
./src/Activity/TreasonKingMessage.o 

CPP_DEPS += \
./src/Activity/ActivityBox.d \
./src/Activity/ActivityMessage.d \
./src/Activity/ActivityMgr.d \
./src/Activity/AnswerMgr.d \
./src/Activity/OnLineAtTimeMgr.d \
./src/Activity/TreasonKingMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/Activity/%.o: ../src/Activity/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


