################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/Pay/360Play.pb.cc \
../MessageStruct/Pay/58Play.pb.cc \
../MessageStruct/Pay/Connecting.pb.cc \
../MessageStruct/Pay/Tencent.pb.cc 

OBJS += \
./MessageStruct/Pay/360Play.pb.o \
./MessageStruct/Pay/58Play.pb.o \
./MessageStruct/Pay/Connecting.pb.o \
./MessageStruct/Pay/Tencent.pb.o 

CC_DEPS += \
./MessageStruct/Pay/360Play.pb.d \
./MessageStruct/Pay/58Play.pb.d \
./MessageStruct/Pay/Connecting.pb.d \
./MessageStruct/Pay/Tencent.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/Pay/%.o: ../MessageStruct/Pay/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


