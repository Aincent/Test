/*
 * Player.cpp
 *
 *  Created on: 2013??10??10??
 *      Author: helh
 */

#include "Player.h"
#include "Network/Message_Facade.h"
#include "ServerMsgDefine.h"
#include "../../ServerManager.h"
#include "Network/MessageManager.h"
#include "Log/Logger.h"
#include "../../MessageBuild.h"
#include "MessageStruct/ServerReturnIntChar.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../../Map/SceneManager.h"
#include "../../Map/GameZone.h"
#include "MessageCommonRet.h"
#include "../FileLoader/NPCLoader.h"
#include "./FileLoader/UpgradeLoader.h"
#include "../Monster/Monster.h"
#include "../GirlWar/GirlWarManager.h"
#include <math.h>
#include "Timer/TimerInterface.h"
#include "../../Skill/SkillLoader/SkillLoader.h"
#include "../../CDTime/CDMgr.h"
#include "../../Skill/SkillManager.h"
#include "MessageStruct/SynchMessage.pb.h"
#include "Goods/PropData.h"
#include "../../Horse/HorseManager.h"
#include "../../Trade/shop.h"
#include "../../EventSet/EventDefine.h"
#include "../../ServerEventDefine.h"
#include "MessageStruct/Friend/FriendMessage.pb.h"
#include "../../Achievement/Achievement.h"
#include "../../Achievement/AchievementLoader.h"
#include "../../Achievement/PlayerTitle.h"
#include "../../EscortEx/EscortMgr.h"
#include "../../Container/ParcelMgr.h"
#include "../../Container/ArticleEquipMgr.h"
#include "../../Container/ArticleStoreHouseMgr.h"
#include "MessageStruct/ServerReturnChar.pb.h"
#include "MessageStruct/CharBase/MeditationProto.pb.h"
#include "../../DanLu/DanLuMgr.h"
#include "../../MagicWeapon/MagicMgr.h"
#include "../../Magic/SMagicMgr.h"
#include "../../HeartMagic/HeartMagicMgr.h"
#include "../../CJobLucky/CJobLuckyMgr.h"
#include "../../LucklyTurntable/LucklyTurntableMgr.h"
#include "../../MoneyShop/MoneyShopMgr.h"
#include "../../WorldBoss/WorldBoss.h"
#include "../../RewardHail/RewardMgr.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "../../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "MessageStruct/Clan/Clan.pb.h"
#include "../AI/AIBrain.h"
#include "../../AntiAddictionSystem/AntiAddictionSystemMgr.h"
#include "../../QuestNew/QuestPart.h"
#include "../../Activity/ActivityMgr.h"
#include <math.h>
#include "../ItemDrop/DropItemMgr.h"
#include "../FileLoader/GirlWarLoader.h"
#include "../GirlWar/GirlWarManager.h"
#include "../GirlWar/AI/PetBrain.h"
#include "../../Map/DuplicateMgr.h"
#include "../../Vitality/VitalityMgr.h"
#include "../../Country/playerCampShopMgr.h"
#include "../../SysSetting/SysSettingLoader.h"
#include "../../Meridians/FileLoader/MeridianLoader.h"
#include "../../CountryRedEnvelope/CountryRedEnvelopeMgr.h"
#include "../../CountryRedEnvelope/CountryRedEnvelopeLoader.h"
#include "../../ExtraAtribute/ExtraAtributeMgr.h"
#include "../../Country/GsCountryMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../../Attribute/AttributePlayer.h"
#include "../../Attribute/AttributeManage.h"
#include "../../Meridians/MeridianMgr.h"
#include "FileLoader/PayLoader.h"
#include "MessageStruct/SystemNotice/SystemNotice.pb.h"
#include "../../Fighting/Fighting.h"
#include "MessageStruct/CharLogin/DayUpdate.pb.h"
#include "../../Festival/FestivalAttr.h"
#include "../../RewardHail/OnceReward.h"
#include "../../RewardHail/FileLoader/MicroRewardLoader.h"
#include "../../Marry/MarryRing.h"
#include "../../Meditation/PlayerMeditation.h"
#include "../../Meditation/MeditationMgr.h"
#include "FileLoader/ArenaLoader.h"
#include "../../Distortion/Distortion.h"
#include "../PlayerRegion/PlayerRegion.h"
#include "../../MonsterExpParam/MonsterExpParam.h"
#include "../../FestivalUI/ChristmasManage.h"
#include "../ObjectFactory.h"
#include "../../FestivalUI/NewYearActivity.h"
#include "../../MapInstance/InstanceMapLoader.h"
#include "../../StatisticMgr/StatisticHandle.h"
#include "../../MapInstance/PlayerChallenge.h"
#include "../../MapInstance/HeroInfo.h"
#include "../../Arena/ArenaMgr.h"
#include "FileLoader/ActivitiesTimerLoader.h"

Player::Player(eObjType type) :
	CreatureObj(type),
	m_synPathTime(0),       m_curIndex(-1),		m_sex(false),
	m_accType(0), 			m_expRation(0), 	m_horseModel(0), 			 m_fashionID(0),
	m_cContribution_Week(0),m_FitchPrize(0), 	m_kingRankFlag(0),			 m_channelID(-1),
	m_dbChannelID(-1),	m_deadTime(0),m_oldPkType(0),
	m_sinState(0), 			m_acupoint(0), 		m_offineTime(0), 			 m_onlineTime(0),
	m_totalOnlineTime(0), 	m_serverScore(0),   m_loginTime(0),	 			 m_nextTalkTime(0),
	m_ip(""),m_fContribution(0),m_clanName(""),     m_maxCanGetEnvelopeCount(0), m_groupID(0),
	m_timerID(0), 			m_fightTime(0),  m_bEscortNeedChangeMap(true),
	m_AttrTick(0), 			m_isFinishInitAttr(false),m_bIsHaveCompensate(false),m_bWillInInstance(false),m_YellowVip(0),
	m_YellowYearVip(0),m_YellowVipLevel(0),m_YellowHighVip(0), m_loginClientType(0),m_accountType(0)
{
	m_GirlWarMgr = new GirlWarManager(this);
	m_quest = new QuestPart(this);
	m_cdMgr = new CDMgr(this);
	m_skillMgr = new SkillMgr(this);
	m_container[ePackageType] = new ArticleParcelMgr(this, 42 * 4);
	m_container[eStoreHouseType] = new ArticleStorehouseMgr(this, 49 * 4);
	m_container[eEquipQuickType] = new ArticleEquipMgr(this,EQUIPITEM_SECTOR_COUNT);
	m_horseMgr = new HorseMgr(this);
	m_magicWeapon = new MagicMgr(this);
	m_sMagic = new SMagicMgr(this);
	m_heartMagic = new HeartMagicMgr(this);

	m_cjobMgr = new CJobLuckyMgr(this);
	m_activity = new ActivityMgr(this);
	m_escort = new EscortMgr(this);
	m_campShopMgr = new PlayerCampShopMgr(this);
	m_baseShopMgr = new BaseShopMgr(this);
	m_answerMgr = new AnswerMgr(this);
	m_festiveAttrMgr = new PlayerFestiveAttr(this);
	m_AttManage = new AttributePlayer();

	memset(m_dirtyData, 0, eCharStructMax);
	memset(m_initpartData, 0, eCharStructMax);  //

	m_Achievement = new Achievement(this);
	m_Destination = new Destination(this);
	m_Title = new Title(this);
	m_instanceMgr = new InstanceMgr(this);
	m_signIn = new SignIn(this);
	m_counterService = new CounterService(this);
	m_onlineReward = new OnlineReward(this);
	m_levelReward = new LevelReward(this);
	m_treasonKingReward = new TreasonKingReward(this);
	m_VipPart = new PlayerVipPart(this);
	m_danluMgr = new DanLuMgr(this); //练炉
	m_lucktableMgr = new LucklyTurntableMgr(this);
	m_AntiAddictionMgr = new AntiAddictionSystemMgr(this);
	m_Daily = new DailyManager(this);
	m_fashionShow = new FashionShow(this);
	m_vitalityMgr = new VitalityMgr(this);
	m_envelopeMgr = new CountryRedEnvelopeMgr(this);
	m_extraAtributeMgr = new ExtraAtributeMgr(this);
	m_highLightMgr = new CHighLightMgr(this);
	m_Fighting	= new Fighting(this);
	m_onceReward = new OnceReward(this);
	m_MarryRing = new MarryRing(*this);
	m_Meditation = new PlayerMeditation(this);
	m_Distortion = new PlayerDistortion(this);
//	m_arenaMgr = new ArenaMgr(this);
	m_Region	 = new PlayerRegion(this);
	m_Christmas	 = new ChristmasManage(*this);

	m_NewYear	 = new NewYearActivity(*this);

	m_arenaMgr = new ArenaMgr(*this);

	m_challengeMgr = new PlayerChallenge(this);

	m_heroMgr = new HeroInfo(this);

	InitEvent();

	Smart_Ptr<Monster> mon;
	m_pTaskMon = mon;
}

Player::~Player()
{
	this->ReleasePlayer();
}

void Player::InitEvent()
{
	if(m_sinValueTimer.IsValid())
	{
		m_sinValueTimer.SetNull();
	}
	m_sinValueTimer = TimerInterface::GetInstance()->RegRepeatedTime(&Player::UpdateSinValue, this, 0, 0, 0, 0, 60 * 1000,2 * 60 * 1000);

	if(!m_addHartedEvent.IsValid())
		m_addHartedEvent =RegEvent(this, ADD_HATRED_EVENT_TYPE, this,&Player::AddPlayerHatred);

	if(!m_addSinValueEvent.IsValid())
		m_addSinValueEvent =RegEvent(this, ADD_SIN_EVENT_TYPE, this,&Player::AddPlayerSinValue);

	if(!m_reduceSinValueEvent.IsValid())
		m_reduceSinValueEvent =RegEvent(this, REDUCE_SIN_EVENT_TYPE, this,&Player::ReducePlayerSinValue);

	if(!m_enterMapEvent.IsValid())
		m_enterMapEvent = RegEvent(this, PLAYER_ENTERMAP_EVENT_TYPE,this,&Player::PlayerEnterMap);//进入某地图事件q

	if(!m_leaveMapEvent.IsValid())
		m_leaveMapEvent = RegEvent(this, PLAYER_LEAVE_MAP_EVENT_TYPE,this,&Player::PlayerLeaveScene);

	if(!m_objDeadEvent.IsValid())
		m_objDeadEvent =RegEvent(this, CREATURE_DEAD_EVENT_TYPE,this,&Player::PlayerBeKilled);

	if(!m_killMonsterEvent.IsValid())
		m_killMonsterEvent =RegEvent(this,MONSTER_KILLED_EVENT_TYPE, this, &Player::KillMonster);

	if(!m_joinCountry.IsValid())
		m_joinCountry = RegEvent(this,PLAYER_JOIN_CAMP, this, &Player::JoinCamp);
}

void Player::ReInitPlayer()
{
	memset(m_dirtyData, 0, eCharStructMax);
	memset(m_initpartData, 0, eCharStructMax);  //

	ClearData();

	Smart_Ptr<Monster> mon;
	m_pTaskMon = mon;

	m_Achievement->ReInit();
	m_GirlWarMgr->ReInit();
	m_quest->ReInit();
	m_cdMgr->ReInit();
	m_skillMgr->ReInit();

	Smart_Ptr<ArticleParcelMgr> Parcel = m_container[ePackageType];
	Smart_Ptr<ArticleStorehouseMgr> Storehouse = m_container[eStoreHouseType];
	Smart_Ptr<ArticleEquipMgr> Equip = m_container[eEquipQuickType];
	Parcel->ReInit();
	Storehouse->ReInit();
	Equip->ReInit();

	m_horseMgr->ReInit();
	m_magicWeapon->ReInit();
	m_cjobMgr->ReInit();
	m_activity->ReInit();
	m_escort->ReInit();
	m_campShopMgr->ReInit();
	m_festiveAttrMgr->ReInit();
	//m_AttManage->ReInit();

	m_Destination->ReInit();
	m_Title->ReInit();
	m_danluMgr->ReInit(); //练炉

	m_signIn->ReInit();
	m_counterService->ReInit();
	m_onlineReward->ReInit();
	m_levelReward->ReInit();

	m_treasonKingReward->ReInit();
	m_VipPart->ReInit();
	m_lucktableMgr->ReInit();
	m_AntiAddictionMgr->ReInit();

	m_Daily->ReInit();
	m_fashionShow->ReInit();
	m_vitalityMgr->ReInit();
	m_envelopeMgr->ReInit();

	m_extraAtributeMgr->ReInit();
	m_highLightMgr->ReInit();
	m_Fighting->ReInit();
	m_onceReward->ReInit();

	m_MarryRing->ReInit();
	m_Meditation->ReInit();
	m_Distortion->ReInit();
	m_Region->ReInit();

	m_Christmas->ReInit();
	m_NewYear->ReInit();

	m_challengeMgr->ReInit();
	m_heroMgr->ReInit();

	InitEvent();
}

void Player::ClearData()
{
	m_path.Clear();
	m_synPathTime = 0;
	m_curIndex = 0;
	m_sex = 0;
	m_accType = 0;
	m_openID = "";
	m_openKey = "";
	m_pf = "";
	m_pfKey = "";
	m_sCoupleName = "";
	m_58playSouce = "";
	m_expRation = 0;
	m_horseModel = 0;
	m_fashionID = 0;
	m_cContribution_Week = 0;
	m_FitchPrize = 0;
	m_kingRankFlag = 0;

	m_channelID = 0;
	m_dbChannelID = 0;
	m_deadTime = 0;
	m_oldPkType = 0;
	m_sinState = 0;
	m_acupoint = 0;
	m_offineTime = 0;
	m_onlineTime = 0;
	m_totalOnlineTime = 0;
	m_serverScore = 0;

	m_loginTime = 0;
	m_nextTalkTime = 0;
	m_rechargeTime = 0;

	m_ip = "";

	m_faJob = 0;
	m_fContribution = 0;
 m_clanName = "";
 m_maxCanGetEnvelopeCount = 0;

 m_cityRewardList.clear();
 m_groupID = 0;
 m_groupAddHp.clear();

 m_AttrTick = 0;
 m_isFinishInitAttr = false;
 m_bIsHaveCompensate = false;
 m_bWillInInstance = false;

 m_YellowVip = 0;
 m_YellowYearVip = 0;
 m_YellowVipLevel = 0;
 m_YellowHighVip = 0;
 m_loginClientType = 0;
 m_accountType = 0;

 memset(m_dirtyData, 0, eCharStructMax);
 memset(m_initpartData, 0, eCharStructMax);  //

 m_attrData.clear();

}

void Player::ReleasePlayer()
{
	m_targetMonster.clear();
	UpdateEnemyList();

	m_Achievement->ReleaseAchievement();
	m_GirlWarMgr->ReleaseGirlWar();
	m_quest->RelaseQuestPart();
	m_skillMgr->ReleaseSkill();
	m_cdMgr->Release();

	Smart_Ptr<ArticleParcelMgr> Parcel = m_container[ePackageType];
	Smart_Ptr<ArticleStorehouseMgr> Storehouse = m_container[eStoreHouseType];
	Smart_Ptr<ArticleEquipMgr> Equip = m_container[eEquipQuickType];
	Parcel->Release();
	Storehouse->Release();
	Equip->Release();

	m_horseMgr->ReleaseHorse();
	m_magicWeapon->ReleaseMagic();
	m_sMagic->ReleaseMagic();
	m_heartMagic->ReleaseMagic();

	m_cjobMgr->ReleaseCJobLucky();
	m_activity->ReleaseActivity();
	m_escort->ReleaseEscortMgr();
	m_campShopMgr->Release();
	m_festiveAttrMgr->Release();

	m_Destination->ReleaseDestination();
	m_Title->Release();
	m_instanceMgr->ReleaseInstanceMgr();
	m_danluMgr->ReleaseDanLuMgr();

	m_signIn->Release();
	m_counterService->Release();
	m_onlineReward->Release();
	m_levelReward->Release();

	m_treasonKingReward->Release();
	m_VipPart->Release();
	m_lucktableMgr->Release();
	m_AntiAddictionMgr->Release();

	m_Daily->Release();
	m_fashionShow->Release();
	m_vitalityMgr->Release();
	m_envelopeMgr->Release();

	m_extraAtributeMgr->Release();
	m_highLightMgr->Release();
	m_Fighting->Release();
	m_onceReward->Release();

	m_MarryRing->Release();
	m_Meditation->Release();
	m_Distortion->Release();
	m_Region->Release();

	m_Christmas->Release();
	m_NewYear->Release();

	m_challengeMgr->Release();
	m_heroMgr->Release();

	//CreatureObj::Release();

	endCheckBagTimer();

	if (m_timerID.IsValid())
	{
		m_timerID.SetNull();
	}

	if (m_sinValueTimer.IsValid())
	{
		m_sinValueTimer.SetNull();
	}

	if (m_addHartedEvent.IsValid())
	{
		m_addHartedEvent.SetNull();
	}

	if (m_addSinValueEvent.IsValid())
	{
		m_addSinValueEvent.SetNull();
	}

	if (m_reduceSinValueEvent.IsValid())
	{
		m_reduceSinValueEvent.SetNull();
	}

	if (m_enterMapEvent.IsValid())
	{
		m_enterMapEvent.SetNull();
	}

	if(m_leaveMapEvent.IsValid())
	{
		m_leaveMapEvent.SetNull();
	}

	if(m_fightGirlWarEvent.IsValid())
	{
		m_fightGirlWarEvent.SetNull();
	}

	if (m_objDeadEvent.IsValid())
	{
		m_objDeadEvent.SetNull();
	}
	if (m_killMonsterEvent.IsValid())
	{
		m_killMonsterEvent.SetNull();
	}
	if (m_joinCountry.IsValid())
	{
		m_joinCountry.SetNull();
	}

	//Smart_Ptr<Object> object = GetMyself();
	//ObjectFactory::GetInstance()->Recycle(object);
}

//取消打坐双修
void Player::CanccelMeditation()
{
	if(IsInCreatureState(ERS_Meditation))
	{
			UnsetCreatureState(ERS_Meditation);
			GetMeditationMgr()->DeleteMeditationTimer();
	}
}

Smart_Ptr<CountryRedEnvelopeMgr>& Player::getEnvelopeMgr()
{
	return m_envelopeMgr;
}

void Player::RemoveTimerInterface()
{
	if (m_sinValueTimer.IsValid())
	{
		m_sinValueTimer.SetNull();
	}
}

bool Player::KillMonster(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if (!data)
	{
		return true;
	}

	if (data->m_other->GetType() != ePlayer && data->m_other->GetType() != ePet)
		return true;

	Smart_Ptr<Player> player;
	Smart_Ptr<GirlWar> girlwar;

	if (data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Smart_Ptr<Monster> mon = data->m_myself;
	if (player && mon && player->GetID() == this->GetID())
	{
		if (mon->GetQuality() == eGatherMonster|| mon->GetQuality() == eElementMonster || mon->GetQuality() == eBoxMonster ||
				mon->GetQuality() == ePVPGatherMonster || mon->GetQuality() == eChallengeBox)
			return true;

		//增加连斩数
		this->AddContinueBeheaded();

		if(mon->GetQuality() == eGuardMonster)
			this->AddSinValue(GameParaLoader::GetInstance()->getAddSinValue());

		double addition = 1;

		int tempPlayerLevel = getAttManage()->getValue<int>(eCharLv);
		int tempMonsterLevel = mon->getAttManage()->getValue<int>(eCharLv);
		int newExp = mon->getAttManage()->getValue<int64>(eCharExp);

		newExp = MonsterExpParam::GetInstance()->GetExp(tempMonsterLevel, tempPlayerLevel, newExp);

		// 判断是否是VIP 打怪经验加成
//		double vipExpAdd = GetVipPart()->GetValueByRight(eVipExpAdd);
//		if (0 < vipExpAdd)
//		{
//			addition += vipExpAdd;
//		}

		//非副本，经验丹才有效
		int64 mapType = GET_MAP_TYPE(GetMapID());
		if(mapType >= eOnlyMap && mapType <= eStaticMap)
		{
			addition += getAttManage()->getValue<double>(eKillMonsterExpRate);
		}
		newExp = (int)(ceil(newExp * addition));

		//经验分配
		AddExp(newExp);
	}

	return true;
}

Smart_Ptr<GirlWarManager>& Player::GetGirlWarManager()
{
	return m_GirlWarMgr;
}

bool Player::PlayerBeKilled(const EventArgs & e)
{
	CreatureKilledEventArgs* data = (CreatureKilledEventArgs*) (&e);
	if (!data)
	{
		return true;
	}

	if (data->m_obj)
	{
		//被怪物杀死
		if (data->m_obj->GetType() == eMonster)
		{
			Smart_Ptr<Monster> mon = data->m_obj;
			//被卫兵杀死
			if (mon->GetQuality() != eGuardMonster)
				return true;
		}
		else
			return true;

		//红名玩家非副本内死亡掉落
		Goods::SimDropSomeGoods dropGoods;
		std::vector<PropBase> dropList;
		if (RedNamePlayerDropEquipAndGoods(dropGoods, dropList))
		{
			QuickMessage::GetInstance()->AddSendMessage(GetID(), GetChannelID(),&dropGoods, MSG_SIM_GM2C_DROP_GOODS_OR_QEUIP);
			DropItemsManager::GetInstance()->HandleBattleDeath(m_myself,dropList, data->m_obj);
		}
	}

	return true;
}

void Player::DeleteMonster(int key)
{
	map<int, Smart_Ptr<Monster> >::iterator it = m_targetMonster.find(key);
	if (it != m_targetMonster.end())
	{

		m_targetMonster.erase(it);
	}
}

void Player::ClearMonter()
{
	m_targetMonster.clear();
}

void Player::ClearMonsterPath()
{
	map<int, Smart_Ptr<Monster> >::iterator it = m_targetMonster.begin();
	for (; it != m_targetMonster.end(); ++it) 
	{
		it->second->GetBrain()->ClearPathAndTime();
	}
}

void Player::UpdateEnemyList()
{
	map<int, Smart_Ptr<Monster> >::iterator it = m_targetMonster.begin();
	for (; it != m_targetMonster.end(); ++it)
	{
		it->second->UpdateEnemyList(m_myself);
	}

	m_targetMonster.clear();

	//删除仇恨列表
	vector<Smart_Ptr<CreatureObj> >::iterator itEnemy = m_enemyList.begin();
	for (; itEnemy != m_enemyList.end(); itEnemy++)
	{
		if ((*itEnemy).Get() == 0)
		{
			continue;
		}

		if ((*itEnemy)->GetType() == eMonster)
		{
			Smart_Ptr<Monster> monster = (*itEnemy);
			if (monster)
			{
				monster->UpdateEnemyList(m_myself);
			}
		}
	}
	m_enemyList.clear();

	map<int, Smart_Ptr<GirlWar> >::iterator itr = m_targetGirlWar.begin();
	for (; itr != m_targetGirlWar.end(); ++itr)
	{
		itr->second->UpdateEnemyList(m_myself);
	}

	m_targetGirlWar.clear();
}

void Player::AddMonster(Smart_Ptr<Monster> &obj)
{
	m_targetMonster[obj->GetKey()] = obj;
}

void Player::SetPath(PlayerSynch::PathToAllPlayer &path)
{
	if(!IsInCreatureState(ERS_Walk))
	{
		SetCreatureState(ERS_Walk);
	}
	m_path = path;
	m_curIndex = 0;
	SetPathTime();

	ClearMonsterPath();
}

void Player::ClearPath()
{
	if(IsInCreatureState(ERS_Walk))
	{
		UnsetCreatureState(ERS_Walk);
	}

	m_path.Clear();
	m_synPathTime = 0;
	m_curIndex = 0;
}

bool Player::GetPath(PlayerSynch::PathToAllPlayer & path, int &index)
{
	if(!m_path.has_charid())
	{
		return false;
	}

	if(m_curIndex < 0 || (m_curIndex + 1) >= m_path.pos_size())
	{
		return false;
	}

	path = m_path;
	index = m_curIndex;

	return true;
}

void Player::SetPathTime()
{
	m_synPathTime = CUtil::GetNowSecond();
}

void Player::AddGirlWar(Smart_Ptr<GirlWar> &obj)
{
	m_targetGirlWar[obj->GetKey()] = obj;
}

void Player::DeleteGirlWar(int key)
{
	map<int, Smart_Ptr<GirlWar> >::iterator itr = m_targetGirlWar.find(key);
	if (itr != m_targetGirlWar.end())
	{
		m_targetGirlWar.erase(itr);
	}
}

void Player::ClearGirlWar()
{
	m_targetGirlWar.clear();
}

void Player::SendObjLeave(Smart_Ptr<Object> & obj)
{
	switch (obj->GetType())
	{
	case ePlayer:
	{
		ServerReturn::ServerRetChar toGate;
		char charid[64] = { 0 };
		Smart_Ptr<Player> player = obj;

		snprintf(charid, 64, "%d", player->GetKey());
		toGate.set_ret(charid);

		QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,&toGate, MSG_SIM_GT2C_SYNPLAYERLEAVE);

		break;
	}
	case eMonster:
	{
		ServerReturn::ServerRetInt toGate;
		Smart_Ptr<Monster> mon = obj;
		toGate.set_ret(mon->GetKey());

		if(mon->GetQuality() == eCompeteMonster)
		{
			QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,&toGate, MSG_SIM_GM2C_SYNCH_COMPETE_LEVEAL);
		}
		else
		{
			QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,&toGate, MSG_SIM_GT2C_SYNMONSTERLEAVE);
		}
		break;
	}
	case ePet:
	{
		ServerReturn::ServerRetInt toPlayer;
		Smart_Ptr<GirlWar> girlWar = obj;
		toPlayer.set_ret(girlWar->GetKey());

		QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,&toPlayer, MSG_SIM_GM2C_SYNGIRLWARLEAVE);

		break;
	}
	default:
	{
		LOG_ERROR(FILEINFO, "unknown object type[%d] leave player view", obj->GetType());
	}
}
}

