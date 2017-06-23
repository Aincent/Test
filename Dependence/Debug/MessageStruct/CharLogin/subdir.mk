################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../MessageStruct/CharLogin/CJobLucky.pb.cc \
../MessageStruct/CharLogin/CharBaseInfo.pb.cc \
../MessageStruct/CharLogin/Chat.pb.cc \
../MessageStruct/CharLogin/ChristmasDB.pb.cc \
../MessageStruct/CharLogin/CreateCharactor.pb.cc \
../MessageStruct/CharLogin/DanLu.pb.cc \
../MessageStruct/CharLogin/DayUpdate.pb.cc \
../MessageStruct/CharLogin/EquipMessage.pb.cc \
../MessageStruct/CharLogin/ExchangMessage.pb.cc \
../MessageStruct/CharLogin/GetCharID.pb.cc \
../MessageStruct/CharLogin/GoodsMessage.pb.cc \
../MessageStruct/CharLogin/Horse.pb.cc \
../MessageStruct/CharLogin/LoginGatePacket.pb.cc \
../MessageStruct/CharLogin/LucklyTurntable.pb.cc \
../MessageStruct/CharLogin/MagicWeapon.pb.cc \
../MessageStruct/CharLogin/Mail.pb.cc \
../MessageStruct/CharLogin/MapInstanceProto.pb.cc \
../MessageStruct/CharLogin/MoneyShop.pb.cc \
../MessageStruct/CharLogin/Monitor.pb.cc \
../MessageStruct/CharLogin/NpcTrade.pb.cc \
../MessageStruct/CharLogin/PlayerInfo.pb.cc \
../MessageStruct/CharLogin/PlayerInfo2Path.pb.cc \
../MessageStruct/CharLogin/PlayerVipProto.pb.cc \
../MessageStruct/CharLogin/Ranking.pb.cc \
../MessageStruct/CharLogin/Recharge.pb.cc \
../MessageStruct/CharLogin/ReqCreateRole.pb.cc \
../MessageStruct/CharLogin/RetCharLoginInfo.pb.cc \
../MessageStruct/CharLogin/Smith.pb.cc \
../MessageStruct/CharLogin/Vitality.pb.cc \
../MessageStruct/CharLogin/WorshipMessage.pb.cc 

OBJS += \
./MessageStruct/CharLogin/CJobLucky.pb.o \
./MessageStruct/CharLogin/CharBaseInfo.pb.o \
./MessageStruct/CharLogin/Chat.pb.o \
./MessageStruct/CharLogin/ChristmasDB.pb.o \
./MessageStruct/CharLogin/CreateCharactor.pb.o \
./MessageStruct/CharLogin/DanLu.pb.o \
./MessageStruct/CharLogin/DayUpdate.pb.o \
./MessageStruct/CharLogin/EquipMessage.pb.o \
./MessageStruct/CharLogin/ExchangMessage.pb.o \
./MessageStruct/CharLogin/GetCharID.pb.o \
./MessageStruct/CharLogin/GoodsMessage.pb.o \
./MessageStruct/CharLogin/Horse.pb.o \
./MessageStruct/CharLogin/LoginGatePacket.pb.o \
./MessageStruct/CharLogin/LucklyTurntable.pb.o \
./MessageStruct/CharLogin/MagicWeapon.pb.o \
./MessageStruct/CharLogin/Mail.pb.o \
./MessageStruct/CharLogin/MapInstanceProto.pb.o \
./MessageStruct/CharLogin/MoneyShop.pb.o \
./MessageStruct/CharLogin/Monitor.pb.o \
./MessageStruct/CharLogin/NpcTrade.pb.o \
./MessageStruct/CharLogin/PlayerInfo.pb.o \
./MessageStruct/CharLogin/PlayerInfo2Path.pb.o \
./MessageStruct/CharLogin/PlayerVipProto.pb.o \
./MessageStruct/CharLogin/Ranking.pb.o \
./MessageStruct/CharLogin/Recharge.pb.o \
./MessageStruct/CharLogin/ReqCreateRole.pb.o \
./MessageStruct/CharLogin/RetCharLoginInfo.pb.o \
./MessageStruct/CharLogin/Smith.pb.o \
./MessageStruct/CharLogin/Vitality.pb.o \
./MessageStruct/CharLogin/WorshipMessage.pb.o 

CC_DEPS += \
./MessageStruct/CharLogin/CJobLucky.pb.d \
./MessageStruct/CharLogin/CharBaseInfo.pb.d \
./MessageStruct/CharLogin/Chat.pb.d \
./MessageStruct/CharLogin/ChristmasDB.pb.d \
./MessageStruct/CharLogin/CreateCharactor.pb.d \
./MessageStruct/CharLogin/DanLu.pb.d \
./MessageStruct/CharLogin/DayUpdate.pb.d \
./MessageStruct/CharLogin/EquipMessage.pb.d \
./MessageStruct/CharLogin/ExchangMessage.pb.d \
./MessageStruct/CharLogin/GetCharID.pb.d \
./MessageStruct/CharLogin/GoodsMessage.pb.d \
./MessageStruct/CharLogin/Horse.pb.d \
./MessageStruct/CharLogin/LoginGatePacket.pb.d \
./MessageStruct/CharLogin/LucklyTurntable.pb.d \
./MessageStruct/CharLogin/MagicWeapon.pb.d \
./MessageStruct/CharLogin/Mail.pb.d \
./MessageStruct/CharLogin/MapInstanceProto.pb.d \
./MessageStruct/CharLogin/MoneyShop.pb.d \
./MessageStruct/CharLogin/Monitor.pb.d \
./MessageStruct/CharLogin/NpcTrade.pb.d \
./MessageStruct/CharLogin/PlayerInfo.pb.d \
./MessageStruct/CharLogin/PlayerInfo2Path.pb.d \
./MessageStruct/CharLogin/PlayerVipProto.pb.d \
./MessageStruct/CharLogin/Ranking.pb.d \
./MessageStruct/CharLogin/Recharge.pb.d \
./MessageStruct/CharLogin/ReqCreateRole.pb.d \
./MessageStruct/CharLogin/RetCharLoginInfo.pb.d \
./MessageStruct/CharLogin/Smith.pb.d \
./MessageStruct/CharLogin/Vitality.pb.d \
./MessageStruct/CharLogin/WorshipMessage.pb.d 


# Each subdirectory must supply rules for building sources it contributes
MessageStruct/CharLogin/%.o: ../MessageStruct/CharLogin/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -I../../../Common/include -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


