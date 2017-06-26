/*
 * ServiceMain.cpp
 *
 *  Created on: 2013��9��12��
 *      Author: helh
 */

#include "ServiceMain.h"
#include <signal.h>
#include "QuickMessage.h"
#include "./Log/Logger.h"
#include "./Network/Message_Facade.h"
#include "util.h"
#include "./Network/NetWorkConfig.h"
#include "ServerManager.h"
#include "Map/ConfigLoader/MapConfig.h"
#include "Map/SceneManager.h"
#include "./Object/Player/PlayerMessage.h"
#include "./Object/FileLoader/Npcshop.h"
#include "./Object/FileLoader/NPCLoader.h"
#include "./Object/FileLoader/MonsterLoader.h"
#include "./Object/FileLoader/AILoader.h"
#include "./Object/FileLoader/BattleInfoLoader.h"
#include "./QuestNew/QuestLoader.h"
#include "FileLoader/UpgradeLoader.h"
#include "./Skill/SkillLoader/SkillLoader.h"
//#include "./Quest/FileLoader/DailyTaskLoader.h"
#include <time.h>
#include "Timer/TimerInterface.h"
#include "./Skill/SkillMessage.h"
#include "./Skill/SkillFactory.h"
#include "./Skill/BuffFactory.h"
#include "Map/CreateMapTask.h"
#include "Thread/ThreadPool.h"
#include "FileLoader/GoodsLoader.h"
#include "./FileLoader/MallLoader.h"
#include "./Object/FileLoader/MakeTypeLoader.h"
#include "./Object/FileLoader/ScortpacksTypeDataLoader.h"
#include "./Object/FileLoader/DropItemLoader.h"
#include "./Container/ContainerMessage.h"
#include "./Object/ItemDrop/DropItemMgr.h"
#include "EquipSmith/EquipSmithEx/EquipSmithExMgr.h"
#include "FileLoader/EquipSmithExLoader.h"
#include "./Horse/HorseLoader/HorseLoader.h"
#include "./Horse/HorseMessage.h"
#include "./Trade/shop.h"
#include "./Trade/ShopMessage.h"
#include "./Trade/ExchangeMessage.h"
#include "./Chat/ChatMgr.h"
#include "./MapInstance/ChallengeMgr.h"
#include "./MapInstance/HeroLoader.h"

#include "./Mail/MailMessage.h"
#include "./Mall/MallMessage.h"
#include "./MapInstance/InstanceMapLoader.h"
//#include "Quest/FileLoader/ChapterLoader.h"
#include "Meridians/MeridianMgr.h"
#include "Meridians/FileLoader/MeridianLoader.h"
#include "./Map/FindPathMgr.h"
#include "MapInstance/MapInstanceMessage.h"
#include "Settings/SettingsMgr.h"
#include "./Destination/Destination.h"
#include "./Destination/DestinationLoader.h"
#include "./Destination/DestinationMessage.h"
#include "./Daily/Daily.h"
#include "./Daily/DailyLoader.h"
#include "./Daily/DailyMgr.h"
#include "./Achievement/Achievement.h"
#include "./Achievement/AchievementLoader.h"
#include "./Achievement/AchievementMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "RewardHail/FileLoader/LevelRewardLoader.h"
#include "RewardHail/FileLoader/OnlineRewardLoader.h"
#include "RewardHail/FileLoader/SignInLoader.h"
#include "RewardHail/RewardMgr.h"
#include "Daily/DailyMgr.h"
#include "Memory/MemAllocator.h"
#include "WorldBoss/WorldBoss.h"
#include "EscortEx/FileLoader/EscortFileLoader.h"
#include "EscortEx/EscortMgr.h"
#include "EscortEx/EscortMessage.h"
#include "MagicWeapon/MagicLoader/MagicLoader.h"
#include "./MagicWeapon/MagicMessage.h"
#include "Magic/SMagicMessage.h"
#include "HeartMagic/HeartMagicMessage.h"
#include "Magic/SMagicLoader.h"
#include "HeartMagic/HeartMagicLoader.h"

#include "CJobLucky/CJobLuckyLoader/CJobLuckyLoader.h"
#include "CJobLucky/CJobMessage.h"
#include "Object/ObjectFactory.h"
#include "DanLu/DanLuLoader.h"
#include "DanLu/DanLuMgr.h"
#include "DanLu/DanLuMessager.h"
#include "Clan/ClanLoader/ClanLoader.h"
#include "Clan/ClanMessage.h"

#include "MoneyShop/MoneyShopMgr.h"
#include "MoneyShop/MoneyShopMessage.h"
#include "MoneyShop/MoneyShopLoader.h"

#include "VIP/VipLoader.h"
#include "VIP/VIPMessage.h"

#include "LucklyTurntable/LucklyTurntableMessage.h"
#include "LucklyTurntable/LucklyTurntableLoader.h"

#include "AntiAddictionSystem/FileLoader/AntiAddictionSystemLoader.h"

#include "RewardHail/FileLoader/TotalLoginRewardLoader.h"

#include "StatisticMgr/StatisticMgr.h"

#include "QuestNew/QuestMessage.h"

#include "MSOpreation/MSopreationManager.h"

#include "./ConfigLoader/MaskWordConfig.h"

#include "./AntiAddictionSystem/AntiAddictionMessage.h"
#include "./Activity/FileLoader/ProtectedKingLoader.h"

#include "Activity/ActivityMessage.h"
#include "Activity/TreasonKingMessage.h"
#include "Activity/FileLoader/TreasonKingLoader.h"
#include "./Object/FileLoader/GirlWarLoader.h"
#include "Object/GirlWar/GirlWarMessage.h"
#include "Container/ContailerLoader/ContailerLoader.h"

#include "MapInstance/CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"

