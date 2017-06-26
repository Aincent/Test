/*
 * CActiveHighLightMgr.cpp
 *
 *  Created on: 2015年7月4日
 *      Author: root
 */

#include "CActiveHighLightMgr.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "../MessageBuild.h"
#include "../MoneyShop/MoneyShopMgr.h"
#include "../WorldBoss/WorldBoss.h"
#include "../LucklyTurntable/LucklyTurntableMgr.h"
#include "../Attribute/AttributeCreature.h"
#include "../CountryRedEnvelope/CountryRedEnvelopeMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Monster/Monster.h"
#include "../EscortEx/EscortMgr.h"
#include "../Activity/ActivityMgr.h"
#include "../ServerEventDefine.h"
#include "../RewardHail/OnceReward.h"
#include "../Clan/ClanMessage.h"
#include "FileLoader/MaterialSynthesisLoader.h"
#include "../Meridians/MeridianMgr.h"
#include "../MagicWeapon/MagicLoader/MagicLoader.h"
#include "../MapInstance/CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"
#include "../Container/ParcelMgr.h"
#include "../Object/Player/FriendManage.h"
#include "../Meridians/FileLoader/MeridianLoader.h"
#include "../DanLu/DanLuLoader.h"
#include "../Horse/HorseLoader/HorseLoader.h"
#include "CountryDefine.h"
#include "../DanLu/DanLuMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../HeartMagic/HeartMagicMgr.h"
#include "../Magic/SMagicMgr.h"
#include "../Country/GsCountryMgr.h"
#include "../Horse/HorseManager.h"
#include "../Container/ArticleEquipMgr.h"
#include "../EquipSmith/MaterialSynthesis/CMeterialSynMessage.h"
#include "../Activity/ActivityBox.h"
#include "../Country/playerCampShopMgr.h"
#include "HintLoader/HintLoader.h"
#include "FileLoader/InterfaceAnswerLoader.h"
#include "../Object/Player/Player.h"
#include "../MapInstance/BeyondBattleMgr.h"
#include "../Arena/ArenaMgr.h"

bool HintValue::operator==(const HintValue& obj)
{
	return this->fValue == obj.fValue && this->sValue == obj.sValue;
}

bool HintValue::operator==(const int& value)
{
	return this->fValue == value;
}
bool HintValue::operator!=(const HintValue& obj)
{
	return !(operator==(obj));
}

bool HintValue::operator!=(const int& value)
{
	return !(operator==(value));
}
bool HintValue::operator>(const int& value)
{
	return this->fValue > value;
}
bool HintValue::operator<(const int& value)
{
	return this->fValue < value;
}

bool HintValue::operator<=(const int& value)
{
	return !operator>(value);
}
bool HintValue::operator>=(const int& value)
{
	return !(operator<(value));
}
bool HintValue::operator<=(const HintValue& obj)
{
	return this->fValue <= obj.fValue;
}
bool HintValue::operator>=(const HintValue& obj)
{
	return this->fValue >= obj.fValue;
}
bool HintValue::operator>(const HintValue& obj)
{
	return !(operator<=(obj));
}
bool HintValue::operator<(const HintValue& obj)
{
	return !(operator>=(obj));
}

HintValue& HintValue::operator-(const HintValue& obj)
{
	this->fValue -= obj.fValue;
	return *this;
}

HintValue& HintValue::operator+(const HintValue& obj)
{
	this->fValue += obj.fValue;
	return *this;
}

HintValue& HintValue::operator=(const HintValue& obj)
{
	if (this != &obj)
	{
		this->fValue = obj.fValue;
		this->sValue = obj.sValue;
	}

	return *this;
}

HintValue& HintValue::operator+=(const HintValue& obj)
{
	this->fValue += obj.fValue;
	return *this;
}
HintValue& HintValue::operator-=(const HintValue& obj)
{
	this->fValue -= obj.fValue;
	return *this;
}
HintValue& HintValue::operator=(const int& value)
{
	this->fValue = value;
	return *this;
}
HintValue& HintValue::operator+=(const int& value)
{
	this->fValue += value;
	return *this;
}
HintValue& HintValue::operator-=(const int& value)
{
	this->fValue -= value;
	return *this;
}


CHighLightMgr::CHighLightMgr(Player* player) : m_owner(player)
{
	ReInit();
}

CHighLightMgr::~CHighLightMgr()
{
	Release();
}

void CHighLightMgr::ReInit()
{
	ClearUIHLData();

	if(!m_handleDataEvent.IsValid())
		m_handleDataEvent = RegEvent(m_owner, PLAYER_UIHL_UPDATEDATA, this, &CHighLightMgr::HandleDataEvent);

	if(!m_vipEvent.IsValid())
		m_vipEvent = RegEvent(m_owner, PLAYER_UPGRADEVIPLV_EVENT_TYPE, this, &CHighLightMgr::HandlePlayerVipUp);

	if(!m_moneyChangeEvent.IsValid())
		m_moneyChangeEvent = RegEvent(m_owner, PLAYER_MONEY_CHANGE, this, &CHighLightMgr::HandleMoneyChange);

	if(!m_packageChangeEvent.IsValid())
		m_packageChangeEvent = RegEvent(m_owner, PLYAER_PACKAGE_CHANGE, this, &CHighLightMgr::HandlePackageGoods);

	if(!m_wearEquipEvent.IsValid())
		m_wearEquipEvent = RegEvent(m_owner, PLAYER_WEAR_EQUIP, this, &CHighLightMgr::HandleWearEquip);

	if(!m_dropEquipEvent.IsValid())
		m_dropEquipEvent = RegEvent(m_owner, PLAYER_TAKE_OFF_EQUIP, this, &CHighLightMgr::HandleDropEquip);

	if(!m_strengthEquipEvent.IsValid())
		m_strengthEquipEvent = RegEvent(m_owner, PLAYER_SMELT_STRONG, this, &CHighLightMgr::HandleStrengthEquip);

	if(!m_inlayEquipEvent.IsValid())
		m_inlayEquipEvent = RegEvent(m_owner, PLAYER_SMELT_INSERT, this, &CHighLightMgr::HandleInlayEquip);

	if(!m_refineEquipEvent.IsValid())
		m_refineEquipEvent = RegEvent(m_owner, PLAYER_SMELT_SMELT, this, &CHighLightMgr::HandleRefineEquip);

	if(!m_upgradeEquipEvent.IsValid())
		m_upgradeEquipEvent = RegEvent(m_owner, PLAYER_EQUIP_UPGRADE, this, &CHighLightMgr::HandleUpgradeEquip);

	if(!m_forceChangeEvent.IsValid())
		m_forceChangeEvent = RegEvent(m_owner, PLYAER_CHANGE_FORCE, this, &CHighLightMgr::HandleForceChange);

	if(!m_removeStoneEvent.IsValid())
		m_removeStoneEvent = RegEvent(m_owner, PLAYER_TAKE_OFF_STONE, this, &CHighLightMgr::HandleRemoveStone);

	if(!m_signInEvent.IsValid())
		m_signInEvent = RegEvent(m_owner, PLAYER_SIGNIN_EVENT_TYPE, this, &CHighLightMgr::HandleSigniIn);

	if(!m_submitTaskEvent.IsValid())
		m_submitTaskEvent = RegEvent(m_owner, PLAYER_QUEST_SUBMITQUEST, this, &CHighLightMgr::HandleSubmitTask);

	if(!m_enterInstEvent.IsValid())
		m_enterInstEvent = RegEvent(m_owner, PLAYER_ENTERINSTANCE_EVENT_TYPE, this, &CHighLightMgr::HandleEnterInst);

	if(!m_resetProtectKing.IsValid())
			m_resetProtectKing = RegEvent(m_owner, PLAYER_RESET_PROTECTKING, this, &CHighLightMgr::HandleResetProtectKing);

	if(!m_moneyInstBuyCntEvent.IsValid())
		m_moneyInstBuyCntEvent = RegEvent(m_owner, PLAYER_MONEYINST_BUYCNT, this, &CHighLightMgr::HandleMoneyInstBuyCnt);

	if(!m_escortStartEvent.IsValid())
		m_escortStartEvent = RegEvent(m_owner, PLAYER_START_ESCORT, this, &CHighLightMgr::HandleEscortStart);

	if(!m_passInstEvent.IsValid())
		m_passInstEvent = RegEvent(m_owner, PLAYER_PASS_INSTANCE, this, &CHighLightMgr::HandlePassInst);

	if(!m_onekeyQuest.IsValid())
		m_onekeyQuest = RegEvent(m_owner, PLAYER_ONE_KEY_COMPLETE, this, &CHighLightMgr::HandleOnekeyQuest);

	if(!m_getMoneyShopEvent.IsValid())
		m_getMoneyShopEvent = RegEvent(m_owner, PLAYER_RECEIVE_MONEY_SHOP, this, &CHighLightMgr::HandleGetMoneyShop);

	if(!m_resetAllInstEvent.IsValid())
		m_resetAllInstEvent = RegEvent(m_owner, PLAYER_RESET_ALLINSTANCE, this, &CHighLightMgr::HandleResetAllInst);

	if(!m_startSweepEvent.IsValid())
		m_startSweepEvent = RegEvent(m_owner, PLAYER_RAIDS_DUPLICATE, this, &CHighLightMgr::HandleStartSweep);

	if (!m_addArenaChallEvent.IsValid())
	{
		m_addArenaChallEvent = RegEvent(m_owner, PLAYER_ADD_ARENACHALL, this, &CHighLightMgr::HandleAddArenaChall);
	}

	if (!m_passTargetEvent.IsValid())
	{
		m_passTargetEvent = RegEvent(m_owner, PLAYER_PASS_TARGET, this, &CHighLightMgr::HandlePassTarget);
	}

	if (!m_drawTargetReward.IsValid())
	{
		m_drawTargetReward = RegEvent(m_owner, PLAYER_DRAW_TARGETREWARD, this, &CHighLightMgr::HandleDrawTarget);
	}

	if (!m_skillUpEvent.IsValid())
	{
		m_skillUpEvent = RegEvent(m_owner, PLAYER_SKILL_UPGRADE, this, &CHighLightMgr::HandleSkillUp);
	}

	if (!m_getSigninAward.IsValid())
	{
		m_getSigninAward = RegEvent(m_owner, PLAYER_GETSIGNIN_AWARD, this, &CHighLightMgr::HandleGetSignAward);
	}
	if (!m_getOnlineAward.IsValid())
	{
		m_getOnlineAward = RegEvent(m_owner, PLAYER_ONLINE_GIFT, this, &CHighLightMgr::HandleGetOnlineAward);
	}
	if (!m_getLevelAward.IsValid())
	{
		m_getLevelAward = RegEvent(m_owner, PLAYER_LEVEL_GIFT, this, &CHighLightMgr::HandleGetLvAward);
	}
	if (!m_getSevenLoginAward.IsValid())
	{
		m_getSevenLoginAward = RegEvent(m_owner, PLAYER_SEVEN_PACKAGE, this, &CHighLightMgr::HandleGetSevenLogin);
	}
}

void CHighLightMgr::Release()
{
	ClearUIHLData();

	if (m_handleDataEvent.IsValid())
	{
		m_handleDataEvent.SetNull();
	}

	if (m_vipEvent.IsValid())
	{
		m_vipEvent.SetNull();
	}

	if (m_moneyChangeEvent.IsValid())
	{
		m_moneyChangeEvent.SetNull();
	}

	if (m_packageChangeEvent.IsValid())
	{
		m_packageChangeEvent.SetNull();
	}

	if (m_wearEquipEvent.IsValid())
	{
		m_wearEquipEvent.SetNull();
	}

	if (m_dropEquipEvent.IsValid())
	{
		m_dropEquipEvent.SetNull();
	}

	if(m_strengthEquipEvent.IsValid())
	{
		m_strengthEquipEvent.SetNull();
	}

	if(m_inlayEquipEvent.IsValid())
	{
		m_inlayEquipEvent.SetNull();
	}

	if(m_refineEquipEvent.IsValid())
	{
		m_refineEquipEvent.SetNull();
	}

	if(m_upgradeEquipEvent.IsValid())
	{
		m_upgradeEquipEvent.SetNull();
	}

	if(m_forceChangeEvent.IsValid())
	{
		m_forceChangeEvent.SetNull();
	}

	if(m_removeStoneEvent.IsValid())
	{
		m_removeStoneEvent.SetNull();
	}

	if(m_signInEvent.IsValid())
	{
		m_signInEvent.SetNull();
	}

	if(m_submitTaskEvent.IsValid())
	{
		m_submitTaskEvent.SetNull();
	}

	if(m_enterInstEvent.IsValid())
	{
		m_enterInstEvent.SetNull();
	}

	if(m_resetProtectKing.IsValid())
	{
		m_resetProtectKing.SetNull();
	}

	if(m_moneyInstBuyCntEvent.IsValid())
	{
		m_moneyInstBuyCntEvent.SetNull();
	}

	if(m_escortStartEvent.IsValid())
	{
		m_escortStartEvent.SetNull();
	}

	if(m_passInstEvent.IsValid())
	{
		m_passInstEvent.SetNull();
	}

	if(m_onekeyQuest.IsValid())
	{
		m_onekeyQuest.SetNull();
	}

	if(m_getMoneyShopEvent.IsValid())
	{
		m_getMoneyShopEvent.SetNull();
	}

	if (m_addArenaChallEvent.IsValid())
		m_addArenaChallEvent.SetNull();

	if(m_resetAllInstEvent.IsValid())
	{
		m_resetAllInstEvent.SetNull();
	}

	if(m_startSweepEvent.IsValid())
	{
		m_startSweepEvent.SetNull();
	}

	if(m_passTargetEvent.IsValid())
	{
		m_passTargetEvent.SetNull();
	}

	if (m_drawTargetReward.IsValid())
	{
		m_drawTargetReward.SetNull();
	}

	if (m_skillUpEvent.IsValid())
	{
		m_skillUpEvent.SetNull();
	}

	if (m_getSigninAward.IsValid())
	{
		m_getSigninAward.SetNull();
	}
	if (m_getOnlineAward.IsValid())
	{
		m_getOnlineAward.SetNull();
	}
	if (m_getLevelAward.IsValid())
	{
		m_getLevelAward.SetNull();
	}
	if (m_getSevenLoginAward.IsValid())
	{
		m_getSevenLoginAward.SetNull();
	}
}