void Player::SendObjectSynch(Smart_Ptr<Object> & obj)
{
	switch (obj->GetType())
	{
	case ePlayer:
	{
		PlayerSynch::PathToAllPlayer path;
		PlayerSynch::SynchPlayerBase info;
		Smart_Ptr<Player> player = obj;
		int index = 0;

		//获取技能
		player->GetSkillMgr()->getSynData(info);

		if (player->GetPath(path, index))
		{
			for (int i = index; i < path.pos_size(); ++i)
			{
				PlayerSynch::CharPos pos = path.pos(i);
				PlayerSynch::CharPos * desPos = info.add_pos();

				desPos->set_xpos(pos.xpos());
				desPos->set_ypos(pos.ypos());
			}
		}

		info.set_charid(player->GetID());
		info.set_key(player->GetKey());
		info.set_name(player->GetName());
		info.set_hp(player->getAttManage()->getValue<int>(eCharHP));
		info.set_hpmax(player->getAttManage()->getValue<int>(eCharHPMax));
		info.set_mp(player->getAttManage()->getValue<int>(eCharMP));
		info.set_mpmax(player->getAttManage()->getValue<int>(eCharMPMax));
		info.set_xpos(player->GetXNewPos());
		info.set_ypos(player->GetYNewPos());
		info.set_movespeed(player->getAttManage()->getValue<int>(eCharMoveSpeed));
		info.set_profession(player->getAttManage()->getValue<BYTE>(eCharProfession));

		//置换生物状态 m_state 字段已去除
		if (player->IsInCreatureStateAll(ERS_Fight))
		{
			info.set_state(eFightState);
		}
		else if (player->IsInCreatureStateAll(ERS_Meditation))
		{
			info.set_state(eMeditation);
			//双修对象
			Smart_Ptr<Player> target = player->GetMeditationMgr()->GetTargetInfo();
			if(target.Get() != NULL)
			{
				::PlayerSynch::MeditationInfo* pItem = info.mutable_meditation();
				pItem->set_targetkey(target->GetKey());
				pItem->set_xpos(target->GetXNewPos());
				pItem->set_ypos(target->GetYNewPos());

				Smart_Ptr<GirlWar> selfgirlwar = player->GetGirlWarManager()->GetGirlWar();
				Smart_Ptr<GirlWar> girlwar = target->GetGirlWarManager()->GetGirlWar();

				pItem->set_selfgirlwarkey( selfgirlwar ? selfgirlwar->GetKey() : 0 );
				pItem->set_targetgirlwarkey( girlwar ? girlwar->GetKey() : 0 );
			}
		}
		else
		{
			info.set_state(eAttackSim);
		}

		info.set_country(player->getAttManage()->getValue<DWORD>(eCharCountry));
		info.set_sex(player->GetSex());
		player->GetSkillMgr()->SetBuffInfo(&info);
		info.set_horse(player->GetHorseModelID());
		info.set_lv(player->getAttManage()->getValue<int>(eCharLv));
		info.set_titleid(player->getAttManage()->getValue<int>(eTitleID));
		info.set_fightpower(player->getAttManage()->getValue<int>(eFightPower));
		info.set_magicweapon(player->GetMagicWeapon()->GetMagicWeapon());
		info.set_clanname(player->GetClanName());
		info.set_clanid(player->getAttManage()->getValue<DWORD>(eCharFaction));
		info.set_viplv(player->getAttManage()->getValue<DWORD>(eVIPLv));
		info.set_clanjob(player->getAttManage()->getValue<BYTE>(eCharFaJob));
		info.set_clanicon(player->getAttManage()->getValue<BYTE>(eCharClanIcon));
		info.set_sinvalue(int(player->getAttManage()->getValue<float>(eSinValue)));
		info.set_fashionid(player->GetFashionID());

		//printf("\n 同步玩家信息: %s, 状态：%d, 变身: %d \n", player->GetName().c_str(), player->IsInCreatureState(ERS_Distortion), player->GetDistortionMgr()->GetMonsterID());

		//变身信息
		if(player->IsInCreatureState(ERS_Distortion))
		{
			info.set_distortion( player->GetDistortionMgr()->GetMonsterID() );
		}

		//不知道主武器的位置，先根据客户端放的位置临时取的
		Smart_Ptr<EquipProp> propBase = player->GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceWeapon - 1);
		Smart_Ptr<EquipProp> costumeProp = player->GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceArmor - 1);
		int costumeID = 0;

		PlayerSynch::EquipData* tEquip = info.mutable_equip();
		if (propBase && (propBase->GetID() != -1))
		{
			tEquip->set_id(propBase->GetID());
			tEquip->set_strenggrade(propBase->GetUpgrade());
		}
		
		if (costumeProp && (costumeProp->GetID() != -1))
		{
			costumeID = costumeProp->GetID();
		}
		info.set_costume(costumeID);

		QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &info,MSG_SIM_GT2C_SYNPLAYERIN);

		break;
	}
	case eMonster:
	{
		Smart_Ptr<Monster> monster = obj;

		if (monster->GetQuality() == eCompeteMonster)
		{
			PlayerSynch::SynchCompeteMonster info;

			monster->SetSynchData(&info);
			QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,&info, MSG_SIM_GM2C_SYNCH_COMPETE_MONSTER);
		}
		else
		{
			PlayerSynch::SynchMonsterBase info;

			monster->SetSynchData(&info);
			QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,&info, MSG_SIM_GT2C_SYNMONSTERIN);
		}

		break;
	}
	case eDropItem:
	{

		break;
	}
	case ePet:
	{
		PlayerSynch::SynchGirlWar info;

		Smart_Ptr<GirlWar> girlWar = obj;

		info.set_id(girlWar->GetID());
		info.set_key(girlWar->GetKey());
		info.set_ownerkey(girlWar->GetOwner()->GetKey());
		info.set_lv(girlWar->getAttManage()->getValue<int>(eCharLv));
		info.set_hp(girlWar->getAttManage()->getValue<int>(eCharHP));
		info.set_hpmax(girlWar->getAttManage()->getValue<int>(eCharHPMax));

		info.set_xpos(girlWar->GetXNewPos());
		info.set_ypos(girlWar->GetYNewPos());

		//主人双修对象
		Smart_Ptr<Player> target = girlWar->GetOwner()->GetMeditationMgr()->GetTargetInfo();
		if(target.Get() != NULL)
		{
			info.set_targetplayerkey(target->GetKey());
			Smart_Ptr<GirlWar> girl = target->GetGirlWarManager()->GetGirlWar();
			info.set_targetgirlkey( girl ? girl->GetKey() : 0 );
		}

		QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &info,MSG_SIM_GM2C_SYNGIRLWAR);
		if (girlWar->GetBrain())
		{
			PlayerSynch::GirlWarPathToPlayer topath;

			girlWar->GetBrain()->SetPath(topath);
			topath.set_key(girlWar->GetKey());
			girlWar->SynchPath(topath);
		}

		break;
	}
	default:
	{
		LOG_ERROR(FILEINFO, "unknown object type[%d] join in player view", obj->GetType());
	}
}
}

void Player::SendSynchCommon(::google::protobuf::Message *from, int messageid)
{
	QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, from,	messageid);
}

void Player::SendSynchMessage(::google::protobuf::Message *from, int msgid)
{
	QuickMessage::GetInstance()->AddSendMessage(GetID(), GetChannelID(),from,msgid);
}

void Player::SendToAround(::google::protobuf::Message *content, int messageid, bool isMyself)
{
	CSceneMgr::GetInstance()->SendSynchMessage(content, messageid, GetMapID(), GetKey(), GetXNewPos(), GetYNewPos(), isMyself);
}

void Player::SendSynchPath(PlayerSynch::PathToAllPlayer & path)
{
//	PlayerSynch::PathToGate synpath;
//	PlayerSynch::PathToAllPlayer* toPlayer = synpath.mutable_path();
//
//	synpath.set_mycharid(GetID());
//	toPlayer->CopyFrom(path);

//		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(
//		                MSG_SIM_GM2GT_SYNPATH, &synpath, m_channelID,
//		                SimpleMessage);
//		Message_Facade::Send(messRet);
		QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &path, MSG_SIM_GT2C_SYNPATH);
}

void Player::SendSynchPath(PlayerSynch::MonsterPathToPlayer & path)
{
	QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &path,MSG_SIM_GT2C_SYNMONSTERPATH);
}

void Player::SendSynchPath(PlayerSynch::GirlWarPathToPlayer & path)
{
	QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &path,MSG_SIM_GM2C_SYNGIRLWARPATH);
}

void Player::SetMeridiansVitality()
{
	const int maxAcupoint = MeridianLoader::GetInstance()->GetMaxMeridian();
	//满经脉直接完成活跃度
	if(m_acupoint >= maxAcupoint)
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(eViAcupointUp);
		if(vi)
			m_vitalityMgr->FinshVitality(eViAcupointUp,vi->count);
	}
}

void Player::SetVitality()
{
	SetMeridiansVitality();
	GetHorseManager()->SetVitality();
	GetDanLuManager()->SetVitality();
	GetMagicWeapon()->SetVitality();
	GetGirlWarManager()->SetVitality();
}

void Player::InitBaseInfo(const PlayerInfo::BaseInfo &info)
{
	SetID(info.charid());
	SetName(info.charname());
	getAttManage()->setValue(eCharExp,info.exp());
	getAttManage()->setValue(eCharLv,info.level());
	getAttManage()->setValue(eCharMoney, info.money());
	getAttManage()->setValue(eCharGolden, info.golden());
	getAttManage()->setValue(eCharBindGolden, info.bindgolden());
	getAttManage()->setValue(ePvpFlag, info.pvpflag());
	getAttManage()->setValue(eGuide, info.guide());
	getAttManage()->setValue(eChallengeMoney, info.challengemoney());

	m_deadTime = info.deadtime();
	m_sex = info.sex();

	getAttManage()->setValue(eCharProfession, info.profession());

	SetXPos(info.xpos());
	SetYPos(info.ypos());

	this->SetHorseModelID(info.horse());

	this->SetFashionID(info.fashionid());

	SetMapID(info.mapid());

	getAttManage()->setValue(eCharCountry, info.country());
	getAttManage()->setValue(eCharHead, info.charhead());

	getAttManage()->setValue(eCharMate, info.mate());

	getAttManage()->setValue(eBattleContribution, info.battlecontri());

	getAttManage()->setValue(eArmyJob, info.armyjob());

	getAttManage()->setValue(eCountryJob, info.countryjob());

	getAttManage()->setValue(eCountryContri,info.countrycontri());

	m_cContribution_Week = info.weekcontribute();
	getAttManage()->setValue(eCurCountryContribute, info.curcontribute());
	m_FitchPrize = info.fitch();
	m_kingRankFlag = info.kingrankflag();

	getAttManage()->setValue(eTitleID, info.titleid());

	m_accType = info.accounttype();
	m_openID = info.openid();
	m_openKey = info.openkey();
	m_pf = info.pf();
	m_pfKey = info.pfkey();
	m_loginClientType = info.clienttype();

	getAttManage()->setValue(eSinValue, float(info.sinvalue()));

	if (getAttManage()->getValue<float>(eSinValue) >= SIN_RED_NAME_COUNT)
	{
		m_sinState = ERS_RED_NAME;
	}

	m_acupoint = info.acupoint();
	SetMeridiansVitality();

	getAttManage()->setValue(eSettings, info.settings());
	getAttManage()->setValue(ePrestige, info.prestige());
	getAttManage()->setValue(eVIPLv, info.viplv());
	getAttManage()->setValue(eAddUpGold, info.addupgolde());
	getAttManage()->setValue(eGoldenCost, info.goldencost());
	getAttManage()->setValue(eContinueAnswer, info.continueanswer());
	getAttManage()->setValue(eRightAnswer, info.rightanswer());
	getAttManage()->setValue(eRanking, info.ranking());
	getAttManage()->setValue(eAwardRanking, info.awardranking());
	m_AntiAddictionMgr->InitAntiAddictionSystem(info.antiaddictionstate(),info.antiaddictiononlinetime());
	m_Daily->InitDailyPractice(info.practicetime());

	this->SetOffLineTime(info.offlinetime());

	m_totalOnlineTime = info.totalonlinetime();
	m_nextTalkTime = info.nexttalktime();

	if (GetCounterService()->getResetDayType() != eDayResetToday)
	{
		getAttManage()->setValue(eHaveTodo, 0);
		getAttManage()->setValue(eHaveTodoAward, 0);
		getAttManage()->setValue(eActivities, 0);
		getAttManage()->setValue(eActivitiesAward, 0);

		if (this->getAttManage()->getValue<DWORD>(eCharCountry) > 0)
			m_maxCanGetEnvelopeCount = this->getEnvelopeMgr()->GetCanGetEnvelopeCountMax(getAttManage()->getValue<int>(eCharLv));
		else
			m_maxCanGetEnvelopeCount = 0;
	}
	else
	{
		m_onlineTime = info.onlinetime();
		getAttManage()->setValue(eHaveTodo, info.havetodo());
		getAttManage()->setValue(eHaveTodoAward, info.havetodoaward());
		getAttManage()->setValue(eActivities, info.activites());
		getAttManage()->setValue(eActivitiesAward, info.activitesaward());

		if (this->getAttManage()->getValue<DWORD>(eCharCountry) > 0)
			m_maxCanGetEnvelopeCount = info.maxcangetenvelopecount();
		else
			m_maxCanGetEnvelopeCount = 0;
	}

	m_onlineReward->InitOnlineReward(info.onlinereward(), info.historyreward());
	m_levelReward->InitLevelReward(info.levelreward());
	m_treasonKingReward->InitTreasonKingReward(info.treasonkingreward());
	m_serverScore = info.score();

	//重新上线时，玩家是没有队伍的，pk模式要校正
	m_oldPkType = (info.pktype() == 0 ? 1 : info.pktype());
	if(m_oldPkType == sTeam)
	{
		m_oldPkType = 1;
	}

	m_ip = info.loginip();
	getAttManage()->setValue(eCredit,info.credit());
	getAttManage()->setValue(eEnvelopeCount,info.envelopecount());
	getAttManage()->setValue(eTeamId,info.teamid());
	static_cast<AttributePlayer*>(getAttManage().Get())->setTime(info.attributecleartime());

	this->SetInitPartDataFlag(eBaseInfo);

//	int iRet = 0;
//	map<string,string> params;
//	Json::Value JsonRes;
//	string script_name = "/v3/user/is_vip";
//	TencentConfig tencentConfig =  CPayLoader::GetInstance()->GetTencentConfig();
//	params["openid"] = m_openID;
//	params["openkey"] = m_openKey;
//	params["pf"] = m_pf;
//	params["pfkey"] = m_pfKey;
//
//    COpenApiV3 sdk(tencentConfig.nAppId, tencentConfig.sAppKey);
//    //对象初始化，设置服务器地址
//    sdk.init();
//    sdk.setServerName(tencentConfig.sServerName);
//    sdk.setDebugSwitch(true);
//
//    iRet=sdk.api(script_name, params,JsonRes);
//    if( 0 != iRet)
//    {
//    	LOG_DEBUG(FILEINFO,"iRet=%d err:%s\n",iRet,sdk.GetErrMsg());
//    }
//    else
//    {
//	    if(JsonRes["ret"].asInt() != 0)
//	    {
//	    	LOG_DEBUG(FILEINFO,"msg = %s ",JsonRes["msg"].asString().c_str());
//	    }
//	   else
//	    {
//			m_YellowVip = JsonRes["is_yellow_vip"].asInt();
//			m_YellowYearVip = JsonRes["is_yellow_year_vip"].asInt(); //是否为年费黄钻用户（0：不是； 1：是）
//			m_YellowVipLevel = JsonRes["yellow_vip_level"].asInt();	//黄钻等级。
//			m_YellowHighVip = JsonRes["is_yellow_high_vip"].asInt(); //是否为豪华版黄钻用户（0：不是； 1：是）。
//
//			LOG_DEBUG(FILEINFO,"msg = %s ",JsonRes["msg"].asString().c_str());
//	    }
//    }

    getAttManage()->setValue(eGourdExp,info.gourdexp());
    m_58playSouce = info.source();
    	m_rechargeTime = info.rechargetime();
}

void Player::SetBaseInfo(PlayerInfo::BaseInfo *info)
{
	info->set_charid(GET_PLAYER_CHARID(GetID()));
	info->set_charname(GetName());
	info->set_exp(getAttManage()->getValue<int64>(eCharExp));
	info->set_level(getAttManage()->getValue<int>(eCharLv));
	info->set_money(getAttManage()->getValue<int>(eCharMoney));
	info->set_golden(getAttManage()->getValue<int>(eCharGolden));
	info->set_bindgolden(getAttManage()->getValue<int>(eCharBindGolden));
	info->set_deadtime(m_deadTime);
	info->set_sex(m_sex);
	info->set_profession(getAttManage()->getValue<BYTE>(eCharProfession));
	info->set_xpos(GetXNewPos());
	info->set_ypos(GetYNewPos());
	info->set_horse(this->GetHorseModelID());
	info->set_mapid(GetMapID());
	info->set_country(getAttManage()->getValue<DWORD>(eCharCountry));
	info->set_charhead(getAttManage()->getValue<BYTE>(eCharHead));
	info->set_pvpflag(getAttManage()->getValue<int>(ePvpFlag));
	info->set_guide(getAttManage()->getValue<int>(eGuide));
	info->set_challengemoney(getAttManage()->getValue<int64>(eChallengeMoney));


	info->set_facontribution(getAttManage()->getValue<int>(eFaContribution));
	info->set_faction(getAttManage()->getValue<DWORD>(eCharFaction));
	info->set_clanname(m_clanName);
	info->set_fajob(getAttManage()->getValue<BYTE>(eCharFaJob));
	info->set_clanicon(getAttManage()->getValue<BYTE>(eCharClanIcon));

	info->set_mate(getAttManage()->getValue<int64>(eCharMate));
	info->set_countryjob(getAttManage()->getValue<WORD>(eCountryJob));
	info->set_battlecontri(getAttManage()->getValue<int>(eBattleContribution));
	info->set_armyjob(getAttManage()->getValue<WORD>(eArmyJob));
	info->set_countrycontri(getAttManage()->getValue<int>(eCountryContri));
	info->set_power(getAttManage()->getValue<int>(eFightPower));
	info->set_fitch(m_FitchPrize);
	info->set_weekcontribute(m_cContribution_Week);
	info->set_curcontribute(getAttManage()->getValue<int>(eCurCountryContribute));
	info->set_kingrankflag(m_kingRankFlag);
	info->set_titleid(getAttManage()->getValue<int>(eTitleID));
	info->set_accounttype(m_accType);
	info->set_serverid(GET_SERVER_CHARID(GetID()));
	info->set_openid(m_openID);
	info->set_openkey(m_openKey);
	info->set_pf(m_pf);
	info->set_pfkey(m_pfKey);
	info->set_sinvalue((int) getAttManage()->getValue<float>(eSinValue));
	info->set_acupoint(m_acupoint);
	info->set_settings(getAttManage()->getValue<int>(eSettings));
	info->set_prestige(getAttManage()->getValue<int>(ePrestige));
	info->set_havetodo(getAttManage()->getValue<int64>(eHaveTodo));
	info->set_havetodoaward(getAttManage()->getValue<int>(eHaveTodoAward));
	info->set_activites(getAttManage()->getValue<int>(eActivities));
	info->set_activitesaward(getAttManage()->getValue<int>(eActivitiesAward));
	info->set_offlinetime(this->GetOffLineTime());
	info->set_onlinereward(m_onlineReward->GetRecord());
	info->set_historyreward(m_onlineReward->GetHistoryReward());
	info->set_levelreward(m_levelReward->GetRecord());
	info->set_treasonkingreward(m_treasonKingReward->GetRewardInfo());
	info->set_fashionid(this->GetFashionID());

	int64 nowonlinetime = CalOnlineTime();
	info->set_onlinetime(nowonlinetime);  //此次登录在线时长

	info->set_totalonlinetime(CUtil::GetNowSecond() - m_loginTime + m_totalOnlineTime); //记录 总在线时长

	info->set_viplv(getAttManage()->getValue<DWORD>(eVIPLv));
	info->set_addupgolde(getAttManage()->getValue<int>(eAddUpGold));
	info->set_goldencost(getAttManage()->getValue<int>(eGoldenCost));
	info->set_continueanswer(getAttManage()->getValue<int>(eContinueAnswer));
	info->set_rightanswer(getAttManage()->getValue<int>(eRightAnswer));
	info->set_score(m_serverScore);

	int antiAddictionState = (int) m_AntiAddictionMgr->GetAntiAddictionState();
	info->set_antiaddictionstate(antiAddictionState);
	int antiAddictionTime = 0;
	int antiAddictionAddTime = 0;
	if (antiAddictionState == eAAS_NotBind || antiAddictionState == eAAS_BindButMinor)
	{
		antiAddictionAddTime = CUtil::GetNowSecond() - m_AntiAddictionMgr->GetLastSaveTime();
		antiAddictionTime = m_AntiAddictionMgr->GetCurrentAntiAddictionTime();
	}
	info->set_antiaddictiononlinetime(antiAddictionTime);
	info->set_antiaddictionaddtime(antiAddictionAddTime);
	m_AntiAddictionMgr->SetLastSaveTime(CUtil::GetNowSecond());

	info->set_nexttalktime(m_nextTalkTime);
	if (m_Daily->GetIsGotPracticeRward())
		info->set_practicetime(0);
	else
		info->set_practicetime(m_Daily->GetPracticeTime());

	info->set_pktype(m_oldPkType);
	info->set_loginip(m_ip);

	info->set_credit(getAttManage()->getValue<int>(eCredit));
	info->set_couplename(GetCoupleName());
	info->set_envelopecount(getAttManage()->getValue<int>(eEnvelopeCount));
	info->set_maxcangetenvelopecount(getMaxCanGetEnvelopeCount());
	info->set_teamid(getAttManage()->getValue<int>(eTeamId));
	info->set_attributecleartime(static_cast<AttributePlayer*>(getAttManage().Get())->getTime());
	info->set_offlinetime(CUtil::GetNowSecond());
	info->set_mgicweaponpower(getFightingManage()->getFighting(eFightingMagicWeapon));
	info->set_gourdexp(getAttManage()->getValue<int>(eGourdExp));
	info->set_source(Get58PlaySouce());
	info->set_rechargetime(GetRechargeTime());
}

