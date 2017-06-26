################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include UrlEncode/subdir.mk
-include StatisticBase/GlobalHandle/subdir.mk
-include StatisticBase/BehaviorHandle/subdir.mk
-include Password/subdir.mk
-include MessageStruct/WorldBoss/subdir.mk
-include MessageStruct/SystemNotice/subdir.mk
-include MessageStruct/SysGT2LS/subdir.mk
-include MessageStruct/Seabeach/subdir.mk
-include MessageStruct/RewardHail/subdir.mk
-include MessageStruct/RedEnvelope/subdir.mk
-include MessageStruct/Quest/subdir.mk
-include MessageStruct/Pay/subdir.mk
-include MessageStruct/PVP/subdir.mk
-include MessageStruct/MonitorMap/subdir.mk
-include MessageStruct/MoneyInsatence/subdir.mk
-include MessageStruct/Meridians/subdir.mk
-include MessageStruct/Marry/subdir.mk
-include MessageStruct/Market/subdir.mk
-include MessageStruct/Map/subdir.mk
-include MessageStruct/Mall/subdir.mk
-include MessageStruct/MSProtoc/subdir.mk
-include MessageStruct/InterfaceAnswer/subdir.mk
-include MessageStruct/Group/subdir.mk
-include MessageStruct/GirlWar/subdir.mk
-include MessageStruct/Friend/subdir.mk
-include MessageStruct/FestivalActive/subdir.mk
-include MessageStruct/Fashion/subdir.mk
-include MessageStruct/Escort/subdir.mk
-include MessageStruct/Daily/subdir.mk
-include MessageStruct/Country/subdir.mk
-include MessageStruct/Clan/subdir.mk
-include MessageStruct/ChristmasActivity/subdir.mk
-include MessageStruct/CharLogin/subdir.mk
-include MessageStruct/CharBattle/subdir.mk
-include MessageStruct/CharBase/subdir.mk
-include MessageStruct/CDTime/subdir.mk
-include MessageStruct/BeyondServerBattle/subdir.mk
-include MessageStruct/BaseShop/subdir.mk
-include MessageStruct/AntiAddiction/subdir.mk
-include MessageStruct/Answer/subdir.mk
-include MessageStruct/Activity/subdir.mk
-include MessageStruct/subdir.mk
-include Goods/subdir.mk
-include FileLoader/subdir.mk
-include EffectMath/subdir.mk
-include Ciconv/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libDependence.a

# Tool invocations
libDependence.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Archiver'
	ar -r  "libDependence.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(ARCHIVES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libDependence.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
