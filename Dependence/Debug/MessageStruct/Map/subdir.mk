################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/Map/ChangeMap.pb.cc \
../MessageStruct/Map/GS2GTAllMap.pb.cc \
../MessageStruct/Map/HeroInstance.pb.cc \
../MessageStruct/Map/SynchPath.pb.cc 

OBJS += \
./MessageStruct/Map/ChangeMap.pb.o \
./MessageStruct/Map/GS2GTAllMap.pb.o \
./MessageStruct/Map/HeroInstance.pb.o \
./MessageStruct/Map/SynchPath.pb.o 

CC_DEPS += \
./MessageStruct/Map/ChangeMap.pb.d \
./MessageStruct/Map/GS2GTAllMap.pb.d \
./MessageStruct/Map/HeroInstance.pb.d \
./MessageStruct/Map/SynchPath.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/Map/%.o: ../MessageStruct/Map/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