//通知所有模块上线计算增加的永久属性,目前只用于玩家上线
void Player::onlineCalculateAtt()
{
	//等级
	onlineCalculateLevelAtt();

	//称号
	GetTitle()->onlineCalculateAtt();

	//官运
	GetCJobLucky()->onlineCalculateAtt();

	//装备
	static_cast<ArticleEquipMgr*>(GetContainer(eEquipQuickType).Get())->onlineCalculateAtt();

	//强化、洗炬、宝石额外属性
	getExtraAtributeMgr()->onlineCalculateAtt();

	//时装
	GetFashionMgr()->OnlineCalAttr();

	//坐骑
	GetHorseManager()->onlineCalculateAtt();

	//神兵
	GetMagicWeapon()->onlineCalculateAtt();

	//法器
	GetSMagic()->onlineCalculateAtt();

	//心法
	GetHeartMagicWeapon()->onlineCalculateAtt();

	//战姬
	GetGirlWarManager()->onlineCalculateAtt();

	//任务
	GetQuestMgr()->onlineCalculateAtt();

	//技能
	GetSkillMgr()->onlineCalculateAtt();

	onlineCalculateMeridianAtt();

	//丹炉
	GetDanLuManager()->onlineCalculateAtt();

	GetMarryRing()->onlineCalculateAtt();

	GetHeroMgr()->onlineCalculateAtt();

	//上线时初始化陈营属性
	onlineCalculateCampAtt();

	AttributeManage::GetInstance()->checkAtt(*this);

	return;
}

//等级增加的属性
void Player::onlineCalculateLevelAtt()
{
	map<ECharAttrType, int64> tempAttList;

	getDesignationLvAttList(getAttManage()->getValue<int>(eCharLv),getAttManage()->getValue<BYTE>(eCharProfession), tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttLevel);

	calculateResultAddAtt(tempAttList,tempValueType);

	calculateSomeLevelFight(getAttManage()->getValue<int>(eCharLv));

	return;
}

//阵营增加的属性
void Player::onlineCalculateCampAtt()
{
	if (getAttManage()->getValue<DWORD>(eCharCountry) == 0)
		return;

	std::vector<std::pair<int, int> > tempAtt;
	GsCountryMgr::GetInstance()->getCampAttr(getAttManage()->getValue<DWORD>(eCharCountry), tempAtt);
	map<ECharAttrType, int64> tempAttList;

	for (size_t i = 0; i < tempAtt.size(); ++i)
	{
		tempAttList[(ECharAttrType) tempAtt[i].first] += tempAtt[i].second;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttCamp);

	calculateResultAddAtt(tempAttList,tempValueType);

	CountryJobAttrAdd();

	calculateCampFight();

	return;
}

//上线计算经脉
void Player::onlineCalculateMeridianAtt()
{
	if(GetAcupoint() <= 0)
		return ;

	map<ECharAttrType, int64> tempAttList;

	MeridianMgr::GetInstance()->getBforeIDAllAtt(tempAttList,GetAcupoint(),(ECharProf)getAttManage()->getValue<BYTE>(eCharProfession));

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttMeridian);

	calculateResultAddAtt(tempAttList,tempValueType);

	calculateMeridianFight();

	return ;
}

void Player::calculateResultAddAtt(map<ECharAttrType, int64>& attList,eAttValueType valueType)
{
	map<ECharAttrType, int64>::iterator tempIt = attList.begin();
	for (; tempIt != attList.end(); ++tempIt)
	{
		if (tempIt->second == 0)
			continue;

		getAttManage()->addValue(tempIt->first,tempIt->second,valueType);
	}

	return;
}

int Player::calculateSomeLevelFight(int level)
{
	map<ECharAttrType,int64> tempAttList;

	getDesignationLvAttList(level,getAttManage()->getValue<BYTE>(eCharProfession), tempAttList);

	int tempResult = m_Fighting->CalculateFighting(tempAttList);
	return m_Fighting->setFighting(eFightingLevel,tempResult);
}

//计算阵营加的战斗力
int Player::calculateCampFight()
{
	std::vector<std::pair<int, int> > tempAtt;
	GsCountryMgr::GetInstance()->getCampAttr(getAttManage()->getValue<DWORD>(eCharCountry), tempAtt);
	map<ECharAttrType, int64> tempAttList;

	for (size_t i = 0; i < tempAtt.size(); ++i)
	{
		tempAttList[(ECharAttrType) tempAtt[i].first] += tempAtt[i].second;
	}

	int tempResult = m_Fighting->CalculateFighting(tempAttList);
	return m_Fighting->setFighting(eFightingCamp,tempResult);
}

//计算经脉战斗力
int Player::calculateMeridianFight()
{
	map<ECharAttrType, int64> tempAttList;

	MeridianMgr::GetInstance()->getBforeIDAllAtt(tempAttList,GetAcupoint(),(ECharProf)getAttManage()->getValue<BYTE>(eCharProfession));

	int tempResult = m_Fighting->CalculateFighting(tempAttList);
	return m_Fighting->setFighting(eFightingMeridian,tempResult);
}

void Player::SynchPlayerInfoToWorld()
{
	CharLogin::SynchToWorldInfo toworld;

	toworld.set_charid(GetID());
	toworld.set_fight(getAttManage()->getValue<int>(eFightPower));
	toworld.set_m_viplevel(getAttManage()->getValue<DWORD>(eVIPLv));
	int yesterdayReward = 0;
	for (uint i = 0; i < this->GetCityRewardList().size(); ++i)
	{
		yesterdayReward |= (1 >> (GetCityRewardList()[i] - 1));
	}
	toworld.set_isyesterdayreward(yesterdayReward);
	Smart_Ptr<SvrItem> worldSvr;

	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(GetID()),worldSvr);
	if (!worldSvr)
		return;

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYN_ATTR, &toworld, worldSvr.Get(), SimpleMessage);
	Message_Facade::Send(messRet);
}

void Player::SaveAllPlayer(void * obj)
{
	//新手副本状态下，不保存信息
	int flag = getAttManage()->getValue<int>(eGuide);
	if(flag == 0)
	{
		return;
	}

	SaveDataInfo sendData;
	SaveAllPlayer(sendData, false, false);

	//同步角色信息到世界服
	SynchPlayerInfoToWorld();

	//为了 10分钟 统计 玩家 属性  不再添加新定时器
	if (m_AttrTick >= 10)
	{
		m_AttrTick = 0;
//		UpateStastic();
	}
	else
	{
		m_AttrTick++;
	}
}

void Player::SaveAllPlayer(SaveDataInfo& sendData, bool flag, bool isDel)
{
	vector<PlayerInfo::SaveTypeInfo> infoVec;

	PlayerInfo::SaveTypeInfo info;
	PlayerInfo::SaveTypeInfo tmpInfo;

	DWORD64 type = 0;
	DWORD64 leftValue = 1;

	for (unsigned int i = 0; i < eCharStructMax; ++i)
	{
		if ((m_dirtyData[i] || flag) && (m_initpartData[i]))
		{
			type |= (leftValue << i);

			switch (i)
			{
			case eBaseInfo:
			{
				PlayerInfo::BaseInfo * bInfo = info.mutable_bsinfo();
				SetBaseInfo(bInfo);

				break;
			}
			case eBattleInfo:
			{
				PlayerInfo::BattleInfo *baInfo = info.mutable_binfo();
				SetBattleInfo(baInfo);

				if(baInfo->hpmax() <= 0)
				{
					LOG_ERROR(FILEINFO, "save player[%lld] battle info[%d] but null", baInfo->hpmax(), baInfo->ByteSize());
				}

				break;
			}
			case eQuestInfo:
			{
				PlayerInfo::QuestInfo * qInfo = info.mutable_questinfo();
				SetQuestInfo(qInfo);
				if(qInfo->active_size() <= 0)
				{
					LOG_ERROR(FILEINFO, "save player[%lld] quest info[%d] but null", GetID(), qInfo->ByteSize());
				}

				break;
			}
			case eCDInfo:
			{
				PlayerInfo::CDInfo * cdInfo = info.mutable_cd();
				m_cdMgr->SetCDInfo(cdInfo);

				break;
			}
			case eSkillInfo:
			{
				PlayerInfo::SkillInfo * sInfo = info.mutable_skill();
				SetSkillInfo(sInfo, flag);

				break;
			}
			case ePackageInfo:
			{
				PlayerInfo::PackageInfo * paInfo = info.mutable_package();
				m_container[ePackageType]->GetArticleInfo(paInfo);

				break;
			}
			case eStoreHouseInfo:
			{
				PlayerInfo::StoreHouseInfo * stInfo = info.mutable_storehouse();
				m_container[eStoreHouseType]->GetArticleInfo(stInfo);

				break;
			}
			case eEquipQuickInfo:
			{
				PlayerInfo::EquipQuickInfo * eqInfo = info.mutable_equipquick();
				m_container[eEquipQuickType]->GetArticleInfo(eqInfo);

				break;
			}
			case eMonitorInfo:
			{
				PlayerInfo::MonitorInfo *mtInfo = info.mutable_monitor();
				mtInfo->CopyFrom(m_monitorInfo);

				break;
			}
			case eHorseInfo:
			{
				PlayerInfo::HorseInfo *hsInfo = info.mutable_horse();
				if (m_horseMgr)
				{
					m_horseMgr->SetHorseToDB(hsInfo);
				}
				break;
			}
			case eFriendInfo:
			{
				break;
			}
			case eInstanceInfo:
			{
				PlayerInfo::InstanceInfoTwoDays *instanceInfo =info.mutable_instance();

				if (m_instanceMgr)
				{
					m_instanceMgr->SetInstanceInfo(instanceInfo);
				}
				break;
			}
			case eMoneyInstanceInfo:
			{
				PlayerInfo::MoneyInstance *instanceMoney =info.mutable_info();
				if(m_instanceMgr)
				{
					m_instanceMgr->SetMoneyInstanceInfo(instanceMoney);
				}
				break;
			}
			case ePrestigeInfo:
			{
				PlayerInfo::PrestigeInfoList *prestigeInfo =info.mutable_prestige();
				if (m_Achievement)
				{
					m_Achievement->SetPrestigeToDB(prestigeInfo);
				}
				break;
			}
			case eDestinationInfo:
			{
				PlayerInfo::DestinationInfoList *destinationInfo =info.mutable_destination();
				if (m_Destination)
				{
					m_Destination->SetDestinationToDB(destinationInfo);
				}
				break;
			}
			case eSignInInfo:
			{
				PlayerInfo::SignInInfo *signinInfo = info.mutable_signin();
				if (m_signIn)
				{
					m_signIn->SetSignInToDB(signinInfo);
				}
				break;
			}
			case eCounterInfo:
			{
				PlayerInfo::CounterServiceList *counterInfo =info.mutable_counter();
				if (m_counterService)
				{
					m_counterService->SetCounterServiceToDB(counterInfo);
				}
				break;
			}
			case eTitleInfo:
			{
				PlayerInfo::TitleInfoList *titleInfo = info.mutable_titles();
				if (m_Title)
				{
					m_Title->SetTitleToDB(titleInfo);
				}
				break;
			}
			case eEscortInfo:
			{
				PlayerInfo::EscortInfo *escortInfo = info.mutable_escort();
				if (m_escort)
				{
					m_escort->SetEscortToDB(escortInfo);
				}
				break;
			}
			case eVipInfo:
			{
				PlayerInfo::PlayerVipInfo *vipInfo = info.mutable_vipinfo();
				if (m_VipPart)
				{
					m_VipPart->SaveToDB(vipInfo);
				}
				break;
			}
			case eDanLuInfo:
			{
				PlayerInfo::DanLuInfo *danluInfo = info.mutable_danlu();
				if ((bool) m_danluMgr)
				{
					m_danluMgr->SetDanLuToDB(danluInfo);
				}
				break;
			}

			case eMagicWeapon:
			{
				PlayerInfo::MagicWeapon *magicInfo = info.mutable_magicw();
				if (m_magicWeapon)
				{
					m_magicWeapon->SetMagicWeapon(magicInfo);
				}

				break;
			}

			case eSMagic:
			{
				PlayerInfo::MagicWeapon *magicInfo = info.mutable_smagic();
				if (m_sMagic)
				{
					m_sMagic->SetMagicWeapon(magicInfo);
				}
				break;
			}

			case eHeartMagic:
			{
				PlayerInfo::MagicWeapon *magicInfo = info.mutable_heartmagic();
				if (m_heartMagic)
				{
					m_heartMagic->SetMagicWeapon(magicInfo);
				}
				break;
			}


			case eCJobItem:
			{
				PlayerInfo::RandCJobLucky *cjob = info.mutable_cjob();
				if (m_cjobMgr)
				{
					m_cjobMgr->SetCJobLucky(cjob);
				}

				break;
			}
			case eLucklyTurntable:
			{
				PlayerInfo::LucklyTurntable *lucktable =info.mutable_lucktable();
				if ((bool) m_lucktableMgr)
				{
					m_lucktableMgr->SetLucklyTurntableInfo(lucktable);
				}
				break;
			}
			case eActivityInfo:
			{
				PlayerInfo::Activity *activitytable = info.mutable_activity();
				if ((bool) m_activity)
				{
					m_activity->SetActivitytableInfo(activitytable);
				}
				break;
			}
			case eGirlWarInfo:
			{
				PlayerInfo::GirlWarInfo *girltable = info.mutable_girlwar();
				if ((bool) m_GirlWarMgr)
				{
					m_GirlWarMgr->SetGirlWarInfo(girltable);
				}
				break;
			}
			case eFashionInfo:
			{
				FashionMsgProto::FashionShow *fashionInfo =info.mutable_fashion();
				if ((bool) m_fashionShow)
				{
					m_fashionShow->SaveWardrobe(*fashionInfo);
				}
				break;
			}
			case eVitalityInfo:
			{
				VitalityProto::ProtoVitalityInfo *vitalityInfo =info.mutable_vitality();
				if ((bool) m_vitalityMgr)
				{
					m_vitalityMgr->SetVitalityToDB(vitalityInfo);
				}
				break;
			}

			case eCampShopInfo:
			{
				PlayerInfo::CampShopInfo *pCamp = info.mutable_campdata();
				if (getCampShopMgr())
				{
					getCampShopMgr()->getShopData(*pCamp);
				}
				break;
			}
			case eFestivalInfo:
			{
				PlayerInfo::FestivalInfo *pFes = info.mutable_festivaldata();
				if(getFestivalMgr())
				{
					getFestivalMgr()->getFestiveAttrData(*pFes);
				}
				break;
			}
			case eOnceRewardInfo:
			{
				PlayerInfo::OnceRewardInfo* onceReward = info.mutable_oncereward();
				if ((bool)GetOnceReward())
				{
					GetOnceReward()->SetInfoToDB(*onceReward);
				}
				break;
			}
			case eMarryRingInfo:
			{
				PlayerInfo::RingDBData* tempRingProto = info.mutable_ringdata();
				m_MarryRing->getDataToDB(*tempRingProto);
				break;
			}
			case eGroupInstanceInfo:
			{
				PlayerInfo::GroupInstance *instanceGroup =info.mutable_groupinfo();
				if(m_instanceMgr)
				{
					m_instanceMgr->SetGroupInstanceInfo(instanceGroup);
				}
				break;
			}
			case eChristmasInfo:
			{
				ChristmasDB::ChristmasAllData* tempChristmas = info.mutable_christmasdata();
				m_Christmas->getDBdata(*tempChristmas);

				m_NewYear->getToDB(*tempChristmas);

				break;
			}
			case eEliteInstanceInfo:
			{

				PlayerInfo::EliteInstance* instanceElite = info.mutable_eliteinfo();
				if(m_instanceMgr)
				{
					m_instanceMgr->SetEliteInstanceInfo(instanceElite);
				}

				break;
			}
			case eCauchemarInstanceInfo:
			{
				PlayerInfo::CauchemarInstance* instanceCauchemar = info.mutable_cauchemarinfo();
				if(m_instanceMgr)
				{
					m_instanceMgr->SetCauchemarInstanceInfo(instanceCauchemar);
				}

				break;
			}
			case eArenaInfo:
			{
				PlayerInfo::ArenaDataInfo* arenaData = info.mutable_arenadata();
				if (m_arenaMgr)
				{
					m_arenaMgr->SaveArenaData(*arenaData);
				}
				break;
			}

			case eChallengeInfo:
			{
				PlayerInfo::ChallengeInstance* challenge = info.mutable_challengedata();
				if(m_challengeMgr)
				{
					m_challengeMgr->GetChallengeInfo(challenge);
				}
				break;
			}

			case eHeroInstanceInfo:
			{
				PlayerInfo::HeroInstanceData* hero = info.mutable_herodata();
				if(m_heroMgr)
				{
					m_heroMgr->SetHeroInfo(hero);
				}
				break;
			}
			case eAdvancedInstanceInfo:
			{
				PlayerInfo::AdvancedInstance* instanceAdvanced = info.mutable_advanced();
				if(m_instanceMgr)
				{
					m_instanceMgr->SetAdvancedInstanceInfo(instanceAdvanced);
				}

				break;
			}
			case eBaseShopInfo:
			{
				PlayerInfo::BaseShopInfo* baseShop = info.mutable_baseshopinfo();
				if(m_baseShopMgr)
				{
					m_baseShopMgr->SetBaseShopInfo(baseShop);
				}

				break;
			}
			case eAnswerInfo:
			{
				PlayerInfo::AnswerInfo* answer = info.mutable_answerinfo();
				if(m_answerMgr)
				{
					m_answerMgr->SetAnswerInfo(answer);
				}

				break;
			}
			default:
			{
				LOG_ERROR(FILEINFO, "timer save player info but type is error");
			}
			}

			m_dirtyData[i] = false;

			if (info.ByteSize() > NORMAL_MSG_SEND_SIZE)
			{
				info.set_type(type);
				info.set_id(GET_PLAYER_CHARID(GetID()));
				info.set_isdel(false);
				type = 0;
				infoVec.push_back(info);
				info.CopyFrom(tmpInfo);  //不能直接用Clear()
			}
		}
	}

	if (type > 0)
	{
		info.set_type(type);
		info.set_id(GET_PLAYER_CHARID(GetID()));
		info.set_isdel(false);
		infoVec.push_back(info);
		info.CopyFrom(tmpInfo);  //
	}

	int GuideFlag = getAttManage()->getValue<int>(eGuide);
	sendData.count = GuideFlag == 0 ? 0 : infoVec.size();

	int sizeFlag = (int)infoVec.size() - 1;
	for(int i = 0; i < sendData.count; ++i)
	{
		PlayerInfo::SaveTypeInfo sendtmpInfo;
		sendtmpInfo.CopyFrom(infoVec[i]);

		if (i == sizeFlag)
			sendtmpInfo.set_isdel(isDel);

		if(sendData.msgtype==0)
		{
			SendToSaveData(&sendtmpInfo);
		}
		else
		{
			SendToSaveData(&sendtmpInfo,sendData);
		}
	}

	//下线时没有任何改变也要发送
	if(isDel && sendData.count==0)
	{
		PlayerInfo::SaveTypeInfo sendtmpInfo;
		sendtmpInfo.set_type(0);
		sendtmpInfo.set_id(GET_PLAYER_CHARID(GetID()));
		sendtmpInfo.set_isdel(isDel);
		SendToSaveData(&sendtmpInfo, sendData);
	}

	if(flag)
	{
		//玩家下线 保存数据 同时  释放 资源
		this->ReleasePlayer();
	}

	//如果没有数据没改变，请求的消息要返回
	if(sendData.count==0 && sendData.msgtype > 0)
	{
		PlayerMessage::GetInstance()->dealWithSaveResult(sendData);
	}

}

void Player::SendToSaveData(PlayerInfo::SaveTypeInfo *info)
{
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GT2GM_SAVE_PLAYERINFO, info, m_dbChannelID, SimpleMessage);
	Message_Facade::Send(messRet);
}

void Player::SendToSaveData(PlayerInfo::SaveTypeInfo *info, SaveDataInfo& sendData)
{
	PlayerMessage::GetInstance()->addSendDataCach(GetID(), sendData);
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GS2DB_SAVE_PLAYERDATA, info, m_dbChannelID, Request);
	messRet->SetAct(new IntAct(GetID()));
	Message_Facade::Send(messRet);
}


void Player::InitBattleInfo(const PlayerInfo::BattleInfo &info)
{
//	SetHPMax(info.hpmax());
	getAttManage()->setValue(eCharHP, info.hp());
	if (getAttManage()->getValue<int>(eCharHP) <= 0)
	{
		if (CUtil::GetNowSecond() - m_deadTime >= 30 * 1000)
		{
			getAttManage()->setValue(eCharHP, info.hpmax());
		}
		else
		{
			SetCreatureState(ERS_Dead);
		}
	}

	getAttManage()->setValue(eCharMP, info.mp());
	getAttManage()->setValue(eCharForce, info.force());
	getAttManage()->setValue(eCharForceMax, info.forcemax());
	getAttManage()->setValue(eCharPatience, info.patience());
	getAttManage()->setValue(ePKCount, info.pkcount());
	getAttManage()->setValue(ePower, info.power());
	getAttManage()->setValue(ePowerMax, info.powermax());

	this->SetInitPartDataFlag(eBaseInfo);
	this->SetInitPartDataFlag(eBattleInfo);
}

void Player::SetBattleInfo(PlayerInfo::BattleInfo *info)
{
	info->set_hp(getAttManage()->getValue<int>(eCharHP));
	info->set_hpmax(getAttManage()->getValue<int>(eCharHPMax));
	info->set_mp(getAttManage()->getValue<int>(eCharMP));
	info->set_mpmax(getAttManage()->getValue<int>(eCharMPMax));
	info->set_force(getAttManage()->getValue<int>(eCharForce));
	info->set_forcemax(getAttManage()->getValue<int>(eCharForceMax));
	info->set_strength(getAttManage()->getValue<int>(eCharStrength));
	info->set_cleverpower(getAttManage()->getValue<int>(eCleverPower));
	info->set_cleverair(getAttManage()->getValue<int>(eCleverAir));
	info->set_physicalpower(getAttManage()->getValue<int>(eCharPhysicalPower));
	info->set_patience(getAttManage()->getValue<int>(eCharPatience));
	info->set_agility(getAttManage()->getValue<int>(eCharAgility));
	info->set_pkcount(getAttManage()->getValue<int>(ePKCount));
	info->set_power(getAttManage()->getValue<int>(ePower));
	info->set_powermax(getAttManage()->getValue<int>(ePowerMax));
	info->set_phyattackmax(getAttManage()->getValue<int>(eCharPhyAttackMax));
	info->set_phydefence(getAttManage()->getValue<int>(eCharPhyDefence));
	info->set_hitrate(getAttManage()->getValue<int>(eCharHitRate));
	info->set_avoidrate(getAttManage()->getValue<int>(eCharAvoidRate));
	info->set_crackdownrate(getAttManage()->getValue<int>(eCrackDownRate));
	info->set_baserte(getAttManage()->getValue<int>(eAvoidCrackRate));
	info->set_movespeed(getAttManage()->getValue<int>(eCharMoveSpeed));

}

void Player::SetBattleInfo(CharBase::CharAttr * info)
{
	info->set_strength(getAttManage()->getValue<int>(eCharStrength));
	info->set_cleverpower(getAttManage()->getValue<int>(eCleverPower));
	info->set_cleverair(getAttManage()->getValue<int>(eCleverAir));
	info->set_physicalpower(getAttManage()->getValue<int>(eCharPhysicalPower));
	info->set_patience(getAttManage()->getValue<int>(eCharPatience));
	info->set_agility(getAttManage()->getValue<int>(eCharAgility));
	info->set_hphyattack(getAttManage()->getValue<int>(eCharPhyAttackMax));
	info->set_phydefence(getAttManage()->getValue<int>(eCharPhyDefence));
	info->set_hitrate(getAttManage()->getValue<int>(eCharHitRate));
	info->set_avoidattack(getAttManage()->getValue<int>(eCharAvoidRate));
	info->set_crackdown(getAttManage()->getValue<int>(eCrackDownRate));
	info->set_avoidcrack(getAttManage()->getValue<int>(eAvoidCrackRate));
	info->set_fight(getAttManage()->getValue<int>(eFightPower));
	info->set_power(getAttManage()->getValue<int>(ePower));
	info->set_powermax(getAttManage()->getValue<int>(ePowerMax));
}

