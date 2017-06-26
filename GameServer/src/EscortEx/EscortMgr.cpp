/*
 * EscortMgr.cpp
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#include "EscortMgr.h"
#include "../Object/Player/Player.h"
#include "MessageStruct/Escort/EscortProto.pb.h"
#include "MessageCommonRet.h"
#include "../QuickMessage.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerEventDefine.h"
#include "../Object/Monster/Monster.h"
#include "../Map/SceneManager.h"
#include "../Object/AI/AIBrain.h"
#include <math.h>
#include "../Object/FileLoader/NPCLoader.h"
#include "../Chat/ChatMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "FileLoader/InternationalLoader.h"
#include "FileLoader/ActivitiesTimerLoader.h"

EscortMgr::EscortMgr(Player *player):m_owner(player)
{
	ReInit();
}

EscortMgr::~EscortMgr()
{
	this->ReleaseEscortMgr();
}

void EscortMgr::ReInit()
{
	m_line = 0;
	m_quality = eEscortBronze;
	m_robTimes = 0;
	m_startTime = 0;
	m_speedTime = 0;
	m_mapid = 0;
	m_xpos = 0;
	m_ypos = 0;
	m_hp = 0;
	m_nRate = 0;
	m_num = 0;
	m_rewardExp = 0;
	m_rewardMoney = 0;

	m_sAttackedName.clear();

	if(!m_enterEvent.IsValid())
		m_enterEvent = RegEvent(m_owner,PLAYER_ENTER_MAP_EVENT_TYPE, this, &EscortMgr::EnterEscortData);

	if(!m_logoutEvent.IsValid())
		m_logoutEvent = RegEvent(m_owner,PLAYER_LEAVE_MAP_EVENT_TYPE, this,&EscortMgr::LeaveEscortData);

	if(!m_killMonsterEvent.IsValid())
		m_killMonsterEvent = RegEvent(m_owner,MONSTER_KILLED_EVENT_TYPE, this, &EscortMgr::KillMonster);

	if(!m_moveEvent.IsValid())
		m_moveEvent = RegEvent(m_owner, PLAYER_POSITION_TOPOSITION, this, &EscortMgr::MoveEscortData);

	if(!m_monsterAttacked.IsValid())
		m_monsterAttacked = RegEvent(m_owner, MONSTER_BEATTACK, this, &EscortMgr::MonsterBeAttack);
}

void EscortMgr::ReleaseEscortMgr()
{
	if (m_enterEvent.IsValid())
	{
		m_enterEvent.SetNull();
	}

	if (m_logoutEvent.IsValid())
	{
		m_logoutEvent.SetNull();
	}

	if (m_killMonsterEvent.IsValid())
	{
		m_killMonsterEvent.SetNull();
	}

	if (m_moveEvent.IsValid())
	{
		m_moveEvent.SetNull();
	}

	if (m_reflushTimer.IsValid())
	{
		m_reflushTimer.SetNull();
	}

	if (m_escortEnd.IsValid())
	{
		m_escortEnd.SetNull();
	}

	if (m_speedUpEnd.IsValid())
	{
		m_speedUpEnd.SetNull();
	}

	if (m_monsterAttacked.IsValid())
	{
		m_monsterAttacked.SetNull();
	}

	if(m_monsterDead.IsValid())
	{
		m_monsterDead.SetNull();
	}

	Clear();
}

void EscortMgr::SetEscortToDB(PlayerInfo::EscortInfo *info)
{
	info->set_num(m_num);

	if (m_line <= 0)
		return;

	info->set_quality(m_quality);
	info->set_line(m_line);
	info->set_robtimes(m_robTimes);
	info->set_lefttimes(m_startTime);
	info->set_speedlefttime(m_speedTime);
	info->set_rewardexp(m_rewardExp);
	info->set_rewardmoney(m_rewardMoney);

	if (m_carMon)
	{
		info->set_mapid(m_carMon->GetMapID());
		info->set_xpos(m_carMon->GetXNewPos());
		info->set_ypos(m_carMon->GetYNewPos());
		info->set_hp(m_carMon->getAttManage()->getValue<int>(eCharHP));
	}
	else
	{
		if (m_mapid == 0 || (m_xpos == 0 && m_ypos == 0))
		{
			info->set_mapid(m_owner->GetMapID());
			info->set_xpos(m_owner->GetXNewPos());
			info->set_ypos(m_owner->GetYNewPos());
		}
		else
		{
			info->set_mapid(m_mapid);
			info->set_xpos(m_xpos);
			info->set_ypos(m_ypos);
		}

		if (m_hp > 0)
			info->set_hp(m_hp);
	}
}

void EscortMgr::InitEscort(const PlayerInfo::EscortInfo &info)
{
	m_num = info.num();

	if (info.line() <= 0)
		return;

	int64 nowTime = CUtil::GetNowSecond();

	sEscortEntry * entry = EscortLoader::GetInstance()->GetEscortEntry(info.line());
	if (entry)
	{
		if (info.lefttimes() > 0
				&& nowTime - info.lefttimes() >= entry->m_nLimitTime)
		{
			m_owner->SetDataFlag(eEscortInfo);

			return ;
		}

		m_line = info.line();
		if(m_line <= 0)
			m_line = 1;
		m_quality = (eCarType)info.quality();
		m_robTimes = info.robtimes();
		m_startTime = info.lefttimes();
		m_speedTime = info.speedlefttime();
		m_mapid = info.mapid();
		m_xpos = info.xpos();
		m_ypos = info.ypos();

		m_rewardExp = info.rewardexp();
		m_rewardMoney = info.rewardmoney();

		if (info.hp() > 0)
			m_hp = info.hp();
	}

	switch(m_quality)
	{
	case eEscortBronze:
		m_nRate = entry->m_nQuality1Probility[0];
		break;
	case eEscortSilver:
		m_nRate = entry->m_nQuality2Probility[0];
		break;
	case eEscortGold:
		m_nRate = entry->m_nQuality3Probility[0];
		break;
	case eEscortPlatinum:
		m_nRate = entry->m_nQuality4Probility[0];
		break;
	case eEscortDiamond:
		m_nRate = entry->m_nQuality5Probility[0];
		break;
	default:
		break;
	}
}

int EscortMgr::SetEscortQuality(eCarType quality)
{
	vector<int> vecType;

	if (m_owner->IsInCreatureState(ERS_Escort))
	{
		return eIsEscorting;
	}

	if (m_line <= 0)
		m_line = 1;

	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
	if (entry)
	{
		DWORD price = entry->m_nQualityCost[quality];

		if (!m_owner->IsEnoughGolden(price))
		{
			return eNotEnoughGold;
		}

		DWORD dotimes = m_owner->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
		if (dotimes >= entry->m_nDayTimes)
		{
			return eEscortTimesOver;
		}

		if (m_quality == eEscortDiamond)
			return eIsAlreadyEscortDiamond;

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costInfo,eCharGolden,m_owner->GetMyself());

//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

		int costGolden = 0;
		int costbGolden = 0;

		m_owner->DecreaseGoldenCost(price, costGolden, costbGolden);
		if(costGolden > 0)
			vecType.push_back(eCharGolden);
		if(costbGolden > 0)
			vecType.push_back(eCharBindGolden);
		m_owner->SynCharAttribute(vecType);

//		StatisticMgr::SetItemCostAfter(costInfo,eCharGolden,costGolden,m_owner->GetMyself());

//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,costbGolden,m_owner->GetMyself());

		m_owner->SetDataFlag(eEscortInfo);
		m_quality = quality;

//		if(eAccountType_User == m_owner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Escort,eStaMinor_Escort_EscortSetQualityEvent,&costAndGotInfo);

		// 统计元宝花费
//		StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_EscortSetQuality,1, price);
	}

	return 0;
}

int EscortMgr::GenenateQuality(DWORD escortId, DWORD &quality)
{
	if (quality == eEscortDiamond)
		return eIsAlreadyEscortDiamond;

	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(escortId);
	if (!entry)
		return eNull;

	int rateStart = 0;
	DWORD freshtimes = m_owner->GetCounterService()->GetNumById(ESCORT_FREETIMES_COUNTER);
	rateStart = m_nRate;

	vector<int> vecType;

	if (m_owner->IsInCreatureState(ERS_Escort))
	{
	   return eIsEscorting;
	}

	// 是否是VIP 增加皇纲刷新次数
	DWORD times = entry->m_nFreeTimes;
	DWORD vipTimes = 0;
//	if (m_owner->GetVipPart())
//	{
//		double times = m_owner->GetVipPart()->GetValueByRight(eVipEscortRefreshCnt);
//		if (times != VIP_NOT_HAVE_THIS_RIGHT)
//			vipTimes = (DWORD)times;
//	}

	if (0 < vipTimes)
	{
		times += vipTimes;
	}

	//freshtimes从0开始递增，每刷新一次＋1
	if (freshtimes >= times)
	{
		int id = GameParaLoader::GetInstance()->getEscortReflushItemId();
		Smart_Ptr<PropBase>  item = m_owner->GetContainer(ePackageType)->GetItemDataPtrByID(id);
		if (!item)
			return eNotFindItem;

		m_owner->GetContainer(ePackageType)->SubmitGrideItem(id);
	}

	DWORD a = rateStart + rand()%(GameParaLoader::GetInstance()->getEscortRandomRate() - rateStart) + 1;		//rate以内的随机数

	if ((a >= entry->m_nQuality1Probility[0])
			&& (a <= entry->m_nQuality1Probility[1]))
	{
		m_quality = eEscortBronze;
		m_nRate = entry->m_nQuality1Probility[0];
	}
	else if ((a > entry->m_nQuality2Probility[0])
			     && (a <= entry->m_nQuality2Probility[1]))
	{
		m_quality = eEscortSilver;
		m_nRate = entry->m_nQuality2Probility[0];
	}
	else if ((a > entry->m_nQuality3Probility[0])
			     && (a <= entry->m_nQuality3Probility[1]))
	{
		m_quality = eEscortGold;
		m_nRate = entry->m_nQuality3Probility[0];
	}
	else if ((a > entry->m_nQuality4Probility[0])
			     && (a <= entry->m_nQuality4Probility[1]))
	{
		m_quality = eEscortPlatinum;
		m_nRate = entry->m_nQuality4Probility[0];
	}
	else if ((a > entry->m_nQuality5Probility[0])
			     && (a <= entry->m_nQuality5Probility[1]))
	{
		m_quality = eEscortDiamond;
		m_nRate = 0;
	}

	m_owner->GetCounterService()->IncreasePlayerCounter(ESCORT_FREETIMES_COUNTER,1);
	//玩家第一次刷新镖车要变成钻石
	if (m_num == 0 && freshtimes == 0)
	{
		m_quality = eEscortDiamond;
		m_nRate = 0;
	}
	quality =  m_quality;
	m_line = escortId;
	m_owner->SetDataFlag(eEscortInfo);

	if (quality == eEscortDiamond)
		ChatMgr::GetInstance()->Marquee(eMarqueeEscort,m_owner->GetName(),"","");

	return 0;
}

int EscortMgr::StartEscort(bool isAdd)
{
	if (isAdd)
	{
		if (m_owner->IsInCreatureState(ERS_Escort))
		{
		   return eIsEscorting;
		}

		//开始押镖的时候算能领取奖励
		m_rewardExp = GetAllExp();
		m_rewardMoney = GetAllMoney();
	}

	//由于以前没有配置，所以此处写死
	if (m_owner->IsOpenSystem(eStEscort)==false)
	{
		return eClanLevelNotEnough;
	}

	//现在没有线路选择，暂时只有1这条线路
	if (m_line <= 0)
		m_line = 1;

	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
	if(entry)
	{
		int64 nowTime = CUtil::GetNowSecond();
		int64 mapid = 0;
		int xpos = 0, ypos = 0;
		Smart_Ptr<Monster> mon;

		//切换地图时，不需要增加次数
		DWORD dotimes = m_owner->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER) + (int)isAdd;
		if(dotimes > entry->m_nDayTimes)
		{
			return eEscortTimesOver;
		}

		//如果是上线继续押镖的，就需要在原来镖车的位置，否则就让他在玩家所在的位置
		if (isAdd)
		{
			m_startTime = nowTime;
			mapid = m_owner->GetMapID();
			xpos = m_owner->GetXNewPos();
			ypos = m_owner->GetYNewPos();
			m_mapid = mapid;
			m_xpos = xpos;
			m_ypos = ypos;

			if(m_escortEnd.IsValid())
			{
				m_escortEnd.SetNull();
			}
			m_num++;
			m_escortEnd = TimerInterface::GetInstance()->RegTimePoint(&EscortMgr::EndEscort, this, 0, 0, 0, 0, entry->m_nLimitTime);

			if (m_NoticeTimer.IsValid())
			{
				LOG_ERROR(FILEINFO, "player[%lld] escort start too more times", GET_PLAYER_CHARID(m_owner->GetID()));
				m_NoticeTimer.SetNull();
			}

			m_NoticeTimer = TimerInterface::GetInstance()->RegRepeatedTime(&EscortMgr::NoticeBeEscort, this, 0, 0, 0, 0,10000,10000);
		}
		else
		{
			if (nowTime - m_startTime >= entry->m_nLimitTime)
			{
				Clear();
				m_owner->SetDataFlag(eEscortInfo);

				return eNull;
			}

			if (m_escortEnd.IsValid())
			{
				m_escortEnd.SetNull();
			}
			m_escortEnd = TimerInterface::GetInstance()->RegTimePoint(&EscortMgr::EndEscort, this, 0, 0, 0, 0, entry->m_nLimitTime - nowTime + m_startTime);

			if (m_NoticeTimer.IsValid())
			{
				LOG_ERROR(FILEINFO, "player[%lld] escort start too more times", GET_PLAYER_CHARID(m_owner->GetID()));
				m_NoticeTimer.SetNull();
			}

			m_NoticeTimer = TimerInterface::GetInstance()->RegRepeatedTime(&EscortMgr::NoticeBeEscort, this, 0, 0, 0, 0,10000,10000);
			mapid = m_mapid;
			xpos = m_xpos;
			ypos = m_ypos;
		}

		//不管镖车所在地图，此gm是否在管理，都得设置他的状态为镖车状态
		m_owner->SetCreatureState(ERS_Escort);

		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(mapid);
		if (!zone)
		{
			zone = CSceneMgr::GetInstance()->GetGameZoneByMapID(GET_MAP_ID(mapid));
			if(!zone)
			{//此种情况，只有在上次接镖的服线跟现在所在的服线不一样了,而且此服还不管理这个地图，那么就不让镖车进场景，状态还得在
				return eRetZoneNotExist;
			}
		}

		if (!isAdd && m_hp > 0)
		{
			mon = zone->CreateMonsterDymic(entry->m_nCarType[m_quality], xpos, ypos, m_hp, m_owner->GetMyself());
			if(!mon)
			{
				return eNull;
			}
		}
		else
		{
			mon = zone->CreateMonsterDymic(entry->m_nCarType[m_quality], xpos, ypos, m_owner->GetMyself());
			if (!mon)
			{
				return eNull;
			}

			m_hp = mon->getAttManage()->getValue<int>(eCharHP);
		}

		mon->SetEscortID(m_line);
		m_carMon = mon;

		if(m_carMon)
		{
			if(m_monsterDead.IsValid())
			{
				m_monsterDead.SetNull();
			}

			m_monsterDead = RegEvent(m_carMon, CREATURE_DEAD_EVENT_TYPE, this, &EscortMgr::MonsterDead);
		}

		if (!isAdd)
		{
			if (nowTime - m_speedTime < entry->m_nSpeedUpTime)
			{
				int speed = mon->getAttManage()->getValue<int>(eCharMoveSpeed);

				speed = speed * entry->m_nSpeedUpRate;
				mon->getAttManage()->setValue(eCharMoveSpeed,speed);
				mon->GetBrain()->ClearPathAndTime();

				if (m_speedUpEnd.IsValid())
				{
					m_speedUpEnd.SetNull();
				}
				m_speedUpEnd = TimerInterface::GetInstance()->RegTimePoint(&EscortMgr::EndSpeedUp, this, 0, 0, 0, 0, entry->m_nSpeedUpTime - nowTime + m_speedTime);
			}
		}

		if (m_owner->GetCounterService() && isAdd)
		{
			m_owner->GetCounterService()->IncreasePlayerCounter(ESCORT_DAY_COUNTER,1);

			EventArgs tempArgs;
			m_owner->FireEvent(PLAYER_START_ESCORT,tempArgs);

//			StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(),eStatic_Escort,eStaMinor_Escort_StartEscort,1);

//			if(eAccountType_User == m_owner->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Escort,eStaMinor_Escort_StartEscortEvent);

		}

		m_owner->SetDataFlag(eEscortInfo);
	}

	m_owner->GetVitalityMgr()->FinshVitality(eViEscort);

	return 0;
}

DWORD EscortMgr::GetEscortLeftTime()
{
	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(No1EscortID);
	if (entry)
	{
		int dotimes = m_owner->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
		if (entry->m_nDayTimes >= (DWORD)dotimes)
		{
			return (entry->m_nDayTimes - dotimes);
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int EscortMgr::GetEscortTotalNum()
{
	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(No1EscortID);
	if (entry)
	{
		return entry->m_nDayTimes;
	}

	return 0;
}

//int EscortMgr::GetHadEscortTimes(int& times)
//{
//	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(No1EscortID);
//	if (entry)
//	{
//		times = m_owner->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
//		return entry->m_nDayTimes;
//	}
//
//	return 0;
//}

//压镖剩余次数
DWORD EscortMgr::getEsortSurplusTime()
{
	sEscortEntry * entry = EscortLoader::GetInstance()->GetEscortEntry(m_line == 0 ? 1 : m_line);
	if (!entry)
		return 0;

	int dotimes = m_owner->GetCounterService()->GetNumById(ESCORT_DAY_COUNTER);
	if (entry->m_nDayTimes >= (DWORD)dotimes)
		return (entry->m_nDayTimes-dotimes);

	return 0;
}

void EscortMgr::EndSpeedUp(void* arg)
{
	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
	if (entry && m_carMon)
	{
		int speed = m_carMon->getAttManage()->getValue<int>(eCharMoveSpeed);

		if (entry->m_nSpeedUpRate > 0)
		{
			speed = speed / entry->m_nSpeedUpRate;
		}
		m_carMon->getAttManage()->setValue(eCharMoveSpeed,speed);
		m_carMon->GetBrain()->ClearPathAndTime();
	}

	m_speedTime = 0;

	if (m_speedUpEnd.IsValid())
	{
		m_speedUpEnd.SetNull();
	}
}

void EscortMgr::Reflush(void* arg)
{
	if (m_carMon)
	{
		return;
	}

	if (PlayerMessage::GetInstance()->IsOnlineEx(m_owner->GetID()))
	{
		if (m_reflushTimer.IsValid())
		{
			m_reflushTimer.SetNull();
		}

		StartEscort(false);
	}
}

void EscortMgr::EndEscort(void* arg)
{
	EscortProto::EscortResult toClient;

	Clear();

	//发送失败消息
	toClient.set_res(eEscortFailed);
	m_owner->SetDataFlag(eEscortInfo);
	m_owner->UnsetCreatureState(ERS_Escort);

	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(),m_owner->GetChannelID(),&toClient,MSG_SIM_GM2C_ESCORT_RESULT);
}

void EscortMgr::NoticeBeEscort(void* arg)
{
	char sChat[1024] = {0};
	int i = 0;
	string str;
	string nameList;

	if (m_sAttackedName.size() > 0)
	{
		ChatInfo::EndChatInfo ret;
		string sys = InternationalLoader::GetInstance()->GetInterStr(1);
		string str1 = InternationalLoader::GetInstance()->GetInterStr(36);
		ret.set_startname(sys);
		ret.set_channel(eChatSystem);
		ret.set_startcharid(-1);

		map<int64,string>::iterator itr = m_sAttackedName.begin();
		for (;itr != m_sAttackedName.end() && i < 10; ++itr, i++)
		{
			nameList += " [";
			nameList += itr->second;
			nameList += "] ";
		}

		sprintf(sChat,str1.c_str(),nameList.c_str());
		str.append(sChat);
		ret.set_str(str);
		if (m_owner && strlen(sChat) > 0)
		{
			ChatMgr::GetInstance()->WorldChat(m_owner->GetID(),ret);
		}
	}

	m_sAttackedName.clear();
}

void EscortMgr::Clear()
{
	Smart_Ptr<Monster> temp;

	if (m_escortEnd.IsValid())
	{
		m_escortEnd.SetNull();
	}

	if (m_speedUpEnd.IsValid())
	{
		m_speedUpEnd.SetNull();
	}

	if (m_NoticeTimer.IsValid())
	{
		m_NoticeTimer.SetNull();
	}

	if(m_monsterDead.IsValid())
	{
		m_monsterDead.SetNull();
	}

	m_line = 0;
	m_quality = eEscortBronze;
	m_robTimes = 0;
	m_startTime = 0;
	m_speedTime = 0;
	m_mapid = 0;
	m_xpos = 0;
	m_ypos = 0;
	m_hp = 0;
	m_rewardExp = 0;
	m_rewardMoney = 0;
	m_nRate = 0;

	m_sAttackedName.clear();

	if (m_owner && m_owner->GetCounterService())
		m_owner->GetCounterService()->ResetCounterInfo(ESCORT_SPEEDUP_COUNTER);

	if(m_carMon)
	{
		m_carMon->SetDead();
	}

	m_carMon = temp;
}

bool EscortMgr::MonsterBeAttack(const EventArgs & e)
{
	MonsterBeAttackArgs *pArgs = (MonsterBeAttackArgs *) (&e);
	if(!pArgs)
		return true;

	Smart_Ptr<Monster> monster = pArgs->m_self;
	Smart_Ptr<Player> player = pArgs->m_attack;
	if (m_carMon
			&& monster
			&& m_carMon->GetKey() == monster->GetKey()
			&& player)
	{
		map<int64,string>::iterator itr = m_sAttackedName.find(player->GetID());
		if (itr == m_sAttackedName.end())
		{
			m_sAttackedName[player->GetID()] = player->GetName();
		}
	}

	return true;
}

bool EscortMgr::MonsterDead(const EventArgs & e)
{
	const CreatureKilledEventArgs *args = (CreatureKilledEventArgs *)(&e);
	if(args == NULL)
	{
		return true;
	}

	if(!m_carMon)
	{
		return true;
	}

	Smart_Ptr<Monster> temp;
	m_carMon = temp;

	if(m_escortEnd.IsValid())
	{
		m_escortEnd.SetNull();
	}

	if(m_speedUpEnd.IsValid())
	{
		m_speedUpEnd.SetNull();
	}

	//设置复活定时器, 该不该复活呢？？？ 通过标识
	if(m_reflushTimer.IsValid())
	{
		m_reflushTimer.SetNull();
	}

	if(args->m_obj.Get() != NULL)
	{
		m_reflushTimer = TimerInterface::GetInstance()->RegTimePoint(&EscortMgr::Reflush, this,
				0, 0, 0, 0, GameParaLoader::GetInstance()->getReflushEscortTime());
	}

	m_owner->SetDataFlag(eEscortInfo);

	return true;
}



bool EscortMgr::KillMonster(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if (!data)
	{
		return true;
	}

	if (!m_carMon)
		return true;

	if (data->m_other
			&& data->m_other->GetType() != ePlayer
			&& data->m_other->GetType() != ePet)
		return true;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if (data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Smart_Ptr<Monster> mon = data->m_myself;
	Smart_Ptr<Monster> temp;

	if (player
			&& mon
			&& mon->GetKey() == m_carMon->GetKey()
			&& m_carMon->getAttManage()->getValue<int>(eCharHP) <= 0)
	{
		if(m_robTimes < DWORD(GameParaLoader::GetInstance()->getRobTimes()))
		{
			++m_robTimes;
		}

		m_mapid = m_carMon->GetMapID();
		m_xpos = m_carMon->GetXNewPos();
		m_ypos = m_carMon->GetYNewPos();
		m_hp = m_carMon->getAttManage()->getValue<int>(eCharHP);

		m_carMon = temp;
		if (m_escortEnd.IsValid())
		{
			m_escortEnd.SetNull();
		}

		if (m_speedUpEnd.IsValid())
		{
			m_speedUpEnd.SetNull();
		}

		if(m_monsterDead.IsValid())
		{
			m_monsterDead.SetNull();
		}

		m_owner->SetDataFlag(eEscortInfo);

		EscortProto::EscortResult toClient;
		vector<int> vecType;
		int exp = getRewardExp();
		int money = getRewardMoney();

//		StatisticInfo::CostAndGotInfo costAndGotInfo;

		exp *= 10;
		exp /= 100;
		mon->getAttManage()->setValue(eCharExp,exp);

		money *= 10;
		money /= 100;

//		StatisticInfo::GotItemInfo * gotExpInfo = costAndGotInfo.add_gotlist();

		player->AddExp(exp);

//		StatisticMgr::AddItemGotInfo(gotExpInfo,eCharExp,exp);

//		StatisticInfo::GotItemInfo * gotMoneyInfo = costAndGotInfo.add_gotlist();

		player->ChangeMoney(money,true);

//		StatisticMgr::AddItemGotInfo(gotMoneyInfo,eCharMoney,money);

		vecType.push_back(eCharExp);
		vecType.push_back(eCharMoney);
		player->SynCharAttribute(vecType);

		//发送给抢劫者，劫镖成功
		toClient.set_res(0);
		toClient.set_exp(exp);
		toClient.set_money(money);
		toClient.set_name(m_owner->GetName());
		QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_SIM_GM2C_ESCORT_RESULT);

//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Escort,eStaMinor_Escort_RobEscortEvent,&costAndGotInfo);

		//发送给押镖者，被劫了
		toClient.set_res(eEscortRobed);
		toClient.set_exp(exp);
		toClient.set_money(money);
		toClient.set_name(player->GetName());
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&toClient,MSG_SIM_GM2C_ESCORT_RESULT);

		//设置复活定时器
		if (m_reflushTimer.IsValid())
		{
			m_reflushTimer.SetNull();
		}
		m_reflushTimer = TimerInterface::GetInstance()->RegTimePoint(&EscortMgr::Reflush, this, 0, 0, 0, 0, GameParaLoader::GetInstance()->getReflushEscortTime());
	}

	return true;
}

bool EscortMgr::MoveEscortData(const EventArgs & e)
{
	vector<int> v_Type;
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
	if (!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;

	if (player && player->GetID() == m_owner->GetID())
	{
		if (!m_carMon)
			return true;

		if (m_carMon->GetMapID() != m_owner->GetMapID())
		{
			return true;
		}

		if (!m_owner->IsInCreatureState(ERS_Escort))
		{
		   return true;
		}

		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
		if (entry)
		{
			NpcInfo info = NPCInfo::GetInstance()->GetNpcInfo(entry->m_nEndNpcID);

			if ((info.mapid == GET_MAP_ID(m_owner->GetMapID()))
					&& (info.mapid == GET_MAP_ID(m_carMon->GetMapID())))
			{
				int playerres = NPCInfo::GetInstance()->ExamineDistance(entry->m_nEndNpcID, m_owner->GetXPos(), m_owner->GetYPos());

				if (!playerres && player->IsInDistance(m_carMon,16))
				{

//					StatisticInfo::CostAndGotInfo costAndGotInfo;

					int exp = 0;
					int money = 0;
					EscortProto::EscortResult toClient;

					exp = GetExp();

//					StatisticInfo::GotItemInfo * gotExpInfo = costAndGotInfo.add_gotlist();

					m_owner->AddExp(exp);

//					StatisticMgr::AddItemGotInfo(gotExpInfo,eCharExp,exp);

//					StatisticInfo::GotItemInfo * gotMoneyInfo = costAndGotInfo.add_gotlist();

					money = GetMoney();
					m_owner->ChangeMoney(money,true);
					v_Type.push_back(eCharMoney);

//					if(money > 0)
//					{
//						StatisticInfo::CostAndGotInfo costAndGotInfo;
//						StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Money,eStaMinor_Money_Escort,&costAndGotInfo);
//					}

//					StatisticMgr::AddItemGotInfo(gotMoneyInfo,eCharMoney,money);

					m_owner->SynCharAttribute(v_Type);
					//必须Clear前获取品质
					int quality = this->GetQuality();
					Clear();
					m_owner->SetDataFlag(eEscortInfo);
					m_owner->UnsetCreatureState(ERS_Escort);

					toClient.set_res(0);
					toClient.set_exp(exp);
					toClient.set_money(money);

					GenenateEscortArgs e(m_owner->GetMyself(),quality);
					m_owner->FireEvent(PLAYER_GENENATE_ESCORT,e);

//					if(eAccountType_User == player->GetAccountType())
//						StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Escort,eStaMinor_Escort_EscortSucessEvent,&costAndGotInfo);

//					StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(),eStatic_Escort,eStaMinor_Escort_EscortSucess);

					QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(),m_owner->GetChannelID(),&toClient,MSG_SIM_GM2C_ESCORT_RESULT);

					return true;
				}
			}
		}
	}

	return true;
}

bool EscortMgr::EnterEscortData(const EventArgs & e)
{
	PlayerMapEventArgs *pArgs = (PlayerMapEventArgs *) (&e);
	Smart_Ptr<Player> player = pArgs->m_obj;

	if (player && player->GetID() == m_owner->GetID())
	{
		//用开始时间来判断是否有镖车
		if (m_startTime > 0 && m_mapid > 0)
		{
			DWORD oMapid = GET_MAP_ID(m_owner->GetMapID());
			DWORD eMapid = GET_MAP_ID(m_mapid);

			if (oMapid != eMapid)
			{
				//地图不相同，得判断是否是切换地图
				sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
				if (entry)
				{
					for (size_t i = 0; i < (entry->m_nByWayOfMap.size() - 1); i++)
					{
						if (eMapid == entry->m_nByWayOfMap[i])
						{
							int newindex = entry->m_nByWayOfMap[i+1];
							MapStartPoint * pointInfo;

							if ((DWORD)newindex != oMapid)
							{
								break;
							}

							pointInfo = CMapConfig::GetInstance()->GetTransferMap(eMapid, newindex);
							if (pointInfo == 0)
							{
								LOG_ERROR(FILEINFO, "MapStartPoint is error");

								return true;
							}

							if (pointInfo->tXPos != m_owner->GetXNewPos() || pointInfo->tYPos != m_owner->GetYNewPos())
							{
								break;
							}

							int tXpos = abs(pointInfo->xpos - m_xpos);
							int tYpos = abs(pointInfo->ypos - m_ypos);
							int tLen = (int)sqrt(tXpos * tXpos + tYpos * tYpos);

							if (tLen >= 14)
							{
								break;
							}

							//如果符合结果，那么就是玩家传送了，那么把镖车也传送过去，不然就在原地图那个点停留
							m_mapid = m_owner->GetMapID();
							m_xpos = m_owner->GetXNewPos();
							m_ypos = m_owner->GetYNewPos();
							m_owner->SetDataFlag(eEscortInfo);

							break;
						}
					}
				}
				else
					return true;
			}

			//如果死了，就让他复活定时器不走
			if (m_reflushTimer.IsValid())
			{
				m_reflushTimer.SetNull();
			}

			StartEscort(false);

			//发送状态信息
			EscortProto::EscortStateInfo stateInfo;

			stateInfo.set_line(m_line);
			stateInfo.set_quality(m_quality);
			stateInfo.set_robtimes(m_robTimes);
			stateInfo.set_time(GetTime());
			stateInfo.set_mapid(GET_MAP_ID(m_mapid));
			stateInfo.set_xpos(m_xpos);
			stateInfo.set_ypos(m_ypos);
			stateInfo.set_speedtime(GetSpeedTime());
			stateInfo.set_money(m_rewardMoney);
			stateInfo.set_exp(m_rewardExp);

			QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(),&stateInfo,MSG_SIM_GM2C_UPDATEESCORTINFO);
		}
	}

	return true;
}

bool EscortMgr::LeaveEscortData(const EventArgs & e)
{
	OneParmaEventArgs *pArgs = (OneParmaEventArgs *) (&e);

	if(pArgs->m_obj->GetType() != ePlayer)
		return true;

	Smart_Ptr<Player> player = pArgs->m_obj;
	if (player && player->GetID() == m_owner->GetID())
	{
		if (m_line > 0 && m_carMon)
		{
			Smart_Ptr<Monster> temp;

			m_mapid = m_carMon->GetMapID();
			m_xpos = m_carMon->GetXNewPos();
			m_ypos = m_carMon->GetYNewPos();
			m_hp = m_carMon->getAttManage()->getValue<int>(eCharHP);

			if (m_carMon)
			{
				m_carMon->SetDead();
			}

			m_carMon = temp;
			if (m_escortEnd.IsValid())
			{
				m_escortEnd.SetNull();
			}

			if (m_speedUpEnd.IsValid())
			{
				m_speedUpEnd.SetNull();
			}

			//如果死了，就让他复活定时器不走
			if (m_reflushTimer.IsValid())
			{
				m_reflushTimer.SetNull();
			}

			m_owner->SetDataFlag(eEscortInfo);

		}
	}

	return true;
}

int EscortMgr::GetAllExp()
{
	return 0;

	if (m_line <= 0)
		m_line = 1;

	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
	if (!entry)
	{
		return 0;
	}

	int lv = m_owner->getAttManage()->getValue<int>(eCharLv);
	float base = (pow(lv * 1.0,(entry->m_expRate1 /10.0)) + lv * entry->m_expRate2 + entry->m_expRate3) * entry->m_nQualityExp[m_quality];

	base = base / 100;
	base /= 10000;
	base = (int)(base);
	base *= 10000;

	return (int)base;
}

int EscortMgr::GetExp()
{
	return 0;

	int exp = 0;

	exp = getRewardExp();

	//扣除被抢经验
	if (m_robTimes > 0)
	{
		exp -= (int)((m_robTimes * 10 / 100.0f) * exp);
	}

	if (exp < 0)
		exp = 0;

	return exp;
}

int EscortMgr::GetMoney()
{
	int money = 0;

	money = getRewardMoney();

	//扣除被抢经验
	if (m_robTimes > 0)
	{
		money -= (int)((m_robTimes * 10 / 100.0) * money);
	}

	if (money < 0)
		money = 0;

	return money;
}

int EscortMgr::GetAllMoney()
{
	if (m_line <= 0)
		m_line = 1;

	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
	if (!entry)
	{
		return 0;
	}

	int lv = m_owner->getAttManage()->getValue<int>(eCharLv);
	float money =  (pow(lv * 1.0,(entry->m_money1 /10.0)) + lv * entry->m_money2 + entry->m_money3) * entry->m_nQualityExp[m_quality];  /*已最低的来算*/

	money = money /100;
	money /= 100;
	money = (int)(money);
	money *= 100;

	return (int)money;
}

