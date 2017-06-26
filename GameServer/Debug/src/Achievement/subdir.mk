################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Achievement/Achievement.cpp \
../src/Achievement/AchievementLoader.cpp \
../src/Achievement/AchievementMessage.cpp \
../src/Achievement/PlayerTitle.cpp 

OBJS += \
./src/Achievement/Achievement.o \
./src/Achievement/AchievementLoader.o \
./src/Achievement/AchievementMessage.o \
./src/Achievement/PlayerTitle.o 

CPP_DEPS += \
./src/Achievement/Achievement.d \
./src/Achievement/AchievementLoader.d \
./src/Achievement/AchievementMessage.d \
./src/Achievement/PlayerTitle.d 


# Each subdirectory must supply rules for building sources it contributes
src/Achievement/%.o: ../src/Achievement/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