void Player::SetCharBaseInfo(CharLogin::CharBaseInfo *info)
{
	PlayerInfo::CDInfo cdInfo;

	info->set_charid(GetID());
	info->set_key(GetKey());
	info->set_name(GetName());
	info->set_level(getAttManage()->getValue<int>(eCharLv));
	info->set_hp(getAttManage()->getValue<int>(eCharHP));
	info->set_hpmax(getAttManage()->getValue<int>(eCharHPMax));
	info->set_mp(getAttManage()->getValue<int>(eCharMP));
	info->set_mpmax(getAttManage()->getValue<int>(eCharMPMax));
	info->set_mapid(GetMapID());
	info->set_xpos(GetXPos());
	info->set_ypos(GetYPos());
	info->set_exp(getAttManage()->getValue<int64>(eCharExp));
	info->set_country(getAttManage()->getValue<DWORD>(eCharCountry));
	info->set_countryjob(getAttManage()->getValue<WORD>(eCountryJob));
	info->set_contrycontribute(getAttManage()->getValue<int>(eCountryContri));
	info->set_sex(GetSex());
	info->set_profession(getAttManage()->getValue<BYTE>(eCharProfession));
	info->set_force(getAttManage()->getValue<int>(eCharForce));
	info->set_forcemax(getAttManage()->getValue<int>(eCharForceMax));
	info->set_movespeed(getAttManage()->getValue<int>(eCharMoveSpeed));
	info->set_horse(GetHorseModelID());
	info->set_sinvalue(int(getAttManage()->getValue<float>(eSinValue)));
	info->set_acupoint(GetAcupoint());
//	info->set_settings(getAttManage()->getValue<int>(eSettings));
	GetSettings(info->mutable_settings());
	info->set_logintime(GetLoginTime());
	info->set_fightpower(getAttManage()->getValue<int>(eFightPower));
	info->set_titleid(getAttManage()->getValue<int>(eTitleID));
	info->set_viplv(getAttManage()->getValue<DWORD>(eVIPLv));
	info->set_addupgold(GetTypeValue(eAddUpGold));
	info->set_faction(getAttManage()->getValue<DWORD>(eCharFaction));
	info->set_job(getAttManage()->getValue<BYTE>(eCharFaJob));
	info->set_clanname(GetClanName());
	info->set_clanicon(getAttManage()->getValue<BYTE>(eCharClanIcon));
	info->set_totalonlinetime(m_totalOnlineTime);

	info->set_antiaddictionstate(m_AntiAddictionMgr->GetAntiAddictionState());
	info->set_antiaddictiononlinetime(m_AntiAddictionMgr->GetCurrentAntiAddictionTime());

	info->set_money(getAttManage()->getValue<int>(eCharMoney));
	info->set_gold(getAttManage()->getValue<int>(eCharGolden));
	info->set_bgold(getAttManage()->getValue<int>(eCharBindGolden));
	info->set_challengemoney(getAttManage()->getValue<int64>(eChallengeMoney));

	info->set_fashionid(this->GetFashionID());
	info->set_rechargestate(m_VipPart->IsOver());
	info->set_coupleid(getAttManage()->getValue<int64>(eCouple));

	info->set_envelopecount(getAttManage()->getValue<int>(eEnvelopeCount));

	info->set_credit(getAttManage()->getValue<int>(eCredit));

	info->set_is_yellow_vip(GetYelloVip());
	info->set_is_yellow_year_vip(GetYellowYearVip());
	info->set_is_yellow_high_vip(GetYellowHighVip());
	info->set_yellow_vip_level(GetYellowVipLevel());
	info->set_couplename(GetCoupleName());
	info->set_guide(getAttManage()->getValue<int>(eGuide));

	if (GET_MAP_TYPE(GetMapID()) >= eCopyMap)
	{
		info->set_isininstance(GetMapID());
	}

	if(m_VipPart)
	{
		info->set_vipcardlv(getAttManage()->getValue<int>(eVipCardLv));
		info->set_vipcardtime(m_VipPart->GetVipCardRestTime());
	}

	MapStaticInfo *sMapInfo = NULL;
	sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(GetMapID()));

	if (sMapInfo)
	{
		info->set_pktype(GetPkTypeByMap(sMapInfo->pktype));
	}

	//由于副本异常退出会导致X，Y坐标错误，需要重新校准
	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
	if (zone)
	{
		if (!zone->IsValidPos(GetXPos(), GetYPos()))
		{
			if (sMapInfo)
			{
				SetXPos(sMapInfo->reliveXPos);
				SetYPos(sMapInfo->reliveYPos);
				info->set_xpos(sMapInfo->reliveXPos);
				info->set_ypos(sMapInfo->reliveYPos);
			}
		}
	}

	PlayerInfo::MonitorInfo *clientMtInfo = info->mutable_mt();

	clientMtInfo->CopyFrom(m_monitorInfo);

	m_skillMgr->clientGetInfo(info->mutable_skillinfo());

	m_cdMgr->SetCDInfo(&cdInfo);

	for (int j = 0; j < cdInfo.cd_size(); ++j)
	{
		PlayerInfo::ChannelCD tmpChannel = cdInfo.cd(j);
		int spaceTime = tmpChannel.endtime() - CUtil::GetNowSecond();

		if (spaceTime <= 0)
		{
			continue;
		}

		CharLogin::CharCDTime *tmpClient = info->add_cd();

		tmpClient->set_type(tmpChannel.type());
		tmpClient->set_time(spaceTime);
	}

	//装备信息
	CharLogin::EquipQuickInfo * tmpEquiInfo = info->mutable_nowequip();
	GetContainer(eEquipQuickType)->GetArticleInfo(tmpEquiInfo);

}

void Player::SetQuestInfo(PlayerInfo::QuestInfo *info)
{
	m_quest->SetQuestInfoToDB(info);
}

void Player::InitQuestInfo(const PlayerInfo::QuestInfo &info)
{
	m_quest->InitQuestInfoFromDB(info);
	this->SetInitPartDataFlag(eQuestInfo);
}

void Player::SynchPlayerCompeteInfoToWS(int ranks, bool isSuccess)
{
	PlayerInfo::ArenaInfo req;
	PlayerInfo::ArenaBaseInfo * info = req.mutable_abinfo();

	req.set_charid(GetID());
	req.set_combatpower(getAttManage()->getValue<int>(eFightPower));
	req.set_ranking(ranks);
	req.set_iswin(isSuccess);
	req.set_challengenum(m_counterService->GetNumById(ARENACHALLROBOT_COUNT));

	info->set_charname(GetName());
	info->set_level(getAttManage()->getValue<int>(eCharLv));
	info->set_profession(getAttManage()->getValue<BYTE>(eCharProfession));
	info->set_country(getAttManage()->getValue<DWORD>(eCharCountry));
	info->set_sex(m_sex);
	info->set_titleid(getAttManage()->getValue<int>(eTitleID));
	info->set_clanname(m_clanName);
	info->set_clanjob(getAttManage()->getValue<BYTE>(eCharFaJob));
	info->set_viplv(getAttManage()->getValue<DWORD>(eVIPLv));
	info->set_hpmax(getAttManage()->getValue<int>(eCharHPMax));
	info->set_mpmax(getAttManage()->getValue<int>(eCharMPMax));
	info->set_movespeed(getAttManage()->getValue<int>(eCharMoveSpeed));
	info->set_phyattackmax(getAttManage()->getValue<int>(eCharPhyAttackMax));
	info->set_phydefence(getAttManage()->getValue<int>(eCharPhyDefence));
	info->set_hitrate(getAttManage()->getValue<int>(eCharHitRate));
	info->set_avoidrate(getAttManage()->getValue<int>(eCharAvoidRate));
	info->set_crackdownrate(getAttManage()->getValue<int>(eCrackDownRate));
	info->set_baserte(getAttManage()->getValue<int>(eAvoidCrackRate));
	info->set_magicw(m_magicWeapon->GetMagicWeapon());

	{
		Smart_Ptr<EquipProp> propBase = GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceFashion - 1);

		if (propBase && (propBase->GetID() != -1))
		{
			info->set_suitrnt(propBase->GetID());
		}

		propBase = GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceWeapon - 1);

		if (propBase)
		{
			if (propBase->GetID() < 0)
			{
				info->mutable_mainequip()->set_id(0);
				info->mutable_mainequip()->set_strenggrade(0);
			}
			else
			{
				info->mutable_mainequip()->set_id(propBase->GetID());
				info->mutable_mainequip()->set_strenggrade(propBase->GetUpgrade());
			}
		}

		propBase = GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceArmor - 1);

		if (propBase)
		{
			if (propBase->GetID() < 0)
			{
				info->set_costume(0);
			}
			else
			{
				info->set_costume(propBase->GetID());
			}
		}
	}

	info->set_horse(this->GetHorseModelID());
	m_skillMgr->SynchSkillInfoToWS(info);

	Smart_Ptr<SvrItem> worldSvr;

	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(GetID()),worldSvr);
	if (worldSvr)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2WS_SYNCARENA, &req, worldSvr.Get(), Request);
		Message_Facade::Send(messRet);
	}
}

void Player::InitSkillInfo(const PlayerInfo::SkillInfo &info)
{
	m_skillMgr->InitSkillInfo(info);
	this->SetInitPartDataFlag(eSkillInfo);
}

void Player::InitMonitorInfo(const PlayerInfo::MonitorInfo &info)
{
	m_monitorInfo.Clear();
	m_monitorInfo.CopyFrom(info);
	this->SetInitPartDataFlag(eMonitorInfo);
}

void Player::SetSkillInfo(PlayerInfo::SkillInfo *info, bool isDel)
{
	if (m_skillMgr)
		m_skillMgr->SetSkillInfo(info, isDel);
}

void Player::InitInstanceInfo(const PlayerInfo::InstanceInfoTwoDays & info)
{
	m_instanceMgr->InitInstanceInfo(info);
	this->SetInitPartDataFlag(eInstanceInfo);
}

void Player::InitMoneyInstance(const PlayerInfo::MoneyInstance &info)
{
		m_instanceMgr->InitMoneyInstanceInfo(info);
		SetInitPartDataFlag(eMoneyInstanceInfo);
}

void Player::InitGroupInstance(const PlayerInfo::GroupInstance &info)
{
	m_instanceMgr->InitGroupInstanceInfo(info);
	SetInitPartDataFlag(eGroupInstanceInfo);
}

void Player::InitPrestigeInfo(const PlayerInfo::PrestigeInfoList &info)
{
	m_Achievement->InitPrestigeInfo(info);
	this->SetInitPartDataFlag(ePrestigeInfo);
}

void Player::InitDestinationInfo(const PlayerInfo::DestinationInfoList &info)
{
	m_Destination->InitDestinationInfo(info);
	this->SetInitPartDataFlag(eDestinationInfo);
}

void Player::InitTitleInfo(const PlayerInfo::TitleInfoList &info)
{
	m_Title->InitTitleInfo(info);
	this->SetInitPartDataFlag(eTitleInfo);
}

void Player::InitSignInInfo(const PlayerInfo::SignInInfo &info)
{
	m_signIn->InitSignInInfo(info);
	this->SetInitPartDataFlag(eSignInInfo);
}

void Player::InitCounterInfo(const PlayerInfo::CounterServiceList &info)
{
	m_counterService->AddCounterInfo(info);
	this->SetInitPartDataFlag(eCounterInfo);
}

void Player::InitEscortInfo(const PlayerInfo::EscortInfo &info)
{
	m_escort->InitEscort(info);
	this->SetInitPartDataFlag(eEscortInfo);
}

void Player::InitVipInfo(const PlayerInfo::PlayerVipInfo &info)
{
	m_VipPart->InitInfo(info);
	this->SetInitPartDataFlag(eVipInfo);
}

void Player::InitBaseShopInfo(const PlayerInfo::BaseShopInfo &info)
{
	m_baseShopMgr->InitBaseShopInfo(info);
	this->SetInitPartDataFlag(eBaseShopInfo);
}

void Player::InitAnswerInfo(const PlayerInfo::AnswerInfo& info)
{
	m_answerMgr->InitAnswerInfo(info);
	this->SetInitPartDataFlag(eAnswerInfo);
}

//void Player::InitArenaInfo(const PlayerInfo::ArenaDBInfo &info)
//{
//	m_arenaMgr->InitInfo(info);
//	this->SetInitPartDataFlag(eArenaInfo);
//}

void Player::InitGirlWarInfo(const PlayerInfo::GirlWarInfo &info)
{
	m_GirlWarMgr->InitGirlWarInfo(info);
	this->SetInitPartDataFlag(eGirlWarInfo);
}

void Player::InitFashionInfo(const FashionMsgProto::FashionShow& info)
{
	m_fashionShow->InitWardrobe(info);
	this->SetInitPartDataFlag(eFashionInfo);
}

void Player::InitVitalityInfo(const VitalityProto::ProtoVitalityInfo& info)
{
	m_vitalityMgr->InitVitalityFromDB(info);
	this->SetInitPartDataFlag(eVitalityInfo);
}

//void Player::InitWorshipInfo(WorshipMsgProto::ClientWorshipInfo& info)
//{
//	m_worshipMgr->InitWorshipInfo(info);
//	this->SetInitPartDataFlag(eWorshipInfo);
//}

void Player::InitOnceRewardInfo(const PlayerInfo::OnceRewardInfo& info)
{
	m_onceReward->InitInfoByDB(info);
	this->SetInitPartDataFlag(eOnceRewardInfo);
}

void Player::InitEliteInstance(const PlayerInfo::EliteInstance &info)
{
	m_instanceMgr->InitEliteInstanceInfo(info);
	SetInitPartDataFlag(eEliteInstanceInfo);
}

void Player::InitCauchemarInstance(const PlayerInfo::CauchemarInstance &info)
{
	m_instanceMgr->InitCauchemarInstanceInfo(info);
	SetInitPartDataFlag(eCauchemarInstanceInfo);
}

void Player::InitAdvancedInstance(const PlayerInfo::AdvancedInstance& info)
{
	m_instanceMgr->InitAdvancedInstanceInfo(info);
	SetInitPartDataFlag(eAdvancedInstanceInfo);
}

void Player::SetGirlWarInfo(PlayerInfo::GirlWarInfo *info)
{
	if (m_GirlWarMgr)
	{
		m_GirlWarMgr->SetGirlWarInfo(info);
	}
}

void Player::MoneyChange(int money, bool type)
{
	if (money <= 0)
	{
		return;
	}

	int tempMoney = getAttManage()->getValue<int>(eCharMoney);
//	int moneyBeforeChange = tempMoney;
	if (type)
	{
		float antiAddictionRation = this->GetAntiAddictionFactor();

		money = int(money * antiAddictionRation);

		tempMoney += money;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddMoney(money));
//		BEHAVIOR_HANDLE_DO(GetMyself(), eBehaviorHandle_AddMoney, StatistSaveType::Int64_Data(money));
	}
	else
	{
		tempMoney -= money;
		if (tempMoney < 0)
			tempMoney = 0;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostMoney(money));
	}

	getAttManage()->setValue(eCharMoney, tempMoney);
	SynCharAttribute(eCharMoney, tempMoney);

	m_dirtyData[eBaseInfo] = true;

//	if(mainType != eStatic_Max && minorType != eStaMinor_Max)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//		if(type)
//		{
//			StatisticInfo::GotItemInfo * gotItem = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItem,eCharMoney,money);
//		}
//		else
//		{
//			StatisticInfo::CostItemInfo * costItem = costAndGotInfo.add_costlist();
//			costItem->set_beforenum(moneyBeforeChange);
//			StatisticMgr::SetItemCost(costItem,eCharMoney,money);
//			StatisticMgr::SetItemCostAfter(costItem,eCharMoney,this->GetMyself());
//		}
//
//		StatisticCurrencyChangeArgs args(this->GetID(),mainType,minorType,&costAndGotInfo);
//		FireGlobalEvent(STATISTIC_CURRENCY_CHANGE, args);
//	}

	EventArgs args;
	FireEvent(PLAYER_MONEY_CHANGE, args);
}

int Player::ChangeMoney(int money, bool isAdd)
{
	int tempMoney = getAttManage()->getValue<int>(eCharMoney);
//	int moneyBeforeChange = tempMoney;
	if (money <= 0)
	{
		return tempMoney;
	}

	if (isAdd)
	{
		float antiAddictionRation = this->GetAntiAddictionFactor();

		money = int(money * antiAddictionRation);

		tempMoney += money;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddMoney(money));
	}
	else
	{
		tempMoney -= money;
		if (tempMoney < 0)
			tempMoney = 0;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostMoney(money));
	}

	getAttManage()->setValue(eCharMoney, tempMoney);

	m_dirtyData[eBaseInfo] = true;

//	if(mainType != eStatic_Max && minorType != eStaMinor_Max)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//		if(isAdd)
//		{
//			StatisticInfo::GotItemInfo * gotItem = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItem,eCharMoney,money);
//		}
//		else
//		{
//			StatisticInfo::CostItemInfo * costItem = costAndGotInfo.add_costlist();
//			costItem->set_beforenum(moneyBeforeChange);
//			StatisticMgr::SetItemCost(costItem,eCharMoney,money);
//			StatisticMgr::SetItemCostAfter(costItem,eCharMoney,this->GetMyself());
//		}
//
//		StatisticCurrencyChangeArgs args(this->GetID(),mainType,minorType,&costAndGotInfo);
//		FireGlobalEvent(STATISTIC_CURRENCY_CHANGE, args);
//	}

	EventArgs args;
	FireEvent(PLAYER_MONEY_CHANGE, args);

	return tempMoney;
}

int Player::ChangeGolden(int value, bool isAdd)
{
	int tempGolden = getAttManage()->getValue<int>(eCharGolden);
	if (value <= 0)
	{
		return tempGolden;
	}

	if (isAdd)
	{
		tempGolden += value;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddGolden(value));
	}
	else
	{
		tempGolden -= value;
		if (tempGolden < 0)
			tempGolden = 0;
		//记录 元宝消耗的
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_myself,eStatic_AboutGolden, eStaMinor_Golen_Cost, value);
		//记录总的元宝消耗
		ChangeGoldenCost(value);
		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostGolden(value));
	}

	getAttManage()->setValue(eCharGolden, tempGolden);
	SynCharAttribute(eCharGolden, tempGolden);
	SynchCharAttributeToWS(eCharGolden, tempGolden);
	m_dirtyData[eBaseInfo] = true;

//	if(mainType != eStatic_Max && minorType != eStaMinor_Max)
//	{
//		StatisticGoldenChangeArgs args(this->GetMyself(),mainType,minorType,num,value);
//		FireGlobalEvent(STATISTIC_GOLDEN_CHANGE, args);
//	}

	return tempGolden;
}

int Player::ChangeGoldenCost(int value, bool isAdd)
{
	int tempGolden = getAttManage()->getValue<int>(eGoldenCost);
	if (value <= 0)
	{
		return tempGolden;
	}

	if (isAdd)
	{
		tempGolden += value;
	}
	else
	{
		tempGolden -= value;
		if (tempGolden < 0)
			tempGolden = 0;

	}

	getAttManage()->setValue(eGoldenCost, tempGolden);
	m_dirtyData[eBaseInfo] = true;

	return tempGolden;
}

int Player::ChangeBindGolden(int value, bool isAdd)
{
	int tempBindGolden = getAttManage()->getValue<int>(eCharBindGolden);
//	int bindGoldenBeforeChange = tempBindGolden;
	if (value <= 0)
	{
		return tempBindGolden;
	}

	if (isAdd)
	{
		tempBindGolden += value;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddBindGolden(value));

	}
	else
	{
		tempBindGolden -= value;
		if (tempBindGolden < 0)
			tempBindGolden = 0;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostBindGolden(value));

		//记录 元宝消耗的
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_myself,eStatic_BindGolden, eStaMinor_BindGolen_Cost, value);

	}

	getAttManage()->setValue(eCharBindGolden, tempBindGolden);

	m_dirtyData[eBaseInfo] = true;

//	if(mainType != eStatic_Max && minorType != eStaMinor_Max)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//		if(isAdd)
//		{
//			StatisticInfo::GotItemInfo * gotItem = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItem,eCharBindGolden,value);
//		}
//		else
//		{
//			StatisticInfo::CostItemInfo * costItem = costAndGotInfo.add_costlist();
//			costItem->set_beforenum(bindGoldenBeforeChange);
//			StatisticMgr::SetItemCost(costItem,eCharBindGolden,value);
//			StatisticMgr::SetItemCostAfter(costItem,eCharBindGolden,this->GetMyself());
//		}
//
//		StatisticCurrencyChangeArgs args(this->GetID(),mainType,minorType,&costAndGotInfo);
//		FireGlobalEvent(STATISTIC_CURRENCY_CHANGE, args);
//	}

	return tempBindGolden;
}

void Player::ChangeChallenge(int64 value, bool isAdd)
{
	int64 temp = getAttManage()->getValue<int64>(eChallengeMoney);
	if(value <= 0)
	{
		return;
	}

	temp = isAdd ? temp+value : temp-value;
	if(temp < 0)
	{
		temp = 0;
	}

	getAttManage()->setValue(eChallengeMoney,temp);
	m_dirtyData[eBaseInfo] = true;

	SynCharAttribute(eChallengeMoney, temp);
}

int Player::ChangeCredit(int value, bool isAdd)
{
	int tempCredit = getAttManage()->getValue<int>(eCredit);
//	int creditBeforeChange = tempCredit;
	if (value <= 0)
	{
		return tempCredit;
	}

	if (isAdd)
	{
		float antiAddictionRation = this->GetAntiAddictionFactor();
		value = int(value * antiAddictionRation);
		tempCredit += value;
		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddCredit(value));
	}
	else
	{
		tempCredit -= value;
		if (tempCredit < 0)
			tempCredit = 0;
		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostCredit(value));

	}

	getAttManage()->setValue(eCredit,tempCredit);

	m_dirtyData[eBaseInfo] = true;

//	if(mainType != eStatic_Max && minorType != eStaMinor_Max)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//		if(isAdd)
//		{
//			StatisticInfo::GotItemInfo * gotItem = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItem,eCredit,value);
//		}
//		else
//		{
//			StatisticInfo::CostItemInfo * costItem = costAndGotInfo.add_costlist();
//			costItem->set_beforenum(creditBeforeChange);
//			StatisticMgr::SetItemCost(costItem,eCredit,value);
//			StatisticMgr::SetItemCostAfter(costItem,eCredit,this->GetMyself());
//		}
//
//		StatisticCurrencyChangeArgs args(this->GetID(),mainType,minorType,&costAndGotInfo);
//		FireGlobalEvent(STATISTIC_CURRENCY_CHANGE, args);
//	}

	return tempCredit;
}

bool Player::DecreaseGoldenCost(int cost, int &golden, int &bgolden)
{
	if(cost <= 0)
		return true;

//	if(cost > getAttManage()->getValue<int>(eCharBindGolden))
//	{
//		bgolden = getAttManage()->getValue<int>(eCharBindGolden);
//		golden = cost - getAttManage()->getValue<int>(eCharBindGolden);
//	}
//	else
//	{
//		bgolden = cost;
//		golden = 0;
//	}
	bgolden = 0;
	golden = cost;
	if(bgolden > 0)
	{
		ChangeBindGolden(bgolden, false);
	}

	if(golden > 0)
	{
		ChangeGolden(golden, false);
	}

	return true;
}

bool Player::IsEnoughGolden(int cost)
{
	int tempGolden = getAttManage()->getValue<int>(eCharGolden);// + getAttManage()->getValue<int>(eCharBindGolden);

	if(tempGolden < cost)
		return false;

	return true;
}

bool Player::CostGolden(int cost)
{
	int tempGolden = getAttManage()->getValue<int>(eCharGolden);

	if (tempGolden < cost)
	{
		return false;
	}
	else
	{
		tempGolden -= cost;

		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostGolden(cost));
	}

	getAttManage()->setValue(eCharGolden, tempGolden);
	m_dirtyData[eBaseInfo] = true;
	return true;
}

int Player::CurHPChange(int hp, bool flag)
{
	int tHP = getAttManage()->getValue<int>(eCharHP);
	if (hp <= 0)
	{
		return tHP;
	}

	int tmp = 0;
	int tHPMax = getAttManage()->getValue<int>(eCharHPMax);

	if (flag)
	{
		if (tHP + hp > tHPMax)
		{
			tmp = tHPMax;
		} else {
			tmp = tHP + hp;
		}
	}
	else
	{
		tmp = tHP - hp;

		if (tmp < 0)
			tmp = 0;
	}

	SetCurHP(tmp);
	m_dirtyData[eBattleInfo] = true;

	return tmp;
}