int EscortMgr::ImediaComplete(int &exp,int& money)
{
	if(!m_owner->IsInCreatureState(ERS_Escort))
	{
		return eNotInEscorting;
	}

	vector<int> vecType;
	{
		//是否有足够的元宝
		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
		if (!entry)
		{
			return eNull;
		}

		DWORD price = entry->m_nImediaComplete;
		if (!m_owner->IsEnoughGolden(price))
		{
			return eNotEnoughGold;
		}

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());
//
//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

		int costGolden = 0;
		int costbGolden = 0;

		m_owner->DecreaseGoldenCost(price, costGolden, costbGolden);
		if(costGolden > 0)
			vecType.push_back(eCharGolden);
		if(costbGolden > 0)
			vecType.push_back(eCharBindGolden);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,costGolden,m_owner->GetMyself());
//
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,costbGolden,m_owner->GetMyself());
		// 统计元宝花费
//		StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_EscortImmediateComplete,1, price);

		//计算总共所得经验
		exp = GetExp();
		m_owner->AddExp(exp);

//		if(exp > 0)
//		{
//			StatisticInfo::GotItemInfo * gotExpInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotExpInfo,eCharExp,exp);
//		}

		money = GetMoney();
		m_owner->ChangeMoney(money,true);

//		if(money > 0)
//		{
//			StatisticInfo::GotItemInfo * gotMoneyInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotMoneyInfo,eCharMoney,money);
//		}