void CHighLightMgr::ComTypeAddData()
{
	ComTypeAddData1();
	ComTypeAddData2();

	ActivityHLTypeAdd();

	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}
}

void CHighLightMgr::ComTypeAddData1()
{
	{ // 霸业 1
		// 霸业开启时
		int count = 0;
		AddUIHLData(eUIHLBig_FactionBattle, eUIHLSmall_FactionBattle_Open, count);
	}
	{ // 竞技场 1
		// 挑战次数未完成时,剩余挑战次数
		int count = 0;
		HintValue value;
		if (m_owner->GetArenaMgr() &&m_owner->IsOpenSystem(eStCompeteInstance))
		{
			value.fValue = m_owner->GetArenaMgr()->GetCanChallCnt();
			value.sValue = m_owner->GetArenaMgr()->GetBaseSumChallCnt();
//			value.sValue = m_owner->GetInstanceMgr()->GetArenaUnChallenge(value.fValue);
			AddUIHLData(eUIHLBig_Arena, eUIHLSmall_Arena_ChallengeNum, value);
		}

		//todo: from world
		// 排名礼包未领取时
		int ranking = m_owner->getAttManage()->getValue<int>(eAwardRanking);
		count = 0;
		if (0 < ranking)
			count = 1;
		else
			count = 0;
		AddUIHLData(eUIHLBig_Arena, eUIHLSmall_Arena_AwardUnGet, count);
	}
	{ //  跨服竞技 1
		int count = 0, challNumCeil = 0;
		Smart_Ptr<CounterService>& counterMgr = m_owner->GetCounterService();

//		if ((bool)counterMgr)
//		{
//			challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit() + CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit();
//			count = challNumCeil - counterMgr->GetNumById(BEYOND_BATTLE);
//		}
//		AddUIHLData(eUIHLBig_BeyondBattle, eUIHLSmall_BeyondBattle_ChallengeNum, count);

		// 单人跨服竞技
		HintValue value;
		if (m_owner->GetInstanceMgr() &&m_owner->IsOpenSystem(eStBeyondSvrBattle))
		{
			// 未完成/总数
			challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit();
			count = challNumCeil - counterMgr->GetNumById(BEYOND_SINGLE_BATTLE);
			value.fValue = count;
			value.sValue = challNumCeil;
			AddUIHLData(eUIHLBig_BeyondBattle, eSmall_SingleBeyondArena_AlreadyChallenge, value);
		}


		// 多人跨服竞技
		if (m_owner->GetInstanceMgr() &&m_owner->IsOpenSystem(eStBeyondSvrBattle))
		{
			// 未完成/总数
			challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit();
			count = challNumCeil - counterMgr->GetNumById(BEYOND_GROUP_BATTLE);
			value.fValue = count;
			value.sValue = challNumCeil;
			AddUIHLData(eUIHLBig_BeyondBattle, eSmall_GroupBeyondArena_AlreadyChallenge, value);
		}


	}
	{ // 新服活动 (运营活动) 1
//		int count = 0;
//		AddUIHLData(eUIHLBig_ActivityHall, eUIHLSmall_ActivityHall_Open, count);
	}
	{ // 装备副本
		HintValue value = 0;
		Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
		if ((bool)instanceMgr)
		{
			value.sValue = instanceMgr->GetSingleInstUnChall(value.fValue);
//			count += instanceMgr->GetAllInstSurplusNum();
		}
		AddUIHLData(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini, value);

	}
	// 进阶副本
	{
		HintValue value = 0;
		Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
		if ((bool)instanceMgr)
		{
			value.sValue = instanceMgr->GetAdvancedInstUnChall(value.fValue);
		}

		AddUIHLData(eBig_Advanced, eSmall_Advanced_AlreadyFini, value);
	}
	{ // 活跃度 1
		int count = 0;
		count += m_owner->GetVitalityMgr()->GetUnGetAwardCnt();
		AddUIHLData(eUIHLBig_Vitality, eUIHLSmall_Vitelity_UnGetReward, count);
	}
	{
		// 奖励大厅 福利大厅 1
		int count = 0;

		Smart_Ptr<SignIn>& signIn  = m_owner->GetSignIn();
		if ((bool)signIn)
		{
			// 每日签到
			HintValue value;
			Smart_Ptr<SignIn>& signIn  = m_owner->GetSignIn();
			if (signIn && m_owner->IsOpenSystem(eStSignLv))
			{
				value.fValue += !signIn->isTodayHaveSignin();
				value.sValue = 1;
			}
			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanSignIn, value);

			count = 0;
			if (signIn && m_owner->IsOpenSystem(eStSignLv))
			{
				count += signIn->GetMendingSignCnt();
				AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanMendingSignIn, count);
			}


			count = 0;
			if (signIn && m_owner->IsOpenSystem(eStSignLv))
			{
				count += signIn->CheckSignInReward();
				AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_SignInTotalAward, count);
			}


			// 七日礼包
			count = 0;
			if (m_owner->IsOpenSystem(eStSevenRewardLv))
			{
				count += signIn->GetTLCanGetAward();
			}

			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_ServenLoginAward, count);

		}
		// 在线
		count = 0;
		Smart_Ptr<OnlineReward>& onLineMgr = m_owner->getOnlineReward();
		if (m_owner->IsOpenSystem(eStOnlineRewardLv) && (bool)onLineMgr)
		{
			count += onLineMgr->GetOLCanGetAward();
		}
		AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOnlineAward, count);

		// 等级
		count = 0;
		Smart_Ptr<LevelReward>& levelMgr = m_owner->GetLevelReward();
		if (m_owner->IsOpenSystem(eStLevelRewardLv) && (bool)levelMgr)
		{
			count += levelMgr->GetLRCanGetAward();
		}
		AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetLevelAward, count);

		Smart_Ptr<DailyManager> dailyMgr = m_owner->GetDailyManager();
		if((bool)dailyMgr)
		{
			// 资源追回
			count = 0;
			if (m_owner->IsOpenSystem(eStResourceRecoverLv))
			{
				count += dailyMgr->GetRecoverResourceCount();
			}
			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanResFound, count);

			// 离线经验
			count = 0;
			if (m_owner->IsOpenSystem(eOfflineRewardLv))
			{
				count += dailyMgr->IsHaveDailyPracticeToGet();
			}
			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOffAward, count);

		}
//		AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetReward, count);
	}
	// 游戏目标 1
	{
		Smart_Ptr<Destination> dest = m_owner->GetDestination();
		if ((bool)dest && m_owner->IsOpenSystem(eStGameObject))
		{
//			count += dest->GetUnGetRewardNum();
			map<int, int> unGet;
			dest->GetPerStageCanGetAward(unGet);
			for (int i = eDestinationStage_1; i < eDestinationStage_Max; ++i)
			{
				E_UIHL_SMALLTYPE smallType;
				switch (i)
				{
				case eDestinationStage_1:
					{
						smallType = eUIHLSmall_GameTarget_OneCanGetReward;
					}
					break;
				case eDestinationStage_2:
					{
						smallType = eUIHLSmall_GameTarget_TwoCanGetReward;
					}
					break;
				case eDestinationStage_3:
					{
						smallType = eUIHLSmall_GameTarget_ThreeCanGetReward;
					}
					break;
				case eDestinationStage_4:
					{
						smallType = eUIHLSmall_GameTarget_FourCanGetReward;
					}
					break;
				case eDestinationStage_5:
					{
						smallType = eUIHLSmall_GameTarget_FiveCanGetReward;
					}
					break;
				case eDestinationStage_6:
					{
						smallType = eUIHLSmall_GameTarget_SixCanGetReward;
					}
					break;
				case eDestinationStage_7:
					{
						smallType = eUIHLSmall_GameTarget_SevenCanGetReward;
					}
					break;
				case eDestinationStage_8:
					{
						smallType = eUIHLSmall_GameTarget_EightCanGetReward;
					}
					break;
				default:
					continue;
				}
				int count = 0;
				map<int, int>::const_iterator it = unGet.find(i);
				if (unGet.end() != it)
				{
					count = it->second;
				}

				AddUIHLData(eUIHLBig_GameTarget, smallType, count);
			}
		}
	}
	// 经验葫芦 1
	{
		int count = 0;
		int tempGroudExp = m_owner->getAttManage()->getValue<int>(eGourdExp);
		if (FriendManage::GetInstance()->getLevel() <= m_owner->getAttManage()->getValue<int>(eCharLv) && 0 < tempGroudExp)
		{
			if ((bool)tempGroudExp)
			{
				count += 1;
			}
		}
		AddUIHLData(eUIHLBig_ExpCalabash, eUIHLSmall_ExpCalabash_CanGetReward, count);
	}
	// 战姬
	{
		AddUIHLData(eUIHLBig_GirlWar, eUIHLSmall_GirlWar_GetNew, 0);
	}
	{ // 转盘 1
		int count = 0;
		HintValue value;
		Smart_Ptr<LucklyTurntableMgr>& LTMgr = m_owner->GetLucklyTableMgr();
		if ((bool)LTMgr)
		{
			const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();

			 count += LTMgr->LucklyTurntable_IsHaveFreeNum();
			 value.fValue = count;
			 value.sValue = proPtr != NULL ? proPtr->freeRoll : 0;

			 AddUIHLData(eUIHLBig_Turntable, eUIHLSmall_Turntable_FreeNumUnUse, value);
		}

	}
	{ // 钱庄 1
		int count = 0;
		HintValue value;
		if (m_owner->IsOpenSystem(eStBank))
		{
			count +=  (bool)m_owner->GetMyself() ? MoneyShopMgr::GetInstance()->MoneyShop_IsHaveFreeNum(m_owner->GetMyself()) : 0;
			value.fValue = count;
			value.sValue = MoneyShopMgr::GetInstance()->GetMoneyShopFreeMax();

			AddUIHLData(eUIHLBig_MoneyShop, eUIHLSmall_MoneyShop_FreeNumUnUse, value);
		}
	}
	{ // 神工坊 1
		int count = 0;
		count += (bool)m_owner->GetMyself() ? CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_owner->GetMyself()) : 0;
		AddUIHLData(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth, count);
	}
	{ // 帮派 1
		int count = 0;
		AddUIHLData(eUIHLBig_Faction, eUIHLSmall_Faction_HaveUnHandleJoin, count);
	}
	{ // 阵营 1
		int count = 0;
		if (m_owner->IsOpenSystem(eStCountry) &&
				m_owner->getAttManage()->getValue<DWORD>(eCharCountry) != 0 &&
				m_owner->getFitchFlag() <= 0)
		{
			const CampJobPrize* pJobinfo = GsCountryMgr::GetInstance()->getCampJobInfo(m_owner->getAttManage()->getValue<WORD>(eCountryJob));
			if (NULL != pJobinfo)
			{
				count += 1;
			}
		}
		AddUIHLData(eUIHLBig_Country, eUIHLSmall_Country_CountryJobReward, count);
		// 阵营红包和 红包奖励
		count = 0;
		AddUIHLData(eUIHLBig_Country, eUIHLSmall_Country_HaveUnGetReward, count);
		// 阵营商店有可购买的物品
		count = 0;
		if (m_owner->IsOpenSystem(eStCountry) &&
				m_owner->getAttManage()->getValue<DWORD>(eCharCountry) != 0 &&
				m_owner->getCampShopMgr())
		{
			count += m_owner->getCampShopMgr()->IsHaveCanBuy();
		}
		AddUIHLData(eUIHLBig_Country, eUIHLSmall_Country_CountryShopCanBuy, count);
	}
	{ // 角色1
		int count = 0;
		// 经脉
		count = 0;
		if(m_owner->IsOpenSystem(eStMeridians))
		{
			count +=  (bool)m_owner->GetMyself() ? MeridianMgr::GetInstance()->IsCanAdvance(m_owner->GetMyself()) : 0;
		}
		// 神兵
		count = 0;
		if (m_owner->IsOpenSystem(eStMagicWeapon))
		{
			count += int(m_owner->GetMagicWeapon()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_MagicCanAdvance, count);

			count = 0;
			count += int(m_owner->GetMagicWeapon()->IsCanSkillRefresh());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_MagicSkillCanRefresh, count);
		}

		// 法器进阶
		count = 0;
		if (m_owner->IsOpenSystem(eStSmagic))
		{
			count += int(m_owner->GetSMagic()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_SMagicCanAdvance, count);
		}

		//心法进阶
		count = 0;
		if (m_owner->IsOpenSystem(eStHeartMagic))
		{
			count += int(m_owner->GetHeartMagicWeapon()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_HeartMagicCanAdvance, count);
		}

		// 丹道
		count = 0;
		if (m_owner->IsOpenSystem(eStDanLu))
		{
			count += m_owner->GetDanLuManager()->IsCanAdvance();
		}
		AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanAdvance, count);

		count = 0;
		if (m_owner->IsOpenSystem(eStDanLu))
		{
			count += m_owner->GetDanLuManager()->IsCanSmelting();
		}
		AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanSmelting, count);
	}
	{ // 技能 1
		int count = 0;
		if (m_owner->IsOpenSystem(eStSkill) && m_owner->GetSkillMgr())
		{
			// 主动技能
			count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_ActiveSkill);
			count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_PassiveSkill);