void Player::ChangeBattleAttr(int endLv)
{
	if (getAttManage()->getValue<int>(eCharLv) == endLv)
	{
		return;
	}

	LvBattleAtt bAttr;
	LvBattleAtt eAttr;

	BattleInfoLoader::GetInstance()->GetBattleByLvAndPro(
			getAttManage()->getValue<int>(eCharLv),
			getAttManage()->getValue<BYTE>(eCharProfession), 
			bAttr);
	BattleInfoLoader::GetInstance()->GetBattleByLvAndPro(endLv,getAttManage()->getValue<BYTE>(eCharProfession), eAttr);
	if (bAttr.lv < 0 || eAttr.lv < 0)
	{
		return;
	}

	int beHP = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePhyPowerToHP, bAttr.physicalpower);
	int beMP = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eCleverAirtoMP, bAttr.cleverAir);
	int bePA = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eStrengthToAt, bAttr.strength);
	int bePD = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePatienceToPD, bAttr.patience);

	int edHP = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePhyPowerToHP, eAttr.physicalpower);
	int edMP = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eCleverAirtoMP, eAttr.cleverAir);
	int edPA = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eStrengthToAt, eAttr.strength);
	int edPD = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePatienceToPD, eAttr.patience);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttLevel);

	//最大血
	if (getAttManage()->getValue<int>(eCharHP) > 0)
	{
		getAttManage()->setValue(eCharHP,(getAttManage()->getValue<int>(eCharHPMax) - beHP) < 0 ? edHP :
						(getAttManage()->getValue<int>(eCharHPMax) - beHP + edHP));

		getAttManage()->setValue(eCharHPMax,getAttManage()->getValue<int>(eCharHP),tempValueType);
	}
	else
	{
		getAttManage()->setValue(eCharHPMax,(getAttManage()->getValue<int>(eCharHPMax) - beHP) < 0 ? edHP :
						(getAttManage()->getValue<int>(eCharHPMax) - beHP + edHP),tempValueType);
	}

	//最大法
	getAttManage()->setValue(eCharMP,(getAttManage()->getValue<int>(eCharMPMax) - beMP) < 0 ?edMP :
					(getAttManage()->getValue<int>(eCharMPMax) - beMP + edMP));
	getAttManage()->setValue(eCharMPMax,getAttManage()->getValue<int>(eCharMP),tempValueType);

	int tempPhyPower = (getAttManage()->getValue<int>(eCharPhysicalPower) - bAttr.physicalpower) < 0 ? eAttr.physicalpower :
					(getAttManage()->getValue<int>(eCharPhysicalPower) - bAttr.physicalpower + eAttr.physicalpower);
	getAttManage()->setValue(eCharPhysicalPower, tempPhyPower);

	int tempCleverAir = (getAttManage()->getValue<int>(eCleverAir) - bAttr.cleverAir) < 0 ? eAttr.cleverAir :
					(getAttManage()->getValue<int>(eCleverAir) - bAttr.cleverAir+ eAttr.cleverAir);
	getAttManage()->setValue(eCleverAir, tempCleverAir);

	int tempCleverPower =(getAttManage()->getValue<int>(eCleverPower) - bAttr.cleverPower)< 0 ? eAttr.cleverPower :
					(getAttManage()->getValue<int>(eCleverPower)- bAttr.cleverPower + eAttr.cleverPower);

	getAttManage()->setValue(eCleverPower, tempCleverPower);

	int tempTrength = ((getAttManage()->getValue<int>(eCharStrength) - bAttr.strength)< 0 ?eAttr.strength :
					(getAttManage()->getValue<int>(eCharStrength)- bAttr.strength + eAttr.strength));
	getAttManage()->setValue(eCharStrength, tempTrength);

	int tempPatience = (getAttManage()->getValue<int>(eCharPatience) - bAttr.patience) < 0 ? eAttr.patience :
					(getAttManage()->getValue<int>(eCharPatience)- bAttr.patience + eAttr.patience);
	getAttManage()->setValue(eCharPatience, tempPatience);

	int tempAgility = ((getAttManage()->getValue<int>(eCharAgility) - bAttr.agility) < 0 ? eAttr.agility :
					(getAttManage()->getValue<int>(eCharAgility) - bAttr.agility + eAttr.agility));

	getAttManage()->setValue(eCharAgility, tempAgility);

	//物攻
	getAttManage()->setValue(eCharPhyAttackMax,(getAttManage()->getValue<int>(eCharPhyAttackMax) - bePA) < 0 ?edPA :
					(getAttManage()->getValue<int>(eCharPhyAttackMax) - bePA + edPA),tempValueType);

	//物防
	getAttManage()->setValue(eCharPhyDefence,(getAttManage()->getValue<int>(eCharPhyDefence) - bePD) < 0 ?edPD :
					(getAttManage()->getValue<int>(eCharPhyDefence) - bePD+ edPD),tempValueType);

	//命中值
	getAttManage()->setValue(eCharHitRate,(getAttManage()->getValue<int>(eCharHitRate) - bAttr.hitRate) < 0 ?eAttr.hitRate :
					(getAttManage()->getValue<int>(eCharHitRate) - bAttr.hitRate+ eAttr.hitRate),tempValueType);

	//闪避值
	getAttManage()->setValue(eCharAvoidRate,(getAttManage()->getValue<int>(eCharAvoidRate) - bAttr.avoidAttack)< 0 ?eAttr.avoidAttack :
					(getAttManage()->getValue<int>(eCharAvoidRate)- bAttr.avoidAttack + eAttr.avoidAttack),tempValueType);

	//暴击值
	getAttManage()->setValue(eCrackDownRate,(getAttManage()->getValue<int>(eCrackDownRate) - bAttr.crackDown)< 0 ?
					eAttr.crackDown :(getAttManage()->getValue<int>(eCrackDownRate)- bAttr.crackDown + eAttr.crackDown),tempValueType);

	//抗暴
	getAttManage()->setValue(eAvoidCrackRate,(getAttManage()->getValue<int>(eAvoidCrackRate) - bAttr.avoidCrack)< 0 ?
					eAttr.avoidCrack :(getAttManage()->getValue<int>(eAvoidCrackRate)- bAttr.avoidCrack + eAttr.avoidCrack),tempValueType);

	m_dirtyData[eBaseInfo] = true;
	m_dirtyData[eBattleInfo] = true;
}

int64 Player::DecreaseExp(int exp)
{
	if (exp <= 0)
	{
		return -1;
	}
	int64 tempExp = getAttManage()->getValue<int64>(eCharExp);
	tempExp -= exp;
	if (tempExp < 0)
		tempExp = 0;

	getAttManage()->setValue(eCharExp, tempExp);

	m_dirtyData[eBaseInfo] = true;

	return tempExp;
}

void Player::SetCurHP(int hp)
{
	if (hp < 0)
		getAttManage()->setValue(eCharHP, 0);
	else if (hp > getAttManage()->getValue<int>(eCharHPMax))
		getAttManage()->setValue(eCharHP,getAttManage()->getValue<int>(eCharHPMax));
	else
		getAttManage()->setValue(eCharHP, hp);

	m_dirtyData[eBattleInfo] = true;
}

void Player::SetHPMax(int max)
{
	if(max < 0)
	{
		max = 0;
	}

	int tHP = getAttManage()->getValue<int>(eCharHP);
	if(max < tHP)
	{
		getAttManage()->setValue(eCharHP,max);
	}
	CreatureObj::SetHPMax(max);

	m_dirtyData[eBattleInfo] = true;
}

int64 Player::AddExp(int exp, double ration)
{
	if (ration == 0)
	{
		return AddExp(exp);
	}
	else
	{
		return AddExp((int)(ceil(exp * ration)));
	}
}

int Player::AddExp(int exp)
{
	int ret = 0;

	if (exp <= 0)
		return -1;

	float antiAddictionRation = this->GetAntiAddictionFactor();

	exp = int(exp * antiAddictionRation);

	if (getAttManage()->getValue<int>(eCharLv) == ((int) UpgradeLoader::GetInstance()->GetMaxLv()))
		return eHighestLv;

	int64 tempExp = getAttManage()->getValue<int64>(eCharExp);
	tempExp += exp;

	BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddExp(exp));

	getAttManage()->setValue(eCharExp, tempExp);

	if (tempExp <= 0)
	{
		return ret;
	}

	UpgradeInfo nowInfo;

	UpgradeLoader::GetInstance()->GetUpgradeByLv(getAttManage()->getValue<int>(eCharLv), nowInfo);
	if (nowInfo.lv == 0)
	{
		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
	}
	else
	{
		bool isUpgrade = false;
		vector<int> vecType;
		vector<int> vecBro;

		if(tempExp >= nowInfo.upgradeExp)
		{
			UpgradeInfo info;

			int64 have_exp = nowInfo.allExp - nowInfo.upgradeExp + tempExp;

			UpgradeLoader::GetInstance()->GetUpgradeByExp(have_exp, info);
			if(info.lv == 0 || info.lv == getAttManage()->getValue<int>(eCharLv))
			{
				LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
				return 0;
			}
			else
			{
				ChangeBattleAttr(info.lv);

				//从新计算一次等级战斗力
				calculateSomeLevelFight(info.lv);

				int oldLv = getAttManage()->getValue<int>(eCharLv);
				getAttManage()->setValue(eCharLv,info.lv);

				tempExp = info.upgradeExp - (info.allExp - have_exp); // m_exp - info.allExp + info.upgradeExp;

				if(tempExp >info.upgradeExp)
					tempExp = info.upgradeExp;

				getAttManage()->setValue(eCharExp,tempExp);

				if(tempExp <0)
				{
					getAttManage()->setValue(eCharExp,0);

					return ret;  //角色升级表 有问题
				}

				//向网关服同步世界服玩家升级信息
				SynchCharAttributeToWS(eCharLv, getAttManage()->getValue<int>(eCharLv));

				//向网关服同步世界服玩家升级信息
				vecBro.push_back(eCharLv);
				vecBro.push_back(eCharHP);
				vecBro.push_back(eCharHPMax);
				vecBro.push_back(eCharMP);
				vecBro.push_back(eCharMPMax);

				vecType.push_back(eCharPhysicalPower);
				vecType.push_back(eCleverPower);
				vecType.push_back(eCleverAir);
				vecType.push_back(eCharStrength);
				vecType.push_back(eCharPatience);
				vecType.push_back(eCharAgility);
				vecType.push_back(eCharPhyAttackMax);
				vecType.push_back(eCharPhyDefence);
				vecType.push_back(eFightPower);
				vecType.push_back(eCharHitRate);
				vecType.push_back(eCharAvoidRate);
				vecType.push_back(eCrackDownRate);
				vecType.push_back(eAvoidCrackRate);
				vecType.push_back(eMainEquip);
				vecType.push_back(eCostume);

				PlayerUpgradeEventArgs tArgs( GetMyself(), oldLv );
				FireEvent(PLAYER_LEVEL_UP_EVENT_TYPE, tArgs);

				OneParmaEventArgs e(this->GetMyself());
				FireGlobalEvent(PLAYER_LEVEL_UP_EVENT_TYPE, e);
				//30级以上就得同步竞技场数据了
				if(this->IsOpenSystem(eStCompeteInstance) && getAttManage()->getValue<int>(eRanking) <= 0)
				{
					SynchPlayerCompeteInfoToWS();
				}

				isUpgrade = true;
			}
		}

		m_dirtyData[eBaseInfo] = true;
		vecType.push_back(eCharExp);

		SynCharAttribute(vecType);
		BroadcastCharAttribute(vecBro);

		if (isUpgrade)
		{
			UpdateUIHighLight(eUIHLHandle_ResetAndSyn);
		}
	}

	return ret;
}

void Player::SynCharAttribute(BYTE type, int64 value, int state)
{
	CharBase::CharAttChange attr;
	attr.add_type(type);
	attr.add_value(value);
	attr.add_state(state);
	QuickMessage::GetInstance()->AddSendMessage(GetID(),GetChannelID(),&attr,MSG_SIM_GT2C_SYNCHATTR);
}

void Player::SynchCharAttributeToWS(BYTE type, int64 value, int state)
{
	CharBase::AttChangeToWorld toWorld;
	CharBase::CharAttChange * attr = toWorld.mutable_attr();
	Smart_Ptr<SvrItem> worldSvr;

	toWorld.set_charid(GetID());
	attr->add_type(type);
	attr->add_value(value);
	attr->add_state(state);

	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(GetID()),worldSvr);
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYNWCHATTR, &toWorld, worldSvr.Get(), SimpleMessage);
	Message_Facade::Send(messRet);
}

void Player::SynCharAttribute(vector<int> &type)
{
	if (type.size() <= 0)
	{
		return;
	}

	CharBase::CharAttChange toGate;

	for (unsigned int i = 0; i < type.size(); ++i)
	{
		toGate.add_type(type[i]);
		toGate.add_value(GetTypeValue(type[i]));
		toGate.add_state(eAttackSim);
	}

//		2014.07.31修改,防止过量的消息发送
	QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &toGate,MSG_SIM_GT2C_SYNCHATTR);
}

void Player::BroadcastCharAttribute(vector<int> &type, bool isMyself)
{
	if (type.size() <= 0)
	{
		return;
	}

	CharBase::SynchObjectAttr synAttr;
	CharBase::CharAttChange * change = synAttr.mutable_attr();

	synAttr.set_key(GetKey());
	for (size_t i = 0; i < type.size(); ++i)
	{
		change->add_type(type[i]);
		change->add_value(GetTypeValue(type[i]));
		change->add_state(eAttackSim);
	}

	CSceneMgr::GetInstance()->SendSynchAttrChange(synAttr, GetMapID(),GetXNewPos(), GetYNewPos(), isMyself);
}

void Player::BroadcastCharAttribute(int type, int value, int state,bool isMyself)
{
	CharBase::SynchObjectAttr synAttr;
	CharBase::CharAttChange * change = synAttr.mutable_attr();

	synAttr.set_key(GetKey());
	{
		change->add_type(type);
		change->add_value(value);
		change->add_state(state);
	}

	CSceneMgr::GetInstance()->SendSynchAttrChange(synAttr, GetMapID(),GetXNewPos(), GetYNewPos(), isMyself);
}

void Player::SendSynchAttr(CharBase::SynchObjectAttr &attr)
{
//	CharBase::SynchObjectAttr toGate;
//
//	toGate.CopyFrom(attr);

	//2014.07.31修正,防止大量的消息发送
	QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID, &attr,MSG_SIM_GT2C_SYNCHOBJECTATTR);
}

void Player::SendSynchAction(CharBattle::SynClientAttack &action)
{
	QuickMessage::GetInstance()->AddSendMessage(GetID(), GetChannelID(),&action,MSG_SIM_GT2C_SYNCHOBJECTACTION);
}

void Player::clearState()
{
		UnsetCreatureState(ERS_Fight);
		UnsetCreatureState(ERS_Gather);
		UnsetCreatureState(ERS_Still);
		UnsetCreatureState(ERS_Ice);
		UnsetCreatureState(ERS_Stun);
		UnsetCreatureState(ERS_Meditation);
		UnsetCreatureState(ERS_ImmunityStill);
		UnsetCreatureState(ERS_ImmunityIce);
		UnsetCreatureState(ERS_ImmunityStun);
}

void Player::Dead(Smart_Ptr<CreatureObj> &obj)
{
	if(IsDead())
	{
		return ;
	}

	//取消打坐
	CanccelMeditation();

	//清除状态
	clearState();

	Smart_Ptr<Player> player;
	SetCreatureState(ERS_Dead);
	m_deadTime = CUtil::GetNowSecond();
	UpdateEnemyList();

	m_dirtyData[eBaseInfo] = true;
	m_skillMgr->RemoveDeadBuff();

	//统计玩家死亡次数
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_myself,eStatic_Charactor,eStaMinor_Charactor_CharDead);

	switch (obj->GetType())
	{
	case ePlayer:
	{
		player = obj;

		break;
	}
	case ePet:
	{
		Smart_Ptr<GirlWar> girlWar = obj;
		player = girlWar->GetOwner();

		break;
	}
	default:
		break;
	}

	//通知客户端谁杀死了
	CharBattle::DeadInfo toClient;
	if(player)
	{
		toClient.set_type(0);
		toClient.set_id(player->GetID());
		toClient.set_name(player->GetName());
	}
	else if(obj)
	{
		toClient.set_type(1);
		toClient.set_id(obj->GetID());
	}
	QuickMessage::GetInstance()->AddSendMessage(GetID(),GetChannelID(),&toClient,MSG_SIM_GM2C_PLAYER_DEADINFO);


	//事件触发
	if (player && player->GetID() != GetID())
	{
		OneParmaEventArgs tArgs(player);
		FireEvent(ADD_SIN_EVENT_TYPE, tArgs);

		if (GetMyself())
		{
			TwoParmaEventArgs tArgs1(GetMyself(), player);
			FireEvent(ADD_HATRED_EVENT_TYPE, tArgs1);
		}

		//同步到世界服
		bool flag = InstanceLoader::GetInstance()->IsNotAddSinValueMapID( GET_MAP_ID(GetMapID()) );

		if (getAttManage()->getValue<DWORD>(eCharFaction) > 0 && !flag)
		{
			Smart_Ptr<SvrItem> worldSvr;
			ClanPackage::SynchClanEventToWS clanEvent;

			clanEvent.set_charid(GetID());
			clanEvent.set_time(CUtil::GetNowSecond());
			clanEvent.set_killer(player->GetName());

			ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(GetID()), worldSvr);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SYN_CLANEVENT, &clanEvent, worldSvr.Get(),SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}

	if (obj->GetID() != GetID())
	{
		//触发生物死亡局部事件
		CreatureKilledEventArgs tArgs(obj);
		FireEvent(CREATURE_DEAD_EVENT_TYPE, tArgs);

		if (m_myself)
		{
			//触发角色死亡全局事件
			PlayerKilledEventArgs tArgsf(obj, m_myself);
			FireGlobalEvent(CREATURE_DEAD_EVENT_TYPE, tArgsf);
		}
		//记录角色死亡次数
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_myself, eStatic_Charactor, eStaMinor_Charactor_CharDead, 1);
	}
}

bool Player::IsDead()
{
	if (IsInCreatureStateAll(ERS_Dead))
	{
		return true;
	}

	return false;
}

bool Player::AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num)
{
	if (IsDead())
	{
		return false;
	}

	return m_skillMgr->addBuff(id, obj, num);
}

bool Player::RemoveBuffByOnlyID(DWORD id)
{
	return m_skillMgr->RemoveBuffByOnlyID(id);
}

bool Player::DeleteBuffByOnlyID(DWORD id)
{
	return m_skillMgr->DeleteBuffByOnlyID(id);
}

//获得为参数指定的BUFF
void Player::getBuffByEffectType(vector<DWORD>& onlyIDList,const BuffGetPublicData& data)
{
	m_skillMgr->getBuffByEffectType(onlyIDList, data);

	return;
}

void Player::removeBuffByGroupID(DWORD groupID)
{
	m_skillMgr->removeBuffByGroupID(groupID);
	return ;
}

bool Player::RemoveBuffById(DWORD id)
{
	return m_skillMgr->RemoveBuffByID(id);
}

void Player::SendMessageToGateServer(int msgid,google::protobuf::Message *content)
{
	if(msgid == MSG_SIM_GM2GT_DROPITEM)
	{
		Goods::SynToClientDrop *ret = static_cast<Goods::SynToClientDrop *>(content);

		 QuickMessage::GetInstance()->AddSendMessage(GetID(), GetChannelID(),ret,MSG_SIM_GT2C_DROPITEM);
	}
	else
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(msgid, content, m_channelID, SimpleMessage);
		Message_Facade::Send(messRet);
	}
}

void Player::AddPhyPower(int value)
{
	if (value <= 0)
		return;

	getAttManage()->addValue(eCharPhysicalPower, value);

	int addHP = 0;

	addHP = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePhyPowerToHP, value);
	CreatureObj::ChangeHPMax(addHP);

	m_dirtyData[eBattleInfo] = true;
}

int64 Player::GetTypeValue(int type)
{
	int64 value = 0;

	switch (type)
	{
	case eCharExp:
	{
		value = getAttManage()->getValue<int64>(eCharExp);;
		break;
	}
	case eCharLv:
	{
		value = getAttManage()->getValue<int>(eCharLv);
		break;
	}
	case eCharMoney:
	{
		value = getAttManage()->getValue<int>(eCharMoney);
		break;
	}
	case eCharGolden:
	{
		value = getAttManage()->getValue<int>(eCharGolden);
		break;
	}
	case eCharBindGolden:
	{
		value = getAttManage()->getValue<int>(eCharBindGolden);
		break;
	}
	case eCharProfession:
	{
		value = getAttManage()->getValue<BYTE>(eCharProfession);
		break;
	}
	case eCharXpos:
	{
		value = GetXNewPos();
		break;
	}
	case eCharYpos:
	{
		value = GetYNewPos();
		break;
	}
	case eCharMapid:
	{
		value = GET_MAP_ID(GetMapID());
		break;
	}
	case eCharCountry:
	{
		value = getAttManage()->getValue<DWORD>(eCharCountry);
		break;
	}
	case eFaContribution:
	{
		value = getAttManage()->getValue<int>(eFaContribution);
		break;
	}
	case eCharFaction:
	{
		value = getAttManage()->getValue<DWORD>(eCharFaction);
		break;
	}
	case eCharFaJob:
	{
		value = getAttManage()->getValue<BYTE>(eCharFaJob);
		break;
	}
	case eCharMate:
	{
		value = getAttManage()->getValue<int64>(eCharMate);
		break;
	}
	case eCountryJob:
	{
		value = getAttManage()->getValue<WORD>(eCountryJob);
		break;
	}
	case eBattleContribution:
	{
		value = getAttManage()->getValue<int>(eBattleContribution);
		break;
	}
	case eArmyJob:
	{
		value = getAttManage()->getValue<WORD>(eArmyJob);
		break;
	}
	case eCharHP:
	{
		value = getAttManage()->getValue<int>(eCharHP);
		break;
	}
	case eCharHPMax:
	{
		value = getAttManage()->getValue<int>(eCharHPMax);
		break;
	}
	case eCharMP:
	{
		value = getAttManage()->getValue<int>(eCharMP);
		break;
	}
	case eCharMPMax:
	{
		value = getAttManage()->getValue<int>(eCharMPMax);
		break;
	}
	case eCharForce:
	{
		value = getAttManage()->getValue<int>(eCharForce);
		break;
	}
	case eCharForceMax:
	{
		value = getAttManage()->getValue<int>(eCharForceMax);
		break;
	}
	case eCharStrength:
	{
		value = getAttManage()->getValue<int>(eCharStrength);
		break;
	}
	case eCharPhysicalPower:
	{
		value = getAttManage()->getValue<int>(eCharPhysicalPower);
		break;
	}
	case eCharPatience:
	{
		value = getAttManage()->getValue<int>(eCharPatience);
		break;
	}
	case eCharAgility:
	{
		value = getAttManage()->getValue<int>(eCharAgility);
		break;
	}
	case eCharPhyAttackMax:
	{
		value = getAttManage()->getValue<int>(eCharPhyAttackMax);
		break;
	}
	case eCharPhyDefence:
	{
		value = getAttManage()->getValue<int>(eCharPhyDefence);
		break;
	}
	case eCharHitRate:
	{
		value = getAttManage()->getValue<int>(eCharHitRate);
		break;
	}
	case eCharAvoidRate:
	{
		value = getAttManage()->getValue<int>(eCharAvoidRate);
		break;
	}
	case eCharMoveSpeed:
	{
		value = getAttManage()->getValue<int>(eCharMoveSpeed);
		break;
	}
	case eCrackDownRate:
	{
		value = getAttManage()->getValue<int>(eCrackDownRate);
		break;
	}
	case ePKCount:
	{
		value = getAttManage()->getValue<int>(ePKCount);
		break;
	}
	case eAvoidCrackRate:
	{
		value = getAttManage()->getValue<int>(eAvoidCrackRate);
		break;
	}
	case eCleverPower:
	{
		value = getAttManage()->getValue<int>(eCleverPower);
		break;
	}
	case eCleverAir:
	{
		value = getAttManage()->getValue<int>(eCleverAir);
		break;
	}
	case ePower:
	{
		value = getAttManage()->getValue<int>(ePower);
		break;
	}
	case eCountryContri:
	{
		value = getAttManage()->getValue<int>(eCountryContri);
		break;
	}
	case eTitleID:
	{
		value = getAttManage()->getValue<int>(eTitleID);
		break;
	}
	case ePowerMax:
	{
		value = getAttManage()->getValue<int>(ePowerMax);
		break;
	}
	case eSettings:
	{
		value = getAttManage()->getValue<int>(eSettings);
		break;
	}
	case eHaveTodo:
	{
		value = getAttManage()->getValue<int64>(eHaveTodo);
		break;
	}
	case ePrestige:
	{
		value = getAttManage()->getValue<int>(ePrestige);
		break;
	}
	case eHaveTodoAward:
	{
		value = getAttManage()->getValue<int64>(eHaveTodoAward);
		break;
	}
	case eActivities:
	{
		value = getAttManage()->getValue<int64>(eActivities);
		break;
	}
	case eActivitiesAward:
	{
		value = getAttManage()->getValue<int64>(eActivitiesAward);
		break;
	}
	case eFightPower:
	{
		value = getAttManage()->getValue<int>(eFightPower);
		break;
	}
	case ePosition:
	{
		value = (GetXNewPos() << 16) | GetYNewPos();
		break;
	}
	case eVIPLv:
	{
		value = getAttManage()->getValue<DWORD>(eVIPLv);
		break;
	}
	case eAddUpGold:
	{
		value = getAttManage()->getValue<int>(eAddUpGold);
		break;
	}
	case eGoldenCost:
	{
		value = getAttManage()->getValue<int>(eGoldenCost);
		break;
	}
	case eEnvelopeCount:
	{
		value = getAttManage()->getValue<int>(eEnvelopeCount);
		break;
	}
	case eCanGetEnvelopeCount:
	{
		value = this->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount();
		break;
	}
	case eMainEquip:	//主武器
	{
		Smart_Ptr<EquipProp> propBase = GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceWeapon - 1);

		if (propBase && (propBase->GetID() != -1))
		{
			value = propBase->GetID();
		}
		break;
	}
	case eCostume:	//服装
	{
		Smart_Ptr<EquipProp> costumeProp =GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceArmor - 1);

		if (costumeProp && (costumeProp->GetID() != -1))
		{
			value = costumeProp->GetID();
		}
		break;
	}
	case eContinueAnswer:
	{
		value = getAttManage()->getValue<int>(eContinueAnswer);
		break;
	}
	case eRightAnswer:
	{
		value  = getAttManage()->getValue<int>(eRightAnswer);
		break;
	}
	case eRanking:
	{
		value = getAttManage()->getValue<int>(eRanking);
		break;
	}
	case eAwardRanking:
	{
		value = getAttManage()->getValue<int>(eAwardRanking);
		break;
	}
	case ePKType:
	{
		value = getAttManage()->getValue<int>(ePKType);
		break;
	}
	case eSinValue:
	{
		value = (int) getAttManage()->getValue<float>(eSinValue);
		break;
	}
	case eContinueBeheaded:
	{
		value = getAttManage()->getValue<int>(eContinueBeheaded);
		break;
	}
	case eCharClanIcon:
	{
		value = getAttManage()->getValue<BYTE>(eCharClanIcon);
		break;
	}
	case eAntiTime:
	{
		value = this->GetAntiAddictionSystemMgr()->GetCurrentAntiAddictionTime();
		break;
	}
	case eCouple:
	{
		value = getAttManage()->getValue<int64>(eCouple);
		break;
	}
	case eCredit:
	{
		value = getAttManage()->getValue<int>(eCredit);
		break;
	}
	case eVipCardLv:
	{
		value = getAttManage()->getValue<int>(eVipCardLv);
		break;
	}
	case eVipCardTime:
	{
		value = this->GetVipPart()->GetVipCardRestTime();
		break;
	}
	default:
	{
		value = getAttManage()->getValue<int>((ECharAttrType)type);
		break;
	}
	}

	return value;
}

