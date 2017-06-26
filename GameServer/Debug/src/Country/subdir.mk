################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Country/CountryOverlordCarbonLoader.cpp \
../src/Country/GsCountryMgr.cpp \
../src/Country/GsCountryWarLoader.cpp \
../src/Country/GsCountryWarMgr.cpp \
../src/Country/playerCampShopMgr.cpp 

OBJS += \
./src/Country/CountryOverlordCarbonLoader.o \
./src/Country/GsCountryMgr.o \
./src/Country/GsCountryWarLoader.o \
./src/Country/GsCountryWarMgr.o \
./src/Country/playerCampShopMgr.o 

CPP_DEPS += \
./src/Country/CountryOverlordCarbonLoader.d \
./src/Country/GsCountryMgr.d \
./src/Country/GsCountryWarLoader.d \
./src/Country/GsCountryWarMgr.d \
./src/Country/playerCampShopMgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/Country/%.o: ../src/Country/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DTEST -DUSE_GM -DUSE_MEMORY_POOL -DUSE_MEMORY_LEAKS -DDEBUG -I/usr/local/lua/include -I../../Dependence -I../../../Common/include -I/usr/include/mysql/ -I/usr/local/protobuf/include -I"/data/game/cx/Server/ybtx/Server_Mob/Dependence/MessageStruct" -I../../TencentSDK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