//			AddUIHLData(eUIHLBig_Skill, eUIHLSmall_Skill_ActiveSkillCanUpgrade, count);
//			 被动技能
//			count = 0;

			AddUIHLData(eUIHLBig_Skill, eUIHLSmall_Skill_CanUpgrade, count);
		}

	}
	{ // 坐骑 1
		int count = 0;
		if (m_owner->IsOpenSystem(eStHorse) && m_owner->GetHorseManager())
		{
			count += int(m_owner->GetHorseManager()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_HorseCanUpgrade, count);
		}
		// 坐骑皮肤
		count = 0;
//		AddUIHLData(eUIHLBig_Horse, eUIHLSmall_Horse_GetNewSkin, count);
	}
	{ // 成就 1
		int count = 0;
		if (m_owner->IsOpenSystem(eStAch) && m_owner->GetAchievement())
		{
			count += m_owner->GetAchievement()->GetAllUnGetReward();
		}
		AddUIHLData(eUIHLBig_Achivement, eUIHLSmall_Acheivement_HaveUnGetReward, count);
	}
	{ // 锻造 1
		int count = 0;
		Smart_Ptr<ArticleEquipMgr> equipCntr = m_owner->GetContainer(eEquipQuickType);
		if ((bool)equipCntr)
		{
			AddUIHLData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_StrengthUnLvCeil, count);
			count = 0;
			AddUIHLData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_InlayUnLvCeil, count);
			count = 0;
			AddUIHLData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_RefineUnLvCeil, count);
			count = 0;
			AddUIHLData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_UpgradeUnLvCeil, count);
		}
	}
//	{ // 试练场 1
//		int count = 0, surplusNum = 0;
//
//		m_owner->GetInstanceMgr()->checkMoneyInstanceEnter(surplusNum);
//		count += surplusNum;
//		AddUIHLData(eUIHLBig_Practice, eUIHLSmall_Practice_HaveCanChallenge, count);
//	}
//	{ // 每日必做
//		int count = 0;
//		count += m_owner->GetDailyManager()->GetEveryDayMustWork();
//		AddUIHLData(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete, count);
//	}
	{ // 董墓Boss战
		int count = 0;
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_CountryBossBattle_Open, count);
	}
	{ // 董墓争夺战
		int count = 0;
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_CountryFightBattle_Open, count);
	}
	{ // 背包
		// 背包满了
		int count = 0;
		Smart_Ptr<ArticleParcelMgr> parcelMgr = m_owner->GetContainer(ePackageType);
		if((bool)parcelMgr)
		{
			count += parcelMgr->IsGirdFull();
		}
		AddUIHLData(eUIHLBig_Package, eUIHLSmall_Package_Full, count);
	}
	{ // 市场
		int count = 0;
		// 有出售出的物品
		AddUIHLData(eUIHLBig_Market, eUIHLSmall_Market_HaveSellItem, count);
	}
	{ // 挑战
		// 护送皇纲
		HintValue value;
		Smart_Ptr<EscortMgr>& escortMgr = m_owner->GetEscortMgr();
		if ((bool)escortMgr && m_owner->IsOpenSystem(eStEscort))
		{
			value.sValue = escortMgr->GetEscortTotalNum();
			value.fValue = escortMgr->GetEscortLeftTime();

		}
		AddUIHLData(eUIHLBig_Escort, eUIHLSmall_Challenge_EscortCanChallenge, value);

	}
	{ // 首次充值
		int count = 0;
		// 是否是首充
		if (!m_owner->GetSignIn()->IsGetFirstAward() &&
				1 <= m_owner->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
		{
			count += 1;
		}
		AddUIHLData(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge, count);
	}
	{ // 黄钻礼包
		// 有奖励时
		int count = 0;
		if (m_owner->GetActivityMgr())
		{
			count += m_owner->GetActivityMgr()->YNoviceRewardNotGet();
		}
		AddUIHLData(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_NoviceGiftBag, count);

		count = 0;
		if (m_owner->GetActivityMgr())
		{
			count += m_owner->GetActivityMgr()->YGrowthRewardNotGet();
		}
		AddUIHLData(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_GrowthGiftBag, count);

		count = 0;
		if (m_owner->GetActivityMgr())
		{
			count += m_owner->GetActivityMgr()->YDailyRewardNotGet();
		}
		AddUIHLData(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_DailyGiftBag, count);

		count = 0;
		if (m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eStGirlWar))
		{
			count += m_owner->GetActivityMgr()->YExclusiveGirlNotGet();
		}
		AddUIHLData(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_ExclusiveGirl, count);
	}

	{ // 活动
		int count = 0;
		// 答题
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_Activity_AnswerOpen, count);
		// 护送黄纲
		//判断是否为双倍经验时间
		count = 0;
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_Activity_DoubleEscortOpen, count);
		// 守护秦王
		count = 0;
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_Activity_DoubleGuardKingOpen, count);
		// 斗战重楼开启时
		count = 0;
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_Activity_BattlePVP, count);
		// 天降宝箱
		count = 0;
		count += ActivityBox::GetInstance()->IsActivityBoxBegin();
		AddUIHLData(eUIHLBig_Activity, eUIHLSmall_Activity_SkyDropBox, count);
	}

	{ // 乱世枭雄
		int count = 0;
		// 世界Boss
		AddUIHLData(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_WorldBoss, count);
		// 野外Boss
		count = 0;
		if (m_owner->IsOpenSystem(eStWildBoss))
		{
			count +=  (bool)m_owner->GetMyself() ? WorldBossMgr::GetInstance()->WorldBossMgr_IsHaveAliveBoss(m_owner->GetMyself()) : 0;
		}
		AddUIHLData(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_BossCanBattle, count);
	}
}

void CHighLightMgr::ComTypeAddData2()
{
//	{ // 每日签到
//		HintValue value;
//		Smart_Ptr<SignIn>& signIn  = m_owner->GetSignIn();
//		if (signIn && m_owner->IsOpenSystem(eSignLv))
//		{
//			value.fValue += !signIn->isTodayHaveSignin();
//			value.sValue = 1;
//		}
//		AddUIHLData(eBig_DaySign, eSmall_DaySign_Sign, value);
//	}

	{ // 日常任务
		HintValue value;
		Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
		if (questMgr && m_owner->IsOpenSystem(eStDailyQuest) && questMgr->getDailyQuests())
		{
			value.fValue = questMgr->getDailyQuests()->GetUnCompleteNum();
			value.sValue = questMgr->getDailyQuests()->getMaxTimes();
		}
		AddUIHLData(eBig_DailyTask, eSmall_DailyTask_AlreadyFini, value);
	}
	{ // 帮派任务
		HintValue value;
		Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
		if (questMgr && m_owner->IsOpenSystem(eStClanTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharFaction) > 0
				&& questMgr->getClanQuests())
		{
			value.fValue = questMgr->getClanQuests()->GetUnCompleteNum();
			value.sValue = questMgr->getClanQuests()->getMaxTimes();
		}
		AddUIHLData(eBig_ClanTask, eSmall_ClanTask_AlreadyFini, value);
	}
	{
		// 阵营任务
		HintValue value;
		Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
		if (questMgr && m_owner->IsOpenSystem(eStCntyTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharCountry) > 0
				&& questMgr->getCampQuests())
		{
			value.fValue = questMgr->getCampQuests()->GetUnCompleteNum();
			value.sValue = questMgr->getCampQuests()->getMaxTimes();
		}
		AddUIHLData(eBig_CntyTask, eSmall_CntyTask_AlreadyFini, value);
	}
	{ // 装备副本
		HintValue value;
		if (m_owner->GetInstanceMgr())
		{
			value.sValue = m_owner->GetInstanceMgr()->GetSingleInstUnChall(value.fValue);
		}

		AddUIHLData(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini, value);
	}
	// 进阶副本
	{
		HintValue value = 0;
		Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
		if ((bool)instanceMgr)
		{
			value.sValue = instanceMgr->GetAdvancedInstUnChall(value.fValue);
		}

		AddUIHLData(eBig_Advanced, eSmall_Advanced_AlreadyFini, value);
	}
	{ // 挑战副本
		HintValue value;
		if (m_owner->GetInstanceMgr())
		{
			value.sValue = m_owner->GetInstanceMgr()->GetChallInstUnChall(value.fValue);
		}
		AddUIHLData(eBig_ChallengeInst, eSmall_ChallengeInst_AlreadyFini, value);
	}
	{ // 守护秦王
		HintValue value;
//		if ((bool)m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eProtectKingLv))
		{
			value.fValue = m_owner->GetActivityMgr()->GetPKUnResetNum();
			value.sValue = m_owner->GetActivityMgr()->GetTotalResetPK();

		}
		AddUIHLData(eBig_ProtectKing, eSmall_ProtectKing_AlreadyFini, value);

	}
	{ // 剿灭反王
		HintValue value;
//		if ((bool)m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eTreasonKingLv))
		{
			value.sValue = m_owner->GetActivityMgr()->GetTreasonSurplusResetCnt(value.fValue);

		}
		AddUIHLData(eBig_TreasonKing, eSmall_TreasonKing_AlreadyFini, value);
	}
	{ // 铜钱副本
		HintValue value;
		if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eStMoneyInstOpenLv))
		{
			int maxCount = 0, maxBuyCount = 0, leftCount = 0, buyCount = 0;
			m_owner->GetInstanceMgr()->getMoneyInstanceCount(maxCount, maxBuyCount, leftCount, buyCount);
			value.fValue = leftCount; // 剩余挑战次数
			value.sValue = maxCount + buyCount; // 总次数
		}
		AddUIHLData(eBig_MoneyInst, eSmall_MoneyInst_AlreadyFini, value);

	}
	{ // 竞技场

	}
	{ // 答题活动
		HintValue value;
		if (m_owner->GetCounterService() && m_owner->IsOpenSystem(eStAnswer))
		{
			int count = InterfaceAnswerLoader::GetInstance()->getOtherData().m_dayAnswerCnt;
			value.fValue = count - m_owner->GetCounterService()->GetNumById(ANSWER_ENTER_COUNT);
			value.sValue = count;
		}
		AddUIHLData(eUIHLBig_Answer, eSmall_Answer_AnswerCnt, value);
	}
//	{ // 精英副本
//		HintValue value;
//		if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eEliteInstLv))
//		{
//			value.sValue = m_owner->GetInstanceMgr()->GetEliteInstUnChall(value.fValue);
//			AddUIHLData(eBig_EliteInst, eSmall_EliteInst_AlreadyFini, value);
//		}
//	}
//	{ // 噩梦副本
//		HintValue value;
//		if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eCauchemarInstLv))
//		{
//			value.sValue = m_owner->GetInstanceMgr()->GetCauchemarInstUnChall(value.fValue);
//			AddUIHLData(eBig_CauchemarInst, eSmall_CauchemarInst_AlreadyFini, value);
//		}
//	}
}

