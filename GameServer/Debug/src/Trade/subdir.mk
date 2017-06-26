################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Trade/BaseShop.cpp \
../src/Trade/BaseShopMessage.cpp \
../src/Trade/Exchang.cpp \
../src/Trade/ExchangeMessage.cpp \
../src/Trade/Player_exchange.cpp \
../src/Trade/ShopLoader.cpp \
../src/Trade/ShopMessage.cpp \
../src/Trade/shop.cpp 

OBJS += \
./src/Trade/BaseShop.o \
./src/Trade/BaseShopMessage.o \
./src/Trade/Exchang.o \
./src/Trade/ExchangeMessage.o \
./src/Trade/Player_exchange.o \
./src/Trade/ShopLoader.o \
./src/Trade/ShopMessage.o \
./src/Trade/shop.o 

CPP_DEPS += \
./src/Trade/BaseShop.d \
./src/Trade/BaseShopMessage.d \
./src/Trade/Exchang.d \
./src/Trade/ExchangeMessage.d \
./src/Trade/Player_exchange.d \
./src/Trade/ShopLoader.d \
./src/Trade/ShopMessage.d \
./src/Trade/shop.d 


# Each subdirectory must supply rules for building sources it contributes
src/Trade/%.o: ../src/Trade/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


