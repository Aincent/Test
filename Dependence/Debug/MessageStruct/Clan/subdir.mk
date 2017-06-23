################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/Clan/Clan.pb.cc \
../MessageStruct/Clan/ClanBattle.pb.cc \
../MessageStruct/Clan/ClanIntegral.pb.cc \
../MessageStruct/Clan/GodTreepro.pb.cc 

OBJS += \
./MessageStruct/Clan/Clan.pb.o \
./MessageStruct/Clan/ClanBattle.pb.o \
./MessageStruct/Clan/ClanIntegral.pb.o \
./MessageStruct/Clan/GodTreepro.pb.o 

CC_DEPS += \
./MessageStruct/Clan/Clan.pb.d \
./MessageStruct/Clan/ClanBattle.pb.d \
./MessageStruct/Clan/ClanIntegral.pb.d \
./MessageStruct/Clan/GodTreepro.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/Clan/%.o: ../MessageStruct/Clan/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