void Player::SetPrestige(uint value)
{
	getAttManage()->setValue(ePrestige, value);

	//获取称号
	if (m_Title)
	{
		m_Title->GetTitleWithPrestige();
	}
}

void Player::AddPrestige(int value, bool add)
{
	if (add)
	{
		getAttManage()->addValue(ePrestige, value);

		//获取称号
		if (m_Title)
		{
			m_Title->GetTitleWithPrestige();
		}
	}
	else
	{
		getAttManage()->addValue(ePrestige, -value);
	}
}

void Player::AddSinValue(float sinValue)
{
	float tempSinValue = getAttManage()->getValue<float>(eSinValue);
	int sinValueBefore = (int) tempSinValue;

	tempSinValue += sinValue;

	if (m_sinState != ERS_RED_NAME && tempSinValue >= SIN_RED_NAME_COUNT)
	{
		m_sinState = ERS_RED_NAME;

//		if(eAccountType_User == this->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(GetID(),eStatic_RedName,eStaMinor_RedName_RedNameBegin);
	}

	if (tempSinValue >= SIN_MAX_LIMIT_COUNT)
		tempSinValue = SIN_MAX_LIMIT_COUNT;

	getAttManage()->setValue(eSinValue, tempSinValue);

	int sinValueAfter = (int) tempSinValue;
	if (sinValueBefore != sinValueAfter)
	{
		std::vector<int> vecBro;
		vecBro.push_back(eSinValue);
		this->SynCharAttribute(vecBro);
		m_dirtyData[eBaseInfo] = true;
	}
}

void Player::ReduceSinValue(float sinValue)
{
	float tempSinValue = getAttManage()->getValue<float>(eSinValue);
	int sinValueBefore = (int) tempSinValue;

	tempSinValue -= sinValue;

	if (m_sinState == ERS_RED_NAME && tempSinValue < SIN_RED_NAME_COUNT)
	{
		m_sinState = 0;
//		if(eAccountType_User == this->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(GetID(),eStatic_RedName,eStaMinor_RedName_RedNameEnd);
	}
	if (tempSinValue < 0.0f)
	{
		tempSinValue = 0.0f;
	}
	int sinValueAfter = (int) tempSinValue;
	getAttManage()->setValue(eSinValue, tempSinValue);
	//罪恶值累积减少超过1则需要跟客户端同步
	if (sinValueBefore != sinValueAfter)
	{
		std::vector<int> vecBro;
		vecBro.push_back(eSinValue);
		this->SynCharAttribute(vecBro);
		m_dirtyData[eBaseInfo] = true;
	}
}

void Player::AddCleverAir(int value)
{
	if (value <= 0)
		return;

	getAttManage()->addValue(eCleverAir, value);

	int addCleverAir = 0;

	addCleverAir = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eCleverAirtoMP, value);

	CreatureObj::ChangeMPMax(addCleverAir);

	m_dirtyData[eBattleInfo] = true;
}

void Player::AddCleverPower(int value)
{
	if (value <= 0)
		return;

	getAttManage()->addValue(eCleverPower, value);

	int addCleverPower = 0;

	addCleverPower = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eCleverPowerToMA, value);

	m_dirtyData[eBattleInfo] = true;
}

void Player::setEnvelopCount(int envelopCount)
{
	if(envelopCount < 0)
		envelopCount = 0;

	getAttManage()->setValue(eEnvelopeCount,envelopCount);

	return ;
}

void Player::AddAgility(int value)
{
	if (value <= 0)
		return;

	getAttManage()->addValue(eCharAgility, value);

	int addMD = 0;

	addMD = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eAgilityToMD, value);

	m_dirtyData[eBattleInfo] = true;
}

void Player::AddPatience(int value)
{
	if (value <= 0)
		return;

	getAttManage()->addValue(eCharPatience, value);

	int addPD = 0;

	addPD = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePatienceToPD, value);
	CreatureObj::ChangePDefence(addPD);

	m_dirtyData[eBattleInfo] = true;
}

void Player::AddStrength(int value)
{
	if (value <= 0)
		return;

	getAttManage()->addValue(eCharStrength, value);

	int addAttack = 0;

	addAttack = BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eStrengthToAt, value);
	CreatureObj::ChangePhyAttackMax(addAttack);

	m_dirtyData[eBattleInfo] = true;
}

void Player::SetTypeValue(CharBase::CharAttChange& attr)
{
	for (int i = 0; i < attr.type_size(); i++)
	{
		switch (attr.type(i))
		{
		case eTeamId:
			{
				getAttManage()->setValue(eTeamId,attr.value(i));
				break;
			}
		case eCouple:
			{
				getAttManage()->setValue(eCouple,attr.value(i));

				break;
			}
		case eCountryJob:
			{
				getAttManage()->setValue(eCountryJob,attr.value(i));
			}
		}
	}

	m_dirtyData[eBaseInfo] = true;
}

void Player::SetTypeValueChar(CharBase::CharAttChangeToGame& attr)
{
	for (int i = 0; i < attr.type_size(); i++)
	{
		switch (attr.type(i))
		{
		case eCoupleName:
			{
				SetCoupleName(attr.svalue(i));
				break;
			}
		}
	}

	m_dirtyData[eBaseInfo] = true;
}


int Player::IsInAttackRate(int skillid)
{
	SkillInfo *info = 0;

	info = SkillLoader::GetInstance()->GetSkillInfoByID(skillid);
	if (info == 0)
	{
		return eAttackSpeedTooFast;
	}

	if (skillid > GameParaLoader::GetInstance()->getSpecialSkillIdBegin())
	{
		if (!m_cdMgr->IsCDEnd(SKILL_COMMON))
		{
			return eAttackSpeedTooFast;
		}
	}

	if (!m_cdMgr->IsCDEnd(info->channelType))
	{
		return eAttackSpeedTooFast;
	}

	return 0;
}

void Player::ReduceSinValue(Smart_Ptr<CreatureObj> &obj)
{
	float sinValue = 0.0f;
	//被其它玩家杀死
	if (obj->GetType() == ePlayer && obj->GetID() != this->GetID())
	{
		Smart_Ptr<Player> enemy = obj;

		int lv = enemy->getAttManage()->getValue<int>(eCharLv) - this->getAttManage()->getValue<int>(eCharLv);
		if (lv > 0)
			sinValue = 1.0f;
		else
			sinValue = 5.0f * (float(abs(lv / 10)) + 1.0f);
	}
	else if (obj->GetType() == eMonster)	//杀死怪物
	{
		Smart_Ptr<Monster> monster = obj;

		int lv = monster->getAttManage()->getValue<int>(eCharLv) - this->getAttManage()->getValue<int>(eCharLv);

		const int diffLv = GameParaLoader::GetInstance()->getMinMonPlayerDiffLv();

		if (lv >= diffLv)
			sinValue = 1.0f / (10.0f * ((float) abs(lv / 5)) + 1.0f);
		else if (abs(lv) < diffLv)
			sinValue = 0.1f;
		else
			sinValue = 1.0f / (10.0f - float(min(lv / 5, 9)));
	}

	this->ReduceSinValue(sinValue);
}

void Player::SendBuffState(CharBattle::BuffOperate * buf)
{
//	CharBattle::BuffOperate selfBuffOp;
//
//	selfBuffOp.CopyFrom(*buf);

	QuickMessage::GetInstance()->AddSendMessage(GetID(), m_channelID,buf, MSG_SIM_GT2C_BUFFOPERATE);
}

bool Player::isBattleAtt(ECharAttrType att)
{
	switch(att)
	{
	case eCharPhyAttackMax:
	case eCharPhyDefence:
	case eCharHPMax:
	case eCharHitRate:
	case eCharAvoidRate:
	case eCrackDownRate:
	case eAvoidCrackRate:
		{
			return true;
		}
	default:
		{
			return false;
		}
	}

	return false;
}

void Player::Attacked(Smart_Ptr<CreatureObj> &obj)
{
	if (m_GirlWarMgr->GetGirlWar())
	{
		m_GirlWarMgr->GetGirlWar()->Attacked(obj);
	}

	PlayerBeAttackArgs tArgsf(m_myself, obj);
	FireGlobalEvent(PLAYER_BEATTACK, tArgsf);

}

void Player::Attack(Smart_Ptr<CreatureObj> &obj)
{
	if (m_GirlWarMgr->GetGirlWar())
	{
		m_GirlWarMgr->GetGirlWar()->Attacked(obj);
	}
}

void Player::UpdateFightState(int64 now)
{
	if (IsInCreatureStateAll(ERS_Fight))
	{
		if (m_fightTime+ GameParaLoader::GetInstance()->getChangePlayerStateTime()<= now)
		{
			UnsetCreatureState(ERS_Fight, false);
			SynCharAttribute(eSimpleState, 0, eNonAttack);
			BroadcastCharAttribute(eSimpleState, 0, eNonAttack, false);
			if (m_GirlWarMgr->GetGirlWar())
			{
				Smart_Ptr<CreatureObj> obj;
				m_GirlWarMgr->GetGirlWar()->SetTarget(obj);
			}
		}
	}
}

int Player::IsNpcExist(int npcId)
{
	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
	if (!zone)
	{
		LOG_ERROR(FILEINFO, "client accept quest but scene is not existed");
		return eRetZoneNotExist;
	}
	Smart_Ptr<NPC> npc = zone->GetNpcByID(npcId);
	if (!npc)
	{
		LOG_ERROR(FILEINFO, "client accept quest but npc is not existed");
		return eRetNpcNotExist;
	}
	return 0;
}

int Player::CheckDistanceWithNpc(int npcId)
{
	return NPCInfo::GetInstance()->ExamineDistance(npcId,GetXNewPos(), GetYNewPos());
}

//增加了isFly以及opeartorType两个参数，参数1表示是什么类型获取物品的
//1表示任务获得物品，2表示拾取获得物品，3表示其他途径获得物品，客户端根据该类型进行作飞行效果操作
//由于有关物品的更新操作都是通过GTAckToCUpdateGoodsRet来进行通知客户端，由于客户端无法识别是哪种操作
//通知更新的，所以这里的参数2表示是哪种操作来通知更新的，0表示通用更新，1表示新增物品通知，2表示删除物品更新，3表示使用物品更新
void Player::SendSectorChange(vector<DWORD> &vec, int isFly)
{
	Goods::GTAckToCUpdateGoodsRet info;

	info.set_status(0);
	info.set_isadd(isFly);

	vector<DWORD>::iterator it = vec.begin();
	for (; it != vec.end(); ++it)
	{
		int contain_enum = (*it) >> 16;
		int pos = (*it) & 0xffff;
		Smart_Ptr<PropBase> base_sptr = m_container[contain_enum]->GetItemDataPtr(pos);

		if(!base_sptr)
			continue;

		Goods::UpdateGoods * sych_itemInfo = info.add_updategoods();

		CharLogin::ClientItemInfo *sych_clientInfo = sych_itemInfo->mutable_iteminfo();
		sych_itemInfo->set_container(contain_enum);
		sych_itemInfo->set_pos(pos);

		base_sptr->SetClientInfo(sych_clientInfo);
	}

	QuickMessage::GetInstance()->AddSendMessage(GetID(), GetChannelID(),&info,MSG_SIM_GT2C_SAVEGOODS);
}

void Player::SynCharAttribute()
{
	vector<int> vecBro;
	vecBro.push_back(eCharMoney);
	vecBro.push_back(eCharHPMax);
	vecBro.push_back(eCharHP);
	vecBro.push_back(eCharMPMax);
	vecBro.push_back(eCharMP);
	vecBro.push_back(eCharPhyAttackMax);
	vecBro.push_back(eCharPhyDefence);
	vecBro.push_back(eCharHitRate);
	vecBro.push_back(eCharAvoidRate);
	vecBro.push_back(eCrackDownRate);
	vecBro.push_back(eReagainRate);
	vecBro.push_back(eAvoidCrackRate);
	vecBro.push_back(eFightPower);
	//vecBro.push_back(eTitleID);
	//vecBro.push_back(eVIPLv);
	//vecBro.push_back(eAddUpGold);
	//vecBro.push_back(eSinValue);
	SynCharAttribute(vecBro);
}

bool Player::IsInDistance(Smart_Ptr<CreatureObj> Obj, uint maxDistance)
{
	int myXPos = GetXNewPos();
	int myYPos = GetYNewPos();
	int targetXPos = Obj->GetXNewPos();
	int targetYPos = Obj->GetYNewPos();

	int RealDistX = abs(myXPos - targetXPos);
	int RealDistY = abs(myYPos - targetYPos);

	return ((RealDistX * RealDistX + RealDistY * RealDistY)<= ((int) (maxDistance * maxDistance)));
}

int Player::ChangeMoveSpeed(const int &value, bool isAdd,eAttValueType tempValueType)
{
	int mSpeed = getAttManage()->getBasedValue<int>(eCharMoveSpeed,tempValueType);
	if (value <= 0)
		return mSpeed;

	if (isAdd)
	{
		mSpeed += value;
	}
	else
	{
		mSpeed -= value;

		if (mSpeed <= 0)
			mSpeed = 1;
	}

	getAttManage()->setValue(eCharMoveSpeed, mSpeed,tempValueType);

	m_dirtyData[eBattleInfo] = true;
	return mSpeed;
}

int Player::ChangePhyAttackMax(int value, bool isAdd,eAttValueType tempValueType)
{
	int pAttack = getAttManage()->getBasedValue<int>(eCharPhyAttackMax,tempValueType);

	if(isAdd)
	{
		pAttack += value;
	}
	else
	{
		pAttack -= value;
	}

	if(pAttack <= 0)
		pAttack = 1;

	getAttManage()->setValue(eCharPhyAttackMax, pAttack,tempValueType);
	m_dirtyData[eBattleInfo] = true;

	return pAttack;
}

int Player::ChangeFightPower(int value, bool isAdd)
{
	int tempFighPower = getAttManage()->getValue<int>(eFightPower);
	if (value <= 0)
		return tempFighPower;

	if (isAdd)
	{
		tempFighPower += value;
	}
	else
	{
		tempFighPower -= value;
		if (tempFighPower < 0)
			tempFighPower = 0;
	}

	getAttManage()->setValue(eFightPower, tempFighPower);
	m_dirtyData[eBaseInfo] = true;
	m_dirtyData[eBattleInfo] = true;

	return tempFighPower;
}

int Player::ChangePDefence(int value, bool isAdd,eAttValueType tempValueType)
{
	int pDefence = getAttManage()->getBasedValue<int>(eCharPhyDefence,tempValueType);

	if(isAdd)
	{
		pDefence += value;
	}
	else
	{
		pDefence -= value;
	}

	if(pDefence <= 0)
		pDefence = 0;

	getAttManage()->setValue(eCharPhyDefence, pDefence,tempValueType);
	m_dirtyData[eBattleInfo] = true;

	return pDefence;
}

int Player::ChangeHRate(int value, bool isAdd,eAttValueType tempValueType)
{
	int hRate = getAttManage()->getBasedValue<int>(eCharHitRate,tempValueType);
	if (value <= 0)
		return hRate;

	if (isAdd)
	{
		hRate += value;
	}
	else
	{
		hRate -= value;
		if (hRate < 0)
			hRate = 0;
	}

	getAttManage()->setValue(eCharHitRate, hRate,tempValueType);
	m_dirtyData[eBattleInfo] = true;

	return hRate;
}

int Player::ChangeARate(int value, bool isAdd,eAttValueType tempValueType)
{
	int aRate = getAttManage()->getBasedValue<int>(eCharAvoidRate,tempValueType);
	if (value <= 0)
		return aRate;

	if (isAdd)
	{
		aRate += value;
	}
	else
	{
		aRate -= value;
		if (aRate < 0)
			aRate = 0;
	}

	getAttManage()->setValue(eCharAvoidRate, aRate,tempValueType);
	m_dirtyData[eBattleInfo] = true;

	return aRate;
}

int Player::ChangeCRate(int value, bool isAdd,eAttValueType tempValueType)
{
	int cRate = getAttManage()->getBasedValue<int>(eCrackDownRate,tempValueType);
	if (value <= 0)
		return cRate;

	if (isAdd)
	{
		cRate += value;
	}
	else
	{
		cRate -= value;
		if (cRate < 0)
			cRate = 0;
	}

	getAttManage()->setValue(eCrackDownRate, cRate,tempValueType);
	m_dirtyData[eBattleInfo] = true;

	return cRate;
}

int Player::ChangeHPRate(int hp, bool flag)
{
	int tmp = getAttManage()->getValue<int>(eCharHP);
	int tMax = getAttManage()->getValue<int>(eCharHPMax);

	if (hp <= 0)
		return tmp;

	if (flag)
	{
		tmp += tMax * hp / 100;
		if (tmp > tMax)
			tmp = tMax;
	}
	else
	{
		tmp -= tMax * hp / 100;
		if (tmp < 0)
			tmp = 0;
	}

	CreatureObj::SetCurHP(tmp);
	m_dirtyData[eBattleInfo] = true;
	BroadcastCharAttribute(eCharHP, tmp, eAttackSim, true);

	return tmp;
}

int Player::ChangeHPMax(int value, bool isAdd,eAttValueType tempValueType)
{
	int hp = getAttManage()->getBasedValue<int>(eCharHPMax,tempValueType);

	if(isAdd)
	{
		hp += value;
	}
	else
	{
		hp -= value;
	}

	if (hp <= 0)
		hp = 1;

	if (hp < getAttManage()->getValue<int>(eCharHP))
		getAttManage()->setValue(eCharHP, hp);

	getAttManage()->setValue(eCharHPMax,hp,tempValueType);

	m_dirtyData[eBattleInfo] = true;

	return hp;
}

int Player::ChangeMPMax(int value, bool isAdd,eAttValueType tempValueType)
{
	int mp = getAttManage()->getBasedValue<int>(eCharMPMax,tempValueType);
	if (value <= 0)
		return mp;

	if (isAdd)
	{
		mp += value;
	}
	else
	{
		mp -= value;
		if (mp <= 0)
			mp = 1;

		if (mp < getAttManage()->getValue<int>(eCharMP))
			CreatureObj::SetCurMP(mp);
	}


	getAttManage()->setValue(eCharMPMax,mp,tempValueType);

	m_dirtyData[eBattleInfo] = true;

	return mp;
}

int Player::ChangeCurMP(int value, bool isAdd)
{
	int tMP = getAttManage()->getValue<int>(eCharMP);
	if (value <= 0)
	{
		return tMP;
	}

	int tmp = 0;
	int tMPMax = getAttManage()->getValue<int>(eCharMPMax);

	if (isAdd)
	{
		if (tMP + value > tMPMax)
		{
			tmp = tMPMax;
		}
		else
		{
			tmp = tMP + value;
		}
	}
	else
	{
		tmp = tMP - value;

		if (tmp < 0)
			tmp = 0;
	}

	SetCurMP(tmp);
	m_dirtyData[eBattleInfo] = true;

	return tmp;
}

int Player::ChangePower(int value, bool isAdd)
{
	int tempPower = getAttManage()->getValue<int>(ePower);
	if (value <= 0)
		return tempPower;

	if (isAdd)
	{
		tempPower += value;
	}
	else
	{
		tempPower -= value;
		if (tempPower < 0)
			tempPower = 0;
	}

	getAttManage()->setValue(ePower, tempPower);
	m_dirtyData[eBattleInfo] = true;

	return tempPower;
}

void Player::RelocatePos(int xpos, int ypos, bool isSyn)
{
	if (!m_myself)
	{
		return;
	}

	Smart_Ptr<Object> obj = m_myself;

	ClearPath();
	SetXNewPos(xpos);
	SetYNewPos(ypos);
	SetDataFlag(eBaseInfo);
	CSceneMgr::GetInstance()->MoveTo(obj);
}

int Player::ChangeBaserte(int value, bool isAdd,eAttValueType tempValueType)
{
	int lucky = getAttManage()->getBasedValue<int>(eAvoidCrackRate,tempValueType);
	if (value <= 0)
		return lucky;

	if (isAdd)
	{
		lucky += value;
	}
	else
	{
		lucky -= value;
		if (lucky < 0)
			lucky = 0;
	}

	getAttManage()->setValue(eAvoidCrackRate, lucky,tempValueType);
	m_dirtyData[eBattleInfo] = true;

	return lucky;
}

int Player::ChangeFContri(int value, bool isAdd)
{
	int tempContribution = getAttManage()->getValue<int>(eFaContribution);
	if (value <= 0)
	{
		return tempContribution;
	}

	if (isAdd)
	{
		tempContribution += value;
	}
	else
	{
		tempContribution -= value;
		if (tempContribution < 0)
			tempContribution = 0;
	}

	getAttManage()->setValue(eFaContribution, tempContribution);

	m_dirtyData[eBaseInfo] = true;

	return tempContribution;
}

int Player::ChangeCountryContribution(int value, bool isAdd, bool sendtoWorld)
{
	//todo:目前有3个贡献度，能增减的只有当前的贡献度，历史和周贡献度只增不减
	int tempCurContribute = getAttManage()->getValue<int>(eCurCountryContribute);
	if (value <= 0)
	{
		return tempCurContribute;
	}

	if (isAdd)
	{
		getAttManage()->addValue<int>(eCountryContri, value);
		m_cContribution_Week += value;
		tempCurContribute += value;
	}
	else
	{
		tempCurContribute -= value;
		if (tempCurContribute < 0)
			tempCurContribute = 0;
	}

	getAttManage()->setValue(eCurCountryContribute,tempCurContribute);

	//同步客户端
	SynCharAttribute(eCurCountryContribute, getAttManage()->getValue<int>(eCurCountryContribute));
	SynCharAttribute(eCountryContri,getAttManage()->getValue<int>(eCountryContri));

	//同步世界服
	if(sendtoWorld)
	{
		SynchCharAttributeToWS(eCurCountryContribute, isAdd ? value : -value);
	}

	m_dirtyData[eBattleInfo] = true;
	m_dirtyData[eBaseInfo] = true;

	if (getCampShopMgr())
	{
		int count = getCampShopMgr()->IsHaveCanBuy();
		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_CountryShopCanBuy), count);
	}

	return tempCurContribute;
}

