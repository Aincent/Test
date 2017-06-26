################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Activity/FileLoader/ActivityBoxLoader.cpp \
../src/Activity/FileLoader/AnswerLoader.cpp \
../src/Activity/FileLoader/OnLineAtTimeLoader.cpp \
../src/Activity/FileLoader/ProtectedKingLoader.cpp \
../src/Activity/FileLoader/TreasonKingLoader.cpp \
../src/Activity/FileLoader/YellowVipLoader.cpp 

OBJS += \
./src/Activity/FileLoader/ActivityBoxLoader.o \
./src/Activity/FileLoader/AnswerLoader.o \
./src/Activity/FileLoader/OnLineAtTimeLoader.o \
./src/Activity/FileLoader/ProtectedKingLoader.o \
./src/Activity/FileLoader/TreasonKingLoader.o \
./src/Activity/FileLoader/YellowVipLoader.o 

CPP_DEPS += \
./src/Activity/FileLoader/ActivityBoxLoader.d \
./src/Activity/FileLoader/AnswerLoader.d \
./src/Activity/FileLoader/OnLineAtTimeLoader.d \
./src/Activity/FileLoader/ProtectedKingLoader.d \
./src/Activity/FileLoader/TreasonKingLoader.d \
./src/Activity/FileLoader/YellowVipLoader.d 


# Each subdirectory must supply rules for building sources it contributes
src/Activity/FileLoader/%.o: ../src/Activity/FileLoader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


