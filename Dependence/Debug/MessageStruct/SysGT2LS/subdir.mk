################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/SysGT2LS/PushConnInfo.pb.cc 

OBJS += \
./MessageStruct/SysGT2LS/PushConnInfo.pb.o 

CC_DEPS += \
./MessageStruct/SysGT2LS/PushConnInfo.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/SysGT2LS/%.o: ../MessageStruct/SysGT2LS/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