void CHighLightMgr::ComTypeResetData()
{
	ComTypeResetData1();
	ComTypeResetData2();
}
void CHighLightMgr::ComTypeResetData1()
{
	{ // 竞技场 1
		// 挑战次数未完成时,剩余挑战次数
		int count = 0;
		HintValue value;
		if (m_owner->GetInstanceMgr() &&m_owner->IsOpenSystem(eStCompeteInstance))
		{
//			value.sValue = m_owner->GetInstanceMgr()->GetArenaUnChallenge(value.fValue);

			value.fValue = m_owner->GetArenaMgr()->GetCanChallCnt();
			value.sValue = m_owner->GetArenaMgr()->GetBaseSumChallCnt();

			AddUIHLData(eUIHLBig_Arena, eUIHLSmall_Arena_ChallengeNum, value);
		}

		// 排名礼包未领取时
		int ranking = m_owner->getAttManage()->getValue<int>(eAwardRanking);
		count = 0;
		if (0 < ranking)
			count = 1;
		else
			count = 0;
		AddUIHLData(eUIHLBig_Arena, eUIHLSmall_Arena_AwardUnGet, count);
	}
	{ //  跨服竞技 1
		int count = 0, challNumCeil = 0;
		Smart_Ptr<CounterService>& counterMgr = m_owner->GetCounterService();

//		if ((bool)counterMgr)
//		{
//			challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit() + CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit();
//			count = challNumCeil - counterMgr->GetNumById(BEYOND_BATTLE);
//		}
//		AddUIHLData(eUIHLBig_BeyondBattle, eUIHLSmall_BeyondBattle_ChallengeNum, count);

		// 单人跨服竞技
		HintValue value;
		if (m_owner->GetInstanceMgr() &&m_owner->IsOpenSystem(eStBeyondSvrBattle))
		{
			// 未完成/总数
			challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit();
			count = challNumCeil - counterMgr->GetNumById(BEYOND_SINGLE_BATTLE);
			value.fValue = count;
			value.sValue = challNumCeil;
			AddUIHLData(eUIHLBig_BeyondBattle, eSmall_SingleBeyondArena_AlreadyChallenge, value);
		}


		// 多人跨服竞技
		if (m_owner->GetInstanceMgr() &&m_owner->IsOpenSystem(eStBeyondSvrBattle))
		{
			// 未完成/总数
			challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit();
			count = challNumCeil - counterMgr->GetNumById(BEYOND_GROUP_BATTLE);
			value.fValue = count;
			value.sValue = challNumCeil;
			AddUIHLData(eUIHLBig_BeyondBattle, eSmall_GroupBeyondArena_AlreadyChallenge, value);
		}
	}
	{ // 副本大厅 1
//		int count = 0;
//		Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
//		if ((bool)instanceMgr)
//		{
//			count += instanceMgr->GetAllInstSurplusNum();
//		}
//		AddUIHLData(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_CanChallenge, count);
		// 守护秦王 1
//		count = 0;
//		if ((bool)m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eProtectKingLv))
//		{
//			count += m_owner->GetActivityMgr()->GetPKSuplusNode();
//		}
//		AddUIHLData(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_GuardKingCanChallenge, count);

		// 剿灭反王
//		count = 0;
//		if ((bool)m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eTreasonKingLv))
//		{
//			count += m_owner->GetActivityMgr()->GetTreansonSuplusPass();
//		}
//		AddUIHLData(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_KillTreasonKingCanChallenge, count);
	}
	{ // 活跃度 1
		int count = 0;
		count += m_owner->GetVitalityMgr()->GetUnGetAwardCnt();
		AddUIHLData(eUIHLBig_Vitality, eUIHLSmall_Vitelity_UnGetReward, count);
	}
	{
		// 奖励大厅 福利大厅 1
		int count = 0;

		Smart_Ptr<SignIn>& signIn  = m_owner->GetSignIn();
		if ((bool)signIn)
		{
			// 签到
			HintValue value;
			Smart_Ptr<SignIn>& signIn  = m_owner->GetSignIn();
			if (signIn && m_owner->IsOpenSystem(eStSignLv))
			{
				value.fValue += !signIn->isTodayHaveSignin();
				value.sValue = 1;
			}
			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanSignIn, value);

			count = 0;
			if (signIn && m_owner->IsOpenSystem(eStSignLv))
			{
				count += signIn->GetMendingSignCnt();
				AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanMendingSignIn, count);
			}


			count = 0;
			if (signIn && m_owner->IsOpenSystem(eStSignLv))
			{
				count += signIn->CheckSignInReward();
				AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_SignInTotalAward, count);
			}

			// 七日礼包
			count = 0;
			if (m_owner->IsOpenSystem(eStSevenRewardLv))
			{
				count += signIn->GetTLCanGetAward();
			}

			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_ServenLoginAward, count);

		}
		// 在线
		count = 0;
		Smart_Ptr<OnlineReward>& onLineMgr = m_owner->getOnlineReward();
		if (m_owner->IsOpenSystem(eStOnlineRewardLv) && (bool)onLineMgr)
		{
			count += onLineMgr->GetOLCanGetAward();
		}
		AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOnlineAward, count);

		// 等级
		count = 0;
		Smart_Ptr<LevelReward>& levelMgr = m_owner->GetLevelReward();
		if (m_owner->IsOpenSystem(eStLevelRewardLv) && (bool)levelMgr)
		{
			count += levelMgr->GetLRCanGetAward();
		}
		AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetLevelAward, count);

		Smart_Ptr<DailyManager> dailyMgr = m_owner->GetDailyManager();
		if((bool)dailyMgr)
		{
			// 资源追回
			count = 0;
			if (m_owner->IsOpenSystem(eStResourceRecoverLv))
			{
				count += dailyMgr->GetRecoverResourceCount();
			}
			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanResFound, count);

			// 离线经验
			count = 0;
			if (m_owner->IsOpenSystem(eOfflineRewardLv))
			{
				count += dailyMgr->IsHaveDailyPracticeToGet();
			}
			AddUIHLData(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOffAward, count);
		}
	}
	// 游戏目标 1
	{
		Smart_Ptr<Destination> dest = m_owner->GetDestination();
		if ((bool)dest && m_owner->IsOpenSystem(eStGameObject))
		{
			map<int, int> unGet;
			dest->GetPerStageCanGetAward(unGet);
			for (int i = eDestinationStage_1; i < eDestinationStage_Max; ++i)
			{
				E_UIHL_SMALLTYPE smallType;
				switch (i)
				{
				case eDestinationStage_1:
					{
						smallType = eUIHLSmall_GameTarget_OneCanGetReward;
					}
					break;
				case eDestinationStage_2:
					{
						smallType = eUIHLSmall_GameTarget_TwoCanGetReward;
					}
					break;
				case eDestinationStage_3:
					{
						smallType = eUIHLSmall_GameTarget_ThreeCanGetReward;
					}
					break;
				case eDestinationStage_4:
					{
						smallType = eUIHLSmall_GameTarget_FourCanGetReward;
					}
					break;
				case eDestinationStage_5:
					{
						smallType = eUIHLSmall_GameTarget_FiveCanGetReward;
					}
					break;
				case eDestinationStage_6:
					{
						smallType = eUIHLSmall_GameTarget_SixCanGetReward;
					}
					break;
				case eDestinationStage_7:
					{
						smallType = eUIHLSmall_GameTarget_SevenCanGetReward;
					}
					break;
				case eDestinationStage_8:
					{
						smallType = eUIHLSmall_GameTarget_EightCanGetReward;
					}
					break;
				default:
					continue;
				}
				int count = 0;
				map<int, int>::const_iterator it = unGet.find(i);
				if (unGet.end() != it)
				{
					count = it->second;
				}

				AddUIHLData(eUIHLBig_GameTarget, smallType, count);
			}
		}
	}
	// 经验葫芦 1
	{
		int count = 0;
		int tempGroudExp = m_owner->getAttManage()->getValue<int>(eGourdExp);
		if (FriendManage::GetInstance()->getLevel() <= m_owner->getAttManage()->getValue<int>(eCharLv) && 0 < tempGroudExp)
		{
			if ((bool)tempGroudExp)
			{
				count += 1;
			}
		}
		AddUIHLData(eUIHLBig_ExpCalabash, eUIHLSmall_ExpCalabash_CanGetReward, count);
	}
	{ // 转盘 1
		int count = 0;
		HintValue value;
		Smart_Ptr<LucklyTurntableMgr>& LTMgr = m_owner->GetLucklyTableMgr();
		if ((bool)LTMgr && m_owner->IsOpenSystem(eStLucklyTurntable))
		{
			const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();

			 count += LTMgr->LucklyTurntable_IsHaveFreeNum();
			 value.fValue = count;
			 value.sValue = proPtr != NULL ? proPtr->freeRoll : 0;
			AddUIHLData(eUIHLBig_Turntable, eUIHLSmall_Turntable_FreeNumUnUse, value);

		}

//
//		AddUIHLData(eBig_Turntable, eSmall_Turntable_Turn, value);

	}
	{ // 钱庄 1
		int count = 0;
		HintValue value;
		if (m_owner->IsOpenSystem(eStBank))
		{
			count +=  (bool)m_owner->GetMyself() ? MoneyShopMgr::GetInstance()->MoneyShop_IsHaveFreeNum(m_owner->GetMyself()) : 0;
			value.fValue = count;
			value.sValue = MoneyShopMgr::GetInstance()->GetMoneyShopFreeMax();

			AddUIHLData(eUIHLBig_MoneyShop, eUIHLSmall_MoneyShop_FreeNumUnUse, value);
		}
	}
	{ // 阵营 1
		int count = 0;
		if (m_owner->getFitchFlag() <= 0)
		{
			const CampJobPrize* pJobinfo = GsCountryMgr::GetInstance()->getCampJobInfo(m_owner->getAttManage()->getValue<WORD>(eCountryJob));
			if (NULL != pJobinfo)
			{
				count += 1;
			}
		}
//		count += m_owner->getEnvelopeMgr()->GetCurrentCanGetEnvelopeCount();

		AddUIHLData(eUIHLBig_Country, eUIHLSmall_Country_CountryJobReward, count);
	}
	{ // 角色1
		int count = 0;
		// 经脉
		count = 0;
		if(m_owner->IsOpenSystem(eStMeridians))
		{
			count +=  (bool)m_owner->GetMyself() ? MeridianMgr::GetInstance()->IsCanAdvance(m_owner->GetMyself()) : 0;
		}
		AddUIHLData(eUIHLBig_Meridian, eUIHLSmall_Meridian_CanAdvance, count);
		// 神兵
		count = 0;
		if (m_owner->IsOpenSystem(eStMagicWeapon))
		{
			count += int(m_owner->GetMagicWeapon()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_MagicCanAdvance, count);

			count = 0;
			count += int(m_owner->GetMagicWeapon()->IsCanSkillRefresh());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_MagicSkillCanRefresh, count);
		}

		count = 0;
		if (m_owner->IsOpenSystem(eStSmagic))
		{
			count += int(m_owner->GetSMagic()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_SMagicCanAdvance, count);
		}

		count = 0;
		if (m_owner->IsOpenSystem(eStHeartMagic))
		{
			count += int(m_owner->GetHeartMagicWeapon()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_HeartMagicCanAdvance, count);
		}

		// 丹道
		count = 0;
		if (m_owner->IsOpenSystem(eStDanLu))
		{
			count += m_owner->GetDanLuManager()->IsCanAdvance();
		}
		AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanAdvance, count);

		count = 0;
		if (m_owner->IsOpenSystem(eStDanLu))
		{
			count += m_owner->GetDanLuManager()->IsCanSmelting();
		}
		AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanSmelting, count);

	}
	{ // 技能 1
		int count = 0;
		if (m_owner->IsOpenSystem(eStSkill) && m_owner->GetSkillMgr())
		{
			// 主动技能
			count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_ActiveSkill);
			count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_PassiveSkill);

//			AddUIHLData(eUIHLBig_Skill, eUIHLSmall_Skill_ActiveSkillCanUpgrade, count);
//			 被动技能
//			count = 0;

			AddUIHLData(eUIHLBig_Skill, eUIHLSmall_Skill_CanUpgrade, count);
		}
	}
	{ // 坐骑 1
		int count = 0;
		if (m_owner->IsOpenSystem(eStHorse) && m_owner->GetHorseManager())
		{
			count += int(m_owner->GetHorseManager()->IsCanAdvance());
			AddUIHLData(eUIHLBig_Role, eUIHLSmall_Role_HorseCanUpgrade, count);
		}
	}
