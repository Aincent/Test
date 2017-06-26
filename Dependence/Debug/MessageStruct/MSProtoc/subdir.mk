################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/MSProtoc/MS_AccountLogin.pb.cc \
../MessageStruct/MSProtoc/MS_Activities.pb.cc \
../MessageStruct/MSProtoc/MS_BranchPlayerOnlineInfo.pb.cc \
../MessageStruct/MSProtoc/MS_BugSubmit.pb.cc \
../MessageStruct/MSProtoc/MS_ClanInfo.pb.cc \
../MessageStruct/MSProtoc/MS_ExchangeGift.pb.cc \
../MessageStruct/MSProtoc/MS_Mail.pb.cc \
../MessageStruct/MSProtoc/MS_Mall.pb.cc \
../MessageStruct/MSProtoc/MS_NewActivities.pb.cc \
../MessageStruct/MSProtoc/MS_OpreationSych.pb.cc \
../MessageStruct/MSProtoc/MS_PlayerInfo.pb.cc \
../MessageStruct/MSProtoc/MS_Recharge.pb.cc \
../MessageStruct/MSProtoc/OpenIDSomething.pb.cc 

OBJS += \
./MessageStruct/MSProtoc/MS_AccountLogin.pb.o \
./MessageStruct/MSProtoc/MS_Activities.pb.o \
./MessageStruct/MSProtoc/MS_BranchPlayerOnlineInfo.pb.o \
./MessageStruct/MSProtoc/MS_BugSubmit.pb.o \
./MessageStruct/MSProtoc/MS_ClanInfo.pb.o \
./MessageStruct/MSProtoc/MS_ExchangeGift.pb.o \
./MessageStruct/MSProtoc/MS_Mail.pb.o \
./MessageStruct/MSProtoc/MS_Mall.pb.o \
./MessageStruct/MSProtoc/MS_NewActivities.pb.o \
./MessageStruct/MSProtoc/MS_OpreationSych.pb.o \
./MessageStruct/MSProtoc/MS_PlayerInfo.pb.o \
./MessageStruct/MSProtoc/MS_Recharge.pb.o \
./MessageStruct/MSProtoc/OpenIDSomething.pb.o 

CC_DEPS += \
./MessageStruct/MSProtoc/MS_AccountLogin.pb.d \
./MessageStruct/MSProtoc/MS_Activities.pb.d \
./MessageStruct/MSProtoc/MS_BranchPlayerOnlineInfo.pb.d \
./MessageStruct/MSProtoc/MS_BugSubmit.pb.d \
./MessageStruct/MSProtoc/MS_ClanInfo.pb.d \
./MessageStruct/MSProtoc/MS_ExchangeGift.pb.d \
./MessageStruct/MSProtoc/MS_Mail.pb.d \
./MessageStruct/MSProtoc/MS_Mall.pb.d \
./MessageStruct/MSProtoc/MS_NewActivities.pb.d \
./MessageStruct/MSProtoc/MS_OpreationSych.pb.d \
./MessageStruct/MSProtoc/MS_PlayerInfo.pb.d \
./MessageStruct/MSProtoc/MS_Recharge.pb.d \
./MessageStruct/MSProtoc/OpenIDSomething.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/MSProtoc/%.o: ../MessageStruct/MSProtoc/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


