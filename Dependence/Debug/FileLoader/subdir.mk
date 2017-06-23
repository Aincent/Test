################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FileLoader/ActivitiesTimerLoader.cpp \
../FileLoader/ArenaLoader.cpp \
../FileLoader/ClanWelfareLoader.cpp \
../FileLoader/EquipSmithExLoader.cpp \
../FileLoader/EquipSmithLoader.cpp \
../FileLoader/FestivalUIFile.cpp \
../FileLoader/GameParaLoader.cpp \
../FileLoader/GodTreeFile.cpp \
../FileLoader/GoodsLoader.cpp \
../FileLoader/InterfaceAnswerLoader.cpp \
../FileLoader/InternationalLoader.cpp \
../FileLoader/MallLoader.cpp \
../FileLoader/MaterialSynthesisLoader.cpp \
../FileLoader/PayLoader.cpp \
../FileLoader/UpgradeLoader.cpp 

OBJS += \
./FileLoader/ActivitiesTimerLoader.o \
./FileLoader/ArenaLoader.o \
./FileLoader/ClanWelfareLoader.o \
./FileLoader/EquipSmithExLoader.o \
./FileLoader/EquipSmithLoader.o \
./FileLoader/FestivalUIFile.o \
./FileLoader/GameParaLoader.o \
./FileLoader/GodTreeFile.o \
./FileLoader/GoodsLoader.o \
./FileLoader/InterfaceAnswerLoader.o \
./FileLoader/InternationalLoader.o \
./FileLoader/MallLoader.o \
./FileLoader/MaterialSynthesisLoader.o \
./FileLoader/PayLoader.o \
./FileLoader/UpgradeLoader.o 

CPP_DEPS += \
./FileLoader/ActivitiesTimerLoader.d \
./FileLoader/ArenaLoader.d \
./FileLoader/ClanWelfareLoader.d \
./FileLoader/EquipSmithExLoader.d \
./FileLoader/EquipSmithLoader.d \
./FileLoader/FestivalUIFile.d \
./FileLoader/GameParaLoader.d \
./FileLoader/GodTreeFile.d \
./FileLoader/GoodsLoader.d \
./FileLoader/InterfaceAnswerLoader.d \
./FileLoader/InternationalLoader.d \
./FileLoader/MallLoader.d \
./FileLoader/MaterialSynthesisLoader.d \
./FileLoader/PayLoader.d \
./FileLoader/UpgradeLoader.d 


# Each subdirectory must supply rules for building sources it contributes
FileLoader/%.o: ../FileLoader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


