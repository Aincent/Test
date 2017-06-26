################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CountryRedEnvelope/CountryRedEnvelopeLoader.cpp \
../src/CountryRedEnvelope/CountryRedEnvelopeMessage.cpp \
../src/CountryRedEnvelope/CountryRedEnvelopeMgr.cpp 

OBJS += \
./src/CountryRedEnvelope/CountryRedEnvelopeLoader.o \
./src/CountryRedEnvelope/CountryRedEnvelopeMessage.o \
./src/CountryRedEnvelope/CountryRedEnvelopeMgr.o 

CPP_DEPS += \
./src/CountryRedEnvelope/CountryRedEnvelopeLoader.d \
./src/CountryRedEnvelope/CountryRedEnvelopeMessage.d \
./src/CountryRedEnvelope/CountryRedEnvelopeMgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/CountryRedEnvelope/%.o: ../src/CountryRedEnvelope/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


