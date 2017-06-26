################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/AchievementProto.pb.cc \
../MessageStruct/DestinationProto.pb.cc \
../MessageStruct/QuickMessage.pb.cc \
../MessageStruct/ServerReturn2Int.pb.cc \
../MessageStruct/ServerReturn3Int.pb.cc \
../MessageStruct/ServerReturn4Int.pb.cc \
../MessageStruct/ServerReturn5Int.pb.cc \
../MessageStruct/ServerReturnBool.pb.cc \
../MessageStruct/ServerReturnChar.pb.cc \
../MessageStruct/ServerReturnInt.pb.cc \
../MessageStruct/ServerReturnIntChar.pb.cc \
../MessageStruct/StatisticInfo.pb.cc \
../MessageStruct/SynchMessage.pb.cc 

OBJS += \
./MessageStruct/AchievementProto.pb.o \
./MessageStruct/DestinationProto.pb.o \
./MessageStruct/QuickMessage.pb.o \
./MessageStruct/ServerReturn2Int.pb.o \
./MessageStruct/ServerReturn3Int.pb.o \
./MessageStruct/ServerReturn4Int.pb.o \
./MessageStruct/ServerReturn5Int.pb.o \
./MessageStruct/ServerReturnBool.pb.o \
./MessageStruct/ServerReturnChar.pb.o \
./MessageStruct/ServerReturnInt.pb.o \
./MessageStruct/ServerReturnIntChar.pb.o \
./MessageStruct/StatisticInfo.pb.o \
./MessageStruct/SynchMessage.pb.o 

CC_DEPS += \
./MessageStruct/AchievementProto.pb.d \
./MessageStruct/DestinationProto.pb.d \
./MessageStruct/QuickMessage.pb.d \
./MessageStruct/ServerReturn2Int.pb.d \
./MessageStruct/ServerReturn3Int.pb.d \
./MessageStruct/ServerReturn4Int.pb.d \
./MessageStruct/ServerReturn5Int.pb.d \
./MessageStruct/ServerReturnBool.pb.d \
./MessageStruct/ServerReturnChar.pb.d \
./MessageStruct/ServerReturnInt.pb.d \
./MessageStruct/ServerReturnIntChar.pb.d \
./MessageStruct/StatisticInfo.pb.d \
./MessageStruct/SynchMessage.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/%.o: ../MessageStruct/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