#include "./Group/GroupMessage.h"
#include "./Group/NewGroupMgr.h"
#include "./Map/DuplicateMgr.h"

#include "./ManagerServerMgr/ManagerServerMgr.h"
#include "Fashion/FashionLoader/FashionLoader.h"
#include "Fashion/FashionMessage.h"
#include "Vitality/VitalityLoader/VitalityLoader.h"
#include "Vitality/VitalityMessage.h"
#include "Marry/MarryMessage.h"
#include "Marry/MarryLoader/MarryLoader.h"
#include "./Object/Player/FriendManage.h"
#include "./Meditation/MeditationMgr.h"

#include "./Country/GsCountryMgr.h"
#include "SysSetting/SysSettingLoader.h"

#include "./Festival/FestivalActive.h"
#include "./Country/GsCountryWarMgr.h"
#include "./Country/GsCountryWarLoader.h"
#include "CountryRedEnvelope/CountryRedEnvelopeMessage.h"
#include "CountryRedEnvelope/CountryRedEnvelopeLoader.h"

#include "ExtraAtribute/ExtraAtributeLoader.h"

#include "FileLoader/GameParaLoader.h"
#include "Attribute/AttributeManage.h"
#include "Activity/FileLoader/YellowVipLoader.h"
#include "Recharge/RechargeMessage.h"
#include "FileLoader/PayLoader.h"
#include "FileLoader/InterfaceAnswerLoader.h"
#include "GMInterfaceAnswerMessage/GMInterfaceAnswerMessage.h"
#include "FileLoader/ActivitiesTimerLoader.h"
#include "Activity/ActivityBox.h"
#include "Activity/FileLoader/ActivityBoxLoader.h"
#include "Object/Player/FriendManage.h"
#include "MapInstance/MoneyInstanceMgr.h"
#include "PVP/PVPMgr.h"
#include "EquipSmith/MaterialSynthesis/CMeterialSynMessage.h"
#include "FileLoader/MaterialSynthesisLoader.h"
#include "RewardHail/FileLoader/MicroRewardLoader.h"
#include "Marry/MarryLoader/MarryInstanceLoader.h"
#include "FileLoader/ArenaLoader.h"
#include "Seabeach/SeabeachMgr.h"
#include "Map/ConfigLoader/RegionManage.h"
#include "FileLoader/FestivalUIFile.h"
#include "FestivalUI/FestivalUIMessage.h"
#include "MonsterExpParam/MonsterExpParam.h"
#include "GuideFileLoader/GuideLoader.h"
#include "Clan/ClanLoader/RobberLoader.h"
#include "Map/ReflushManage/ReflushFile.h"
#include "Activity/FileLoader/OnLineAtTimeLoader.h"
#include "Activity/OnLineAtTimeMgr.h"

#include "FileLoader/ClanWelfareLoader.h"
#include "ActiveUIHighLight/HintLoader/HintLoader.h"
#include "FileLoader/GodTreeFile.h"
#include "Arena/ArenaMessage.h"
#include "Trade/BaseShopMessage.h"
#include "Trade/ShopLoader.h"
#include "Activity/FileLoader/AnswerLoader.h"

bool CServiceMain::m_flag = false;

CServiceMain::CServiceMain()
{
}

CServiceMain::~CServiceMain()
{
}

int CServiceMain::Init()
{
	std::string strFileFullPath;
	std::string strFullPath;

	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	strFullPath = strFileFullPath + "Config/GameServerConfig.lua";

	if(m_conf.LoadConfig(strFullPath))
	{
		return -1;
	}

	return 0;
}


int CServiceMain::InitFile()
{
	std::string strFileFullPath;

	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}


	int gamePara = GameParaLoader::GetInstance()->Init(strFileFullPath);
	if(gamePara)
	{
		LOG_ERROR(FILEINFO, "load game para lua error");

		return gamePara;
	}
	//���ص�ͼ��Ϣ
	int res = CMapConfig::GetInstance()->LoadMap(strFileFullPath);
	if(res)
	{
		LOG_ERROR(FILEINFO, "load map config error");

		return res;
	}

	//�������ս�����ￄ1�7
	string charBattleAtt = strFileFullPath + "Data/";
	if(BattleInfoLoader::GetInstance()->Init(charBattleAtt))
	{
		LOG_ERROR(FILEINFO, "load charactor attribute config error");

		return -1;
	}

	//����npc
	string npc = strFileFullPath + "Data/NPCTypeData.csv";
	if(NPCInfo::GetInstance()->Init(npc))
	{
		LOG_ERROR(FILEINFO, "load npc config error");

		return -1;
	}

	//npc shop
	string npcshop = strFileFullPath + "Data/NPCShopDate.csv";
	if(NpcShop::GetInstance()->Init(npcshop))
	{
		LOG_ERROR(FILEINFO,"load npc shop config error");
	}

	//����ai
	string aiPath = strFileFullPath + "Data/";
	if(AILoader::GetInstance()->Init(aiPath))
	{
		LOG_ERROR(FILEINFO, "load ai config error");

		return -1;
	}

	//���ع���
	string monster = strFileFullPath + "Data/MonsterTypeData.csv";
	if(MonsterLoader::GetInstance()->InitMonster(monster))
	{
		LOG_ERROR(FILEINFO, "load monster config error");

		return -1;
	}

	//读取任务表格
	string quest = strFileFullPath + "Data/";
	if(QuestLoader::GetInstance()->Init(quest))
	{
		LOG_ERROR(FILEINFO, "load quest info error");

		return -1;
	}

	//读取属性
	if(!AttributeManage::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "load Attribute info error");

		return -1;
	}