//		if(money > 0)
//		{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_Escort,&costAndGotInfo);
//		}

		vecType.push_back(eCharMoney);

		m_owner->SynCharAttribute(vecType);

		Clear();
		m_owner->SetDataFlag(eEscortInfo);
		m_owner->UnsetCreatureState(ERS_Escort);

//		if(eAccountType_User == m_owner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),
//				                                                 eStatic_Escort,eStaMinor_Escort_EscortImmediateCompleteEvent,
//				                                                 &costAndGotInfo);
	}

//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(),eStatic_Escort,eStaMinor_Escort_EscortSucess);

	return 0;
}

bool EscortMgr::SpeedUpEscort()
{
	if(!m_owner->IsInCreatureState(ERS_Escort))
	{
		return eNotInEscorting;
	}

	if(m_speedUpEnd.IsValid())
	{
		return eEscortIsSpeedingUP;
	}

	//是否有足够的元宝
	sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
	if(!entry)
	{
		return eNull;
	}

	if(!m_carMon)
	{
		return eNotInEscorting;
	}

	if(!m_carMon->GetBrain() || !m_carMon->GetBrain()->GetOwner())
	{
		return eNotInEscorting;
	}

	vector<int> vecType;

	int speedtimes = m_owner->GetCounterService()->GetNumById(ESCORT_SPEEDUP_COUNTER);
	DWORD price = (DWORD)(entry->m_nSpeedUpCost * pow(1.0f,speedtimes));

	if (!m_owner->IsEnoughGolden(price))
	{
		return eNotEnoughGold;
	}

	if (0 < price)
	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costInfo,eCharGolden,m_owner->GetMyself());
