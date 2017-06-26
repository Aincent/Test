################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Worship/subdir.mk
-include src/WorldBoss/subdir.mk
-include src/Vitality/VitalityLoader/subdir.mk
-include src/Vitality/subdir.mk
-include src/VIP/subdir.mk
-include src/Trade/subdir.mk
-include src/SysSetting/subdir.mk
-include src/StatisticMgr/subdir.mk
-include src/Skill/SkillLoader/subdir.mk
-include src/Skill/Effect/subdir.mk
-include src/Skill/subdir.mk
-include src/Settings/subdir.mk
-include src/Seabeach/subdir.mk
-include src/RewardHail/FileLoader/subdir.mk
-include src/RewardHail/subdir.mk
-include src/Recharge/subdir.mk
-include src/QuestNew/subdir.mk
-include src/PVP/subdir.mk
-include src/Order/subdir.mk
-include src/Object/PlayerRegion/subdir.mk
-include src/Object/Player/subdir.mk
-include src/Object/NPC/subdir.mk
-include src/Object/Monster/subdir.mk
-include src/Object/ItemDrop/subdir.mk
-include src/Object/GirlWar/AI/subdir.mk
-include src/Object/GirlWar/subdir.mk
-include src/Object/FileLoader/subdir.mk
-include src/Object/AI/subdir.mk
-include src/Object/subdir.mk
-include src/MonsterExpParam/subdir.mk
-include src/MoneyShop/subdir.mk
-include src/Meridians/FileLoader/subdir.mk
-include src/Meridians/subdir.mk
-include src/Meditation/subdir.mk
-include src/Marry/MarryLoader/subdir.mk
-include src/Marry/subdir.mk
-include src/MapInstance/CrossSvrCompetitionLoader/subdir.mk
-include src/MapInstance/subdir.mk
-include src/MapCarbon/subdir.mk
-include src/Map/ReflushManage/subdir.mk
-include src/Map/ConfigLoader/subdir.mk
-include src/Map/subdir.mk
-include src/ManagerServerMgr/subdir.mk
-include src/Mall/subdir.mk
-include src/Mail/subdir.mk
-include src/MagicWeapon/MagicLoader/subdir.mk
-include src/MagicWeapon/subdir.mk
-include src/Magic/subdir.mk
-include src/MSOpreation/subdir.mk
-include src/LucklyTurntable/subdir.mk
-include src/Horse/HorseLoader/subdir.mk
-include src/Horse/subdir.mk
-include src/HeartMagic/subdir.mk
-include src/GuideFileLoader/subdir.mk
-include src/Group/subdir.mk
-include src/GMInterfaceAnswerMessage/subdir.mk
-include src/Fighting/subdir.mk
-include src/FestivalUI/subdir.mk
-include src/Festival/subdir.mk
-include src/Fashion/FashionLoader/subdir.mk
-include src/Fashion/subdir.mk
-include src/ExtraAtribute/subdir.mk
-include src/EscortEx/FileLoader/subdir.mk
-include src/EscortEx/subdir.mk
-include src/Escort/subdir.mk
-include src/EquipSmith/MaterialSynthesis/subdir.mk
-include src/EquipSmith/EquipSmithEx/subdir.mk
-include src/EquipSmith/EquipSmith/subdir.mk
-include src/Distortion/subdir.mk
-include src/Destination/subdir.mk
-include src/DanLu/subdir.mk
-include src/Daily/subdir.mk
-include src/CountryRedEnvelope/subdir.mk
-include src/Country/subdir.mk
-include src/Counter/subdir.mk
-include src/Container/ContailerLoader/subdir.mk
-include src/Container/subdir.mk
-include src/ConfigLoader/subdir.mk
-include src/Clan/ClanLoader/subdir.mk
-include src/Clan/subdir.mk
-include src/Chat/subdir.mk
-include src/CJobLucky/CJobLuckyLoader/subdir.mk
-include src/CJobLucky/subdir.mk
-include src/CDTime/subdir.mk
-include src/Attribute/subdir.mk
-include src/Arena/subdir.mk
-include src/AntiAddictionSystem/FileLoader/subdir.mk
-include src/AntiAddictionSystem/subdir.mk
-include src/Activity/FileLoader/subdir.mk
-include src/Activity/subdir.mk
-include src/ActiveUIHighLight/HintLoader/subdir.mk
-include src/ActiveUIHighLight/subdir.mk
-include src/Achievement/subdir.mk
-include src/subdir.mk
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
all: ../../../Bin_Mob/GameServer_d

# Tool invocations
../../../Bin_Mob/GameServer_d: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++ -L/usr/local/lua/lib -L../../../Common/lib/CentOS_6_5_64 -L../../Dependence/Debug -L/usr/lib64/mysql -L/usr/local/protobuf/lib -L../../TencentSDK/lib6.5.64 -o "../../../Bin_Mob/GameServer_d" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ../../../Bin_Mob/GameServer_d
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