//	string taskallreward = strFileFullPath + "Data/Dailytaskallreward.csv";
//	if(QuestLoader::GetInstance()->InitTwentyReward(taskallreward))
//	{
//		LOG_ERROR(FILEINFO, "load Dailytaskallreward info error");
//
//		return -1;
//	}
//
//	string tasktype = strFileFullPath + "Data/Dailytasktype.csv";
//	if(QuestLoader::GetInstance()->InitRefreshData(tasktype))
//	{
//		LOG_ERROR(FILEINFO, "load Dailytasktype info error");
//
//		return -1;
//	}
//
//	string taskLua = strFileFullPath + "Data/dailytask.lua";
//	if(QuestLoader::GetInstance()->InitLua(taskLua))
//	{
//		LOG_ERROR(FILEINFO, "load dailytask lua error");
//
//		return -1;
//	}
//
//	string taskrefresh = strFileFullPath + "Data/Dailytaskrefresh.csv";
//	if(QuestLoader::GetInstance()->InitRefreshConsume(taskrefresh))
//	{
//		LOG_ERROR(FILEINFO, "load Dailytaskrefresh info error");
//
//		return -1;
//	}

//	string taskcost = strFileFullPath + "Data/Dailytaskcost.csv";
//	if(QuestLoader::GetInstance()->InitOneFinishConsume(taskcost))
//	{
//		LOG_ERROR(FILEINFO, "load Dailytaskcost info error");
//
//		return -1;
//	}

//	string section = strFileFullPath + "Data/Tasksection.csv";
//	if(QuestLoader::GetInstance()->InitSection(section))
//	{
//		LOG_ERROR(FILEINFO, "load Quest Chapter Reward info error");
//
//		return -1;
//	}

	//角色升级经验
	string upgrade = strFileFullPath + "Data/PlayerUpgrade.csv";
	if(UpgradeLoader::GetInstance()->Init(upgrade))
	{
		LOG_ERROR(FILEINFO, "Load upgrade info error");

		return -1;
	}

	//抄1�7能和buff
	if(SkillLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load skill and buff error");

		return -1;
	}


//	string dailyQuest = strFileFullPath + "Data/task/";
//	if(DailyTaskLoader::GetInstance()->Init(dailyQuest))
//	{
//		LOG_ERROR(FILEINFO, "Load daily task error");
//		return -1;
//	}

	//读取物品表
	if(GoodsLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load Goods error");

		return -1;
	}

	//读取商城商品表
	if(MallLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load mall error");

		return -1;
	}
	//每日必做
	if(haveToDoLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load haveToDo error");

		return -1;
	}
	//奖励包

	if(ScortpacksLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load Scortpacks error");

		return -1;
	}

	//读取掉落表
	if(DropItemLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "load drop item error");

		return -1;
	}

	if(EquipSmithExLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "load smith info error");

		return -1;
	}

	//读取坐骑
	string HorseStr = strFileFullPath + "Data/";
	if(HorseLoader::GetInstance()->InitHorse(HorseStr))
	{
		LOG_ERROR(FILEINFO, "load horse error");

		return -1;
	}

	//读取时装配置
	if(FashionLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "load Fashion error");

		return -1;
	}

	//读取副本配置

	string instanceStr = strFileFullPath + "Data/InstanceTypeData.txt";
	if(InstanceLoader::GetInstance()->Init(instanceStr))
	{
		LOG_ERROR(FILEINFO, "load instance error");

		return -1;
	}

	string instanceRewardStr = strFileFullPath + "Data/InstanceReward.csv";
	if(InstanceLoader::GetInstance()->InitReward(instanceRewardStr))
	{
		LOG_ERROR(FILEINFO, "load instance reward error");

		return -1;
	}

	string SysSettingStr = strFileFullPath + "Data/SystemUnlock.csv";
	if(SysSettingLoader::GetInstance()->Init(SysSettingStr))
	{
		LOG_ERROR(FILEINFO, "load SystemUnlock error");

		return -1;
	}

	//游戏目标表格
	string destinationStr = strFileFullPath + "Data/GametargetTypeData.csv";
	if(DestinationLoader::GetInstance()->InitDestination(destinationStr))
	{
		LOG_ERROR(FILEINFO, "load Destination error");

		return -1;
	}

	//每日必作表格
	string dailyStr = strFileFullPath + "Data/";
	if(DailyLoader::GetInstance()->InitDaily(dailyStr))
	{
		LOG_ERROR(FILEINFO, "load Daily error");

		return -1;
	}

	//成就表格
	//string prestigeStr = strFileFullPath + "Data/AchievementTitle/AchievementTypeData.csv";
	if(AchievementLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "load Prestige error");

		return -1;
	}

//	//称号表格
//	string titleStr = strFileFullPath + "Data/AchievementTitle/TitleTypeData.csv";
//	if(AchievementLoader::GetInstance()->InitTitle(titleStr))
//	{
//		LOG_ERROR(FILEINFO, "load Title error");
//
//		return -1;
//	}

	//读取章节奖励