//	{ // 试练场 1
//		int count = 0, surplusNum = 0;
//
//		m_owner->GetInstanceMgr()->checkMoneyInstanceEnter(surplusNum);
//		count += surplusNum;
//		AddUIHLData(eUIHLBig_Practice, eUIHLSmall_Practice_HaveCanChallenge, count);
//	}
	{ // 乱世枭雄
		int count = 0;
		// 野外Boss
		count = 0;
		if (m_owner->IsOpenSystem(eStWildBoss))
		{
			count +=  (bool)m_owner->GetMyself() ? WorldBossMgr::GetInstance()->WorldBossMgr_IsHaveAliveBoss(m_owner->GetMyself()) : 0;
		}
		AddUIHLData(eUIHLBig_TroubledTimes, eUIHLSmall_TroubledTimes_BossCanBattle, count);
	}
	{ // 黄钻礼包
		// 有奖励时
		int count = 0;
		if (m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eStGirlWar))
		{
			count += m_owner->GetActivityMgr()->YExclusiveGirlNotGet();
		}
		AddUIHLData(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_ExclusiveGirl, count);
	}

	{ // 成就
		int count = 0;
		if (m_owner->IsOpenSystem(eStAch) && m_owner->GetAchievement())
		{
			count += m_owner->GetAchievement()->GetAllUnGetReward();
		}
		AddUIHLData(eUIHLBig_Achivement, eUIHLSmall_Acheivement_HaveUnGetReward, count);
	}

	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}
}
void CHighLightMgr::ComTypeResetData2()
{
//	{ // 每日签到
//		HintValue value;
//		Smart_Ptr<SignIn>& signIn  = m_owner->GetSignIn();
//		if (signIn && m_owner->IsOpenSystem(eSignLv))
//		{
//			value.fValue += !signIn->isTodayHaveSignin();
//			value.sValue = 1;
//		}
//		AddUIHLData(eBig_DaySign, eSmall_DaySign_Sign, value);
//	}

	{ // 日常任务
		HintValue value;
		Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
		if (questMgr && m_owner->IsOpenSystem(eStDailyQuest) && questMgr->getDailyQuests())
		{
			value.fValue = questMgr->getDailyQuests()->GetUnCompleteNum();
			value.sValue = questMgr->getDailyQuests()->getMaxTimes();
		}
		AddUIHLData(eBig_DailyTask, eSmall_DailyTask_AlreadyFini, value);
	}
	{ // 帮派任务
		HintValue value;
		Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
		if (questMgr && m_owner->IsOpenSystem(eStClanTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharFaction) > 0
				&& questMgr->getClanQuests())
		{

			value.fValue = questMgr->getClanQuests()->GetUnCompleteNum();
			value.sValue = questMgr->getClanQuests()->getMaxTimes();
		}
		AddUIHLData(eBig_ClanTask, eSmall_ClanTask_AlreadyFini, value);
	}
	{
		// 阵营任务
		HintValue value;
		Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
		if (questMgr && m_owner->IsOpenSystem(eStCntyTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharCountry) > 0
				&& questMgr->getCampQuests())
		{
			value.fValue = questMgr->getCampQuests()->GetUnCompleteNum();
			value.sValue = questMgr->getCampQuests()->getMaxTimes();
		}
		AddUIHLData(eBig_CntyTask, eSmall_CntyTask_AlreadyFini, value);
	}
	{ // 装备副本
		HintValue value;
		if (m_owner->GetInstanceMgr())
		{
			value.sValue = m_owner->GetInstanceMgr()->GetSingleInstUnChall(value.fValue);
		}
		AddUIHLData(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini, value);
	}
	// 进阶副本
	{
		HintValue value = 0;
		Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
		if ((bool)instanceMgr)
		{
			value.sValue = instanceMgr->GetAdvancedInstUnChall(value.fValue);
		}

		AddUIHLData(eBig_Advanced, eSmall_Advanced_AlreadyFini, value);
	}
	{ // 挑战副本
		HintValue value;
		if (m_owner->GetInstanceMgr())
		{
			value.sValue = m_owner->GetInstanceMgr()->GetChallInstUnChall(value.fValue);
		}
		AddUIHLData(eBig_ChallengeInst, eSmall_ChallengeInst_AlreadyFini, value);
	}
	{ // 守护秦王
		HintValue value;
//		if ((bool)m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eProtectKingLv))
		{
			value.fValue = m_owner->GetActivityMgr()->GetPKUnResetNum();
			value.sValue = m_owner->GetActivityMgr()->GetTotalResetPK();

		}
		AddUIHLData(eBig_ProtectKing, eSmall_ProtectKing_AlreadyFini, value);

	}
	{ // 剿灭反王
		HintValue value;
//		if ((bool)m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eTreasonKingLv))
		{
			value.sValue = m_owner->GetActivityMgr()->GetTreasonSurplusResetCnt(value.fValue);

		}
		AddUIHLData(eBig_TreasonKing, eSmall_TreasonKing_AlreadyFini, value);
	}
	{ // 铜钱副本
		HintValue value;
		if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eStMoneyInstOpenLv))
		{
			int maxCount = 0, maxBuyCount = 0, leftCount = 0, buyCount = 0;
			m_owner->GetInstanceMgr()->getMoneyInstanceCount(maxCount, maxBuyCount, leftCount, buyCount);
			value.fValue = leftCount; // 剩余挑战次数
			value.sValue = maxCount + buyCount; // 总次数
		}
		AddUIHLData(eBig_MoneyInst, eSmall_MoneyInst_AlreadyFini, value);

	}
	{ // 答题活动
		HintValue value;
		if (m_owner->GetCounterService() && m_owner->IsOpenSystem(eStAnswer))
		{
			int count = InterfaceAnswerLoader::GetInstance()->getOtherData().m_dayAnswerCnt;
			value.fValue = count - m_owner->GetCounterService()->GetNumById(ANSWER_ENTER_COUNT);
			value.sValue = count;
		}
		AddUIHLData(eUIHLBig_Answer, eSmall_Answer_AnswerCnt, value);
	}
	// 护送皇纲
	HintValue value;
	Smart_Ptr<EscortMgr>& escortMgr = m_owner->GetEscortMgr();
	if ((bool)escortMgr && m_owner->IsOpenSystem(eStEscort))
	{
		value.sValue = escortMgr->GetEscortTotalNum();
		value.fValue = escortMgr->GetEscortLeftTime();

	}
	AddUIHLData(eUIHLBig_Escort, eUIHLSmall_Challenge_EscortCanChallenge, value);

//	{ // 精英副本
//		HintValue value;
//		if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eEliteInstLv))
//		{
//			value.sValue = m_owner->GetInstanceMgr()->GetEliteInstUnChall(value.fValue);
//			AddUIHLData(eBig_EliteInst, eSmall_EliteInst_AlreadyFini, value);
//		}
//	}
//	{ // 噩梦副本
//		HintValue value;
//		if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eCauchemarInstLv))
//		{
//			value.sValue = m_owner->GetInstanceMgr()->GetCauchemarInstUnChall(value.fValue);
//			AddUIHLData(eBig_CauchemarInst, eSmall_CauchemarInst_AlreadyFini, value);
//		}
//	}
}


void CHighLightMgr::AddUIHLData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value)
{
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);
		if (smallCntr.end() != itSmall)
		{
			itSmall->second = value;
		}
		else
		{
			m_data[bigType][smallType] = value;
		}
	}
	else
	{
		m_data[bigType][smallType] = value;
	}
}

void CHighLightMgr::AddUIHLData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int value)
{
	UIHLContainer::iterator itBig = m_data.find(bigType);
	HintValue __value;
	__value.fValue = value;

	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);
		if (smallCntr.end() != itSmall)
		{
			itSmall->second = __value;
		}
		else
		{
			m_data[bigType][smallType] = __value;
		}
	}
	else
	{
		m_data[bigType][smallType] = __value;
	}
}

void CHighLightMgr::ClearUIHLData()
{
	m_data.clear();
}

void CHighLightMgr::DeleteUIHLData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType)
{
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);
		if (smallCntr.end() != itSmall)
		{
			smallCntr.erase(itSmall);
		}
	}
}

void CHighLightMgr::DeleteUIHLData(E_UIHL_BIGTYPE bigType)
{
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		m_data.erase(itBig);
	}
}

int CHighLightMgr::SynchUIHighLight()
{
	SystemNoticeProto::ToClientUIHL clientUIHL;
	SystemNoticeProto::HintClientTaskPanel clientTaskPanel;
	int ret = -1;
	vector<int> vec;

	UIHLContainer::iterator itBig= m_data.begin();
	for ( ; m_data.end() != itBig; ++itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.begin();

		SystemNoticeProto::UIHL UIHL;
		for ( ; smallCntr.end() != itSmall; ++itSmall)
		{
			vector<E_HINT_SHOW> show = HintLoader::GetInstance()->ShowWhat(UIHLType(itBig->first, itSmall->first));
			for (uint i = 0; i < show.size(); ++i)
			{
				switch (show[i])
				{
				case eHintShow_UIHL:
					{
						UIHL.add_smalltype(itSmall->first);
						UIHL.add_value(itSmall->second.fValue);
					}
					break;
				case eHintShow_TaskPanel:
					{
						clientTaskPanel.add_bigtype(itBig->first);
						clientTaskPanel.add_smalltype(itSmall->first);
						clientTaskPanel.add_fvalue(itSmall->second.fValue);
						clientTaskPanel.add_svalue(itSmall->second.sValue);
					}
					break;
				default:
					{

					}
				}
			}
		}
		if (0 < UIHL.smalltype_size())
		{
			UIHL.set_bigtype(itBig->first);
			clientUIHL.add_list()->CopyFrom(UIHL);
		}
	}

	if (0 < clientUIHL.list_size())
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &clientUIHL, MSG_SIM_NOTICE_UIHIGHLIGHT);
		ret = 0;
	}
	if (0 < clientTaskPanel.bigtype_size())
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &clientTaskPanel, MSG_SIM_NOTICE_TASKPANEL);
		ret = 0;
	}

	return ret;
}

int CHighLightMgr::SynchUIHighLight(E_UIHL_BIGTYPE bigType)
{
	int ret = -1;
	SystemNoticeProto::ToClientUIHL clientUIHL;
	SystemNoticeProto::HintClientTaskPanel clientTaskPanel;

	UIHLContainer::iterator itBig = m_data.find(bigType);
//	bool flag = false;
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.begin();
//		bool isHaveValue = false;

		SystemNoticeProto::UIHL UIHL;
		for ( ; smallCntr.end() != itSmall; ++itSmall)
		{
			vector<E_HINT_SHOW> show = HintLoader::GetInstance()->ShowWhat(UIHLType(itBig->first, itSmall->first));
			for (uint i = 0; i < show.size(); ++i)
			{
				switch (show[i])
				{
				case eHintShow_UIHL:
					{
						UIHL.add_smalltype(itSmall->first);
						UIHL.add_value(itSmall->second.fValue);
					}
					break;
				case eHintShow_TaskPanel:
					{
						clientTaskPanel.add_bigtype(itBig->first);
						clientTaskPanel.add_smalltype(itSmall->first);
						clientTaskPanel.add_fvalue(itSmall->second.fValue);
						clientTaskPanel.add_svalue(itSmall->second.sValue);
					}
					break;
				default:
					{

					}
					break;
				}
			}

//			UIHL.add_smalltype(itSmall->first);
//			UIHL.add_value(itSmall->second.fValue);
//			isHaveValue = true;
		}
		if (0 < UIHL.smalltype_size())
		{
			UIHL.set_bigtype(itBig->first);
			clientUIHL.add_list()->CopyFrom(UIHL);
//			flag =true;
		}
	}

	if (0 < clientUIHL.list_size())
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &clientUIHL, MSG_SIM_NOTICE_UIHIGHLIGHT);
		ret = 0;
	}
	if (0 < clientTaskPanel.bigtype_size())
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &clientTaskPanel, MSG_SIM_NOTICE_TASKPANEL);
		ret = 0;
	}

	return ret;
}


int CHighLightMgr::SynchUIHighLight(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType)
{
	int ret = -1;
	SystemNoticeProto::ToClientUIHL clientUIHL;
	SystemNoticeProto::HintClientTaskPanel clientTaskPanel;

	UIHLContainer::iterator itBig = m_data.find(bigType);
	bool flag = false;
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);

		SystemNoticeProto::UIHL UIHL;
		if (smallCntr.end() != itSmall)
		{
			vector<E_HINT_SHOW> show = HintLoader::GetInstance()->ShowWhat(UIHLType(itBig->first, itSmall->first));
			for (uint i = 0; i < show.size(); ++i)
			{
				switch (show[i])
				{
				case eHintShow_UIHL:
					{
						UIHL.add_smalltype(itSmall->first);
						UIHL.add_value(itSmall->second.fValue);
					}
					break;
				case eHintShow_TaskPanel:
					{
						clientTaskPanel.add_bigtype(itBig->first);
						clientTaskPanel.add_smalltype(itSmall->first);
						clientTaskPanel.add_fvalue(itSmall->second.fValue);
						clientTaskPanel.add_svalue(itSmall->second.sValue);
					}
					break;
				default:
					{

					}
					break;

				}
			}

//			UIHL.add_smalltype(itSmall->first);
//			UIHL.add_value(itSmall->second.);
//			isHaveValue = true;
		}

		if (0 < UIHL.smalltype_size())
		{
			UIHL.set_bigtype(itBig->first);
			clientUIHL.add_list()->CopyFrom(UIHL);
			flag = true;
		}
	}

	if (0 < clientUIHL.list_size())
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &clientUIHL, MSG_SIM_NOTICE_UIHIGHLIGHT);
		ret = 0;
	}
	if (0 < clientTaskPanel.bigtype_size())
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &clientTaskPanel, MSG_SIM_NOTICE_TASKPANEL);
		ret = 0;
	}


	return ret;
}

