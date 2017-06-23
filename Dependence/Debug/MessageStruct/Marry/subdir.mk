################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/Marry/MarryInstance.pb.cc \
../MessageStruct/Marry/MarryMessage.pb.cc \
../MessageStruct/Marry/MarryServer.pb.cc \
../MessageStruct/Marry/MarryUI.pb.cc 

OBJS += \
./MessageStruct/Marry/MarryInstance.pb.o \
./MessageStruct/Marry/MarryMessage.pb.o \
./MessageStruct/Marry/MarryServer.pb.o \
./MessageStruct/Marry/MarryUI.pb.o 

CC_DEPS += \
./MessageStruct/Marry/MarryInstance.pb.d \
./MessageStruct/Marry/MarryMessage.pb.d \
./MessageStruct/Marry/MarryServer.pb.d \
./MessageStruct/Marry/MarryUI.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/Marry/%.o: ../MessageStruct/Marry/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