//	string chapter = strFileFullPath + "Data/task/";
//	if(ChapterLoader::GetInstance()->Init(chapter))
//	{
//		LOG_ERROR(FILEINFO, "Load chapter task error");
//		return -1;
//	}

	//读取章节奖励
	string meridian = strFileFullPath + "Data/";
	if(MeridianLoader::GetInstance()->Init(meridian))
	{
		LOG_ERROR(FILEINFO, "Load chapter task error");
		return -1;
	}

	//在线奖励配置
	string onlineReward = strFileFullPath + "Data/";
	if(OnlineRewardLoader::GetInstance()->Init(onlineReward))
	{
		LOG_ERROR(FILEINFO, "Load online reward error");
		return -1;
	}

	//等级奖励配置
	string levelReward = strFileFullPath + "Data/";
	if(LevelRewardLoader::GetInstance()->Init(levelReward))
	{
		LOG_ERROR(FILEINFO, "Load level reward error");
		return -1;
	}

	//签到奖励配置
	string siginReward = strFileFullPath + "Data/";
	if(SignInLoader::GetInstance()->Init(siginReward))
	{
		LOG_ERROR(FILEINFO, "Load sigin reward error");
		return -1;
	}

	string Escortfile = strFileFullPath + "Data/EscortTypeData.csv";
	if(EscortLoader::GetInstance()->InitEscort(Escortfile))
	{
		LOG_ERROR(FILEINFO, "Load Escort File");
		return -1;
	}

	//读取丹道配置表
	if(DanLuLoader::GetInstance()->InitFile(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load DanLu conf File");
		return -1;
	}

//	string vipfile = strFileFullPath + "Data/VIP/VIPTypeData.csv";
	if(VipLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load VIP conf File");
		return -1;
	}

	//钱庄配置
	string Moneyfile = strFileFullPath + "Data/";
	if(MoneyShopLoader::GetInstance()->InitMoneyShop(Moneyfile))
	{
		LOG_ERROR(FILEINFO, "Load MoneyShop conf File");
		return -1;
	}
	//神兵
	string MagicFile = strFileFullPath + "Data/";
	if(MagicLoader::GetInstance()->Init(MagicFile))
	{
		LOG_ERROR(FILEINFO, "Load Magic Weapon File");
		return -1;
	}

	//法器
	string strSMagicFile = strFileFullPath + "Data/";
	if(SMagicLoader::GetInstance()->Init(strSMagicFile))
	{
		LOG_ERROR(FILEINFO, "Load SMagicFile error");
		return -1;
	}

	//心法
	string strHeartMagicFile = strFileFullPath + "Data/";
	if(HeartMagicLoader::GetInstance()->Init(strHeartMagicFile))
	{
		LOG_ERROR(FILEINFO, "Load HeartMagic File error");
		return -1;
	}

	//官印
	if(CJobLuckyLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Load Cjob lucky File");
		return -1;
	}

	//转盘
	if(LucklyTurntableLoader::GetInstance()->InitDate(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "LucklyTurntable  File");
		return -1;
	}

	string sevenAwFile = strFileFullPath + "Data/";
	if(TotalLoginRewardLoader::GetInstance()->Init(sevenAwFile))
	{
		LOG_ERROR(FILEINFO, "SevenRward  File");
		return -1;
	}

	if(ClanLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "load clan File error");
		return -1;
	}

	//防沉迷省市代码
	string antiAddictionFile = strFileFullPath + "Data/CityCode.csv";
	if(AntiAddictionSystemLoader::GetInstance()->InitAntiAddictionSystem(antiAddictionFile))
	{
		LOG_ERROR(FILEINFO, "load AntiAddictionSystem  File error");
		return -1;
	}

	//屏蔽字
	if(CMaskWordConf::GetInstance()->LoadFile(strFileFullPath + "Data/MaskWord.txt"))
	{
		LOG_ERROR(FILEINFO, "Read File error [Data/MaskWord.csv]");
		return -1;
	}

	if(TreasonKingLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Read TreasonKing File error");
		return -1;
	}

	//守护亲王
	if(ProtectedKingLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Read File error [Data/Activity/*]");
		return -1;
	}

	//战姬
	if(GirlWarLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Read File error [Data/GirlWar/*]");
		return -1;
	}

	// 跨服竞技
	if (CCrossSvrCompeteLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Read CrossSvrCompete File Error");
		return -1;
	}

	if(ContailerLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Read Contailer File Error");
		return -1;
	}

	//国际化表格
	string internationalStr = strFileFullPath + "Data/International.txt";
	if(InternationalLoader::GetInstance()->InitInternational(internationalStr))
	{
		LOG_ERROR(FILEINFO, "load international error");

		return -1;
	}

	// 活跃度
	if (VitalityLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Read VitalityLoader File Error");
		return -1;
	}

	if(MarryLoader::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Marry Loader File Error");
		return -1;
	}

	if(CountryRedEnvelopeLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Envelope Loader File Error");
		return -1;
	}

	//额外属性奖励表
	string extraAtributeStr = strFileFullPath + "Data/StarAttAdd.csv";
	if(ExtraAtributeLoader::GetInstance()->InitExtraAtribute(extraAtributeStr))
	{
		LOG_ERROR(FILEINFO, "load ExtraAtribute error");

		return -1;
	}

	//陈营
	GsCountryMgr::GetInstance();

	if(YellowVipLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Yellow Vip Loader File Error");
		return -1;
	}

	if(CPayLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "RePriceTypeData Loader File Error");
		return -1;
	}

	if(InterfaceAnswerLoader::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "InterfaceAnswerLoader Loader File Error");
		return -1;
	}

	if(ActivitiesTimerLoader::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "ActivitiesTimerLoader Loader File Error");
		return -1;
	}

	if(ActivityBoxLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "ActivitiesTimerLoader Loader File Error");
		return -1;
	}

	string meterialCfgPath = strFileFullPath + "/Data/";
	if (CMeterialSynthesisLoader::GetInstance()->Init(meterialCfgPath))
	{
		LOG_ERROR(FILEINFO, "CMeterialSynthesisLoader Loader File Error");
		return -1;
	}

	string microRewardCfgPath = strFileFullPath + "./Data/";
	if (MicroRewardLoader::GetInstance()->Init(microRewardCfgPath))
	{
		LOG_ERROR(FILEINFO, "OnceReward Loader File Error");
		return -1;
	}

	if(MarryInstanceLoader::GetInstance()->initFile(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "MarryInstanceLoader Loader File Error");
		return -1;
	}

	string arenaCfgPath = strFileFullPath + "./Data/";
	if (ArenaLoader::GetInstance()->Init(arenaCfgPath))
	{
		LOG_ERROR(FILEINFO, "ArenaLoader Loader File Error");
		return -1;
	}

	if (RegionManage::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "Region Loader File Error");
		return -1;
	}

	string RobberPath = strFileFullPath + "Data/Robber.lua";
	if(RobberLoader::GetInstance()->InitLua(RobberPath))
	{
		LOG_ERROR(FILEINFO, "RobberLoader File Error");
		return -1;
	}

	if (FestivalUIFile::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "FestivalUIFile Loader File Error");
		return -1;
	}

	if (ReflushFile::GetInstance()->init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "ReflushFile Loader File Error");
		return -1;
	}

	if (FestivalUIFile::GetInstance()->initNewYear(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "FestivalUIFile Loader File Error");
		return -1;
	}

	if (ClanWelfareLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "ClanWelfare Loader File Error");
		return -1;
	}

	if(OnLineAtTimeLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "OnLineAtTimeLoader Loader File Error");
		return -1;
	}

	// 客户端提示
	if (HintLoader::GetInstance()->Init(strFileFullPath))
	{
		LOG_ERROR(FILEINFO, "HintLoader Loader File Error");
		return -1;
	}

	if(GodTreeFile::GetInstance()->initFile(strFileFullPath))
	{
		LOG_ERROR(FILEINFO,"loader godtree error");
		return -1;
	}

	if(ChallengeMgr::GetInstance()->Init() != 0)
	{
		LOG_ERROR(FILEINFO,"loader Challenge error");
		return -1;
	}

	if(!HeroLoader::GetInstance()->InitData())
	{
		LOG_ERROR(FILEINFO,"loader HeroLoader error");
		return -1;
	}

	if(ShopLoader::GetInstance()->Init(strFileFullPath) != 0)
	{
		LOG_ERROR(FILEINFO,"loader shoploader error");
		return -1;
	}

	if(AnswerLoader::GetInstance()->init(strFileFullPath) != 0)
	{
		LOG_ERROR(FILEINFO,"loader AnswerLoader error");
		return -1;
	}

	return 0;
}

int CServiceMain::InitInstance()
{
	CSkillFactory::InitObject();
	BuffFactory::InitObject();
	ObjectFactory::GetInstance();
	DuplicateMgr::GetInstance();
	GlobalEvent::GetInstance();
	QuickMessage::GetInstance();
	CSceneMgr::GetInstance();
	GameParaLoader::GetInstance();
	PlayerMessage::GetInstance();
	FriendManage::GetInstance();
	SkillMessage::GetInstance();
	CCreateMapTask::GetInstance();
	ContainerMessage::GetInstance();
	DropItemsManager::GetInstance();
	EquipSmithExLoader::GetInstance();
	EquipSmithExMgr::GetInstance();

	HorseMessage::GetInstance();
	ShopMessage::GetInstance();
	ShopMgr::GetInstance();
	ExchangeMessage::GetInstance();
	ChatMgr::GetInstance();

	NewGroupMgr::GetInstance();

	GroupMessageMgr::GetInstance();
	MailMessageMgr::GetInstance();
	MallMessageMgr::GetInstance();
	BaseShopMessage::GetInstance();
	ShopLoader::GetInstance();
	InstanceLoader::GetInstance();
	QuestLoader::GetInstance();
	//ChapterLoader::GetInstance();
	MeridianMgr::GetInstance();
	MapInstanceMessage::GetInstance();
	SettingsMgr::GetInstance();
	RewardMgr::GetInstance();
	DestinationLoader::GetInstance();
	DestinationMessage::GetInstance();
	DailyLoader::GetInstance();
	DailyMgr::GetInstance();
	AchievementLoader::GetInstance();
	AchievementMessage::GetInstance();
	WorldBossMgr::GetInstance();
	EscortLoader::GetInstance();
//	EscortMgr::GetInstance();
	EscortMessage::GetInstance();
	ClanMessage::GetInstance();
	ActivityMessage::GetInstance();
	DanLuLoader::GetInstance();
	DanLuMessage::GetInstance();

	MoneyShopMgr::GetInstance();
	MoneyShopMessage::GetInstance();
	MoneyShopLoader::GetInstance();

	VipLoader::GetInstance();
	VIPMessage::GetInstance();
	MagicMessage::GetInstance();
	SMagicMessage::GetInstance();
	HeartMagicMessage::GetInstance();
	CJobMessage::GetInstance();

	LucklyTurntableLoader::GetInstance();
	LucklyTurntableMessage::GetInstance();

	TotalLoginRewardLoader::GetInstance();

	StatisticMgr::GetInstance();
	CreateGroupKeyMgr::GetInstance();

	QuestNewMessage::GetInstance();
	
	MSopreationManager::GetInstance();

	AntiAddictionMessage::GetInstance();

	CMaskWordConf::GetInstance();

	TreasonKingMessage::GetInstance();

	GirlWarMessage::GetInstance();

	ContailerLoader::GetInstance();

	InternationalLoader::GetInstance();

	CountryRedEnvelopeLoader::GetInstance();

	CountryWarLoader::GetInstance();
	GsCountryWarMgr::GetInstance();
	FestivalActiveMgr::GetInstance();

	ExtraAtributeLoader::GetInstance();

	ManagerMessage::GetInstance();
	FashionMessage::GetInstance();
	VitalityMessage::GetInstance();
//	WorshipMessage::GetInstance();
	MarryMessage::GetInstance();
	MarryLoader::GetInstance();
	CountryRedEnvelopeMessage::GetInstance();
	CPayLoader::GetInstance();
	RechargeMessage::GetInstance();
	GMInterfaceAnswerMessage::GetInstance();
	ActivitiesTimerLoader::GetInstance();
	ActivityBox::GetInstance();
	ActivityBoxLoader::GetInstance();
	FriendManage::GetInstance();
	PVPMgr::GetInstance();
	SeabeachMgr::GetInstance();
	CMeterialSynMessage::GetInstance();
	MoneyInstanceMgr::GetInstance();
	MarryInstanceLoader::GetInstance();
	MeditationMgr::GetInstance();
	MoneyInstanceMgr::GetInstance();
	MonsterExpParam::GetInstance();
	FestivalUIMessage::GetInstance();
	GuideLoader::GetInstance();
	OnLineAtTimeLoader::GetInstance();
	OnLineAtTimeMgr::GetInstance();
	ArenaMessage::GetInstance();
	AnswerLoader::GetInstance();

	return 0;
}

void CServiceMain::Start()
{
	/*signal(SIGUSR1,CServiceMain::Stop);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGINT,SIG_IGN);*/

	#ifndef TEST
	signal(SIGUSR1,CServiceMain::Stop);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGINT,SIG_IGN);
	#endif

	if(Init())
	{
		cout << "��ȡ��ʼ��lua�ļ�����" << endl;

		return ;
	}

	if(CLoggerMT::GetInstance()->Init(m_conf.GetLogConf().fileLen, m_conf.GetLogConf().info, m_conf.GetLogConf().debug,
			m_conf.GetLogConf().warning, m_conf.GetLogConf().error, m_conf.GetLogConf().fatal, m_conf.GetLogConf().display,
			m_conf.GetLogConf().filePath, m_conf.GetLogConf().module) != 0)
	{
		cout << "��ʼ����־����" << endl;

		return ;
	}

	//ȫ�ֶ�ʱ��
	TimerInterface::GetInstance()->Init();
	CThreadPool::GetInstance()->InitThread(m_conf.GetIOThread());

	if(Message_Facade::Init(ServerConHandler::GetInstance(), m_conf.GetBlockThread(), m_conf.GetIOThread()))
	{
		LOG_ERROR(FILEINFO, "������ʼ��ʧ��");

		return;
	}

	if(InitFile() != 0)
	{
		LOG_ERROR(FILEINFO, "��ʼ���ļ�ʧ��");

		return ;
	}

	if(InitInstance())
	{
		LOG_ERROR(FILEINFO, "��ʼ������ʧ��");

		return;
	}

	ServerConHandler::GetInstance()->RegisterMsg();
	ServerConHandler::GetInstance()->SetServerId(m_conf.GetServerID());

	vector<ConnectionConfig> con = m_conf.GetConnCof();
	vector<ConnectionConfig>::iterator itCon = con.begin();
	for(; itCon!=con.end(); ++itCon)
	{
		ConnectionConfig connConf = *itCon;
		Message_Facade::AddConnectionConfig(connConf);
	}

	if(Message_Facade::Run())
	{
		LOG_ERROR(FILEINFO, "����㿪��ʧ�ￄ1�7");

		return;
	}

	srand((unsigned int)time(0));
	DWORD64 bFps = 0;
	DWORD64 bAllFps = 0;
	DWORD64 eFps = 0;
	DWORD64 eAllFps = 0;
	DWORD64 singleFlushTime = 0;
	int64 skilltime = 0, messagetime = 0, createmaptime = 0, scenetime = 0, flushtime = 0, timertime = 0, pathtime = 0, quicktime = 0, datetime = 0;

	sleep(1);
	while(!m_flag)
	{//�߼�
//		printf("skilltime:%lld, messageTime:%lld, createmapTime:%lld, sceneTime:%lld, flushTime:%lld, timerTime:%lld, pathTime:%lld, quickTime:%lld\n"
//				, skilltime, messagetime, createmaptime, scenetime, flushtime, timertime, pathtime, quicktime);
		bFps = CUtil::GetNowSecond();

		try
		{
			CSkillFactory::Update();
			BuffFactory::Update();

#ifdef DEBUG
			skilltime = CUtil::GetNowSecond() - bFps;
#endif
		}
		catch(exception &e)
		{
			LOG_ERROR(FILEINFO, "skill and buff update error [errmsg=%s]",e.what());
		}
		catch(...)
		{
			LOG_ERROR(FILEINFO, "skill and buff update unknown error");
		}

		try
		{
#ifdef DEBUG
			int64 messagebegin = CUtil::GetNowSecond();
#endif
			Message_Facade::ProcessMessage();
#ifdef DEBUG
			messagetime = CUtil::GetNowSecond() - messagebegin;
#endif
		}
		catch(exception &e)
		{
			LOG_ERROR(FILEINFO, "process message error [errmsg=%s]",e.what());
		}
		catch(...)
		{
			LOG_ERROR(FILEINFO, "process message unknown error");
		}

		try
		{
#ifdef DEBUG
			int64 mapbegin = CUtil::GetNowSecond();
#endif
			CCreateMapTask::GetInstance()->UpdateMap();
#ifdef DEBUG
			int64 scenebegin = CUtil::GetNowSecond();
			createmaptime = scenebegin - mapbegin;
#endif

			DWORD64 tempNow = CUtil::GetNowMicrosecod();
			if(bAllFps != 0)
			{
				eAllFps = tempNow;

				if(eAllFps - bAllFps > 1000 * 1000)
				{
					LOG_FATAL(FILEINFO, "fps is slow  skill time[%lld], message time[%lld], createtime[%lld], scene time[%lld],flush time[%lld], timer time[%lld], path time[%lld], quick message time[%lld], date time[%lld]",skilltime,messagetime,createmaptime,scenetime,flushtime,timertime,pathtime,quicktime,datetime);
				}

				singleFlushTime += eAllFps - bAllFps;
			}

			bAllFps = tempNow;
			if(singleFlushTime > 1000 * 100)
			{
				CSceneMgr::GetInstance()->Update(singleFlushTime / 1000);
				singleFlushTime = singleFlushTime % 1000;
			}

			int64 timerbegin = CUtil::GetNowSecond();
#ifdef DEBUG
			scenetime = timerbegin - scenebegin;
#endif

			TimerInterface::GetInstance()->StartTick(timerbegin);

#ifdef DEBUG
			timertime = CUtil::GetNowSecond() - timerbegin;
#endif
		}
		catch(exception &e)
		{
			LOG_ERROR(FILEINFO, "scene update error [errmsg=%s]",e.what());
		}
		catch(...)
		{
			LOG_ERROR(FILEINFO, "scene update unknown error");
		}

		try
		{
#ifdef DEBUG
			int64 quickbegin = CUtil::GetNowSecond();
#endif

			QuickMessage::GetInstance()->UpdateMessage();

#ifdef DEBUG
			quicktime = CUtil::GetNowSecond() - quickbegin;
#endif

		}
		catch(exception &e)
		{
			LOG_ERROR(FILEINFO, "send quick message to player error [errmsg=%s]",e.what());
		}
		catch(...)
		{
			LOG_ERROR(FILEINFO, "send quick message to player unknown error");
		}

		try
		{
#ifdef DEBUG
			int64 pathbegin = CUtil::GetNowSecond();
#endif

			FindPathMgr::GetInstance()->UpdatePath();

#ifdef DEBUG
			pathtime = CUtil::GetNowSecond() - pathbegin;
#endif

		}
		catch(exception &e)
		{
			LOG_ERROR(FILEINFO, "date time service polling error [errmsg=%s]",e.what());
		}
		catch(...)
		{
			LOG_ERROR(FILEINFO, "date time service polling unknown error");
		}

#ifdef DEBUG
			int64 flushbegin = CUtil::GetNowSecond();
#endif
		Message_Facade::MessageFlush();
#ifdef DEBUG
		flushtime = CUtil::GetNowSecond() - flushbegin;
#endif

		eFps = CUtil::GetNowSecond();
		eFps -= bFps;

		if(eFps < 100)
		{
			CUtil::MSleep(100 - eFps);
		}
		else
		{
			CUtil::MSleep(10);
		}
	}

	Message_Facade::Stop();

	UnInit();
}