int Player::ChangeCurForce(int value, bool isAdd)
{
	int tempForce = getAttManage()->getValue<int>(eCharForce);
//	int forceBeforeChange = tempForce;
	if (value <= 0)
	{
		return tempForce;
	}

	if (isAdd)
	{
		float antiAddictionRation = this->GetAntiAddictionFactor();

		value = int(value * antiAddictionRation);

		tempForce += value;
		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddForce(value));
	}
	else 
	{
		tempForce -= value;
		if (tempForce < 0)
			tempForce = 0;
		BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::CostForce(value));
	}


	getAttManage()->setValue(eCharForce, tempForce);

	m_dirtyData[eBattleInfo] = true;

//	if(mainType != eStatic_Max && minorType != eStaMinor_Max)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//		if(isAdd)
//		{
//			StatisticInfo::GotItemInfo * gotItem = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotItem,eCharForce,value);
//		}
//		else
//		{
//			StatisticInfo::CostItemInfo * costItem = costAndGotInfo.add_costlist();
//			StatisticMgr::SetItemCostBefore(costItem,eCharForce,forceBeforeChange,this->GetMyself());
//			StatisticMgr::SetItemCostAfter(costItem,eCharForce,this->GetMyself());
//		}
//
//		StatisticCurrencyChangeArgs args(this->GetID(),mainType,minorType,&costAndGotInfo);
//		FireGlobalEvent(STATISTIC_CURRENCY_CHANGE, args);
//	}

	EventArgs arg;
	FireEvent(PLYAER_CHANGE_FORCE, arg);

	return tempForce;
}
//void Player::SetState(int state)
//{
//	if(state != m_state)
//	{
//		LOG_FATAL(FILEINFO,"+++++++++++++++name:%s---state:%d++++++++++++++++++++++++",GetName().c_str(),state);
//		m_state = state;
//	}
//}
bool Player::isSameTeam(const Smart_Ptr<Player> &otherPlayer)
{
	if (!otherPlayer)
		return false;

	if (otherPlayer->GetGroupID() > 0 && otherPlayer->GetGroupID() == this->GetGroupID())
		return true;

	return false;
}

void Player::UpdateSinValue(void * obj)
{
	if (this && GetType() == ePlayer)
	{
		//事件触发
		if (GetCreatureSelf())
		{
			OneParmaEventArgs tArgs(GetCreatureSelf());
			FireEvent(REDUCE_SIN_EVENT_TYPE, tArgs);
		}
	}
}

//void Player::UpateStastic()
//{
//	for (int i = eCharExp; i < eCharAttrMax; i++)
//	{
//		//记录属性变更的值
////		StatisticMgr::GetInstance()->StatisticPlayerItemsMap(m_myself,eStatic_Charactor, eStaMinor_Charactor_AttrChange, i,GetTypeValue(i), false);
//	}
//}
//增加玩家罪恶值
bool Player::AddPlayerSinValue(const EventArgs & e)
{
	OneParmaEventArgs *Args = (OneParmaEventArgs *) (&e);
	Smart_Ptr<CreatureObj> obj = Args->m_obj;
	vector<int> vecType;
	if (obj)
	{
		if (obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			/*增加  在某些地图中  杀人 是否  增加 红名的 判断*/
			int mapID = GET_MAP_ID(player->GetMapID());
			if (InstanceLoader::GetInstance()->IsNotAddSinValueMapID(mapID)|| GET_MAP_TYPE(player->GetMapID()) == eBeyondMap)
				return true;

			int mapType = GET_MAP_TYPE(player->GetMapID());
			if(mapType == ePVEMap)
			{
				Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
				if(instance)
				{
						InstanceMapEntry* instanceInfo = instance->GetInstanceScript();
						if(instanceInfo!=NULL)
						{
							if(instanceInfo->m_eType==eCountryBoss || instanceInfo->m_eType==eCountryWar || instanceInfo->m_eType==ePVP)
							{
								return true;
							}
						}
				}
			}

			if (this->m_sinState != ERS_RED_NAME)
			{
				player->AddSinValue(100);
//				StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player,eStatic_RedName,eStaMinor_RedName_KillCount);
			}
			else
			{
				//减少罪恶值
				this->ReduceSinValue(obj);
			}
		}
	}
	return true;
}

//减少玩家罪恶值
bool Player::ReducePlayerSinValue(const EventArgs & e)
{
	OneParmaEventArgs *Args = (OneParmaEventArgs *) (&e);
	Smart_Ptr<CreatureObj> obj = Args->m_obj;
	vector<int> vecType;
	if (obj && obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;
		//每2小时减100折合成两分钟减少1.666罪恶值
		player->ReduceSinValue(GameParaLoader::GetInstance()->getReduceSinValuePer());
	}
	return true;
}

int Player::DecreaseExp(float ratio)
{
	UpgradeInfo nowInfo;

	UpgradeLoader::GetInstance()->GetUpgradeByLv(getAttManage()->getValue<int>(eCharLv), nowInfo);

	int reduceExp = int(double(nowInfo.allExp) * double(ratio));

	return this->DecreaseExp(reduceExp);
}

bool Player::AddPlayerHatred(const EventArgs & e)
{
	//todo 是否需要在游戏服上面保存玩家的仇恨列表,需要后续请加上
	TwoParmaEventArgs *Args = (TwoParmaEventArgs *) (&e);
	Friend::AddPlayerHarted addHarted;

	if (Args->m_myself && Args->m_other)
	{
		Smart_Ptr<Player> myself = Args->m_myself;

		/*增加  在某些地图中  杀人 是否  增加 红名的 判断*/
		int mapID = GET_MAP_ID(myself->GetMapID());
		if (InstanceLoader::GetInstance()->IsNotAddSinValueMapID(mapID))
			return true;

		addHarted.set_myself(Args->m_myself->GetID());
		addHarted.set_other(Args->m_other->GetID());

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2GT_ADDHATRED, &addHarted, myself->GetChannelID(),SimpleMessage);
		Message_Facade::Send(messRet);
	}

	return true;
}

bool Player::PlayerEnterMap(const EventArgs & e)
{
	PlayerLoginEventArgs *Args = (PlayerLoginEventArgs *) (&e);
	if (Args)
	{
		Smart_Ptr<Player> player = Args->m_obj;

		//取消打坐双修
		CanccelMeditation();

		//同步竞技场数据
		if(IsOpenSystem(eStCompeteInstance) && getAttManage()->getValue<int>(eRanking) <= 0 && 1 == getAttManage()->getValue<int>(eGuide))
//				!(player->GetArenaMgr()->IsFakeRank()))
		{
			SynchPlayerCompeteInfoToWS();
		}
	}
	return true;
}

bool Player::PlayerChangeScene(const EventArgs & e)
{
	return true;
}

bool Player::PlayerLeaveScene(const EventArgs & e)
{
	OneParmaEventArgs *pArgs = (OneParmaEventArgs *)(&e);
	if(pArgs == NULL || pArgs->m_obj.Get() == NULL)
	{
		return true;
	}

	if(pArgs->m_obj->GetType() != ePlayer)
		return true;

	CanccelMeditation();

	MeditationMgr::GetInstance()->RemoveBeInvites( pArgs->m_obj->GetID() );

	return true;
}

bool Player::FightGirWar(const EventArgs & e)
{
	PlayerGirlwarFight* pArgs = (PlayerGirlwarFight*)(&e);

	if(pArgs->m_obj.Get() == NULL || pArgs->m_obj->GetType() != ePet)
	{
		return true;
	}

	Smart_Ptr<GirlWar> girlwar = pArgs->m_obj;
	Smart_Ptr<Player> player = girlwar->GetOwner();

	if(player.Get() == NULL)
	{
		return true;
	}

	MeditationMgr::GetInstance()->PlayerFightGirlWar( player );

	return true;
}


bool Player::PlayerEnterGroup(const EventArgs & e)
{
	return true;
}

void Player::ChangeAttrWhenGroupAdd(const Smart_Ptr<GroupInfo> &group_ptr)
{
	//传队伍过来明确语义
	short tempCounts = group_ptr->memberList.size();
	tempCounts = tempCounts - 1 - m_groupAddHp.size();

	if (tempCounts <= 0)
		return;

	float tempAllAdd = .0f;
	float tempIntAdd = 0.02f;
	for (short i = 0; i < tempCounts; ++i)
	{
		m_groupAddHp.push_back(tempIntAdd);
		tempAllAdd += tempIntAdd;
	}

	if (tempAllAdd <= 0)
		return;

	vector<int> typeBro;

	getAttManage()->addPercentValue(eCharHPMax,tempAllAdd);

	typeBro.push_back(eCharHPMax);
	this->BroadcastCharAttribute(typeBro);

	return;
}

bool Player::PlayerLeaveGroup(const EventArgs & e)
{
	return true;
}

void Player::ChangeAttrWhenGroupReduce(const int & member_size)
{
	if (member_size <= 0)
		return;

	float all_add = 0;

	for (short i = 0; i < member_size; ++i)
	{
		if (m_groupAddHp.size() <= 0)
			break;

		all_add += m_groupAddHp.back();
		m_groupAddHp.pop_back();
	}

	getAttManage()->addPercentValue(eCharHPMax,-all_add);

	vector<int> typeBro;
	typeBro.push_back(eCharHPMax);

	this->BroadcastCharAttribute(typeBro);
}

void Player::ChangeAttrWhenGroupMemberChangeMap(const Smart_Ptr<GroupInfo> &group_ptr)
{
	int fun_num = (group_ptr->memberList.size() - 1) - m_groupAddHp.size();
	if (GET_MAP_ID(this->GetMapID()) == group_ptr->mapID)
	{
		//同一地图 如果 没有 加成
		if (fun_num > 0)
		{
			this->ChangeAttrWhenGroupAdd(group_ptr);
		}
		else if (fun_num < 0)
		{
			this->ChangeAttrWhenGroupReduce(abs(fun_num));
		}
	}
	else
	{
		//不同地图  直接 减掉
		this->ChangeAttrWhenGroupReduce(m_groupAddHp.size());
	}
}

bool Player::PlayerDead(const EventArgs & e)
{
//	PlayerDeadEventArgs *Args = (PlayerDeadEventArgs *) (&e);
	return true;
}

bool Player::IsAttackDistance(int xpos, int ypos)
{
	int len = 0;
	int xtemp = abs(xpos - GetXNewPos());
	int ytemp = abs(ypos - GetYNewPos());

	if (getAttManage()->getValue<BYTE>(eCharProfession) == 1)
	{
		len = GameParaLoader::GetInstance()->getMuscleDis();
	}
	else
	{
		len = GameParaLoader::GetInstance()->getRangedDis();
	}

	if (xtemp > len || ytemp > len)
	{
		return false;
	}

	return true;
}

bool Player::IsMoveToPosition(int xpos, int ypos)
{
	if (m_curIndex < 0 || (m_curIndex + 1) >= m_path.pos_size())
	{
//		printf("\n----------------------- player[ %lld] current index is zero--------------------------\n", GetID() & 0xffffffff);
		return false;
	}

	float len = 0;
	int i = 0;
	bool isEnd = false;

//	cout<<endl;
//	cout<< "+++++++++++++++++( " <<  xpos << ", " << ypos << " )" <<endl;

	for (i = m_curIndex; i < m_path.pos_size() - 1; ++i)
	{
		if (m_path.pos(i).xpos() == xpos && m_path.pos(i).ypos() == ypos)
		{
			isEnd = true;
			break;
		}

		int xlen = abs(m_path.pos(i).xpos() - m_path.pos(i + 1).xpos());
		int ylen = abs(m_path.pos(i).ypos() - m_path.pos(i + 1).ypos());

		len += sqrt(xlen * xlen + ylen * ylen);
//		cout << "( " << m_path.pos(i).xpos() << " , " << m_path.pos(i).ypos() << " )        " << "( " << m_path.pos(i+1).xpos() << " , " << m_path.pos(i+1).ypos() << " )  len="<<len<<endl ;
	}

	if(!isEnd)
	{
		if(m_path.pos(i).xpos() == xpos && m_path.pos(i).ypos() == ypos)
		{//跟最后一个点一样
			int xlen = abs(m_path.pos(i - 1).xpos() - m_path.pos(i).xpos());
			int ylen = abs(m_path.pos(i - 1).ypos() - m_path.pos(i).ypos());

			len += sqrt(xlen * xlen + ylen * ylen);
		}
		else
		{//不等于最后一个点，代表点错误
			return false;
		}
	}

	FieldInfo * info = CMapConfig::GetInstance()->GetFieldInfoByid(GET_MAP_ID(GetMapID()));
	int64 nowtime = CUtil::GetNowSecond();
	float distance = getAttManage()->getValue<int>(eCharMoveSpeed)	* (nowtime - m_synPathTime);  // / 1000 / info->GetSectorSize();

	distance /= 1000;
	distance /= info->GetSectorSize();

	if (distance < (len - 4))
	{
//		printf("\n----------------------- player[ %lld] distance[%f] beyond len[%f]--------------------------\n", GetID() & 0xffffffff, distance, len);
		return false;
	}

	m_curIndex = i;
	m_synPathTime = nowtime;
	SetXNewPos(xpos);
	SetYNewPos(ypos);

	m_dirtyData[eBaseInfo] = true;

	return true;
}

void Player::SetSettings(int value, int type)
{
	int tempSet = getAttManage()->getValue<int>(eSettings);

	switch(type)
	{
		case 	eSettingsRanks:
		case	eNearByPlayer:
		case	eHideEffects:
		case eHidePets:
			{
				tempSet = (tempSet & (~(1 << type))) | (value << type);
				break;
			}
		case 	eBgSoundVolume:
			{
				tempSet = (tempSet & (~(0xff << type))) | (value << type);
				break;
			}
		case  eEfSoundVolume:
			{
				tempSet = (tempSet & (~(0xff << 12))) | (value << 12);
				break;
			}
		case	eSpSoundVolume:
			{
				tempSet = (tempSet & (~(0xff << 20))) | (value << 20);
				break;
			}
		default:
			break;
	}


	getAttManage()->setValue(eSettings, tempSet);

	m_dirtyData[eBaseInfo] = true;
}


int Player::GetSettings(int type)
{
	int tempSet = getAttManage()->getValue<int>(eSettings);

	return (tempSet & (1 << type)) > 0 ? true : false;
}

void Player::GetSettings(PlayerInfo::SettingInfo *info)
{
	int tempSet = getAttManage()->getValue<int>(eSettings);

	for(size_t i = 0; i < eSettingsMax; i++)
	{
		switch(i)
		{
			case 	eSettingsRanks:
			case	eNearByPlayer:
			case	eHideEffects:
			case eHidePets:
				{
					info->add_type(i);
					info->add_value(GetSettings(i));
					break;
				}
			case 	eBgSoundVolume:
				{
					info->add_type(eBgSoundVolume);
					info->add_value((tempSet >> i) & 0xff);
					break;
				}
			case  eEfSoundVolume:
				{
					info->add_type(eEfSoundVolume);
					info->add_value((tempSet >>12) & 0xff);
					break;
				}
			case	eSpSoundVolume:
				{
					info->add_type(eSpSoundVolume);
					info->add_value((tempSet >> 20) & 0xff);

					break;
				}
			default:
				break;
		}
	}
}

void Player::AddAcupoint(int acupoint)
{
	MeridianStruct *info = MeridianLoader::GetInstance()->GetMeridianStruct(m_acupoint + acupoint);

	if (info->i_type == ePtBig)
	{
		ChatMgr::GetInstance()->Marquee(eMarqueeAcupoint, GetName(),ChatMgr::GetInstance()->GetStringByInt(m_acupoint + acupoint - 1),ChatMgr::GetInstance()->GetStringByInt(m_acupoint + acupoint));
	}

	BEHAVIOR_HANDLE_DO(GetMyself(), BehaviorHandType::AddAcupoint(acupoint));

	m_acupoint += acupoint;
	m_dirtyData[eBaseInfo] = true;
}

int Player::IsAttacked(Smart_Ptr<CreatureObj> &obj, int type)
{
	if(obj.Get() == NULL || obj->IsDead() || !obj->IsInMap())
	{
		return eBattleTargetNull;
	}

	//镖车的特殊处理
	if (obj->GetType() == eMonster)
	{
		Smart_Ptr<Monster> mon = obj;

		if( mon->IsEscortCar() && mon->GetOwner().Get() == NULL)
		{
			return eBattleTargetNull;
		}

		if (mon->IsEscortCar() && mon->GetOwner() == m_myself)
		{
			return eCantAttackMyselfEscort;
		}
		else if (mon->IsEscortCar())
		{
			Smart_Ptr<Player> player = mon->GetOwner();

			if (player && player->GetEscortMgr() && player->GetEscortMgr()->GetRobTimes() >= 5)
			{
				return eEscortRobBeyondTimes;
			}
		}

		if(mon->GetQuality() == eFestivalMonster || mon->GetQuality() == eBatterFlag)
		{
			return eNoAttack;
		}

		//是否是同队伍的的镖车
		if (this->IsHaveGroup() && mon->IsEscortCar())
		{
			if (this->GetGroupID() == mon->GetOwner()->GetGroupID())
				return eCantAttackTeamEscort;
		}
	}

	if (obj->GetType() == ePlayer && obj->GetKey() != GetKey())
	{
		const int protectLv = GameParaLoader::GetInstance()->getProtectLv();
		if (obj->getAttManage()->getValue<int>(eCharLv) <= protectLv|| getAttManage()->getValue<int>(eCharLv) <= protectLv)
		{
			return eLVNotEnoughToAttack;
		}

		Smart_Ptr<Player> tempPlayer = obj;
		if(tempPlayer->IsInCreatureState(ERS_Invincible))
		{
			return eStateNotAttack;
		}

		if (GET_MAP_TYPE(GetMapID()) == eBeyondMap)
		{
			Smart_Ptr<CGameDuplicate> tDup = DuplicateMgr::GetInstance()->GetDuplicate(GetMapID());
			if (tDup)
			{
				if (!tDup->IsAttacked(GetID(), obj->GetID()))
				{
					return eCantAttackSameTeam;
				}
			}
		}

		if (GET_MAP_TYPE(GetMapID()) == ePVEMap)
		{
			Smart_Ptr<CGameDuplicate> tDup = DuplicateMgr::GetInstance()->GetDuplicate(GetMapID());
			if (tDup)
			{
				if (!tDup->IsAttacked(GetID(), obj->GetID()))
				{
					return eCantAttackSafeZone;
				}
			}
		}
	}

	switch (type)
	{
	case eEnemyTarget:
	{
		if(obj->GetType() == ePet)
		{
			Smart_Ptr<GirlWar> girlWar = obj;
			if( girlWar->GetOwner() && GetID() == girlWar->GetOwner()->GetID() )
			{
				return eCantAttackMyself;
			}
		}

		switch (getAttManage()->getValue<int>(ePKType))
		{
		case sPeace:
		{
			//和平模式下只能对怪物进行攻击
			if (obj->GetType() == ePlayer && obj->GetID() == GetID())
			{
				return eCantAttackMyself;
			}

			if (obj->GetType() == eMonster)
			{
				Smart_Ptr<Monster> mon = obj;

				if (mon->IsEscortCar())
				{
					return eCantAttackByPeace;
				}
			}

			if (obj->GetType() == ePet)
			{
				return eCantAttackByPeace;
			}

			//怪是同一阵营也不能攻击
			if (obj->GetType() != eMonster || getAttManage()->getValue<DWORD>(eCharCountry)== obj->getAttManage()->getValue<DWORD>(eCharCountry))
			{
				return eCantAttackByPeace;
			}

			break;
		}
		case sAll:
		{
			if (obj->GetType() == ePlayer)
			{
				if (obj->GetID() == GetID())
				{
					return eCantAttackMyself;
				}
			}

			if (obj->GetType() == ePet)
			{
				Smart_Ptr<GirlWar> girlWar = obj;
				Smart_Ptr<Player> player = girlWar->GetOwner();

				if (player->GetKey() == GetKey())
				{
					return eCantAttackMyself;
				}
			}

			break;
		}
		case sGoodEvil:
		{
			break;
		}

		case sCountry:
		{
			if (obj->GetType() == ePlayer && obj->GetID() == GetID())
			{
				return eCantAttackMyself;
			}

			if (getAttManage()->getValue<DWORD>(eCharCountry) == obj->getAttManage()->getValue<DWORD>(eCharCountry))
			{
				return eCantAttackSameCountry;
			}

			break;
		}

		case sTeam:
		{
			if (obj->GetType() == ePlayer && obj->GetID() == GetID())
			{
				return eCantAttackMyself;
			}

			int64 group = GetGroupID();
			int64 targetGroup = 0;
			if(obj->GetType() == ePlayer)
			{
					Smart_Ptr<Player> target = obj;
					targetGroup = target->GetGroupID();
			}
			else if(obj->GetType() == ePet)
			{
					Smart_Ptr<GirlWar> pet = obj;
					targetGroup = pet->GetOwner()->GetGroupID();
			}

			//相同队伍id不能攻击
			if(group > 0 && group == targetGroup)
			{
				return eNoAttack;
			}
			break;
		}

		case sAlliance:
		{						//把联盟模式暂时用在帮派中
			if (obj->GetType() == ePlayer)
			{
				if( obj->GetID() == GetID() )
				{
					return eCantAttackMyself;
				}

				Smart_Ptr<Player> player = obj;
				if (player->getAttManage()->getValue<DWORD>(eCharFaction) != 0
						&& player->getAttManage()->getValue<DWORD>(eCharFaction)== getAttManage()->getValue<DWORD>(eCharFaction))
				{
					return eNoAttack;
				}
			}
			else if (obj->GetType() == eMonster)
			{//帮派站中，将怪物的阵营变成玩家的帮派id，这个怪物就等于属于这个帮派了
				if (obj->getAttManage()->getValue<DWORD>(eCharCountry) == getAttManage()->getValue<DWORD>(eCharFaction))
					return eCantAttackSameCountry;
			}
			else if (obj->GetType() == ePet)
			{
				Smart_Ptr<GirlWar> girlWar = obj;
				Smart_Ptr<Player> player = girlWar->GetOwner();
				if (player->getAttManage()->getValue<DWORD>(eCharFaction) != 0
						&& player->getAttManage()->getValue<DWORD>(eCharFaction)== getAttManage()->getValue<DWORD>(eCharFaction))
				{
					return eNoAttack;
				}
			}

			break;
		}
		}

		break;
	}
	case eFriendTarget:
	{
		break;
	}
	case eMyself:
	{
		if (GetKey() != obj->GetKey())
		{
			return eNoAttack;
		}

		break;
	}
	case eNoPlayer:
	{

		if (obj->GetType() == ePlayer && obj->GetID() != GetID())
		{
			return eNoAttack;
		}

		if (obj->GetType() == eMonster)
		{
			Smart_Ptr<Monster> mon = obj;

			if (getAttManage()->getValue<int>(ePKType) == sAlliance)
			{
				if (obj->getAttManage()->getValue<DWORD>(eCharCountry) == getAttManage()->getValue<DWORD>(eCharFaction))
					return eCantAttackSameCountry;
			}

			if(obj->getAttManage()->getValue<DWORD>(eCharCountry) == getAttManage()->getValue<DWORD>(eCharCountry))
			{
				return eNoAttack;
			}

			if (mon->IsEscortCar())
			{
				return eNoAttack;
			}
		}

		//不可以攻击战姬
		if(obj->GetType() == ePet)
			return eNoAttack;

		break;
	}
	}
	return 0;
}


//检测时限物品定时器
void Player::beginCheckBagTimer()
{
	if (m_propcheckTimer.IsValid())
		m_propcheckTimer.SetNull();
	m_propcheckTimer = TimerInterface::GetInstance()->RegRepeatedTime(&Player::onCheckBag, this, 0, 0, 0, 0, check_prop_time,check_prop_time);
}

void Player::endCheckBagTimer()
{
	if (m_propcheckTimer.IsValid())
	{
		m_propcheckTimer.SetNull();
	}
}

void Player::onCheckBag(void* args)
{
	GetPackage()->checkTime();
}

Smart_Ptr<DanLuMgr> & Player::GetDanLuManager(){return m_danluMgr;}

Smart_Ptr<MagicMgr> & Player::GetMagicWeapon()
{
	return m_magicWeapon;
}

Smart_Ptr<SMagicMgr> & Player::GetSMagic()
{
	return m_sMagic;
}

Smart_Ptr<HeartMagicMgr> & Player::GetHeartMagicWeapon()
{
	return m_heartMagic;
}

