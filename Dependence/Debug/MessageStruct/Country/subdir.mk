################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/Country/Country.pb.cc \
../MessageStruct/Country/CountryBoss.pb.cc \
../MessageStruct/Country/CountryOverlord.pb.cc \
../MessageStruct/Country/CountryWar.pb.cc 

OBJS += \
./MessageStruct/Country/Country.pb.o \
./MessageStruct/Country/CountryBoss.pb.o \
./MessageStruct/Country/CountryOverlord.pb.o \
./MessageStruct/Country/CountryWar.pb.o 

CC_DEPS += \
./MessageStruct/Country/Country.pb.d \
./MessageStruct/Country/CountryBoss.pb.d \
./MessageStruct/Country/CountryOverlord.pb.d \
./MessageStruct/Country/CountryWar.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/Country/%.o: ../MessageStruct/Country/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


