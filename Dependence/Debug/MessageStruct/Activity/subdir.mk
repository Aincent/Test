################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/Activity/ActivityBox.pb.cc \
../MessageStruct/Activity/OnLineAtTime.pb.cc \
../MessageStruct/Activity/ProtectKing.pb.cc \
../MessageStruct/Activity/TreasonKingInfo.pb.cc \
../MessageStruct/Activity/YellowVip.pb.cc 

OBJS += \
./MessageStruct/Activity/ActivityBox.pb.o \
./MessageStruct/Activity/OnLineAtTime.pb.o \
./MessageStruct/Activity/ProtectKing.pb.o \
./MessageStruct/Activity/TreasonKingInfo.pb.o \
./MessageStruct/Activity/YellowVip.pb.o 

CC_DEPS += \
./MessageStruct/Activity/ActivityBox.pb.d \
./MessageStruct/Activity/OnLineAtTime.pb.d \
./MessageStruct/Activity/ProtectKing.pb.d \
./MessageStruct/Activity/TreasonKingInfo.pb.d \
./MessageStruct/Activity/YellowVip.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/Activity/%.o: ../MessageStruct/Activity/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


