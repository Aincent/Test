################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/TradeMgr/subdir.mk
-include src/Ranking/subdir.mk
-include src/Monitor/subdir.mk
-include src/Mail/subdir.mk
-include src/InstanceMap/subdir.mk
-include src/GoodsMgr/subdir.mk
-include src/GT_MSOpreation/subdir.mk
-include src/FriendMgr/subdir.mk
-include src/CommonMessage/subdir.mk
-include src/Chat/subdir.mk
-include src/CharactorMgr/MapLoader/subdir.mk
-include src/CharactorMgr/subdir.mk
-include src/CharBattle/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ../../../Bin_Mob/GateServer_d

# Tool invocations
../../../Bin_Mob/GateServer_d: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ -L/usr/local/lua/lib -L../../Dependence/Debug -L/usr/lib64/mysql -L/usr/local/protobuf/lib -L../../../Common/lib/CentOS_6_5_64 -o "../../../Bin_Mob/GateServer_d" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS) ../../../Bin_Mob/GateServer_d
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