int CHighLightMgr::UpdateDataAndSyn(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value, bool flag)
{
	int ret = -1;
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);

		if (smallCntr.end() != itSmall)
		{
			bool isSyn = false;
			if (flag)
			{
				if (itSmall->second == 0)
				{
					isSyn = true;
				}
				itSmall->second += value;
			}
			else
			{
				itSmall->second -= value;
				if (itSmall->second < 0)
				{
					itSmall->second = 0;
				}
				if (itSmall->second == 0)
				{
					isSyn = true;
				}
			}

			if (isSyn)
			{
				SynchUIHighLight(bigType);
			}

			ret = 0;
		}

	}

	return ret;
}

int CHighLightMgr::ModifyData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value)
{
	int ret = -1;
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);

		if (smallCntr.end() != itSmall)
		{
			if (itSmall->second != value)
			{
				if (0 > value.fValue)
				{
					value.fValue = 0;
				}
				itSmall->second = value;

				ret = 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	return ret;
}

int CHighLightMgr::ModifyData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int value)
{
	int ret = -1;
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);

		if (smallCntr.end() != itSmall)
		{
			if (itSmall->second != value)
			{
				itSmall->second = value;
				ret = 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	return ret;
}

int CHighLightMgr::UpdateData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value, bool flag)
{
	int ret = -1;
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue>& smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);
		if (flag)
		{
			itSmall->second += value;
		}
		else
		{
			itSmall->second -= value;
			if (itSmall->second < 0)
			{
				itSmall->second = 0;
			}
		}
		ret = 0;
	}

	return ret;
}

void CHighLightMgr::GetValueByType(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue& value)
{
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue> smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);
		if (smallCntr.end() != itSmall)
		{
			value = itSmall->second;
		}
	}
}

void CHighLightMgr::GetValueByType(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int& value)
{
	UIHLContainer::iterator itBig = m_data.find(bigType);
	if (m_data.end() != itBig)
	{
		map<E_UIHL_SMALLTYPE, HintValue> smallCntr = itBig->second;
		map<E_UIHL_SMALLTYPE, HintValue>::iterator itSmall = smallCntr.find(smallType);
		if (smallCntr.end() != itSmall)
		{
			value = itSmall->second.fValue;
		}
	}
}

void CHighLightMgr::ActivityHLTypeAdd()
{
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_ChargeReturn] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_RedList] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_DailyCharge] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_SingleCharge] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_AccumulateCharge] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_FirstCharge] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_MagicUpgrade] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_AcitivityHall_HorseUpgrade] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_RelayCharge] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_InvestmentActivity_LvInvestment] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_InvestmentActivity_SevenDayRebate] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_AccumulateConsume] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_Compensation] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_FullService] = 0;
	m_data[eUIHLBig_ActivityHall][eUIHLSmall_ActivityHall_AccDayRecharge] = 0;

//	m_data[eUIHLBig_FirstChargeRet][eUIHLSmall_ActivityHall_ChargeReturn] = 0;
//	m_data[eUIHLBig_DailyCharge][eUIHLSmall_ActivityHall_FirstCharge] = 0;
//	m_data[eUIHLBig_SingleCharge][eUIHLSmall_ActivityHall_SingleCharge] = 0;
//	m_data[eUIHLBig_AccumulateCharge][eUIHLSmall_ActivityHall_AccumulateCharge] = 0;
//
//	m_data[eUIHLBig_RelayCharge][eUIHLSmall_ActivityHall_RelayCharge] = 0;
//	m_data[eUIHLBig_AccumulateConsume][eUIHLSmall_ActivityHall_AccumulateConsume] = 0;


}

bool CHighLightMgr::HandleDataEvent(const EventArgs& e)
{
	UpdateUIHLDataArgs* args = (UpdateUIHLDataArgs*)&e;
	if (NULL == args)
	{
		return true;
	}

	Smart_Ptr<Player> player = args->m_obj;

	if((bool)player && player->GetID() == m_owner->GetID())
	{
		for (uint i = 0; i < args->m_content.size(); ++i)
		{
			UpdateUIHLDataArgs::Content& data = args->m_content[i];

			HandleData(data.handle, data.bigType, data.smallType, data.fValue, data.flag, data.sValue);

		}
	}

	return true;
}

bool CHighLightMgr::HandlePlayerVipUp(const EventArgs& e)
{
	ComTypeResetData();
	SynchUIHighLight();
	return true;
}

bool CHighLightMgr::HandleMoneyChange(const EventArgs& e)
{
	ChangeComCall();

	bool isSyn = false;
	int count = 0;
	if (m_owner->IsOpenSystem(eStMeridians))
	{
		// 角色 经脉升阶
		isSyn = false;
		count = 0;
		count +=  (bool)m_owner->GetMyself() ? MeridianMgr::GetInstance()->IsCanAdvance(m_owner->GetMyself()) : 0;
		if (0 == ModifyData(eUIHLBig_Meridian, eUIHLSmall_Meridian_CanAdvance, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Meridian);
		}
	}


	return true;
}

void CHighLightMgr::ChangeComCall()
{

	bool isSyn = false;
	int count = 0;
	if (m_owner->IsOpenSystem(eStSkill) && m_owner->GetSkillMgr())
	{
		// 主动技能
		isSyn = false;
		count = 0;
		count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_ActiveSkill);
		count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_PassiveSkill);
		if (0 == ModifyData(eUIHLBig_Skill, eUIHLSmall_Skill_CanUpgrade, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Skill);
		}
	}

	if (m_owner->IsOpenSystem(eStHorse) && m_owner->GetHorseManager())
	{
		// 坐骑升阶
		isSyn = false;
		count = 0;
		count += m_owner->GetHorseManager()->IsCanAdvance();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_HorseCanUpgrade, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Role);
		}
	}

	if (m_owner->IsOpenSystem(eStMagicWeapon) && m_owner->GetMagicWeapon())
	{
		// 角色 神兵升阶
		isSyn = false;
		count = 0;
		count += m_owner->GetMagicWeapon()->IsCanAdvance();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_MagicCanAdvance, count) && !isSyn)
		{
			isSyn = true;
		}

		count = 0;
		count += m_owner->GetMagicWeapon()->IsCanSkillRefresh();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_MagicSkillCanRefresh, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Role);
		}
	}

	if (m_owner->IsOpenSystem(eStSmagic) && m_owner->GetSMagic())
	{
		isSyn = false;
		count = 0;
		count += m_owner->GetSMagic()->IsCanAdvance();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_SMagicCanAdvance, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Role);
		}
	}

	if (m_owner->IsOpenSystem(eStHeartMagic) && m_owner->GetHeartMagicWeapon())
	{
		isSyn = false;
		count = 0;
		count += m_owner->GetHeartMagicWeapon()->IsCanAdvance();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_HeartMagicCanAdvance, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Role);
		}
	}

	if (m_owner->IsOpenSystem(eStDanLu) && m_owner->GetDanLuManager())
	{
		// 角色 丹炉升阶
		isSyn = false;
		count = 0;
		count += m_owner->GetDanLuManager()->IsCanAdvance();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanAdvance, count) && !isSyn)
		{
			isSyn = true;
		}

		count = 0;
		count += m_owner->GetDanLuManager()->IsCanSmelting();
		if (0 == ModifyData(eUIHLBig_Role, eUIHLSmall_Role_DanLuCanSmelting, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Role);
		}
	}

	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

}

bool CHighLightMgr::EquipSmithUIHL(SystemNoticeProto::ToClientUIHL* toC)
{

	int count = 0;
	bool isSyn = false;
	map<int, vector<bool> > allCanIndex;
	vector<bool> canIndex;
	Smart_Ptr<ArticleEquipMgr> equipCntr = m_owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
		count = 0;
		if (m_owner->IsOpenSystem(eEquipStrengtLv))
		{
			canIndex.clear();
			equipCntr->IsAllEquipCanStrength(count, &canIndex);
			if (0 == ModifyData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_StrengthUnLvCeil, count) && !isSyn)
			{
				isSyn = true;
			}
			if (0 < canIndex.size())
			{
				allCanIndex[0] = canIndex;
			}
		}
		count = 0;
		if (m_owner->IsOpenSystem(eEquipInlayLv))
		{
			canIndex.clear();
			int quickCan = 0; // 快速升级宝石
			int inlayCan = 0; // 镶嵌宝石
			equipCntr->IsAllEquipCanInlay(inlayCan, quickCan, &canIndex);
			count = inlayCan + quickCan;
			if (0 == ModifyData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_InlayUnLvCeil, count) ||
					0 < quickCan)
			{
				isSyn = true;
			}
			if (0 < canIndex.size())
			{
				allCanIndex[1] = canIndex;
			}

		}
		count = 0;
		if (m_owner->IsOpenSystem(eEquipRefineLv))
		{
			canIndex.clear();
			equipCntr->IsAllEquipCanRefine(count, &canIndex);
			if (0 == ModifyData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_RefineUnLvCeil, count) && !isSyn)
			{
				isSyn = true;
			}
			if (0 < canIndex.size())
			{
				allCanIndex[2] = canIndex;
			}
		}
		count = 0;
		if (m_owner->IsOpenSystem(eEquipUpgradeLv) )
		{
			canIndex.clear();
			equipCntr->IsAllEquipCanUpgrade(count, &canIndex);
			if (0 == ModifyData(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_UpgradeUnLvCeil, count) && !isSyn)
			{
				isSyn = true;
			}
			if (0 < canIndex.size())
			{
				allCanIndex[3] = canIndex;
			}
		}


		if (isSyn)
		{
			if (0 < allCanIndex.size() && NULL != toC)
			{
				for (map<int, vector<bool> >::iterator it = allCanIndex.begin(); it != allCanIndex.end(); ++it)
				{
					SystemNoticeProto::UIHL* UIHL = toC->add_list();
					if (NULL != UIHL)
					{
						UIHL->set_bigtype(it->first);
						for (uint i = 0; i < it->second.size(); ++i)
						{
							UIHL->add_smalltype(i + 1);
							UIHL->add_value(it->second[i]);
						}
					}
				}
			}

			return true;
		}
	}

	return false;

}

void CHighLightMgr::SendEquipSmithHLToC(SystemNoticeProto::ToClientUIHL toClient)
{
	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &toClient, MSG_SIM_GM2C_NOTICESMITHUIHL);

}

void CHighLightMgr::UpdateUIHighLight(E_UIHL_HANDLE handleType, UIHLType type,	int count,  bool flag, int value)
{
	UpdateUIHLDataArgs args(m_owner->GetMyself());
	args.AddContent(handleType, type.first, type.second, count, flag, value);

	HandleDataEvent(args);
}
void CHighLightMgr::UpdateUIHighLight(E_UIHL_HANDLE handleType, UIHLType type, HintValue value, bool flag)
{
	UpdateUIHighLight(handleType, type, value.fValue, flag, value.sValue);
}
void CHighLightMgr::UpdateUIHighLight(vector<E_UIHL_HANDLE> handleTypes, vector<E_UIHL_BIGTYPE> bigTypes, vector<E_UIHL_SMALLTYPE> smallTypes,
			vector<int> counts, vector<bool> flags, vector<int> values)
{
	if (0 < values.size())
	{
		UpdateUIHLDataArgs args(m_owner->GetMyself(), handleTypes, bigTypes, smallTypes, counts, flags, values);
		HandleDataEvent(args);
	}
	else
	{
		UpdateUIHLDataArgs args(m_owner->GetMyself(), handleTypes, bigTypes, smallTypes, counts, flags);
		HandleDataEvent(args);
	}
}


bool CHighLightMgr::HandlePackageGoods(const EventArgs& e)
{
	ChangeComCall();

	int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(m_owner->GetMyself());
	UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);

	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_owner->GetContainer(ePackageType);
	if (parcelMgr)
	{
		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());
	}

	return true;
}