//
//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

		int costGolden = 0;
		int costbGolden = 0;

		m_owner->DecreaseGoldenCost(price, costGolden, costbGolden);
		if(costGolden > 0)
			vecType.push_back(eCharGolden);
		if(costbGolden > 0)
			vecType.push_back(eCharBindGolden);

		m_owner->SynCharAttribute(vecType);

//		StatisticMgr::SetItemCostAfter(costInfo,eCharGolden,costGolden,m_owner->GetMyself());
//
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,costbGolden,m_owner->GetMyself());
//
//		if(eAccountType_User == m_owner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),
//																																eStatic_Escort,eStaMinor_Escort_SpeedUpEvent,
//																																&costAndGotInfo);
	}

	//启动加速定时器
	int speed = m_carMon->getAttManage()->getValue<int>(eCharMoveSpeed);

	speed = speed * entry->m_nSpeedUpRate;
	m_carMon->getAttManage()->setValue(eCharMoveSpeed,speed);
	m_carMon->GetBrain()->ClearPathAndTime();

	m_speedTime = CUtil::GetNowSecond();
	if(m_speedUpEnd.IsValid())
	{
		m_speedUpEnd.SetNull();
	}

	m_speedUpEnd = TimerInterface::GetInstance()->RegTimePoint(&EscortMgr::EndSpeedUp, this, 0, 0, 0, 0, entry->m_nSpeedUpTime);
	m_owner->GetCounterService()->IncreasePlayerCounter(ESCORT_SPEEDUP_COUNTER,1);
	m_owner->SetDataFlag(eEscortInfo);

	return 0;
}