Smart_Ptr<CJobLuckyMgr> & Player::GetCJobLucky()
{
	return m_cjobMgr;
}

Smart_Ptr<ActivityMgr> & Player::GetActivityMgr()
{
	return m_activity;
}

Smart_Ptr<EscortMgr> & Player::GetEscortMgr()
{
	return m_escort;
}

void Player::AddTaskMonster(Smart_Ptr<Monster> mon)
{
	m_pTaskMon = mon;
}

Smart_Ptr<Monster> Player::GetTaskMon()
{
	return m_pTaskMon;
}

Smart_Ptr<LucklyTurntableMgr> & Player::GetLucklyTableMgr()
{
	return m_lucktableMgr;
}

void Player::SynchActiveHighLight()
{
	// UI高亮特殊处理   UI高亮如无法在CActiveHighLight类添加  才在这里添加  尽量在CActiveHighLight添加
//	if (!m_myself)
//		return;
//
//	int tmpRet = 0;
//
//	SystemNoticeProto::ClientNoticeUIHL toClient;
//	SystemNoticeProto::UIHighLight*pHighLight = NULL;
//	bool bossFlag = WorldBossMgr::GetInstance()->WorldBossMgr_IsHaveAliveBoss();
//	if (bossFlag)
//	{
//		pHighLight = toClient.add_list();
//		pHighLight->set_type(eHL_Threat);
//		pHighLight->set_value(1);
//	}
//
//	//补偿活动
//	tmpRet = 0;
//	pHighLight = toClient.add_list();
//	pHighLight->set_type(eHL_Compensate);
//	pHighLight->set_value(m_bIsHaveCompensate);
//
//	QuickMessage::GetInstance()->AddSendMessage(this->GetID(),this->GetChannelID(), &toClient, MSG_SIM_HIGHLIGHT_ACTIVEINFO);
}

void Player::UpdateUIHighLight(E_UIHL_HANDLE handleType, UIHLType type, int count,  bool flag, int value)
{
	if ((bool)m_highLightMgr)
	{
// 		m_highLightMgr->UpdateActiveDataAndSyn(someType, count, whatType, flag);
		UpdateUIHLDataArgs args(GetMyself());
		args.AddContent(handleType, type.first, type.second, count, flag, value);

		FireEvent(PLAYER_UIHL_UPDATEDATA, args);
	}
}
void Player::UpdateUIHighLight(vector<E_UIHL_HANDLE> handleTypes, vector<E_UIHL_BIGTYPE> bigTypes, vector<E_UIHL_SMALLTYPE> smallTypes,
		vector<int> counts, vector<bool> flags, vector<int> values)
{
	if ((bool)m_highLightMgr)
	{
		if (0 < values.size())
		{
			UpdateUIHLDataArgs args(GetMyself(), handleTypes, bigTypes, smallTypes, counts, flags, values);
			FireEvent(PLAYER_UIHL_UPDATEDATA, args);
		}
		else
		{
			UpdateUIHLDataArgs args(GetMyself(), handleTypes, bigTypes, smallTypes, counts, flags);
			FireEvent(PLAYER_UIHL_UPDATEDATA, args);
		}
	}
}

void Player::UpdateUIHighLight(E_UIHL_HANDLE handleType, HintValue value, UIHLType type,
		bool flag)
{
	UpdateUIHighLight(handleType, type, value.fValue, flag, value.sValue);
}

int64 Player::CalOnlineTime()
{
	return CUtil::GetNowSecond() + m_onlineTime - m_loginTime;
}

void Player::SetInitPartDataFlag(int type)
{
	if (type < eSkillInfo || type >= eCharStructMax)
		return;
	if (m_initpartData[type])
		return;

	//玩家 所有 有关数据  全部  初始化 完成才 启动  定时器(目前 是 eFashionInfo 最后 完成  初始化)
	if (type == eCharStructMax - 1)
	{
		if (m_timerID.IsValid())
		{
			m_timerID.SetNull();
		}
		m_timerID = TimerInterface::GetInstance()->RegRepeatedTime(&Player::SaveAllPlayer, this, 0, 0, 0, 0, SAVE_CHARCACHE_TODB,SAVE_CHARCACHE_TODB);
	}
	m_initpartData[type] = true;
}

bool Player::GetInitPartDataFlag()
{
	for (int i = eSkillInfo; i < eCharStructMax; ++i)
	{
		if (!m_initpartData[i] && i != eFriendInfo)  //由于GM中 没有 涉及好友信息 故过滤此相关
			return false;
	}

	return true;
}

float Player::GetAntiAddictionFactor(void)
{
	return m_AntiAddictionMgr->GetAntiAddictionFactor();
}

int Player::GetPkTypeByMap(int pkType)
{
	int type = 0;

	switch (pkType)
	{
	case peace:
	{
		type = sPeace;
		break;
	}
	case neutral:
	{
		type = m_oldPkType;
		break;
	}
	case all:
	{
		type = sAll;
		break;
	}
	case goodEvil:
	{
		type = sGoodEvil;
		break;
	}
	case alliance:
	{
		type = sAlliance;
		break;
	}
	case country:
	{
		type = sCountry;
		break;
	}
	}

	return type;
}

void Player::SetPkTypeByMap(int pkType)
{
	int tempType = 0;
	switch (pkType)
	{
	case peace:
	{
		tempType = sPeace;
		break;
	}
	case neutral:
	{
		tempType = m_oldPkType;
		break;
	}
	case all:
	{
		tempType = sAll;
		break;
	}
	case goodEvil:
	{
		tempType = sGoodEvil;
		break;
	}
	case alliance:
	{
		tempType = sAlliance;
		break;
	}
	case country:
	{
		tempType = sCountry;
		break;
	}
	}

	if (tempType == 0)
		return;

	getAttManage()->setValue(ePKType, tempType);

	return;
}

bool Player::RedNamePlayerDropEquipAndGoods(Goods::SimDropSomeGoods & dropGoods,std::vector<PropBase> &dropList)
{
	if (this->GetSinState() != ERS_RED_NAME)
		return false;
	//掉1％本级经验
	DecreaseExp(GameParaLoader::GetInstance()->getDecreaseExpRate());

	if (GET_MAP_TYPE(this->GetMapID()) >= eCopyMap)
		return false;

	int sinValueFactor = (int) getAttManage()->getValue<float>(eSinValue) / 1000;
	int ratioEquip = min(30 + 5 * (sinValueFactor - 1), 80);
	int ratioGoods = min(35 + 10 * (sinValueFactor - 1), 80);
	int equipDropCount = 1 + random() % (1 + sinValueFactor);
	int goodsDropCount = 1 + sinValueFactor + random() % 2;
	//vip
	if (this->getAttManage()->getValue<DWORD>(eVIPLv) > 0)
	{
		ratioEquip = ratioEquip / 2;
		ratioGoods = ratioGoods / 2;
	}

	Goods::DropGoodsInfo * dropGoodsInfo = dropGoods.mutable_goodsinfo();

	//掉落装备
	int tempRandomFactor = random() % 100 + 1;
	if (tempRandomFactor <= ratioEquip)
	{
		std::vector<Smart_Ptr<PropBase> > equipPropList;
		Smart_Ptr<ArticleEquipMgr> equipQuickMgr = this->GetContainer(eEquipQuickType);
		if (equipQuickMgr)
		{
			for (size_t i = 0; i < equipQuickMgr->GetSize(); ++i)
			{
				int equipId = equipQuickMgr->GetItemDataPtr(i)->GetID();
				const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(equipId);
				if(ginfo==NULL)
					continue;
				if (equipId != -1 	&& 
				 ginfo->type >= eWeapon &&
				 ginfo->type <= eJewelry	&&
				 equipQuickMgr->GetItemDataPtr(i)->IsBind()== false)
				{
					equipPropList.push_back(equipQuickMgr->GetItemDataPtr(i));
				}
			}
			//随机
			std::random_shuffle(equipPropList.begin(), equipPropList.end());
			//掉落equipDropCount个
			int size = min(equipDropCount, (int) equipPropList.size());
			for (int i = 0; i < size; ++i)
			{
				dropList.push_back(*equipPropList[i]);
				Goods::DiscardUpdateGoodsRet * equip = dropGoodsInfo->add_goods();
				equipQuickMgr->DiscardEquipOrGoods(equipPropList[i]->GetID(),eEquipQuickType, equip);
			}
		}
	}

	//掉落物品
	tempRandomFactor = random() % 100 + 1;
	if (tempRandomFactor <= ratioGoods)
	{
		std::vector<Smart_Ptr<PropBase> > goodsList;
		Smart_Ptr<ArticleBaseMgr> packageMgr = this->GetContainer(ePackageType);
		for (size_t i = 0; i < packageMgr->GetSize(); ++i)
		{
			int equipId = packageMgr->GetItemDataPtr(i)->GetID();
			const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(equipId);
			if(ginfo==NULL)
					continue;
			if (equipId != -1 && packageMgr->GetItemDataPtr(i)->IsBind() == false)
			{
				goodsList.push_back(packageMgr->GetItemDataPtr(i));
			}
		}
		//随机
		std::random_shuffle(goodsList.begin(), goodsList.end());
		//掉落goodsDropCount个
		int size = min(goodsDropCount, (int) goodsList.size());
		for (int i = 0; i < size; ++i)
		{
			dropList.push_back(*goodsList[i]);
			Goods::DiscardUpdateGoodsRet * good = dropGoodsInfo->add_goods();
			packageMgr->DiscardEquipOrGoods(goodsList[i]->GetID(), ePackageType,good);
		}
	}

	if (dropList.size() == 0)
		return false;

	dropGoods.set_error(0);

	return true;
}

//是否 现在能够说话
bool Player::IsCanTaling()
{
	if (m_nextTalkTime == 0)
		return true;
	return m_nextTalkTime <= (int64) CUtil::GetNowSecond();
}

void Player::AddContinueBeheaded()
{
	vector<int> vecBro;
	int buffid = -1;

	//连斩CD时间
	if (m_cdMgr->IsCDEnd(CD_CONTINUE_BEHEADED))
	{
		getAttManage()->setValue(eContinueBeheaded, 1);
	}
	else
	{
		int tempBehead = getAttManage()->getValue<int>(eContinueBeheaded);
		tempBehead++;
		SkillLoader::GetInstance()->GetBuffIdByCount(tempBehead, buffid);
		getAttManage()->setValue(eContinueBeheaded, tempBehead);

		if (buffid != -1)
		{
			Smart_Ptr<CreatureObj> obj = this->GetMyself();
			AddBuff(buffid, obj);
			SetDataFlag(eBaseInfo);
		}
		vecBro.push_back(eContinueBeheaded);

		SynCharAttribute(vecBro);
	}

	m_cdMgr->StartCD(CD_CONTINUE_BEHEADED, 8000);
}

void Player::SendToPlayerResultCodeMessage(const int& retcode)
{
	ServerReturn::ServerRetInt info;

	info.set_ret(retcode);

	QuickMessage::GetInstance()->AddSendMessage(this->GetID(),this->GetChannelID(), &info, MSG_SIM_GM2C_PACKINFO);
}

//通知客户端每日信息更新
void Player::SendDayUpdate(const std::vector<int>& data )
{
	DayUpdate::DayInfoUpdate info;
	for(size_t i=0; i<data.size(); ++i)
	{
		info.add_info(data[i]);
	}

	QuickMessage::GetInstance()->AddSendMessage(this->GetID(),this->GetChannelID(), &info, MSG_REQ_GS2C_DAYUPDATE);
}

void Player::ResetPlayerSomethig()
{
	//用之前的数据初初化一次数据
	m_counterService->InitLastResetTime(m_counterService->GetPlayerLastResetTime());

	//重置资源追回,必须在所有能追回的模块之前重置并设置数据
	m_Daily->ResetDaily();

	m_Daily->InitRecoverResource();

	m_counterService->ResetCounterInfo();

	m_instanceMgr->ResetInstance();

	m_quest->ResetQuest();

	m_treasonKingReward->ResetTreasonKingReward();

	m_vitalityMgr->ResetVitality();

	m_activity->ReSetYellowVipState();

	this->SendToPlayerResultCodeMessage(eTodayRestOver);

	m_signIn->ResetSigIn();

	m_baseShopMgr->ResetBaseInfo();

	m_answerMgr->ResetAnswerInfo();

	//同步更新模块每日信息更新
	std::vector<int> updatelist;
	updatelist.push_back(eDayInfoUpade_Instance);
	updatelist.push_back(eDayInfoUpade_TREASON_KING);
	updatelist.push_back(eDayInfoUpade_PROTECT_KING);
	updatelist.push_back(eDayInfoUpade_ESCORT);
	updatelist.push_back(eDayInfoUpade_LuckTable);
	updatelist.push_back(eDayInfoUpade_MoneyShop);
	updatelist.push_back(eDayInfoUpade_Complete);
	updatelist.push_back(eDayInfoUpade_CampShop);
	updatelist.push_back(eDayInfoUpade_Benefits);
	updatelist.push_back(eDayInfoUpade_RedBag);
	updatelist.push_back(eDayInfoUpade_Active);
	updatelist.push_back(eDayInfoUpade_Beyond_batter);
	updatelist.push_back(eDayInfoUpade_Vitality);
	updatelist.push_back(eDayInfoUpade_BaseShop);

	if(GetYelloVip()==1)
	{
		updatelist.push_back(eDayInfoUpade_YellowVip);
	}
	SendDayUpdate(updatelist);

	//清除竞技场cd
	GetCDMgr()->ResetCDTime(ArenaLoader::GetInstance()->GetChannelType());

	//同步更新任务信息
	GetQuestMgr()->FillInQuestInfoCmd();

	m_counterService->initFinish();

	m_Christmas->playerLogin();

	m_arenaMgr->Reset();


}

void Player::ResetPLayerSomethingAferResetTime()
{
	m_onlineReward->ResetOnlineReward();

	// UI高亮 必须 最后重置
	UpdateUIHighLight(eUIHLHandle_ResetAndSyn, UIHLType(eUIHLBig_Null, eUIHLSmall_Null));
}

//结束连斩BUFF
void Player::EndContinueBeheaded()
{
	m_skillMgr->EndContinueBeheaded();
	SetDataFlag(eSkillInfo);
}

int64 Player::GetCurTotalOnlineTime()
 {
	int64 nowonlinetime = CUtil::GetNowSecond() - m_loginTime	+ m_totalOnlineTime;
	return nowonlinetime;  //记录 总在线时长
}

void Player::AddEnemy(Smart_Ptr<CreatureObj> &obj)
{
	if (!obj)
	{
		return;
	}

	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for (; it != m_enemyList.end(); ++it)
	{
		if ((*it).Get() == obj.Get() || obj.Get() == m_myself.Get())
		{
			return;
		}
	}

	m_enemyList.push_back(obj);
}

void Player::DeleteTargetEnemy(Smart_Ptr<CreatureObj> & player)
{
	vector<Smart_Ptr<CreatureObj> >::iterator it = m_enemyList.begin();
	for (; it != m_enemyList.end();)
	{
		if ((*it).Get() == 0)
		{
			it = m_enemyList.erase(it);
			continue;
		}

		if (player.Get() == (*it).Get())
		{
			it = m_enemyList.erase(it);
			continue;
		}

		it++;
	}
}
void Player::ClearEnemy()
{
}

void Player::CountryJobAttrAdd()
{
	if (getAttManage()->getValue<WORD>(eCountryJob) <= 0)
		return;

	const CampJobPrize* pInfo = GsCountryMgr::GetInstance()->getCampJobInfo(getAttManage()->getValue<WORD>(eCountryJob));
	if (pInfo == NULL|| (pInfo->attriRate >= -0.001f && pInfo->attriRate <= 0.001f))
		return;

	//物理攻击
	getAttManage()->addPercentValue(eCharPhyAttackMax, pInfo->attriRate);

	//物理防御
	getAttManage()->addPercentValue(eCharPhyDefence, pInfo->attriRate);

	//最大血量
	getAttManage()->addPercentValue(eCharHPMax, pInfo->attriRate);
}

void Player::CountryJobAttrRemove()
{
	if (getAttManage()->getValue<WORD>(eCountryJob) <= 0)
		return;

	const CampJobPrize* pInfo = GsCountryMgr::GetInstance()->getCampJobInfo(getAttManage()->getValue<WORD>(eCountryJob));
	if (pInfo == NULL|| (pInfo->attriRate >= -0.001f && pInfo->attriRate <= 0.001f))
		return;

	//物理攻击
	getAttManage()->addPercentValue(eCharPhyAttackMax, -pInfo->attriRate);

	//物理防御
	getAttManage()->addPercentValue(eCharPhyDefence, -pInfo->attriRate);

	//最大血量
	getAttManage()->addPercentValue(eCharHPMax, -pInfo->attriRate);
}

//增减属性
void Player::addAttr(int operate, const std::vector<pair<int, int> >& attrs,bool syn,eAttValueType tempValueType)
{
	vector<int> attrChange;
	for (size_t i = 0; i < attrs.size(); ++i)
	{
		switch (attrs[i].first)
		{
		case eCharPhyAttackMax:
			ChangePhyAttackMax(attrs[i].second, true,tempValueType);
			break;
		case eCharPhyDefence:
			ChangePDefence(attrs[i].second, true,tempValueType);
			break;
		case eCharHPMax:
			ChangeHPMax(attrs[i].second, true,tempValueType);
			break;
		default:
			break;
		}
		attrChange.push_back(attrs[i].first);
	}

	if (syn)
	{
		SynCharAttribute(attrChange);
	}
	//计算一次阵营战斗力
	calculateCampFight();
	m_attrData[operate] = attrs;
}

void Player::removeAttr(int operate,eAttValueType tempValueType)
{
	std::map<int, std::vector<std::pair<int, int> > >::iterator it =m_attrData.find(operate);
	if (it == m_attrData.end())
		return;

	vector<int> attrChange;
	for (size_t i = 0; i < it->second.size(); ++i)
	{
		switch (it->second[i].first)
		{
		case eCharPhyAttackMax:
			ChangePhyAttackMax(it->second[i].second, false,tempValueType);
			break;
		case eCharPhyDefence:
			ChangePDefence(it->second[i].second, false,tempValueType);
			break;
		case eCharHPMax:
			ChangeHPMax(it->second[i].second, false,tempValueType);
			break;
		default:
			break;
		}
		attrChange.push_back(it->second[i].first);
	}
	SynCharAttribute(attrChange);

	m_attrData.erase(it);
}

//是否已经开放
bool Player::IsOpenSystem(int type)
{
	return getAttManage()->getValue<int>(eCharLv)>= SysSettingLoader::GetInstance()->GetLvByType(type);
}

bool Player::JoinCamp(const EventArgs& e)
{
	const int Get_Envelope_Count_Per_10_Lv = CountryRedEnvelopeLoader::GetInstance()->getGetenvelopeCountPer10lv();

	OneParmaEventArgs* data = (OneParmaEventArgs*) (&e);
	if (!data)
	{
		return true;
	}

	Smart_Ptr<Player> player = data->m_obj;
	if (player && player->GetID() == GetID())
	{
		this->setMaxCanGetEnvelopeCount(Get_Envelope_Count_Per_10_Lv);
	}

	return true;
}

Smart_Ptr<ExtraAtributeMgr>& Player::getExtraAtributeMgr()
{
	return m_extraAtributeMgr;
}

void Player::SynchRechargeToWS(int golden)
{
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(GetID()),worldSvr);
	if (worldSvr)
	{
		ServerReturn::ServerDoubleInt toWorld;
		toWorld.set_retf(GetID());
		toWorld.set_rets(golden);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_RECHARGE, &toWorld, worldSvr.Get(),SimpleMessage);
		Message_Facade::Send(messRet);
	}
}


//获取指定等级所加的属性
void Player::getDesignationLvAttList(int level, BYTE profession,map<ECharAttrType, int64>& attList)
{
	LvBattleAtt tempAtt;
	BattleInfoLoader::GetInstance()->GetBattleByLvAndPro(level, profession,tempAtt);

	attList[eCharPhysicalPower] += tempAtt.physicalpower;	//体质
	attList[eCleverAir] += tempAtt.cleverAir;	  	//灵气
	attList[eCleverPower] += tempAtt.cleverPower; 	//灵力
	attList[eCharStrength] += tempAtt.strength; 		//力量
	attList[eCharAgility] += tempAtt.agility; 		//敏捷
	attList[eCharHitRate] += tempAtt.hitRate;			//命中
	attList[eCharAvoidRate] += tempAtt.avoidAttack;		//闪避
	attList[eCrackDownRate] += tempAtt.crackDown;		//爆击
	attList[eAvoidCrackRate] += tempAtt.avoidCrack; 		//抗爆

	attList[eCharHPMax] +=BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePhyPowerToHP, tempAtt.physicalpower);
	attList[eCharMPMax] +=BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eCleverAirtoMP, tempAtt.cleverAir);
	attList[eCharPhyAttackMax] +=BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(eStrengthToAt, tempAtt.strength);
	attList[eCharPhyDefence] += BattleInfoLoader::GetInstance()->GetSecondAttrByFirstAttr(ePatienceToPD, tempAtt.patience);
	attList[eCharMoveSpeed] +=BattleInfoLoader::GetInstance()->getBaseMoveSpeed();

	return;
}

Smart_Ptr<Fighting> Player::getFightingManage()
{
	return m_Fighting;
}

Smart_Ptr<MarryRing> Player::GetMarryRing()
{
	return m_MarryRing;
}

Smart_Ptr<PlayerRegion> Player::GetRegion()
{
	return m_Region;
}

Smart_Ptr<ChristmasManage> Player::GetChristmas()
{
	return m_Christmas;
}

Smart_Ptr<NewYearActivity> Player::GetNewYear()
{
	return m_NewYear;
}

void Player::InitArenaInfo(const PlayerInfo::ArenaDataInfo &info)
{
	m_arenaMgr->InitArenaData(info);
	this->SetInitPartDataFlag(eArenaInfo);
}

Smart_Ptr<ArenaMgr>& Player::GetArenaMgr()
{
	return m_arenaMgr;
}

int Player::GetLoginClientType()
{
	return m_loginClientType;
}
void Player::SetLoginClientType(int loginClientType)
{
	m_loginClientType = loginClientType;
}

void Player::UpdateInfoByClientType()
{
	if (eClientType_Micro == GetLoginClientType())
	{
		Smart_Ptr<CounterService>& counter = GetCounterService();
		if ((bool)counter)
		{
			counter->IncreasePlayerCounter(LOGINMICRO_COUNT, 1);
		}
		MicroRewardInfo info;
		MicroRewardLoader::GetInstance()->GetMicroReward(info);
		if (info.addExpRatio > 0)
		{
			getAttManage()->addValue(eKillMonsterExpRate,info.addExpRatio);
		}
	}
}

void Player::GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos)
{
	if(!m_myself)
	{
		return;
	}

	if (m_curIndex < 0 || (m_curIndex + 1) >= m_path.pos_size())
	{
		txpos = GetXNewPos();
		typos = GetYNewPos();

		return;
	}

	float len = 0;
	int i = 0;

	for (i = m_curIndex; i < m_path.pos_size() - 1; ++i)
	{
		if (m_path.pos(i).xpos() == xpos && m_path.pos(i).ypos() == ypos)
		{
			break;
		}

		int xlen = abs(m_path.pos(i).xpos() - m_path.pos(i + 1).xpos());
		int ylen = abs(m_path.pos(i).ypos() - m_path.pos(i + 1).ypos());

		len += sqrt(xlen * xlen + ylen * ylen);

		float timeDis = len / (float)getAttManage()->getValue<int>(eCharMoveSpeed);
		float moveDis = timeDis * moveSpeed;

		int realDistX = abs(xpos - m_path.pos(i).xpos());
		int realDistY = abs(ypos - m_path.pos(i).ypos());
		float realDist = sqrt(realDistX * realDistX + realDistY * realDistY);

		if((moveDis + 1) >= realDist && (moveDis - 1) <= realDist)
		{//1的范围内，就算是最佳点
			txpos = m_path.pos(i).xpos();
			typos = m_path.pos(i).ypos();

			return;
		}
		else if((moveDis - 1) > realDist)
		{//超出1太多，就用上一个点
			break;
		}
	}

	if(i < m_path.pos_size() - 1)
	{
		--i;
		if(i < m_curIndex)
			i = m_curIndex;
	}

	txpos = m_path.pos(i).xpos();
	typos = m_path.pos(i).ypos();
}

Smart_Ptr<PlayerFestiveAttr> Player::getFestivalMgr()
{
		return m_festiveAttrMgr;
}