void CServiceMain::UnInit()
{
	ChallengeMgr::GetInstance()->DestroyInstance();
	MeridianLoader::GetInstance()->DestroyInstance();
	OnlineRewardLoader::GetInstance()->DestroyInstance();
	LevelRewardLoader::GetInstance()->DestroyInstance();
	SignInLoader::GetInstance()->DestroyInstance();
	MagicMessage::GetInstance()->DestroyInstance();
	SMagicMessage::GetInstance()->DestroyInstance();
	HeartMagicMessage::GetInstance()->DestroyInstance();
	HorseMessage::GetInstance()->DestroyInstance();
	EquipSmithExMgr::GetInstance()->DestroyInstance();
	ContainerMessage::GetInstance()->DestroyInstance();
	CCreateMapTask::GetInstance()->DestroyInstance();
	SkillMessage::GetInstance()->DestroyInstance();
	QuickMessage::GetInstance()->DestroyInstance();
	ClanLoader::GetInstance()->DestroyInstance();
	ClanMessage::GetInstance()->DestroyInstance();

	HorseLoader::GetInstance()->DestroyInstance();
	EquipSmithExLoader::GetInstance()->DestroyInstance();
	DropItemLoader::GetInstance()->DestroyInstance();
	GoodsLoader::GetInstance()->DestroyInstance();
	MallLoader::GetInstance()->DestroyInstance();
	haveToDoLoader::GetInstance()->DestroyInstance();
	ScortpacksLoader::GetInstance()->DestroyInstance();
	SkillLoader::GetInstance()->DestroyInstance();
	UpgradeLoader::GetInstance()->DestroyInstance();
	QuestLoader::GetInstance()->DestroyInstance();
	MonsterLoader::GetInstance()->DestoryInstance();
	AILoader::GetInstance()->DestroyInstance();
	NPCInfo::GetInstance()->DestroyInstance();
	BattleInfoLoader::GetInstance()->DestroyInstance();
	CMapConfig::GetInstance()->DestroyInstance();
	ChatMgr::GetInstance()->DestroyInstance();
	MailMessageMgr::GetInstance()->DestroyInstance();
	MallMessageMgr::GetInstance()->DestroyInstance();
	BaseShopMessage::GetInstance()->DestroyInstance();
	ShopLoader::GetInstance()->DestroyInstance();
	FashionLoader::GetInstance()->DestroyInstance();
	ArenaLoader::GetInstance()->DestroyInstance();

	ServerConHandler::GetInstance()->DestoryInstance();

	ShopMgr::GetInstance()->DestroyInstance();
	ExchangeMessage::GetInstance()->DestroyInstance();

	NewGroupMgr::GetInstance()->DestroyInstance();

	GroupMessageMgr::GetInstance()->DestroyInstance();

	InstanceLoader::GetInstance()->DestroyInstance();
	MeridianMgr::GetInstance()->DestroyInstance();
	MapInstanceMessage::GetInstance()->DestroyInstance();
	SettingsMgr::GetInstance()->DestroyInstance();
	RewardMgr::GetInstance()->DestroyInstance();
	DestinationLoader::GetInstance()->DestroyInstance();
	DestinationMessage::GetInstance()->DestroyInstance();
	DailyLoader::GetInstance()->DestroyInstance();
	DailyMgr::GetInstance()->DestroyInstance();
	AchievementLoader::GetInstance()->DestroyInstance();
	AchievementMessage::GetInstance()->DestroyInstance();
	WorldBossMgr::GetInstance()->DestroyInstance();
	ShopMessage::GetInstance()->DestroyInstance();
	NpcShop::GetInstance()->DestroyInstance();
	EscortLoader::GetInstance()->DestroyInstance();
	EscortMessage::GetInstance()->DestroyInstance();

	DanLuLoader::GetInstance()->DestroyInstance();
	DanLuMessage::GetInstance()->DestroyInstance();

	MoneyShopMgr::GetInstance()->DestroyInstance();
	MoneyShopMessage::GetInstance()->DestroyInstance();

	VipLoader::GetInstance()->DestroyInstance();
	VIPMessage::GetInstance()->DestroyInstance();
	MoneyShopLoader::GetInstance()->DestroyInstance();

	MagicLoader::GetInstance()->DestroyInstance();
	SMagicLoader::GetInstance()->DestroyInstance();
	HeartMagicLoader::GetInstance()->DestroyInstance();

	CJobMessage::GetInstance()->DestroyInstance();
	LucklyTurntableLoader::GetInstance()->DestroyInstance();
	LucklyTurntableMessage::GetInstance()->DestroyInstance();

	TotalLoginRewardLoader::GetInstance()->DestroyInstance();

	StatisticMgr::GetInstance()->DestroyInstance();
	CreateGroupKeyMgr::GetInstance()->DestroyInstance();

	QuestNewMessage::GetInstance()->DestroyInstance();

	MSopreationManager::GetInstance()->DestroyInstance();
	AntiAddictionMessage::GetInstance()->DestroyInstance();
	CMaskWordConf::GetInstance()->DestroyInstance();
	ProtectedKingLoader::GetInstance()->DestroyInstance();

	TreasonKingMessage::GetInstance()->DestroyInstance();
	TreasonKingLoader::GetInstance()->DestroyInstance();
	ActivityMessage::GetInstance()->DestroyInstance();
	CJobLuckyLoader::GetInstance()->DestroyInstance();
	AntiAddictionSystemLoader::GetInstance()->DestroyInstance();
	CCrossSvrCompeteLoader::GetInstance()->DestroyInstance();
	InternationalLoader::GetInstance()->DestroyInstance();

	FestivalActiveMgr::GetInstance()->DestroyInstance();
	GsCountryMgr::GetInstance()->DestroyInstance();
	GsCountryWarMgr::GetInstance()->DestroyInstance();
	CountryWarLoader::GetInstance()->DestroyInstance();
	ManagerMessage::GetInstance()->DestroyInstance();
	FashionMessage::GetInstance()->DestroyInstance();
	VitalityLoader::GetInstance()->DestroyInstance();
	VitalityMessage::GetInstance()->DestroyInstance();
//	WorshipMessage::GetInstance()->DestroyInstance();
	MarryMessage::GetInstance()->DestroyInstance();
	MarryLoader::GetInstance()->DestroyInstance();
	CountryRedEnvelopeLoader::GetInstance()->DestroyInstance();
	CountryRedEnvelopeMessage::GetInstance()->DestroyInstance();
	ExtraAtributeLoader::GetInstance()->DestroyInstance();
	GameParaLoader::GetInstance()->DestroyInstance();
	CPayLoader::GetInstance()->DestroyInstance();
	RechargeMessage::GetInstance()->DestroyInstance();
	ActivitiesTimerLoader::GetInstance()->DestroyInstance();
	ActivityBox::GetInstance()->DestroyInstance();
	ActivityBoxLoader::GetInstance()->DestroyInstance();
	PVPMgr::GetInstance()->DestroyInstance();
	SeabeachMgr::GetInstance()->DestroyInstance();
	CMeterialSynthesisLoader::GetInstance()->DestroyInstance();
	CMeterialSynMessage::GetInstance()->DestroyInstance();
	MicroRewardLoader::GetInstance()->DestroyInstance();
	MoneyInstanceMgr::GetInstance()->DestroyInstance();
	MarryInstanceLoader::GetInstance()->DestroyInstance();
	MeditationMgr::GetInstance()->DestroyInstance();
	MonsterExpParam::GetInstance()->DestroyInstance();
	RobberLoader::GetInstance()->DestroyInstance();
	GuideLoader::GetInstance()->DestroyInstance();
	OnLineAtTimeLoader::GetInstance()->DestroyInstance();
	OnLineAtTimeMgr::GetInstance()->DestroyInstance();
//	ArenaMessage::GetInstance()->DestroyInstance();
	ClanWelfareLoader::GetInstance()->DestroyInstance();
	HintLoader::GetInstance()->DestroyInstance();
	AnswerLoader::GetInstance()->DestroyInstance();

	//-----------------------------------------------------------------------------
	// 逻辑层销毁在以上位置
	//-----------------------------------------------------------------------------

	PlayerMessage::GetInstance()->DestroyInstance();
	FriendManage::GetInstance()->DestroyInstance();
	DuplicateMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	DropItemsManager::GetInstance()->DestroyInstance();
	CSkillFactory::DestroyAll();
	BuffFactory::DestroyAll();
	ObjectFactory::GetInstance()->DestroyInstance();

//	sleep(60 * 7);
	GlobalEvent::GetInstance()->DestroyInstance();
	CThreadPool::GetInstance()->DestroyInstance();
	TimerInterface::GetInstance()->DestoryInstance();
	CLoggerMT::GetInstance()->DestoryInstance();
	google::protobuf::ShutdownProtobufLibrary();
}

void CServiceMain::Stop(int signal)
{
	m_flag = true;
	printf("sadfffffffffffdddddddddddddddddddddddddd");

//	Message_Facade::UnWait();
}