int EscortMgr::AbandonEscort()
{
	if (!m_owner->IsInCreatureState(ERS_Escort))
	{
	   return eNotInEscorting;
	}

	Clear();
	m_owner->SetDataFlag(eEscortInfo);
	m_owner->UnsetCreatureState(ERS_Escort);
//
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(),eStatic_Escort,eStaMinor_Escort_GiveUpEscort,1);
//
//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Escort,eStaMinor_Escort_GiveUpEscortEvent);

	return eEscortFailed;
}

int EscortMgr::GetTime()
{
	int time = 0; //CUtil::GetNowSecond() - m_startTime;

	if (m_owner->IsInCreatureState(ERS_Escort))
	{
		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
		if(entry )
		{
			time = entry->m_nLimitTime - (CUtil::GetNowSecond() - m_startTime);
			if(time < 0)
				time = 0;
		}
	}

	return time;
}

int EscortMgr::GetSpeedTime()
{
	int time = 0;

	if (m_owner->IsInCreatureState(ERS_Escort) && m_speedTime > 0)
	{
		sEscortEntry *entry = EscortLoader::GetInstance()->GetEscortEntry(m_line);
		if (entry )
		{
			time = entry->m_nSpeedUpTime - (CUtil::GetNowSecond() - m_speedTime);
			if (time < 0)
				time = 0;
		}
	}

	return time;
}