bool CHighLightMgr::HandleForceChange(const EventArgs& e)
{
	bool isSyn = false;
	int count = 0;
	if (m_owner->IsOpenSystem(eStMeridians))
	{
		// 角色 经脉升阶
		isSyn = false;
		count = 0;
		count +=  (bool)m_owner->GetMyself() ? MeridianMgr::GetInstance()->IsCanAdvance(m_owner->GetMyself()) : 0;
		if (0 == ModifyData(eUIHLBig_Meridian, eUIHLSmall_Meridian_CanAdvance, count) && !isSyn)
		{
			isSyn = true;
		}

		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Meridian);
		}
	}

	if (m_owner->IsOpenSystem(eStSkill) && m_owner->GetSkillMgr())
	{
		// 主动技能
		isSyn = false;
		count = 0;
		count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_ActiveSkill);
		count += m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_PassiveSkill);
		if (0 == ModifyData(eUIHLBig_Skill, eUIHLSmall_Skill_CanUpgrade, count) && !isSyn)
		{
			isSyn = true;
		}


		if (isSyn)
		{
			SynchUIHighLight(eUIHLBig_Skill);
		}
	}


	return 0;
}

bool CHighLightMgr::HandleWearEquip(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}
bool CHighLightMgr::HandleDropEquip(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}

bool CHighLightMgr::HandleStrengthEquip(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}

bool CHighLightMgr::HandleInlayEquip(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}

bool CHighLightMgr::HandleRefineEquip(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}

bool CHighLightMgr::HandleUpgradeEquip(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}

bool CHighLightMgr::HandleRemoveStone(const EventArgs& e)
{
	SystemNoticeProto::ToClientUIHL toClient;
	if (EquipSmithUIHL(&toClient))
	{
		SynchUIHighLight(eUIHLBig_EquipSmith);

		SendEquipSmithHLToC(toClient);
	}

	return true;
}

bool CHighLightMgr::HandleSigniIn(const EventArgs& e)
{
	int count = 0;

	if (m_owner->GetSignIn())
	{
//		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanSignIn),
//				m_owner->GetSignIn()->GetSignInCanGetAward());
		{
			HintValue value;
			value.fValue = !m_owner->GetSignIn()->isTodayHaveSignin();
			value.sValue = 1;
			UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanSignIn), value);
		}

		{
			count += m_owner->GetSignIn()->GetMendingSignCnt();
			UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanMendingSignIn), count);
		}


		count = 0;
		{
			count += m_owner->GetSignIn()->CheckSignInReward();
			UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_SignInTotalAward), count);
		}
	}

	return true;
}

bool CHighLightMgr::HandleSubmitTask(const EventArgs& e)
{
	const PlayerAcceptQuestEventArgs* arg = dynamic_cast<const PlayerAcceptQuestEventArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}

	Entry_Quest* quest = QuestLoader::GetInstance()->GetQuestInfo(arg->m_questid);
	switch (quest->m_nMainType)
	{
	case QuestType_Functional:
		{
			HintValue value;
			Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
			if (questMgr && m_owner->IsOpenSystem(eStDailyQuest) && questMgr->getDailyQuests())
			{
				value.fValue = questMgr->getDailyQuests()->GetUnCompleteNum();
				value.sValue = questMgr->getDailyQuests()->getMaxTimes();
			}
			UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_DailyTask, eSmall_DailyTask_AlreadyFini), value);

//			UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);

		}
		break;
	case QuestType_Clan:
		{
			HintValue value;
			Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
			if (questMgr && m_owner->IsOpenSystem(eStClanTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharFaction) > 0
					&& questMgr->getClanQuests())
			{
				value.fValue = questMgr->getClanQuests()->GetUnCompleteNum();
				value.sValue = questMgr->getClanQuests()->getMaxTimes();
			}
			UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ClanTask, eSmall_ClanTask_AlreadyFini), value);

//			UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);

		}
		break;
	case QuestType_Camp:
		{
			// 阵营任务
			HintValue value;
			Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
			if (questMgr && m_owner->IsOpenSystem(eStCntyTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharCountry) > 0
					&& questMgr->getCampQuests())
			{
				value.fValue = questMgr->getCampQuests()->GetUnCompleteNum();
				value.sValue = questMgr->getCampQuests()->getMaxTimes();
			}
			UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_CntyTask, eSmall_CntyTask_AlreadyFini), value);
		}
		break;
	}

	return true;
}

bool CHighLightMgr::HandleEnterInst(const EventArgs& e)
{
	const PlayerEnterInstanceEventArgs* arg = dynamic_cast<const PlayerEnterInstanceEventArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}
	HintValue value;

	if (m_owner->GetInstanceMgr())
	{
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(arg->m_mapId));
		if (entry)
		{
			switch (entry->m_eType)
			{
			case ePersonalInstance:
				{
					value.sValue = m_owner->GetInstanceMgr()->GetSingleInstUnChall(value.fValue);

					UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini), value);
				}
				break;

			case eChallenge:
			{
				value.sValue = m_owner->GetInstanceMgr()->GetChallInstUnChall(value.fValue);
				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ChallengeInst, eSmall_ChallengeInst_AlreadyFini), value);
			}
			break;

			case eBattleGroudInstance:
				{
					HintValue value;
					int index = 0, type = 0, challNumCeil = 0, count = 0;
					BeyondBattleMgr::GetInstance()->GetPlayerIndexAndType(arg->m_mapId, m_owner->GetID(), index, type);

					if (m_owner->IsOpenSystem(eStBeyondSvrBattle) && m_owner->GetCounterService())
					{
						if (type == eBeyondBattleOne)
						{
							challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetSingleInvit();
							count = challNumCeil - m_owner->GetCounterService()->GetNumById(BEYOND_SINGLE_BATTLE);
							value.fValue = count;
							value.sValue = challNumCeil;
							UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_BeyondBattle, eSmall_SingleBeyondArena_AlreadyChallenge),
									value);
						}
						else if (eBeyondBattleThree)
						{
							// 多人跨服竞技
							challNumCeil =  CCrossSvrCompeteLoader::GetInstance()->GetGroupInvit();
							count = challNumCeil - m_owner->GetCounterService()->GetNumById(BEYOND_GROUP_BATTLE);
							value.fValue = count;
							value.sValue = challNumCeil;
							UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_BeyondBattle, eSmall_GroupBeyondArena_AlreadyChallenge),
								value);
						}
					}

//					UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_BeyondBattle, eUIHLSmall_BeyondBattle_ChallengeNum), 1, false);
				}
				break;
			case eCompeteInstance:
				{
					if (m_owner->IsOpenSystem(eStCompeteInstance))
					{
						UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_Arena, eUIHLSmall_Arena_ChallengeNum), 1, false);
//						UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
					}
				}
				break;
			case eEliteInstance:
				{
					if (m_owner->IsOpenSystem(eStEliteInstLv))
					{
						value.sValue = m_owner->GetInstanceMgr()->GetEliteInstUnChall(value.fValue);
						UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini), value);
					}
				}
				break;
			case eCauchemarInstance:
				{
					if (m_owner->IsOpenSystem(eStCauchemarInstLv))
					{
						value.sValue = m_owner->GetInstanceMgr()->GetCauchemarInstUnChall(value.fValue);
						UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini), value);
					}
				}
				break;
			default:
				{

				}
			}
		}
	}

	return true;

}

bool CHighLightMgr::HandleResetProtectKing(const EventArgs& e)
{
//	if (m_owner->GetActivityMgr() && m_owner->IsOpenSystem(eProtectKingLv))
	{
//		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_GuardKingCanChallenge), m_owner->GetActivityMgr()->GetPKSuplusNode());

		HintValue value;
		value.fValue = m_owner->GetActivityMgr()->GetPKUnResetNum();
		value.sValue = m_owner->GetActivityMgr()->GetTotalResetPK();

		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ProtectKing, eSmall_ProtectKing_AlreadyFini), value);
	}

	return true;
}

bool CHighLightMgr::HandleMoneyInstBuyCnt(const EventArgs& e)
{
	HintValue value;
	if (m_owner->GetInstanceMgr() && m_owner->IsOpenSystem(eStMoneyInstOpenLv))
	{
//		UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_Practice, eUIHLSmall_Practice_HaveCanChallenge), 1, true);

		int maxCount = 0, maxBuyCount = 0, leftCount = 0, buyCount = 0;
		m_owner->GetInstanceMgr()->getMoneyInstanceCount(maxCount, maxBuyCount, leftCount, buyCount);
		value.fValue = leftCount; // 剩余挑战次数
		value.sValue = maxCount + buyCount; // 总次数
		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_MoneyInst, eSmall_MoneyInst_AlreadyFini), value);
	}

	return true;
}

bool CHighLightMgr::HandleEscortStart(const EventArgs& e)
{
	//-------押镖次数达到上限的时候   同步 活动图标 显示信息-----------//
//			m_owner->UpdateAtiveHighLight(eHL_Threat, 1, false, false);
	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Escort, eUIHLSmall_Challenge_EscortCanChallenge), 1, false);
//	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
	return true;
}

bool CHighLightMgr::HandlePassInst(const EventArgs& e)
{
	const PlayerInstancePassEventArgs* arg = dynamic_cast<const PlayerInstancePassEventArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}
	HintValue value;
	if (m_owner->GetInstanceMgr())
	{
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(arg->m_mapId);
		if (entry)
		{
			switch (entry->m_eType)
			{
			case ePersonalInstance:
				{

				}
				break;

			case eChallenge:
			{
				value.sValue = m_owner->GetInstanceMgr()->GetChallInstUnChall(value.fValue);
				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ChallengeInst, eSmall_ChallengeInst_AlreadyFini), value);
			}
			break;
			case eAdvancedInstance:
			{
				Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
				if ((bool)instanceMgr)
				{
					value.sValue = instanceMgr->GetAdvancedInstUnChall(value.fValue);
				}

				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_Advanced, eSmall_Advanced_AlreadyFini), value);
			}
			break;
			default:
				{

				}
			}
		}
	}

	return true;
}

bool CHighLightMgr::HandleOnekeyQuest(const EventArgs& e)
{
	const OneKeyCompleteArgs* arg = dynamic_cast<const OneKeyCompleteArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}

	Entry_Quest* quest = QuestLoader::GetInstance()->GetQuestInfo(arg->m_eQuestID);
	switch (quest->m_nMainType)
	{
	case QuestType_Functional:
		{
			HintValue value;
			Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
			if (questMgr && m_owner->IsOpenSystem(eStDailyQuest) && questMgr->getDailyQuests())
			{
				value.fValue = questMgr->getDailyQuests()->GetUnCompleteNum();
				value.sValue = questMgr->getDailyQuests()->getMaxTimes();
				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_DailyTask, eSmall_DailyTask_AlreadyFini), value);
			}

//			UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);

		}
		break;
	case QuestType_Clan:
		{
			HintValue value;
			Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
			if (questMgr && m_owner->IsOpenSystem(eStClanTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharFaction) > 0
					&& questMgr->getClanQuests())
			{
				value.fValue = questMgr->getClanQuests()->GetUnCompleteNum();
				value.sValue = questMgr->getClanQuests()->getMaxTimes();
				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ClanTask, eSmall_ClanTask_AlreadyFini), value);
			}


//			UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);

		}
		break;
	case QuestType_Camp:
		{
			// 阵营任务
			HintValue value;
			Smart_Ptr<QuestPart> questMgr = m_owner->GetQuestMgr();
			if (questMgr && m_owner->IsOpenSystem(eStCntyTaskOpenLv) && m_owner->getAttManage()->getValue<int>(eCharCountry) > 0
					&& questMgr->getCampQuests())
			{
				value.fValue = questMgr->getCampQuests()->GetUnCompleteNum();
				value.sValue = questMgr->getCampQuests()->getMaxTimes();
				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_CntyTask, eSmall_CntyTask_AlreadyFini), value);
			}

		}
		break;
	}
	return true;
}

bool CHighLightMgr::HandleGetMoneyShop(const EventArgs& e)
{
	int count = 0;
	HintValue value;
	if (m_owner->IsOpenSystem(eStBank))
	{
		count +=  (bool)m_owner->GetMyself() ? MoneyShopMgr::GetInstance()->MoneyShop_IsHaveFreeNum(m_owner->GetMyself()) : 0;
		value.fValue = count;
		value.sValue = MoneyShopMgr::GetInstance()->GetMoneyShopFreeMax();

		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MoneyShop, eUIHLSmall_MoneyShop_FreeNumUnUse), value);
	}

//	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
	return true;
}