int EscortMgr::RecoverResource(int num,Daily::RecoverResourceInfo *info)
{
	sEscortEntry * entry = EscortLoader::GetInstance()->GetEscortEntry(No1EscortID);
	if (!entry)
	{
		return eNull;
	}

	int haveNum = entry->m_nDayTimes - num;
	if (haveNum <=0)
		return eEscortTimesOver;

	sRecorceSourceParam *ptr = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(eRecorceSourceParamType_escort);
	if (!ptr)
	{
		return eNull;
	}

	int exp = CalAddExp(haveNum, m_owner->getAttManage()->getValue<int>(eCharLv), entry);

	int money = CalAddMoney(haveNum, m_owner->getAttManage()->getValue<int>(eCharLv), entry);

	Daily::RecoverResourceReward * reward = info->mutable_reward();
	reward->set_money(money);
	reward->set_exp(exp);
	info->set_restcount(haveNum);
	info->set_id(eRecorceSourceParamType_escort);

	return 0;
}

int EscortMgr::CalAddExp(const int& haveNum,const int &lv, const sEscortEntry* entry)
{
	return 0;

	if (!entry)
	{
		return 0;
	}

	float exp = GameParaLoader::GetInstance()->GetEscortReward(lv,entry->m_expRate1,entry->m_expRate2,entry->m_expRate3,entry->m_nQualityExp[0]);
//  exp = (pow(lv * 1.0,(entry->m_expRate1 /10.0)) + lv * entry->m_expRate2 + entry->m_expRate3) * entry->m_nQualityExp[0];  /*已最低的来算*/

	exp = exp / 100;
	exp /= 10000;
	exp = (int)(exp + 0.5);
	exp *= 10000;

	return (int)(exp *haveNum);
}

int EscortMgr::CalAddMoney(const int& haveNum,const int &lv, const sEscortEntry* entry)
{
	if (!entry)
	{
		return 0;
	}

	float money = GameParaLoader::GetInstance()->GetEscortReward(lv,entry->m_money1,entry->m_money2,entry->m_money3,entry->m_nQualityExp[0]);
//	money =  (pow(lv * 1.0,(entry->m_money1 /10.0)) + lv * entry->m_money2 + entry->m_money3) * entry->m_nQualityExp[0];  /*已最低的来算*/

	money = money /100;
	money /= 100;
	money = (int)(money + 0.5);
	money *= 100;

	return (int)(money * haveNum);
}

Smart_Ptr<Monster> EscortMgr::GetEscortMonster()
{
	return m_carMon;
}