bool CHighLightMgr::HandleResetAllInst(const EventArgs& e)
{
	const ResetAllInstArgs* arg = dynamic_cast<const ResetAllInstArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}

	if (m_owner->GetInstanceMgr())
	{
//		int surplusNum = m_owner->GetInstanceMgr()->GetAllInstSurplusNum();
//		UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_CanChallenge), surplusNum);

		// 0单人副本 ； 1组队副本 ； 2挑战副本
		HintValue value;
		switch (arg->m_instType)
		{
		case 0:
			{
				if (m_owner->IsOpenSystem(eStSingleInstanceLv))
				{
					value.sValue = m_owner->GetInstanceMgr()->GetSingleInstUnChall(value.fValue);
					UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini), value);
				}
			}
			break;
		case 2:
			{
				if (m_owner->IsOpenSystem(eStChallengeInstanceLv))
				{
					value.sValue = m_owner->GetInstanceMgr()->GetChallInstUnChall(value.fValue);
					UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ChallengeInst, eSmall_ChallengeInst_AlreadyFini), value);
				}
			}
			break;
		case eAdvancedInstance:
			{
				if (m_owner->IsOpenSystem(eStAdvanceInstOpenLv))
				{
					value.sValue = m_owner->GetInstanceMgr()->GetAdvancedInstUnChall(value.fValue);
					UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_Advanced, eSmall_Advanced_AlreadyFini), value);
				}
			}
			break;
		default:
			{

			}
		}
	}
	return true;
}

bool CHighLightMgr::HandleStartSweep(const EventArgs& e)
{
//	PlayerRaidsDuplicate tempArgs(m_owner->GetMyself(),sceneId);
	const PlayerRaidsDuplicate* arg = dynamic_cast<const PlayerRaidsDuplicate*>(&e);

	if (NULL == arg)
	{
		return true;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(arg->m_MapID);
	if (NULL == entry)
	{
		return true;
	}

	if (m_owner->GetInstanceMgr())
	{
//		UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_CanChallenge), 1, false);

		HintValue value;
		switch (entry->m_eType)
		{
		case ePersonalInstance:
			{
				if(m_owner->IsOpenSystem(eStSingleInstanceLv))
				{
					value.sValue = m_owner->GetInstanceMgr()->GetSingleInstUnChall(value.fValue);
					UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_EquipInst, eUIHLSmall_EquipInst_AlreadyFini), value);
				}
			}
			break;

		case eChallenge:
		{
			if(m_owner->IsOpenSystem(eStChallengeInstanceLv))
			{
				value.sValue = m_owner->GetInstanceMgr()->GetChallInstUnChall(value.fValue);
				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_ChallengeInst, eSmall_ChallengeInst_AlreadyFini), value);
			}
		}
		break;
		case eAdvancedInstance:
		{
			if(m_owner->IsOpenSystem(eStAdvanceInstOpenLv))
			{
				Smart_Ptr<InstanceMgr> instanceMgr = m_owner->GetInstanceMgr();
				if ((bool)instanceMgr)
				{
					value.sValue = instanceMgr->GetAdvancedInstUnChall(value.fValue);
				}

				UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eBig_Advanced, eSmall_Advanced_AlreadyFini), value);
			}
		}
		break;
		default:
			{
			}
		}
	}

	return true;
}

bool CHighLightMgr::HandleAddArenaChall(const EventArgs& e)
{
	const ArenaAddChallArgs* arg = dynamic_cast<const ArenaAddChallArgs*>(&e);

	if (NULL == arg)
	{
		return true;
	}

	m_owner->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_Arena, eUIHLSmall_Arena_ChallengeNum), arg->m_addNum, true);
//	m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), arg->m_addNum, true);

	return true;
}

bool CHighLightMgr::HandlePassTarget(const EventArgs& e)
{
	const PassTargetArgs* arg = dynamic_cast<const PassTargetArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}

	sDestinationEntry* entry = DestinationLoader::GetInstance()->GetDestinationEntry(arg->m_targetID);
	if (NULL == entry)
	{
		return true;
	}

	E_UIHL_SMALLTYPE smallType;
	switch (entry->i_stage)
	{
	case eDestinationStage_1:
		{
			smallType = eUIHLSmall_GameTarget_OneCanGetReward;
		}
		break;
	case eDestinationStage_2:
		{
			smallType = eUIHLSmall_GameTarget_TwoCanGetReward;
		}
		break;
	case eDestinationStage_3:
		{
			smallType = eUIHLSmall_GameTarget_ThreeCanGetReward;
		}
		break;
	case eDestinationStage_4:
		{
			smallType = eUIHLSmall_GameTarget_FourCanGetReward;
		}
		break;
	case eDestinationStage_5:
		{
			smallType = eUIHLSmall_GameTarget_FiveCanGetReward;
		}
		break;
	case eDestinationStage_6:
		{
			smallType = eUIHLSmall_GameTarget_SixCanGetReward;
		}
		break;
	case eDestinationStage_7:
		{
			smallType = eUIHLSmall_GameTarget_SevenCanGetReward;
		}
		break;
	case eDestinationStage_8:
		{
			smallType = eUIHLSmall_GameTarget_EightCanGetReward;
		}
		break;
	default:
		return true;
	}
	if (m_owner->IsOpenSystem(eStGameObject))
	{
		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_GameTarget, smallType), 1, true);
	}

	return true;
}

bool CHighLightMgr::HandleDrawTarget(const EventArgs& e)
{
	const DrawTargetArgs* arg = dynamic_cast<const DrawTargetArgs*>(&e);
	if (NULL == arg)
	{
		return true;
	}

	sDestinationEntry* entry = DestinationLoader::GetInstance()->GetDestinationEntry(arg->m_targetID);
	if (NULL == entry)
	{
		return true;
	}

	E_UIHL_SMALLTYPE smallType;
	switch (entry->i_stage)
	{
	case eDestinationStage_1:
		{
			smallType = eUIHLSmall_GameTarget_OneCanGetReward;
		}
		break;
	case eDestinationStage_2:
		{
			smallType = eUIHLSmall_GameTarget_TwoCanGetReward;
		}
		break;
	case eDestinationStage_3:
		{
			smallType = eUIHLSmall_GameTarget_ThreeCanGetReward;
		}
		break;
	case eDestinationStage_4:
		{
			smallType = eUIHLSmall_GameTarget_FourCanGetReward;
		}
		break;
	case eDestinationStage_5:
		{
			smallType = eUIHLSmall_GameTarget_FiveCanGetReward;
		}
		break;
	case eDestinationStage_6:
		{
			smallType = eUIHLSmall_GameTarget_SixCanGetReward;
		}
		break;
	case eDestinationStage_7:
		{
			smallType = eUIHLSmall_GameTarget_SevenCanGetReward;
		}
		break;
	case eDestinationStage_8:
		{
			smallType = eUIHLSmall_GameTarget_EightCanGetReward;
		}
		break;
	default:
		return true;
	}
	if (m_owner->IsOpenSystem(eStGameObject))
	{
		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_GameTarget, smallType), 1, false);
	}

	return true;
}

bool CHighLightMgr::HandleSkillUp(const EventArgs& e)
{
	if (m_owner->GetSkillMgr())
	{
		if (!m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_ActiveSkill))
		{
			UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Skill, eUIHLSmall_Skill_CanUpgrade), 1, false);
		}
		if (!m_owner->GetSkillMgr()->IsHaveSkillUp(eSkillInterface_PassiveSkill))
		{
			UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Skill, eUIHLSmall_Skill_CanUpgrade), 1, false);
		}
	}

	return true;
}

bool CHighLightMgr::HandleGetSignAward(const EventArgs& e)
{
//	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanSignIn), 1, false);

	return true;
}
bool CHighLightMgr::HandleGetOnlineAward(const EventArgs& e)
{
	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOnlineAward), 1, false);

	return true;
}
bool CHighLightMgr::HandleGetLvAward(const EventArgs& e)
{
	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetLevelAward), 1, false);

	return true;
}
bool CHighLightMgr::HandleGetSevenLogin(const EventArgs& e)
{
	UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_ServenLoginAward), 1, false);

	return true;
}


vector<UIHLType> CHighLightMgr::GetHLTypeByActivity(ActivityExType activityType)
{
	vector<UIHLType> vec;
	UIHLType UIHL;
	switch (activityType)
	{
	case ActivityType_FirstRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_ChargeReturn;
			vec.push_back(UIHL);
//			UIHL.first = eUIHLBig_FirstChargeRet;
//			UIHL.second = eUIHLSmall_ActivityHall_ChargeReturn;
//			vec.push_back(UIHL);
		}
		break;
	case ActivityType_Ranking:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_RedList;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_DailyRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_DailyCharge;
			vec.push_back(UIHL);
//			UIHL.first = eUIHLBig_DailyCharge;
//			UIHL.second = eUIHLSmall_ActivityHall_FirstCharge;
//			vec.push_back(UIHL);
		}
		break;
	case ActivityType_SingleRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_SingleCharge;
			vec.push_back(UIHL);
//			UIHL.first = eUIHLBig_SingleCharge;
//			UIHL.second = eUIHLSmall_ActivityHall_SingleCharge;
//			vec.push_back(UIHL);
		}
		break;
	case ActivityType_AccRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_AccumulateCharge;
			vec.push_back(UIHL);
//			UIHL.first = eUIHLBig_AccumulateCharge;
//			UIHL.second = eUIHLSmall_ActivityHall_AccumulateCharge;
//			vec.push_back(UIHL);
		}
		break;
	case ActivityType_DailyFirstRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_FirstCharge;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_MagicWeaponAdvance:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_MagicUpgrade;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_HorseAdvance:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_AcitivityHall_HorseUpgrade;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_RelayRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_RelayCharge;
			vec.push_back(UIHL);
//			UIHL.first = eUIHLBig_RelayCharge;
//			UIHL.second = eUIHLSmall_ActivityHall_RelayCharge;
//			vec.push_back(UIHL);
		}
		break;
	case ActivityType_Compensation:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_Compensation;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_GoldenInvestLv:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_InvestmentActivity_LvInvestment;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_GoldenInvestSevenDays:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_InvestmentActivity_SevenDayRebate;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_FullService:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_FullService;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_AccDayRecharge:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_AccDayRecharge;
			vec.push_back(UIHL);
		}
		break;
	case ActivityType_AccConsume:
		{
			UIHL.first = eUIHLBig_ActivityHall;
			UIHL.second = eUIHLSmall_ActivityHall_AccumulateConsume;
			vec.push_back(UIHL);

//			UIHL.first = eUIHLBig_AccumulateConsume;
//			UIHL.second = eUIHLSmall_ActivityHall_AccumulateConsume;
//			vec.push_back(UIHL);
		}
		break;
	default:
		{

		}
	}
	return vec;
}

void CHighLightMgr::HandleData(E_UIHL_HANDLE handle, E_UIHL_BIGTYPE bigType,
		E_UIHL_SMALLTYPE smallType, int fValue, bool flag, int sValue)
{
	HintValue value(fValue, sValue);
	switch (handle)
	{
	case eUIHLHandle_Add:
		{
			AddUIHLData(bigType, smallType, value);
		}
		break;
	case eUIHLHandle_Del:
		{
			DeleteUIHLData(bigType, smallType);
		}
		break;
	case eUIHLHandle_Mod:
		{
			ModifyData(bigType, smallType, value);
		}
		break;
	case eUIHLHandle_Syn:
		{
			if (eUIHLBig_Null == bigType && eUIHLSmall_Null == smallType)
			{
				SynchUIHighLight();
			}
			else
			{
				SynchUIHighLight(bigType);
			}
		}
		break;
	case eUIHLHandle_UpAndSyn:
		{
			UpdateDataAndSyn(bigType, smallType, value, flag);
		}
		break;
	case eUIHLHandle_Up:
		{
			UpdateData(bigType, smallType, value, flag);
		}
		break;
	case eUIHLHandle_Init:
		{
			ComTypeAddData();
		}
		break;
	case eUIHLHandle_ModAndSyn:
		{
			HintValue value_ = 0;
			GetValueByType(bigType, smallType, value_);
			if (value != value_)
			{
				ModifyData(bigType, smallType, value);
				SynchUIHighLight(bigType);
			}
		}
		break;
	case eUIHLHandle_UpAndForceSyn:
		{
			UpdateData(bigType, smallType, value, flag);
			SynchUIHighLight(bigType);
		}
		break;
	case eUIHLHandle_ResetAndSyn:
		{
			ComTypeResetData();
			SynchUIHighLight();
		}
		break;
	default:
		{

		}
	}
}

//void CHighLightMgr::SetProtoData(E_HINT_SHOW showType, E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value,
//		::google::protobuf::Message* proto)
//{
//	switch (showType)
//	{
//	case eHintShow_UIHL: // UI高亮
//		{
//
//		}
//		break;
//	case eHintShow_TaskPanel: // 任务面板
//		{
//
//		}
//		break;
//	}
//}
//Smart_Ptr<::google::protobuf::Message>  CHighLightMgr::AllocProto(E_HINT_SHOW showType)
//{
//	switch (showType)
//	{
//	case eHintShow_UIHL: // UI高亮
//		{
//			return new SystemNoticeProto::ToClientUIHL();
//		}
//		break;
//	case eHintShow_TaskPanel: // 任务面板
//		{
//			return new  SystemNoticeProto::ToClientUIHL();
//		}
//		break;
//	}
//
//	return NULL;
//}
