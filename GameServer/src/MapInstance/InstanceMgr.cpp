/*
 * InstanceMgr.cpp
 *
 *  Created on: 2015年1月5日
 *      Author: root
 */

#include "InstanceMgr.h"
#include <math.h>
#include "util.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "InstanceMapLoader.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../Daily/DailyLoader.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "../Object/FileLoader/DropItemLoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "../ServerEventDefine.h"
#include "../Attribute/AttributeCreature.h"
#include "./FileLoader/GameParaLoader.h"
#include "MoneyInstanceDefine.h"
#include "MoneyInstanceMgr.h"
#include "../Map/DuplicateMgr.h"
#include "ChallengeMgr.h"
#include "PlayerChallenge.h"
#include "FileLoader/ActivitiesTimerLoader.h"
#include "FileLoader/ArenaLoader.h"
#include "FileLoader/UpgradeLoader.h"

InstanceMgr::InstanceMgr(Player* player):m_owner(player)
{
	m_Instance.clear();
	m_YesterdayInstance.clear();
	m_InstanceElite.clear();
	m_InstanceCauchemar.clear();
	m_InstanceAdvanced.clear();
	m_InstanceExp.clear();
	m_InstanceMoney.clear();
}

InstanceMgr::~InstanceMgr()
{
	this->ReleaseInstanceMgr();
}

void InstanceMgr::ReleaseInstanceMgr()
{
	m_Instance.clear();
	m_YesterdayInstance.clear();
	m_InstanceVip.clear();

	m_MoneyInstance.Reset();
	m_GroupInstance.clear();
	m_InstanceElite.clear();
	m_InstanceCauchemar.clear();
	m_InstanceAdvanced.clear();
	m_InstanceExp.clear();
	m_InstanceMoney.clear();
}

//保存铜钱副本数据
void InstanceMgr::SetMoneyInstanceInfo(PlayerInfo::MoneyInstance *instanceInfo)
{
	instanceInfo->set_mapid(m_MoneyInstance.mapId);
	instanceInfo->set_num(m_MoneyInstance.num);
	instanceInfo->set_buytimes(m_MoneyInstance.buytimes);
	instanceInfo->set_updatetime(m_MoneyInstance.updatetime);
}
//初始化铜钱副本数据
void InstanceMgr::InitMoneyInstanceInfo(const PlayerInfo::MoneyInstance& instanceInfo)
{
	m_MoneyInstance.mapId = instanceInfo.mapid();
	m_MoneyInstance.num = instanceInfo.num();
	m_MoneyInstance.buytimes = instanceInfo.buytimes();
	m_MoneyInstance.pass = instanceInfo.pass();
	m_MoneyInstance.reset = instanceInfo.reset();
	m_MoneyInstance.updatetime = instanceInfo.updatetime();

	if(m_MoneyInstance.mapId <= 0)
	{
		const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
		m_MoneyInstance.mapId = config.mapID;
	}

	if(isMoneyInstanceUpdate(m_MoneyInstance.updatetime))
	{
		ResetMoneyInstance();
	}
}

bool InstanceMgr::isMoneyInstanceUpdate(DWORD64 saveTime)
{
	  int hour=0,minute=0,seconds=0;
		GameParaLoader::GetInstance()->getDayUpdateTime(hour,minute,seconds);
			//清零时间
			time_t timep;
			struct tm p;
			time(&timep);

			localtime_r(&timep,&p);
			p.tm_hour = 0;
			p.tm_min = 0;
			p.tm_sec = 0;
			timep=mktime(&p) ;
			DWORD64 clearTime=(DWORD64)timep;
			clearTime = (clearTime+hour*60*60+minute*60)*1000;

			//时间为0时，必须清
			if(saveTime <= 0)
			{
				return true;
			}

			//当前时间
			DWORD64 curTime=CUtil::GetNowSecond();
			if(clearTime>saveTime && curTime>clearTime)
				return true;

			return false;
}

void InstanceMgr::ResetMoneyInstance()
{
		m_MoneyInstance.num = 0;
		m_MoneyInstance.buytimes = 0;
		m_MoneyInstance.updatetime = CUtil::GetNowSecond();
		m_owner->SetDataFlag(eMoneyInstanceInfo);
}

int InstanceMgr::checkMoneyInstanceEnter(int& surplusNum)
{
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(m_MoneyInstance.mapId);
		if(entry==NULL)
		{
			return -1;
		}

		//次数检测
		int total = (int)entry->m_nReaptTimes + m_MoneyInstance.buytimes;
		if(m_MoneyInstance.num >= total)
		{
			return eInstanceTimesNotEnough;
		}

		surplusNum = total - m_MoneyInstance.num;

		return 0;
}

void InstanceMgr::EnterMoneyInstance()
{
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(m_MoneyInstance.mapId);
		if(entry==NULL)
		{
				return;
		}

		//次数检测
		int total = (int)entry->m_nReaptTimes + m_MoneyInstance.buytimes;
		if( m_MoneyInstance.num < total )
		{
				m_MoneyInstance.num++;
				m_owner->SetDataFlag(eMoneyInstanceInfo);

//				m_owner->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_Practice, eUIHLSmall_Practice_HaveCanChallenge), 1, false);
				HintValue value;
				int maxCount = 0, maxBuyCount = 0, leftCount = 0, buyCount = 0;
				m_owner->GetInstanceMgr()->getMoneyInstanceCount(maxCount, maxBuyCount, leftCount, buyCount);
				value.fValue = leftCount; // 剩余挑战次数
				value.sValue = maxCount + buyCount; // 总次数
				m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, value, UIHLType(eBig_MoneyInst, eSmall_MoneyInst_AlreadyFini));
		}
		else
		{
			LOG_ERROR(FILEINFO,"enter moneyIntanceId limit error: charid=%lld",m_owner->GetID());
		}
}

void InstanceMgr::getMoneyInstanceCount(int &maxCount, int &maxBuyCount, int &leftCount, int &buyCount)
{
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(m_MoneyInstance.mapId);
		if(entry==NULL)
		{
				return;
		}

		const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
		int total = config.maxCount;
//		int times = (int)m_owner->GetVipPart()->GetValueByRight(eVipMoneyInstAdd);
//		if(times != VIP_NOT_HAVE_THIS_RIGHT && times > 0)
//		{
//			total += times;
//		}
		maxCount =	(int)entry->m_nReaptTimes;
		maxBuyCount = total;
		buyCount = m_MoneyInstance.buytimes > maxBuyCount ? maxBuyCount : m_MoneyInstance.buytimes;
		leftCount = maxCount+m_MoneyInstance.buytimes > m_MoneyInstance.num ? maxCount+m_MoneyInstance.buytimes-m_MoneyInstance.num : 0;
}

int InstanceMgr::getMoneyInstanceBuyCount()
{
	return m_MoneyInstance.buytimes;
}

void InstanceMgr::addMoneyInstanceBuyCount()
{
	m_MoneyInstance.buytimes++;
}

void InstanceMgr::SetInstanceInfo(PlayerInfo::InstanceInfoTwoDays *instanceInfo)
{
	PlayerInfo::InstanceInfoList* iList = instanceInfo->mutable_todayinfo();
	map<int, sInstanceDiffculty>::iterator it;
	for(it = m_Instance.begin(); it != m_Instance.end(); ++it)
	{
		PlayerInfo::InstanceDiffculty * instanceDiffculty = iList->add_diffcultyinfo();
		it->second.SetProto(*instanceDiffculty);
	}

	map<int, PlayerInfo::InstanceVip>::iterator vitr;
	for(vitr = m_InstanceVip.begin(); vitr != m_InstanceVip.end(); ++vitr)
	{
		PlayerInfo::InstanceVip * instanceVip = iList->add_vipinfo();
		instanceVip->CopyFrom(vitr->second);
	}

	for(it = m_InstanceExp.begin(); it != m_InstanceExp.end(); ++it)
	{
		PlayerInfo::InstanceDiffculty * instanceDiffculty = iList->add_expinfo();
		it->second.SetProto(*instanceDiffculty);
	}

	for(it = m_InstanceMoney.begin(); it != m_InstanceMoney.end(); ++it)
	{
		PlayerInfo::InstanceDiffculty * instanceDiffculty = iList->add_moneyinfo();
		it->second.SetProto(*instanceDiffculty);
	}

	PlayerInfo::InstanceInfoList* yesterdayinfo = instanceInfo->mutable_yesterdayinfo();
	for(it = m_YesterdayInstance.begin(); it != m_YesterdayInstance.end(); ++it)
	{
		PlayerInfo::InstanceDiffculty * instanceDiffculty = yesterdayinfo->add_diffcultyinfo();
		it->second.SetProto(*instanceDiffculty);
	}
}

void InstanceMgr::InitInstanceInfo(const PlayerInfo::InstanceInfoTwoDays &instanceInfo)
{
	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();

	PlayerInfo::InstanceInfoList info = instanceInfo.todayinfo();

	for(int i = 0; i < info.diffcultyinfo_size(); i++)
	{
		PlayerInfo::InstanceDiffculty *diffcultyInfo = info.mutable_diffcultyinfo(i);

		if(tempType != eDayResetToday)
		{
			if(eDayResetYesterday == tempType)//上一次登录是昨天，玩家今天第一次登录m_YesterdayInstanceSweep直接使用昨天保存的副本信息
			{
				map<int,sInstanceDiffculty>::iterator it = m_YesterdayInstance.find(diffcultyInfo->mapid());
				if(it != m_YesterdayInstance.end())
				{
					it->second = sInstanceDiffculty(diffcultyInfo);
				}
				else
				{
					m_YesterdayInstance[diffcultyInfo->mapid()] = sInstanceDiffculty(diffcultyInfo);
				}
			}

			diffcultyInfo->set_num(0);
			diffcultyInfo->set_time(0);
			diffcultyInfo->set_reset(0);
		}

		map<int, sInstanceDiffculty>::iterator it = m_Instance.find(diffcultyInfo->mapid());
		if(it != m_Instance.end())
		{
			it->second = sInstanceDiffculty(diffcultyInfo);;
		}
		else
		{
			m_Instance[diffcultyInfo->mapid()] = sInstanceDiffculty(diffcultyInfo);
		}
	}

	PlayerInfo::InstanceInfoList yesterdayInfo = instanceInfo.yesterdayinfo();
	if(eDayResetToday == tempType)//今天已经登陆过，那么今天第一次登录时数据库或数据库缓存里已有m_YesterdayInstanceSweep所需要的数据
	{
		for(int i = 0; i < yesterdayInfo.diffcultyinfo_size(); i++)
		{
			PlayerInfo::InstanceDiffculty diffcultyInfo = yesterdayInfo.diffcultyinfo(i);

			m_YesterdayInstance[diffcultyInfo.mapid()] = sInstanceDiffculty(&diffcultyInfo);
		}
	}

	for(int j = 0; j < info.vipinfo_size(); j++)
	{
		PlayerInfo::InstanceVip * instanceVip = info.mutable_vipinfo(j);
		if(tempType != eDayResetToday)
		{
			instanceVip->set_num(0);
			instanceVip->set_pass(false);
		}

		map<int, PlayerInfo::InstanceVip>::iterator it = m_InstanceVip.find(instanceVip->mapid());
		if(it != m_InstanceVip.end())
		{
			it->second.CopyFrom(*instanceVip);
		}
		else
		{
			m_InstanceVip[instanceVip->mapid()] = *instanceVip;
		}
	}

	for(int i = 0; i < info.expinfo_size(); i++)
	{
		PlayerInfo::InstanceDiffculty *diffcultyInfo = info.mutable_expinfo(i);

		if(tempType != eDayResetToday)
		{
			if(eDayResetYesterday == tempType)//上一次登录是昨天，玩家今天第一次登录m_YesterdayInstanceSweep直接使用昨天保存的副本信息
			{
				map<int,sInstanceDiffculty>::iterator it = m_YesterdayInstance.find(diffcultyInfo->mapid());
				if(it != m_YesterdayInstance.end())
				{
					it->second = sInstanceDiffculty(diffcultyInfo);
				}
				else
				{
					m_YesterdayInstance[diffcultyInfo->mapid()] = sInstanceDiffculty(diffcultyInfo);
				}
			}

			diffcultyInfo->set_num(0);
			diffcultyInfo->set_time(0);
			diffcultyInfo->set_reset(0);
		}

		m_InstanceExp[diffcultyInfo->mapid()] = sInstanceDiffculty(diffcultyInfo);
	}

	for(int i = 0; i < info.moneyinfo_size(); i++)
	{
		PlayerInfo::InstanceDiffculty *diffcultyInfo = info.mutable_moneyinfo(i);

		if(tempType != eDayResetToday)
		{
			if(eDayResetYesterday == tempType)//上一次登录是昨天，玩家今天第一次登录m_YesterdayInstanceSweep直接使用昨天保存的副本信息
			{
				map<int,sInstanceDiffculty>::iterator it = m_YesterdayInstance.find(diffcultyInfo->mapid());
				if(it != m_YesterdayInstance.end())
				{
					it->second = sInstanceDiffculty(diffcultyInfo);
				}
				else
				{
					m_YesterdayInstance[diffcultyInfo->mapid()] = sInstanceDiffculty(diffcultyInfo);
				}
			}

			diffcultyInfo->set_num(0);
			diffcultyInfo->set_time(0);
			diffcultyInfo->set_reset(0);
		}

		m_InstanceMoney[diffcultyInfo->mapid()] = sInstanceDiffculty(diffcultyInfo);
	}
}

void InstanceMgr::ResetInstance()
{
	m_YesterdayInstance.clear();
	m_YesterdayInstance = m_Instance;

	map<int,sInstanceDiffculty>::iterator itr = m_Instance.begin();
	for(; itr != m_Instance.end(); ++itr)
	{
		itr->second.ReSet();
	}

	map<int,sInstanceDiffculty>::iterator itrExp = m_InstanceExp.begin();
	for(; itrExp != m_InstanceExp.end(); ++itrExp)
	{
		m_YesterdayInstance[itrExp->first] = itrExp->second;

		itrExp->second.ReSet();
	}

	map<int,sInstanceDiffculty>::iterator itrMoney = m_InstanceMoney.begin();
	for(; itrMoney != m_InstanceMoney.end(); ++itrMoney)
	{
		m_YesterdayInstance[itrMoney->first] = itrMoney->second;
		itrMoney->second.ReSet();
	}

	m_InstanceVip.clear();
	m_owner->SetDataFlag(eInstanceInfo);

	m_owner->GetChallenge()->Reset();

	ResetMoneyInstance();

	map<int,sInstanceDiffculty>::iterator itrElite = m_InstanceElite.begin();
	for(; itrElite != m_InstanceElite.end(); ++itrElite)
	{
		m_YesterdayInstance[itrElite->first] = itrElite->second;
	}

	ResetEliteInstance();

	map<int,sInstanceDiffculty>::iterator itrCauchemar = m_InstanceCauchemar.begin();
	for(; itrCauchemar != m_InstanceCauchemar.end(); ++itrCauchemar)
	{
		m_YesterdayInstance[itrCauchemar->first] = itrCauchemar->second;
	}

	ResetCauchemarInstance();

	ResetGroupInstance();

	map<int,sInstanceDiffculty>::iterator itrAdvanced = m_InstanceAdvanced.begin();
	for(; itrAdvanced != m_InstanceAdvanced.end(); ++itrAdvanced)
	{
		m_YesterdayInstance[itrAdvanced->first] = itrAdvanced->second;
	}

	ResetAdvancedInstance();
}

int InstanceMgr::ResetInstance(int sceneId)
{
	vector<int> vec;
	int vipTimes = 0;

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::ResetInstance: intanceId is invalid %d.",sceneId);
		return eInstanceInfoError;
	}

	switch(entry->m_eType)
	{
	case eAdvancedInstance:
		{
			map<int,sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
			if(it != m_Instance.end())
			{
				if(m_owner->GetVipPart())
				{
					double times = m_owner->GetVipPart()->GetValueByRight(eVipAdvancedRestFreeCnt);
					if (times != VIP_NOT_HAVE_THIS_RIGHT)
					{
						vipTimes = (int)times + entry->m_nResetNum;
					}
					else
					{
						vipTimes = entry->m_nResetNum;
					}
				}

				if (vipTimes <= it->second.reset)
				{
					return eInstanceResetCntEnougth;
				}

				if(!(m_owner->CostGolden(entry->m_nResetPurchase)))
				{
					return eNotEnoughGold;
				}

				it->second.num = 0;
				it->second.time = 0;
				it->second.reset = it->second.reset + 1;

				if(entry->m_nResetPurchase > 0)
					vec.push_back(eCharGolden);

				m_owner->SynCharAttribute(vec);
				m_owner->SetDataFlag(eAdvancedInstanceInfo);
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
	default:
		{
			return eInstanceTypeError;
		}
	}

	ResetAllInstArgs e(m_owner->GetMyself(), eAdvancedInstance);
	m_owner->FireEvent(PLAYER_RESET_ALLINSTANCE, e);

	return 0;
}

void InstanceMgr::ResetAllInstance(int type,int& code)
{
	//0单人副本 ； 1组队副本 ； 2挑战副本
	code = 0;
	int vipTimes = 0;
	vector<int> vec;
	switch(type)
	{
		case ePersonalInstance:
		case eEliteInstance:
		case eCauchemarInstance:
		case eExpInstance:
		case eMoneyInstance1:
		{
			if(type == eExpInstance && m_owner->GetVipPart())
			{
				double times = m_owner->GetVipPart()->GetValueByRight(eVipExpRestFreeCnt);
				if (times != VIP_NOT_HAVE_THIS_RIGHT)
				{
					vipTimes = (int)times;
				}
			}

			DWORD nReset = getResetTime(type);
			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntryByType((InstanceType)type);
			if(entry==NULL)
			{
				code = eInstanceInfoError;
				return;
			}

			if(nReset >=  (entry->m_nResetNum + vipTimes))
			{
				code = eInstanceResetCntEnougth;
				return;
			}

			if(!m_owner->CostGolden(entry->m_nResetPurchase))
			{
				code = eNotEnoughGold;
				return;
			}

			vec.push_back(eCharGolden);

			if(type == ePersonalInstance)
			{
				map<int,sInstanceDiffculty>::iterator itr = m_Instance.begin();
				for(; itr != m_Instance.end(); ++itr)
				{
					itr->second.num = 0;
					itr->second.time = 0;
					itr->second.reset = itr->second.reset + 1;
				}
			}
			else if(type == eEliteInstance)
			{
				map<int,sInstanceDiffculty>::iterator itrElite = m_InstanceElite.begin();
				for(; itrElite != m_InstanceElite.end(); ++itrElite)
				{
					itrElite->second.num = 0;
					itrElite->second.time = 0;
					itrElite->second.reset = itrElite->second.reset + 1;
				}
			}
			else if(type == eCauchemarInstance)
			{
				map<int,sInstanceDiffculty>::iterator itrCauchemar = m_InstanceCauchemar.begin();
				for(; itrCauchemar != m_InstanceCauchemar.end(); ++itrCauchemar)
				{
					itrCauchemar->second.num = 0;
					itrCauchemar->second.time = 0;
					itrCauchemar->second.reset = itrCauchemar->second.reset + 1;
				}
			}
			else if(type == eExpInstance)
			{
				map<int,sInstanceDiffculty>::iterator itr = m_InstanceExp.begin();
				for(; itr != m_InstanceExp.end(); ++itr)
				{
					itr->second.num = 0;
					itr->second.time = 0;
					itr->second.reset = itr->second.reset + 1;
				}
			}
			else
			{
				map<int,sInstanceDiffculty>::iterator itr = m_InstanceMoney.begin();
				for(; itr != m_InstanceMoney.end(); ++itr)
				{
					itr->second.num = 0;
					itr->second.time = 0;
					itr->second.reset = itr->second.reset + 1;
				}
			}

			break;
		}
	}

	if(vec.size() > 0)
	{
		m_owner->SynCharAttribute(vec);
	}

	ResetAllInstArgs e(m_owner->GetMyself(), type);
	m_owner->FireEvent(PLAYER_RESET_ALLINSTANCE, e);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	//0单人副本 ； 1组队副本 ； 2挑战副本
//	code = 0;
//	int golden = 0;
//	int bgolden = 0;
//	if(m_timerID.IsValid())
//	{
//		code = eInInstanceSweep;
//		return;
//	}
//
//	if(type<0||type>2)
//	{
//		code = eNotPassInstance;
//		return;
//	}
//
//	//
//	int vipSingleTimes = 0, vipGroupTimes = 0, vipChallengeTimes = 0;
//	if(m_owner->GetVipPart())
//	{
//		double times = m_owner->GetVipPart()->GetValueByRightWithoutCard(eVipSingleInstResetFreeCnt) +  m_owner->GetVipPart()->GetValueByRightWithoutCard(eVipSingleInstAdd);
//		if (times != VIP_NOT_HAVE_THIS_RIGHT)
//		{
//			vipSingleTimes = (int)times;
//		}
//
//		times = m_owner->GetVipPart()->GetValueByRight(eVipTeamInstResetFreeCnt);
//		if (times != VIP_NOT_HAVE_THIS_RIGHT)
//		{
//			vipGroupTimes = (int)times;
//		}
//
//		times = m_owner->GetVipPart()->GetValueByRight(eVipChallInstRestFreeCnt);
//		if(times != VIP_NOT_HAVE_THIS_RIGHT)
//		{
//			vipChallengeTimes = (int)times;
//		}
//	}
//
//	int cost=0;
//	DWORD resetimes=0;
//	map<int,sInstanceDiffculty>::iterator it = m_Instance.begin();
//	for(; it!=m_Instance.end(); ++it)
//	{
//			//挑战副本不走这里
//			if(type==2)
//			{
//				break;
//			}
//
//			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(it->first);
//			if(entry==NULL)
//			{
//					continue;
//			}
//
//			//个人
//			if(type==0 && entry->m_eType==ePersonalInstance && entry->m_nResetPurchase>0 && it->second.reset >= m_owner->GetVipPart()->GetValueByRightWithoutCard(eVipSingleInstResetFreeCnt))
//			{
//				resetimes = it->second.reset - m_owner->GetVipPart()->GetValueByRightWithoutCard(eVipSingleInstResetFreeCnt);
//				cost = entry->m_nResetPurchase * (((int)pow(static_cast<float>(2), (int)resetimes)));
//
//				if (vipSingleTimes <= (int)resetimes)
//				{
//					code = eInstanceResetCntEnougth;
//					return;
//				}
//				break;
//			}
//			//组队
//			if(type==1 && entry->m_eType==eGroupInstance && entry->m_nResetPurchase>0)
//			{
//				resetimes = it->second.reset;
//				cost = entry->m_nResetPurchase * (((int)pow(static_cast<float>(2), (int)resetimes)));
//				if((int)resetimes >= vipGroupTimes)
//				{
//					code = eInstanceResetCntEnougth;
//					return;
//				}
//
//				break;
//			}
//	}
//
//	//挑战
//	if(type==2)
//	{
//		int layer = m_owner->GetChallenge()->GetCurLayer();
//		if(layer > 1)
//		{
//			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(CHALLEGEMAPFIRSTID);
//			if(entry==NULL)
//			{
//				return;
//			}
//
//			if(m_owner->GetCounterService())
//			{
//				int resettimes = m_owner->GetCounterService()->GetNumById(CHALLEGE_INSTANCE_COUNTER);
//				if(resettimes >= vipChallengeTimes)
//				{
//					code = eInstanceResetCntEnougth;
//					return;
//				}
//
//				cost = entry->m_nResetPurchase * (((int)pow(static_cast<float>(2), resettimes)));
//			}
//		}
//		else
//		{
//			code = eNotPassInstance;
//			return;
//		}
//	}
//
//	if(type==0)
//	{
//		if(!m_owner->CostGolden(cost))
//		{
//			code = eNotEnoughGold;
//			return;
//		}
//	}
//	else if(!m_owner->IsEnoughGolden(cost))
//	{
//			code = eNotEnoughGold;
//			return;
//	}
//
//	//重置
//	map<int,sInstanceDiffculty>::iterator itr = m_Instance.begin();
//	for(; itr != m_Instance.end(); ++itr)
//	{
//			if(type==2)
//			{
//				break;
//			}
//			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(itr->first);
//			if(entry==NULL)
//			{
//				continue;
//			}
//			//个人或者组队
//			if((type==0 && entry->m_eType==ePersonalInstance)||
//					(type==1 && entry->m_eType==eGroupInstance))
//			{
//				itr->second.num = 0;
//				itr->second.time = 0;
//				itr->second.reset = itr->second.reset + 1;
//			}
//	}
//
//	if(type==0)
//	{
//		map<int,sInstanceDiffculty>::iterator itrElite = m_InstanceElite.begin();
//		for(; itrElite != m_InstanceElite.end(); ++itrElite)
//		{
//			itrElite->second.num = 0;
//			itrElite->second.time = 0;
//			itrElite->second.reset = itrElite->second.reset + 1;
//		}
//
//		map<int,sInstanceDiffculty>::iterator itrCauchemar = m_InstanceCauchemar.begin();
//		for(; itrCauchemar != m_InstanceCauchemar.end(); ++itrCauchemar)
//		{
//			itrCauchemar->second.num = 0;
//			itrCauchemar->second.time = 0;
//			itrCauchemar->second.reset = itrCauchemar->second.reset + 1;
//		}
//	}
//
//	//挑战
//	if(type==2)
//	{
//		//设置成第一张挑战地图
//		m_owner->GetChallenge()->Reset();
//		//挑战重置的次数加1
//		m_owner->GetCounterService()->IncreasePlayerCounter(CHALLEGE_INSTANCE_COUNTER,1);
//	}
//
//	m_owner->SetDataFlag(eInstanceInfo);
//
//	if(cost > 0)
//	{
//		vector<int> vec;
////		StatisticInfo::CostAndGotInfo costAndGotInfo;
////		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
////		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());
////
////		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
////		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());
//
//		if(type == 0)
//		{
//			golden = cost;
//		}
//		else
//		{
//			m_owner->DecreaseGoldenCost(cost,golden,bgolden);
//		}
//
//		if(golden > 0)
//			vec.push_back(eCharGolden);
//		if(bgolden > 0)
//			vec.push_back(eCharBindGolden);
//
//		m_owner->SynCharAttribute(vec);
////		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,golden,m_owner->GetMyself());
////		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,bgolden,m_owner->GetMyself());
////		if(eAccountType_User == m_owner->GetAccountType())
////			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Instance,eStaMinor_Instance_ResetAllEvent,&costAndGotInfo);
//	}
//
//	// 统计元宝花费 重置所有副本
////	StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_ResetAllInstance,1, cost);
//
//	ResetAllInstArgs e(m_owner->GetMyself(), type);
//	m_owner->FireEvent(PLAYER_RESET_ALLINSTANCE, e);
}



void InstanceMgr::AddPassInstance(int sceneId, int num,int type, bool isPass)
{
	switch(type)
	{
	case ePersonalInstance:
//	case eMarryInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_Instance.find(sceneId);
			if(it != m_Instance.end())
			{
				if(isPass)
				{
					it->second.pass = isPass;
				}

				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = isPass;
				instanceDiffculty.num = num;
				instanceDiffculty.time = 0;
				instanceDiffculty.reset = 0;
				m_Instance[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
	case eVipInstance:
		{
			map<int, PlayerInfo::InstanceVip>::iterator it = m_InstanceVip.find(sceneId);
			if(it != m_InstanceVip.end())
			{
				it->second.set_pass(isPass);
				int newnum = it->second.num() + num;
				it->second.set_num(newnum);
			}
			else
			{
				PlayerInfo::InstanceVip instanceVip;
				instanceVip.set_mapid(sceneId);
				instanceVip.set_pass(isPass);
				instanceVip.set_num(num);
				m_InstanceVip[sceneId] = instanceVip;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
	case eGroupInstance:
		{
			map<int, GroupInstance>::iterator it = m_GroupInstance.find(sceneId);
			if(it != m_GroupInstance.end())
			{
				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				GroupInstance groupInstance;
				groupInstance.mapId = sceneId;
				groupInstance.num = num;
				m_GroupInstance[sceneId] = groupInstance;
			}

			m_owner->SetDataFlag(eGroupInstanceInfo);
			break;
		}
	case eEliteInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceElite.find(sceneId);
			if(it != m_InstanceElite.end())
			{
				if(isPass)
				{
					it->second.pass = isPass;
				}

				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = isPass;
				instanceDiffculty.num = num;
				instanceDiffculty.time = 0;
				instanceDiffculty.reset = 0;
				m_InstanceElite[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eEliteInstanceInfo);
			break;
		}
	case eCauchemarInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceCauchemar.find(sceneId);
			if(it != m_InstanceCauchemar.end())
			{
				if(isPass)
				{
					it->second.pass = isPass;
				}

				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = isPass;
				instanceDiffculty.num = num;
				instanceDiffculty.time = 0;
				instanceDiffculty.reset = 0;
				m_InstanceCauchemar[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eCauchemarInstanceInfo);
			break;
		}
		case eAdvancedInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
			if(it != m_InstanceAdvanced.end())
			{
				if(isPass)
				{
					it->second.pass = isPass;
				}

				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = isPass;
				instanceDiffculty.num = num;
				instanceDiffculty.time = 0;
				instanceDiffculty.reset = 0;
				m_InstanceAdvanced[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eAdvancedInstanceInfo);
			break;
		}
		case eExpInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceExp.find(sceneId);
			if(it != m_InstanceExp.end())
			{
				if(isPass)
				{
					it->second.pass = isPass;
				}

				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = isPass;
				instanceDiffculty.num = num;
				instanceDiffculty.time = 0;
				instanceDiffculty.reset = 0;
				m_InstanceExp[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
		case eMoneyInstance1:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceMoney.find(sceneId);
			if(it != m_InstanceMoney.end())
			{
				if(isPass)
				{
					it->second.pass = isPass;
				}

				int newnum = it->second.num + num;
				it->second.num = newnum;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = isPass;
				instanceDiffculty.num = num;
				instanceDiffculty.time = 0;
				instanceDiffculty.reset = 0;
				m_InstanceMoney[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
	}

//	if (1 == num)
//	{
//		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_CanChallenge), 1, false);
//	}
}

int InstanceMgr::GetLeftValidTimes(int sceneId)
{
	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::GetLeftValidTimes: intanceId is invalid %d.",sceneId);
		return -1;
	}

	int vipTimes = 0;
	switch(entry->m_eType)
	{
	case eChallenge:
	{
		return 1;
	}
	break;

	case ePersonalInstance:
		{
			int nNum = 0;
			map<int,sInstanceDiffculty>::iterator iter = m_Instance.begin();
			for(; iter!=m_Instance.end(); ++iter)
			{
				nNum += iter->second.num;
			}

			int leftTimes = entry->m_nReaptTimes - nNum;

			return leftTimes;

			break;
		}
	case eGroupInstance:
		{
			map<int,GroupInstance>::iterator itr = m_GroupInstance.find(sceneId);
			if(itr != m_GroupInstance.end())
			{
				return entry->m_nReaptTimes + itr->second.buynum - itr->second.num;
			}
			else
			{
				return entry->m_nReaptTimes;
			}

			break;
		}
	case eMarryInstance:
	{
		//先查找有无该副本，如无找出一个已经打过的计算次数
		map<int,sInstanceDiffculty>::iterator it = m_Instance.find(sceneId);
		if(it != m_Instance.end())
		{
			int leftTimes = (entry->m_nReaptTimes) -  it->second.num;
			return leftTimes;
		}
		else
		{
			return (entry->m_nReaptTimes);
		}

		break;
	}
	case eVipInstance:
		{
			double times = 0;
			int vip = m_owner->getAttManage()->getValue<DWORD>(eVIPLv);
			if(vip >= 3 && vip < 6)
			{
				times = m_owner->GetVipPart()->GetValueByRight(eVipThirdLevelInstance);
			}
			else if(vip >= 6 && vip < 9)
			{
				times = m_owner->GetVipPart()->GetValueByRight(eVipSxithLevelInstance);
			}
			else if(vip >= 9)
			{
				times = m_owner->GetVipPart()->GetValueByRight(eVipNinthLevelInstance);
			}

			if (times != VIP_NOT_HAVE_THIS_RIGHT)
			{
				vipTimes = (int)times;
			}

			map<int, PlayerInfo::InstanceVip>::iterator it = m_InstanceVip.find(sceneId);
			if(it != m_InstanceVip.end())
			{
				DWORD leftTimes = (vipTimes + entry->m_nReaptTimes) -  it->second.num();
				return leftTimes;
			}

			break;
		}
	case eEliteInstance:
	{
		int nNum = 0;
		map<int, sInstanceDiffculty>::iterator it = m_InstanceElite.begin();
		for(; it != m_InstanceElite.end(); ++it)
		{
			nNum += it->second.num;
		}

		int leftTimes = entry->m_nReaptTimes - nNum;
		return leftTimes;

		break;
	}
	case eCauchemarInstance:
	{
		int nNum = 0;
		map<int, sInstanceDiffculty>::iterator it = m_InstanceCauchemar.begin();
		for(; it != m_InstanceCauchemar.end(); ++it)
		{
			nNum += it->second.num;
		}

		int leftTimes = entry->m_nReaptTimes - nNum;
		return leftTimes;

		break;
	}
	case eAdvancedInstance:
	{
		map<int, sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
		if(it != m_InstanceAdvanced.end())
		{
			int leftTimes = entry->m_nReaptTimes - it->second.num;
			return leftTimes;
		}

		break;
	}
	case eExpInstance:
	{
		int nNum = 0;
		map<int, sInstanceDiffculty>::iterator it = m_InstanceExp.begin();
		for(; it != m_InstanceExp.end(); ++it)
		{
			nNum += it->second.num;
		}

		int leftTimes = entry->m_nReaptTimes - nNum;
		return leftTimes;

		break;
	}
	case eMoneyInstance1:
	{
		int nNum = 0;
		map<int, sInstanceDiffculty>::iterator it = m_InstanceMoney.begin();
		for(; it != m_InstanceMoney.end(); ++it)
		{
			nNum += it->second.num;
		}

		int leftTimes = entry->m_nReaptTimes - nNum;
		return leftTimes;

		break;
	}
	default :
		{
			break;
		}
	}

	//假如没有进入副本
	if(entry)
	{
		return (vipTimes + entry->m_nReaptTimes);
	}

	return -1;
}

int InstanceMgr::GetAllInstSurplusNum()
{
	vector<int> singleInst, groupInst, vipInst;
	Smart_Ptr<AttributeCreature> attr = m_owner->getAttManage();
	if (!attr)
	{
		return 0;
	}
	int lv = attr->getValue<int>(eCharLv);

	int totalNum = 0;

//	double times = m_owner->GetVipPart()->GetValueByRight(eVipSingleInstResetFreeCnt);
//	if (times != VIP_NOT_HAVE_THIS_RIGHT)
//	{ // 单人副本VIP次数
//		singleVipTimes = (int)times;
//	}

//	times = m_owner->GetVipPart()->GetValueByRight(eVipTeamInstResetFreeCnt);
//	if (times != VIP_NOT_HAVE_THIS_RIGHT)
//	{ // 组队副本VIP次数
//		groupVipTimes = (int)times;
//	}

	if (m_owner->IsOpenSystem(eStSingleInstanceLv))
	{
		InstanceLoader::GetInstance()->GetInstanceTypeIdList(ePersonalInstance, 1, singleInst, lv);
		for (uint i  = 0; i < singleInst.size(); ++i)
		{ // 单人副本
			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(singleInst[i]);
			if (NULL == entry)
			{
				continue;
			}
			map<int,sInstanceDiffculty>::iterator it = m_Instance.find(singleInst[i]);
			if (it == m_Instance.end())
			{
				totalNum += entry->m_nReaptTimes;
			}
			else
			{
				int num = entry->m_nReaptTimes -  it->second.num;
				if (0 >= num)
				{
					continue;
				}
				totalNum += num;
			}
		}
	}

	if (m_owner->IsOpenSystem(eStVipInstanceLv))
	{
		InstanceLoader::GetInstance()->GetInstanceTypeIdList(eVipInstance, 1, vipInst, lv);
		for (uint i = 0; i < vipInst.size(); ++i)
		{
			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(vipInst[i]);
			if (NULL == entry)
			{
				continue;
			}
			// VIP副本

			if(entry->m_nVipLimit > attr->getValue<DWORD>(eVIPLv))
			{
				continue;
			}

			map<int, PlayerInfo::InstanceVip>::iterator it = m_InstanceVip.find(vipInst[i]);
			if (it == m_InstanceVip.end())
			{
				totalNum += entry->m_nReaptTimes;
			}
			else
			{
				int num = entry->m_nReaptTimes -  it->second.num();
				if (0 >= num)
				{
					continue;
				}
				totalNum += num;
			}
		}
	}

	if (m_owner->IsOpenSystem(eStChallengeInstanceLv))
	{
		int curlayer = m_owner->GetChallenge()->GetCurLayer();
		int maxlayer = ChallengeMgr::GetInstance()->GetMaxLayers();
		if (0 < curlayer)
		{
			totalNum += (maxlayer - curlayer + 1) ;
		}
	}

	return totalNum;
}


int InstanceMgr::GetSingleInstUnChall(int& unChallenge)
{
	return GetSomeInstUnChall(ePersonalInstance, eStSingleInstanceLv, unChallenge);
}

int InstanceMgr::GetChallInstUnChall(int& unChallenge)
{
	Smart_Ptr<AttributeCreature> attr = m_owner->getAttManage();
	if (!attr)
	{
		return 0;
	}

	int totalNum = 0;

	if (m_owner->IsOpenSystem(eStChallengeInstanceLv))
	{
		int max = ChallengeMgr::GetInstance()->GetMaxLayers();
		int cur = m_owner->GetChallenge()->GetCurLayer();
		if (0 < cur)
		{
			totalNum = max;
			unChallenge = max - cur + 1;
		}
	}


	return totalNum;
}

//得到类型副本的重置次数
int InstanceMgr::getResetTime(int type)
{
	switch(type)
	{
	case ePersonalInstance:
		{
			map<int,sInstanceDiffculty>::iterator iter = m_Instance.begin();
			for(; iter!=m_Instance.end(); ++iter)
			{
				InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(iter->first);
				if(entry==NULL||entry->m_eType!=type)
						continue;
				return iter->second.reset;
			}
			break;
		}
	case eEliteInstance:
		{
			map<int,sInstanceDiffculty>::iterator iter = m_InstanceElite.begin();
			for(; iter!=m_InstanceElite.end(); ++iter)
			{
				InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(iter->first);
				if(entry==NULL||entry->m_eType!=type)
						continue;
				return iter->second.reset;
			}
			break;
		}
	case eCauchemarInstance:
		{
			map<int,sInstanceDiffculty>::iterator iter = m_InstanceCauchemar.begin();
			for(; iter!=m_InstanceCauchemar.end(); ++iter)
			{
				InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(iter->first);
				if(entry==NULL||entry->m_eType!=type)
						continue;
				return iter->second.reset;
			}

			break;
		}
	case eExpInstance:
		{
			map<int,sInstanceDiffculty>::iterator iter = m_InstanceExp.begin();
			for(; iter!=m_InstanceExp.end(); ++iter)
			{
				InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(iter->first);
				if(entry==NULL||entry->m_eType!=type)
						continue;
				return iter->second.reset;
			}
			break;
		}
	case eMoneyInstance1:
		{
			map<int,sInstanceDiffculty>::iterator iter = m_InstanceMoney.begin();
			for(; iter!=m_InstanceMoney.end(); ++iter)
			{
				InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(iter->first);
				if(entry==NULL||entry->m_eType!=type)
						continue;
				return iter->second.reset;
			}
			break;
		}
	}

	return 0;
}

int InstanceMgr::GetMoneyInstHasChall(int& hasChallenge)
{
	int totalNum = 0;
	// 本来允许的次数
	const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(config.mapID);
	if (NULL != entry)
	{
		totalNum += entry->m_nReaptTimes;
	}
	//本日金钱购买的次数
	totalNum += getMoneyInstanceBuyCount();



	return totalNum;
}

// 获取精英副本已挑战次数，返回精英副本所有次数
int InstanceMgr::GetEliteInstUnChall(int& unChallenge)
{
	return GetSomeInstUnChall(eEliteInstance, eStEliteInstLv, unChallenge);
}
// 获取噩梦副本已挑战次数，返回噩梦副本所有次数
int InstanceMgr::GetCauchemarInstUnChall(int& unChallenge)
{
	return GetSomeInstUnChall(eCauchemarInstance, eStCauchemarInstLv, unChallenge);
}

int InstanceMgr::GetAdvancedInstUnChall(int& unChallenge)
{
	return GetSomeInstUnChall(eAdvancedInstance, eStAdvanceInstOpenLv, unChallenge);
}

int InstanceMgr::GetSomeInstUnChall(InstanceType instType, ESysSettingType lvLimitType, int& unChallenge)
{
	vector<int> inst;
	Smart_Ptr<AttributeCreature> attr = m_owner->getAttManage();
	if (!attr)
	{
		return 0;
	}
	int lv = attr->getValue<int>(eCharLv);

	int totalNum = 0;

	if (m_owner->IsOpenSystem(lvLimitType))
	{
		InstanceLoader::GetInstance()->GetInstanceTypeIdList(instType, 1, inst, lv);
		for (uint i  = 0; i < inst.size(); ++i)
		{
			InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(inst[i]);
			if (NULL == entry)
			{
				continue;
			}
			bool flag = false;
			map<int,sInstanceDiffculty>::iterator it;
			switch (instType)
			{
			case ePersonalInstance:
				{
					it = m_Instance.find(inst[i]);
					flag = (it==m_Instance.end()) ? false : true;
				}
				break;
			case eEliteInstance:
				{
					it = m_InstanceElite.find(inst[i]);
					flag = (it==m_InstanceElite.end()) ? false : true;
				}
				break;
			case eCauchemarInstance:
				{
					it = m_InstanceCauchemar.find(inst[i]);
					flag = (it==m_InstanceCauchemar.end()) ? false : true;
				}
				break;
			case eAdvancedInstance:
				{
					it = m_InstanceAdvanced.find(inst[i]);
					flag = (it==m_InstanceAdvanced.end()) ? false : true;
				}
				break;
			case eExpInstance:
				{
					it = m_InstanceExp.find(inst[i]);
					flag = (it==m_InstanceExp.end()) ? false : true;
				}
				break;
			case eMoneyInstance1:
				{
					it = m_InstanceMoney.find(inst[i]);
					flag = (it==m_InstanceMoney.end()) ? false : true;
				}
				break;
			default:
				{
				}
			}

			totalNum += entry->m_nReaptTimes;

			if (flag)
			{
				unChallenge += (entry->m_nReaptTimes - it->second.num);
			}
			else
			{
				unChallenge += entry->m_nReaptTimes;
			}
		}
	}

	return totalNum;
}

int InstanceMgr::GetLvEnough(int sceneId,int nLv)
{
	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::GetLeftValidTimes: intanceId is invalid %d.",sceneId);
		return eInstanceNotExsitence;
	}

	if((DWORD)nLv < entry->m_nLevel)
	{
		return eLVNotMatch;
	}

	return 0;
}

void InstanceMgr::GetVipInstanceInfo(MapInstanceProto::VipInstance & list)
{
	map<int, PlayerInfo::InstanceVip>::iterator itr = m_InstanceVip.begin();
	for(; itr != m_InstanceVip.end(); ++itr)
	{
		MapInstanceProto::VipInstanceList *info = list.add_info();
		info->set_mapid(itr->second.mapid());
		info->set_num(itr->second.num());
		info->set_pass(itr->second.pass());
		info->set_time(itr->second.time());
	}

	m_owner->SetDataFlag(eInstanceInfo);
}


void InstanceMgr::GetCommonInstanceInfo(MapInstanceProto::CommonInstanceInfo & info)
{
	int nNum = 0;
	int nReset = 0;
	map<int, sInstanceDiffculty>::iterator itr = m_Instance.begin();
	for(; itr != m_Instance.end(); ++itr)
	{
		MapInstanceProto::CommonInstanceInfoList* pList = info.add_info();
		pList->set_mapid(itr->second.mapId);
		pList->set_pass(itr->second.pass);

		nNum += itr->second.num;
		nReset = itr->second.reset;
	}

	info.set_num(nNum);
	info.set_reset(nReset);
}

//获取经验副本信息
void InstanceMgr::GetExpInstanceInfo(MapInstanceProto::ExpInstanceInfo & info)
{
	int nNum = 0;
	int nReset = 0;
	map<int, sInstanceDiffculty>::iterator itr = m_InstanceExp.begin();
	for(; itr != m_InstanceExp.end(); ++itr)
	{
		MapInstanceProto::ExpInstanceInfoList* pList = info.add_info();
		pList->set_mapid(itr->second.mapId);
		pList->set_pass(itr->second.pass);

		nNum += itr->second.num;
		nReset = itr->second.reset;
	}

	info.set_num(nNum);
	info.set_reset(nReset);
}

//获取铜钱副本信息
void InstanceMgr::GetMoneyInstanceInfo(MapInstanceProto::MoneyInstanceInfo & info)
{
	int nNum = 0;
	int nReset = 0;
	map<int, sInstanceDiffculty>::iterator itr = m_InstanceMoney.begin();
	for(; itr != m_InstanceMoney.end(); ++itr)
	{
		MapInstanceProto::MoneyInstanceInfoList* pList = info.add_info();
		pList->set_mapid(itr->second.mapId);
		pList->set_pass(itr->second.pass);

		nNum += itr->second.num;
		nReset = itr->second.reset;
	}

	info.set_num(nNum);
	info.set_reset(nReset);
}


void InstanceMgr::GetYesterdayInstanceInfo(int recorceSourceParamType,int instanceType, Daily::RecoverResourceInfoList * info)
{
	bool bResult = false;
	Daily::RecoverResourceInfo instanceInfo  ;//info->add_infolist();
	instanceInfo.set_id(recorceSourceParamType);
	Daily::RecoverResourceReward * reward = instanceInfo.mutable_reward();
	//itemID-number
	std::map<int,int> itemRewardMap;
	map<DWORD, InstanceMapEntry> entrys =  InstanceLoader::GetInstance()->GetEntry();
	map<DWORD, InstanceMapEntry>::iterator itr = entrys.begin();
	for(; itr != entrys.end(); ++itr)
	{
		if(itr->second.m_eType == instanceType &&  (DWORD)m_owner->getAttManage()->getValue<int>(eCharLv) >= itr->second.m_nLevel)
		{
			int num  = itr->second.m_nReaptTimes;
			map<int, sInstanceDiffculty>::iterator it =  m_YesterdayInstance.find(itr->second.m_nMapID);
			if(it != m_YesterdayInstance.end())
			{
				num -= it->second.num;
			}
			else
			{
				continue;
			}

			if(num <= 0)
				continue;

			bResult = true;

			InstanceMapReward* entryReward = InstanceLoader::GetInstance()->GetEntryReward(itr->second.m_nMapID);
			if(entryReward)
			{
				instanceInfo.set_restcount(instanceInfo.restcount() + num);
				reward->set_exp(reward->exp() + entryReward->m_nAwardExp * num);
				reward->set_money(reward->money() + entryReward->m_nAwardMoney * num);
				reward->set_force(reward->force() + entryReward->m_nAwardSprit * num);
				for(uint i = 0;i < entryReward->m_nAward.size();++i)
				{
					std::map<int,int>::iterator it = itemRewardMap.find(entryReward->m_nAward[i]);
					if(it!=itemRewardMap.end())
					{
						it->second += entryReward->m_nAwardNum[i] * num;
					}
					else
					{
						itemRewardMap[entryReward->m_nAward[i]] = entryReward->m_nAwardNum[i] * num;
					}
				}
			}
		}
	}

	std::map<int,int>::iterator itemIt = itemRewardMap.begin();
	for(;itemIt!=itemRewardMap.end();++itemIt)
	{
		Daily::RecoverResourceItemReward * itemReward = reward->add_item();
		itemReward->set_itemid(itemIt->first);
		itemReward->set_itemcount(itemIt->second);
	}

	if(bResult)
		info->add_infolist()->CopyFrom(instanceInfo);
}

void InstanceMgr::GetYesterdayInstanceInfo(Daily::RecoverResourceInfoList * info)
{
	GetYesterdayInstanceInfo(eRecorceSourceParamType_singleInstance,ePersonalInstance,info);
	GetYesterdayInstanceInfo(eRecorceSourceParamType_singleInstance,eEliteInstance,info);
	GetYesterdayInstanceInfo(eRecorceSourceParamType_singleInstance,eCauchemarInstance,info);
	GetYesterdayInstanceInfo(eRecorceSourceParamType_expInstance,eExpInstance,info);
	GetYesterdayInstanceInfo(eRecorceSourceParamType_moneyInstance,eMoneyInstance1,info);
	GetYesterdayInstanceInfo(eRecorceSourceParamType_advancedInstance,eAdvancedInstance,info);
}

int InstanceMgr::RecoverResourceArena(int num, Daily::RecoverResourceInfo *info)
{
	const int Arena_MapID = GameParaLoader::GetInstance()->getArenaMapId();
	InstanceMapEntry *mapptr = InstanceLoader::GetInstance()->GetEntry(Arena_MapID);
	if(mapptr == NULL)
		return 0;
	int haveNum = mapptr->m_nReaptTimes - num;
	if(haveNum <=0)
		return eInstanceTimesNotEnough;

	sRecorceSourceParam *ptr = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(eRecorceSourceParamType_arena);
	if(!ptr)
		return 0;

	int tempLevel = m_owner->getAttManage()->getValue<int>(eCharLv);

	UpgradeInfo nowInfo;

	UpgradeLoader::GetInstance()->GetUpgradeByLv(tempLevel, nowInfo);
	if(nowInfo.lv == 0)
	{
		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
		return 0;
	}

	int winexp = ArenaLoader::GetInstance()->GetChallengeWin(nowInfo.factoryExp);
	int loseexp = ArenaLoader::GetInstance()->GetChallengeLose(nowInfo.factoryExp);

	int exp = GameParaLoader::GetInstance()->GetAreaExp(winexp,loseexp) * haveNum;

	Daily::RecoverResourceReward * reward = info->mutable_reward();
	reward->set_exp(exp);
	info->set_restcount(haveNum);
	info->set_id(eRecorceSourceParamType_arena);

	return 0;
}

bool InstanceMgr::CanEnterInstance(int instanceType,int specialType)
{
	int recoverSourceParamType = 0;
	//组队副本
	if(instanceType == eGroupInstance && specialType == 1)
	{
		recoverSourceParamType = eRecorceSourceParamType_groupInstance;
	}
	else if(instanceType == ePersonalInstance && specialType == 2)
	{
		recoverSourceParamType = eRecorceSourceParamType_challengeInstance;
	}
	else
	{
		return true;
	}
	return true;
}

int InstanceMgr::SendAwardToPlayer(bool isSuccess, DWORD sceneId, DWORD startTime, DWORD endTime,DWORD fight)
{
	int rate = 0;
	vector<int> vec;
	vector<DWORD> idList;
	vector<DWORD> numList;
	vector<WORD> tempFlagList;
	vector<DWORD> tempStreghtList;
	vector<PropItem> items;

	MapInstanceProto::ToGTAwardInfo toGT;
	MapInstanceProto::SimInstanceEnd* toMemberAward = toGT.mutable_awardinfo();		//发送奖励

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::startSweep: intanceId is invalid %d.",sceneId);
		return eInstanceInfoError;
	}

	InstanceMapReward* entryReward = InstanceLoader::GetInstance()->GetEntryReward(sceneId);
	if (!entryReward)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::startSweep: entry Reward is invalid %d.",sceneId);
		return eInstanceInfoError;
	}

	if(isSuccess)
	{
		toMemberAward->set_mapid(entry->m_nMapID);
		toMemberAward->set_issuccess(isSuccess);
		MapInstanceProto::SimInstanceAward* awards = toMemberAward->mutable_awards();

		//关卡积分
		awards->set_barrierpoints(100);
		//消耗的时间
		unsigned long elapse = (endTime-startTime);
		awards->set_elapsedtime(elapse/1000);

		switch(entry->m_eType)
		{
		case eGroupInstance:
		{
			//奖励的经验
			awards->set_awardexp((int)(entryReward->m_nAwardExp));
			//真气
			awards->set_awardinfuriate((int)(entryReward->m_nAwardSprit));
			 //铜钱
			awards->set_awardmoney((int)(entryReward->m_nAwardMoney ));

			m_owner->ChangeMoney((int)(entryReward->m_nAwardMoney),true);
			vec.push_back(eCharMoney);

			m_owner->AddExp((int)(entryReward->m_nAwardExp));
			vec.push_back(eCharExp);

			m_owner->ChangeCurForce((int)(entryReward->m_nAwardSprit),true);
			vec.push_back(eCharForce);

			awards->set_personalpower(m_owner->getAttManage()->getValue<int>(eFightPower));
			awards->set_grouppower(fight);

			if(!GetInstancePass(entry->m_eType,sceneId))
			{
				map<DWORD,DWORD>::iterator itr = entryReward->m_nTongAward.begin();
				for(; itr != entryReward->m_nTongAward.end(); ++itr)
				{
					Smart_Ptr<PropBase> newItem = new PropBase(itr->first,itr->second);
					if(!newItem)
					{
						return eItemError;
					}

					CharLogin::ClientItemInfo* awardItme = awards->add_items();
					newItem->SetClientInfo(awardItme);
					awardItme->set_num(itr->second);

					idList.push_back(itr->first);
					numList.push_back(itr->second);
				}

				SetInstancePass(entry->m_eType,sceneId,isSuccess);
			}
			else
			{
				for(size_t j = 0; j < entryReward->m_nAward.size(); j++)
				{
					Smart_Ptr<PropBase> newItem = new PropBase(entryReward->m_nAward[j],entryReward->m_nAwardNum[j]);
					if(!newItem)
					{
						return eItemError;
					}

					CharLogin::ClientItemInfo* awardItme = awards->add_items();
					newItem->SetClientInfo(awardItme);
					awardItme->set_num(entryReward->m_nAwardNum[j]);

					idList.push_back(entryReward->m_nAward[j]);
					numList.push_back(entryReward->m_nAwardNum[j]);
				}
			}

			tempFlagList.resize(idList.size(),1);
			tempStreghtList.resize(numList.size(),0);
			if(idList.size() > 0)
			{
				Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
				if(!pPacket)
				{
					return ePlayerPackageFail;
				}

				int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
				if (!res)
				{
					vector<int> tempPos;
					pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);
				}
				else
				{
					for(size_t i = 0; i < idList.size(); i++)
					{
						PropItem item;
						item.id = idList[i];
						item.num = numList[i];

						items.push_back(item);
					}

					if(items.size() > 0)
					{
						char title[128] = {0};
						sprintf(title,InternationalLoader::GetInstance()->GetInterStr(68).c_str(),entry->m_sName.c_str());
						string warning1 = entry->m_sName + InternationalLoader::GetInstance()->GetInterStr(69);
						MailMessageMgr::GetInstance()->SendMail(m_owner->GetID(),items,warning1.c_str(),title);
					}
				}
			}


			if(vec.size() > 0)
			{
				m_owner->SynCharAttribute(vec);
			}

			break;
		}
		case ePersonalInstance:
		case eMarryInstance:
		case eEliteInstance:
		case eCauchemarInstance:
		case eVipInstance:
			{
				//奖励的经验
				awards->set_awardexp((int)(entryReward->m_nAwardExp));
			   //真气
				awards->set_awardinfuriate((int)(entryReward->m_nAwardSprit));
			   //铜钱
				awards->set_awardmoney((int)(entryReward->m_nAwardMoney));


				m_owner->ChangeMoney((int)(entryReward->m_nAwardMoney),true);
				vec.push_back(eCharMoney);


				m_owner->AddExp((int)(entryReward->m_nAwardExp));
				vec.push_back(eCharExp);

				m_owner->ChangeCurForce((int)(entryReward->m_nAwardSprit),true);
				vec.push_back(eCharForce);

				if(vec.size() > 0)
				{
					m_owner->SynCharAttribute(vec);
				}

				for(size_t j = 0; j < entryReward->m_nAward.size(); j++)
				{
					Smart_Ptr<PropBase> newItem = new PropBase(entryReward->m_nAward[j],entryReward->m_nAwardNum[j]);
					if(!newItem)
					{
						return eItemError;
					}

					CharLogin::ClientItemInfo* awardItme = awards->add_items();
					newItem->SetClientInfo(awardItme);
					awardItme->set_num(entryReward->m_nAwardNum[j]);

					idList.push_back(entryReward->m_nAward[j]);
					numList.push_back(entryReward->m_nAwardNum[j]);
				}


				tempFlagList.resize(idList.size(),1);
				tempStreghtList.resize(numList.size(),0);
				if(idList.size() > 0)
				{
					Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
					if(!pPacket)
					{
						return ePlayerPackageFail;
					}

					int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
					if (!res)
					{
						vector<int> tempPos;
						pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);
					}
					else
					{
						for(size_t i = 0; i < idList.size(); i++)
						{
							PropItem item;
							item.id = idList[i];
							item.num = numList[i];

							items.push_back(item);
						}

						if(items.size() > 0)
						{
							char title[128] = {0};
							sprintf(title,InternationalLoader::GetInstance()->GetInterStr(70).c_str(),entry->m_sName.c_str());
							string warning1 = entry->m_sName + InternationalLoader::GetInstance()->GetInterStr(71);
							MailMessageMgr::GetInstance()->SendMail(m_owner->GetID(),items,warning1.c_str(),title);
						}
					}
				}

				break;
			}
		case eAdvancedInstance:
		case eExpInstance:
		case eMoneyInstance1:
			{
				//奖励的经验
				awards->set_awardexp((int)(entryReward->m_nAwardExp));
			   //真气
				awards->set_awardinfuriate((int)(entryReward->m_nAwardSprit));
			   //铜钱
				awards->set_awardmoney((int)(entryReward->m_nAwardMoney));


				m_owner->ChangeMoney((int)(entryReward->m_nAwardMoney),true);
				vec.push_back(eCharMoney);


				m_owner->AddExp((int)(entryReward->m_nAwardExp));
				vec.push_back(eCharExp);

				m_owner->ChangeCurForce((int)(entryReward->m_nAwardSprit),true);
				vec.push_back(eCharForce);

				if(vec.size() > 0)
				{
					m_owner->SynCharAttribute(vec);
				}

				if(!GetInstancePass(entry->m_eType,sceneId))
				{
					map<DWORD,DWORD>::iterator itr = entryReward->m_nTongAward.begin();
					for(; itr != entryReward->m_nTongAward.end(); ++itr)
					{
						Smart_Ptr<PropBase> newItem = new PropBase(itr->first,itr->second);
						if(!newItem)
						{
							return eItemError;
						}

						CharLogin::ClientItemInfo* awardItme = awards->add_items();
						newItem->SetClientInfo(awardItme);
						awardItme->set_num(itr->second);

						idList.push_back(itr->first);
						numList.push_back(itr->second);
					}

					SetInstancePass(entry->m_eType,sceneId,isSuccess);
				}
				else
				{
					rate =  GetRateSuccessReward(entry->m_eType,sceneId);
					for(size_t j = 0; j < entryReward->m_nAward.size(); j++)
					{
						Smart_Ptr<PropBase> newItem = new PropBase(entryReward->m_nAward[j],entryReward->m_nAwardNum[j] * rate);
						if(!newItem)
						{
							return eItemError;
						}

						CharLogin::ClientItemInfo* awardItme = awards->add_items();
						newItem->SetClientInfo(awardItme);
						awardItme->set_num(entryReward->m_nAwardNum[j] * rate);

						idList.push_back(entryReward->m_nAward[j]);
						numList.push_back(entryReward->m_nAwardNum[j] * rate);
					}
				}

				tempFlagList.resize(idList.size(),1);
				tempStreghtList.resize(numList.size(),0);
				if(idList.size() > 0)
				{
					Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
					if(!pPacket)
					{
						return ePlayerPackageFail;
					}

					int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
					if (!res)
					{
						vector<int> tempPos;
						pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);
					}
					else
					{
						for(size_t i = 0; i < idList.size(); i++)
						{
							PropItem item;
							item.id = idList[i];
							item.num = numList[i];

							items.push_back(item);
						}

						if(items.size() > 0)
						{
							char title[128] = {0};
							sprintf(title,InternationalLoader::GetInstance()->GetInterStr(66).c_str(),entry->m_sName.c_str());
							string warning1 = entry->m_sName + InternationalLoader::GetInstance()->GetInterStr(67);
							MailMessageMgr::GetInstance()->SendMail(m_owner->GetID(),items,warning1.c_str(),title);
						}
					}
				}

				break;
			}
		default:
			{
				break;
			}
		}

		toGT.set_charid(m_owner->GetID());

		//发送奖励物品
		LOG_DEBUG(FILEINFO, "Send to intance award to player simple message player %lld",m_owner->GetID());
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2GT_ENDINSTANCE, &toGT, m_owner->GetChannelID(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
	else
	{
		toMemberAward->set_mapid(entry->m_nMapID);
		toMemberAward->set_issuccess(isSuccess);
		toGT.set_charid(m_owner->GetID());

		//发送奖励物品
		LOG_DEBUG(FILEINFO, "Send to intance award to player simple message");
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2GT_ENDINSTANCE, &toGT, m_owner->GetChannelID(), SimpleMessage);
		Message_Facade::Send(messRet);
	}

	return 0;
}

int InstanceMgr::NewStartSweep(int sceneId,MapInstanceProto::SimSweepInstanceAward& toClient,bool flag)
{
	int expId = GameParaLoader::GetInstance()->getExchangeExpItemId();
	int moneyId = GameParaLoader::GetInstance()->getExchangeMoneyItemId();
	int gasId = GameParaLoader::GetInstance()->getExchangeForceItemId();
	int ret = CanSweep();
	if(0 != ret)
		return ret;

	if(!CMapConfig::GetInstance()->IsHaveMap(sceneId))
	{
		return eInstanceInfoError;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::NewStartSweep: intanceId is invalid %d.",sceneId);
		return eInstanceInfoError;
	}

	InstanceMapReward* entryReward = InstanceLoader::GetInstance()->GetEntryReward(sceneId);
	if (!entryReward)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::NewStartSweep: entry Reward is invalid %d.",sceneId);
		return eInstanceInfoError;
	}

	// 检查关卡限制
	if (entry->m_nLevel > 0 && (DWORD)m_owner->getAttManage()->getValue<int>(eCharLv) < entry->m_nLevel)
	{
		return eLVNotEnough;
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
	if(!pPacket)
	{
		return ePlayerPackageFail;
	}

	ret = CanNewStartSweep(sceneId,entry->m_eType);
	if(0 != ret)
		return ret;

	if (0 >= GetLeftValidTimes(sceneId))
	{
		if(entry->m_eType == eAdvancedInstance)
		{
			ret = ResetInstance(sceneId);
		}
		else
		{
			ResetAllInstance(entry->m_eType,ret);
		}

		if(0 != ret)
			return ret;

//		return eInstanceSweepTimeLimit;
	}

	AddPassInstance(sceneId,1,entry->m_eType,true);

	m_owner->SetDataFlag(eInstanceInfo);
	m_owner->SetDataFlag(eBattleInfo);

	m_owner->GetVitalityMgr()->FinshMapInstanceVitality(sceneId);


	//结束扫荡
	vector<int64> goods;
	MonsterInfo info;
	vector<DropStaticInfo> item;

	MonsterLoader::GetInstance()->GetMonsterInfo(entry->m_nTarget[0], info);
	if(info.id <= 0)
	{
		LOG_ERROR(FILEINFO, "drop allot but monster id is error");

		return eInstanceInfoError;
	}


	if(entryReward->m_nAwardMoney > 0)
		goods.push_back(((int64)(entryReward->m_nAwardMoney))<<32 | moneyId);
	if(entryReward->m_nAwardExp > 0)
		goods.push_back(((int64)(entryReward->m_nAwardExp))<<32 | expId);

	if(entryReward->m_nAwardSprit > 0)
		goods.push_back(((int64)(entryReward->m_nAwardSprit))<<32 | gasId);

	toClient.set_mapid(sceneId);
	toClient.set_elapsedtime(0);
	toClient.set_awardmoney((int)(entryReward->m_nAwardMoney));
	toClient.set_awardexp((int)(entryReward->m_nAwardExp ));
	toClient.set_awardinfuriate((int)(entryReward->m_nAwardSprit));

	for(size_t j = 0; j < entryReward->m_nAward.size(); j++)
	{
		int64 info = entryReward->m_nAwardNum[j];
		info= info << 32;	//物品数量
		DWORD itemid =entryReward->m_nAward[j];
		info  = info | itemid;
		goods.push_back(info);

		Smart_Ptr<PropBase> newItem = new PropBase(entryReward->m_nAward[j],entryReward->m_nAwardNum[j]);
		if(newItem)
		{
			CharLogin::ClientItemInfo* awardItme = toClient.add_items();
			newItem->SetClientInfo(awardItme);
		}
	}

	DropItemLoader::GetInstance()->GetDropItem(info.fallID, item);
	vector<DropStaticInfo>::iterator itrEnd = item.end();
	for(vector<DropStaticInfo>::iterator itr = item.begin(); itr != itrEnd; ++itr)
	{
		int64 info = itr->m_Counts;
		info= info << 32;	//物品数量
		DWORD itemid =itr->m_ItemID;
		info  = info | itemid;
		goods.push_back(info);

		Smart_Ptr<PropBase> newItem = new PropBase(itr->m_ItemID,itr->m_Counts);
		if(newItem)
		{
			CharLogin::ClientItemInfo* awardItme = toClient.add_items();
			newItem->SetClientInfo(awardItme);
		}
	}

	string warning = entry->m_sName + InternationalLoader::GetInstance()->GetInterStr(5);
	string warning1 = entry->m_sName + InternationalLoader::GetInstance()->GetInterStr(6);

	MailMessageMgr::GetInstance()->SendSysMail(goods, m_owner->GetName(),0,warning.c_str(),warning1.c_str());

	PlayerRaidsDuplicate tempArgs(m_owner->GetMyself(),sceneId);
	m_owner->FireEvent(PLAYER_RAIDS_DUPLICATE,tempArgs);
	if(flag)
	{
		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&toClient,MSG_SIM_GM2C_OVERSWEEPINSTANCE);
	}

	return 0;
}

int InstanceMgr::CanSweep()
{
	if(DuplicateMgr::GetInstance()->IsInWillCreateInstance(m_owner->GetID()))
	{
		return eInstanceCreating;
	}

	if(m_owner->isBWillInInstance())
	{
		return eInInstanceMap;
	}

	if(GET_MAP_TYPE(m_owner->GetMapID()) >= eCopyMap)
	{
		return eInInstanceMap;
	}

	return 0;
}

// 获取竞技场未挑战次数 返回总次数
//int InstanceMgr::GetArenaUnChallenge(int& unChallenge)
//{
//	int totalNum = 0;
//	const int Arena_MapID = GameParaLoader::GetInstance()->getArenaMapId();
//	InstanceMapEntry *mapptr = InstanceLoader::GetInstance()->GetEntry(Arena_MapID);
//	if (mapptr != NULL)
//	{
//		// 总次数
//		int vipTimes = 0;
//		if (m_owner->GetVipPart() && 0 < m_owner->getAttManage()->getValue<DWORD>(eVIPLv))
//		{
//			double times = m_owner->GetVipPart()->GetValueByRight(eVipArenaCanChallenge);
//			if (VIP_NOT_HAVE_THIS_RIGHT != times)
//			{
//				vipTimes = (int)times;
//			}
//		}
//		if (vipTimes > 0)
//		{
//			totalNum = vipTimes;
//		}
//		else
//		{
//			totalNum = (int)mapptr->m_nReaptTimes;
//		}
//
//		// 已经挑战次数
//		Smart_Ptr<CounterService>& counterMgr = m_owner->GetCounterService();
//		if ((bool)counterMgr)
//		{
//			unChallenge = totalNum - counterMgr->GetNumById(COMPETE_INSTANCE_COUNTER);
//		}
//	}
//
//	return totalNum;
//}

//保存组队副本数据
void InstanceMgr::SetGroupInstanceInfo(PlayerInfo::GroupInstance *instanceInfo)
{
	map<int,GroupInstance>::iterator itr = m_GroupInstance.begin();
	for(; itr != m_GroupInstance.end(); ++itr)
	{
		PlayerInfo::InstanceGroupList* pList = instanceInfo->add_grouplist();
		pList->set_mapid(itr->first);
		pList->set_num(itr->second.num);
		pList->set_vipnum(itr->second.vipnum);
		pList->set_buynum(itr->second.buynum);
		pList->set_pass(itr->second.pass);
	}
}

//初始化组队副本数据
void InstanceMgr::InitGroupInstanceInfo(const PlayerInfo::GroupInstance& instanceInfo)
{
	for(int i = 0; i < instanceInfo.grouplist_size(); i++)
	{
		map<int,GroupInstance>::iterator itr = m_GroupInstance.find(instanceInfo.grouplist(i).mapid());
		if(itr != m_GroupInstance.end())
		{
			itr->second = GroupInstance(instanceInfo.grouplist(i));
		}
		else
		{
			m_GroupInstance[instanceInfo.grouplist(i).mapid()] = GroupInstance(instanceInfo.grouplist(i));
		}
	}

	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();
	if(tempType != eDayResetToday)
	{
		ResetGroupInstance();
	}
}

void InstanceMgr::ResetGroupInstance()
{
	map<int,GroupInstance>::iterator itr = m_GroupInstance.begin();
	for(; itr != m_GroupInstance.end(); ++itr)
	{
		itr->second.Reset();
	}

	m_owner->SetDataFlag(eGroupInstanceInfo);
}

void InstanceMgr::GetGroupInstanceInfo(MapInstanceProto::GroupInstance& instanceInfo)
{
	map<int,GroupInstance>::iterator itr = m_GroupInstance.begin();
	for(; itr != m_GroupInstance.end(); ++itr)
	{
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(itr->second.mapId);
		if (!entry)
		{
			LOG_DEBUG(FILEINFO,"InstanceMgr::ShopGroupInstance: intanceId is invalid %d.",itr->second.mapId);
			return;
		}


		MapInstanceProto::InstanceGroupList* pList = instanceInfo.add_grouplist();
		pList->set_mapid(itr->first);
		pList->set_num(itr->second.num);
	}
}

int InstanceMgr::ShopGroupInstance(int mapId,MapInstanceProto::RetCostByGroupInstance& info)
{
	int golden = 0;
	int bgolden = 0;
	vector<int> vec;
	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(mapId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::ShopGroupInstance: intanceId is invalid %d.",mapId);
		return eInstanceInfoError;
	}

	if(entry->m_eType != eGroupInstance)
	{
		return eInstanceTypeError;
	}


	map<int,GroupInstance>::iterator itr = m_GroupInstance.find(mapId);
	if(itr != m_GroupInstance.end())
	{
//		if((DWORD)itr->second.vipnum >= m_owner->GetVipPart()->GetValueByRight(eVipTeamInstResetFreeCnt))
//		{
//			return eMoneyInstanceBuyLimit;
//		}

		if(m_owner->IsEnoughGolden(entry->m_nResetPurchase))
		{
			m_owner->DecreaseGoldenCost(entry->m_nResetPurchase,golden,bgolden);
			if(golden > 0)
				vec.push_back(eCharGolden);
			if(bgolden > 0)
				vec.push_back(eCharBindGolden);

			m_owner->SynCharAttribute(vec);
		}
		else
		{
			return eNotEnoughGold;
		}

		itr->second.buynum += 1;
		itr->second.vipnum += 1;

		info.set_buynum(itr->second.vipnum);
		info.set_num(entry->m_nReaptTimes + itr->second.buynum - itr->second.num);
	}
	else
	{
		GroupInstance groupInstance;
		groupInstance.mapId = mapId;

//		if((DWORD)groupInstance.vipnum >= m_owner->GetVipPart()->GetValueByRight(eVipTeamInstResetFreeCnt))
//		{
//			return eMoneyInstanceBuyLimit;
//		}

		if(m_owner->IsEnoughGolden(entry->m_nResetPurchase))
		{
			m_owner->DecreaseGoldenCost(entry->m_nResetPurchase,golden,bgolden);
			if(golden > 0)
				vec.push_back(eCharGolden);
			if(bgolden > 0)
				vec.push_back(eCharBindGolden);

			m_owner->SynCharAttribute(vec);
		}
		else
		{
			return eNotEnoughGold;
		}

		groupInstance.buynum += 1;
		groupInstance.vipnum += 1;

		info.set_buynum(groupInstance.vipnum);
		info.set_num(entry->m_nReaptTimes + groupInstance.buynum - groupInstance.num);

		m_GroupInstance[mapId] = groupInstance;
	}

	return 0;
}

//获取精英副本信息
void InstanceMgr::GetEliteInstanceInfo(MapInstanceProto::EliteInstance& info)
{
	int nNum = 0;
	int nReset = 0;
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceElite.begin();
	for(; itr != m_InstanceElite.end(); itr++)
	{
		MapInstanceProto::InstanceEliteList* pList = info.add_list();
		pList->set_mapid(itr->second.mapId);
		pList->set_pass(itr->second.pass);

		nNum += itr->second.num;
		nReset = itr->second.reset;
	}

	info.set_num(nNum);
	info.set_reset(nReset);
}


//初始化精英副本信息
void InstanceMgr::InitEliteInstanceInfo(const PlayerInfo::EliteInstance& info)
{
	for(int i = 0; i < info.list_size(); i++)
	{
		map<int,sInstanceDiffculty>::iterator itr = m_InstanceElite.find(info.list(i).mapid());
		if(itr != m_InstanceElite.end())
		{
			itr->second.mapId =  info.list(i).mapid();
			itr->second.num =  info.list(i).num();
			itr->second.pass =  info.list(i).pass();
			itr->second.reset =  info.list(i).reset();
			itr->second.time =  info.list(i).time();
		}
		else
		{
			sInstanceDiffculty instanceDiffculty;
			instanceDiffculty.mapId =  info.list(i).mapid();
			instanceDiffculty.num =  info.list(i).num();
			instanceDiffculty.pass =  info.list(i).pass();
			instanceDiffculty.reset =  info.list(i).reset();
			instanceDiffculty.time =  info.list(i).time();

			m_InstanceElite[info.list(i).mapid()] = instanceDiffculty;
		}
	}

	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();
	if(tempType != eDayResetToday)
	{
		if(eDayResetYesterday == tempType)//上一次登录是昨天，玩家今天第一次登录m_YesterdayInstanceSweep直接使用昨天保存的副本信息
		{
			for(int i = 0; i < info.list_size(); i++)
			{
				map<int,sInstanceDiffculty>::iterator it = m_YesterdayInstance.find(info.list(i).mapid());
				if(it != m_YesterdayInstance.end())
				{
					it->second.mapId =  info.list(i).mapid();
					it->second.num =  info.list(i).num();
					it->second.pass =  info.list(i).pass();
					it->second.reset =  info.list(i).reset();
					it->second.time =  info.list(i).time();
				}
				else
				{
					sInstanceDiffculty instanceDiffculty;
					instanceDiffculty.mapId =  info.list(i).mapid();
					instanceDiffculty.num =  info.list(i).num();
					instanceDiffculty.pass =  info.list(i).pass();
					instanceDiffculty.reset =  info.list(i).reset();
					instanceDiffculty.time =  info.list(i).time();

					m_YesterdayInstance[info.list(i).mapid()] = instanceDiffculty;
				}
			}
		}

		ResetEliteInstance();
	}
}

//保存精英副本信息
void InstanceMgr::SetEliteInstanceInfo(PlayerInfo::EliteInstance *info)
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceElite.begin();
	for(; itr != m_InstanceElite.end(); ++itr)
	{
		PlayerInfo::InstanceEliteList * pElite = info->add_list();
		pElite->set_mapid(itr->second.mapId);
		pElite->set_num(itr->second.num);
		pElite->set_pass(itr->second.pass);
		pElite->set_reset(itr->second.reset);
		pElite->set_time(itr->second.time);
	}
}

void InstanceMgr::ResetEliteInstance()
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceElite.begin();
	for(; itr != m_InstanceElite.end(); ++itr)
	{
		itr->second.ReSet();
	}

	m_owner->SetDataFlag(eEliteInstanceInfo);
}

//获取恶梦副本信息
void InstanceMgr::GetCauchemarInstanceInfo(MapInstanceProto::CauchemarInstance& info)
{
	int nNum = 0;
	int nReset = 0;
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceCauchemar.begin();
	for(; itr != m_InstanceCauchemar.end(); itr++)
	{
		MapInstanceProto::InstanceCauchemarList* pList = info.add_list();
		pList->set_mapid(itr->second.mapId);
		pList->set_pass(itr->second.pass);

		nNum += itr->second.num;
		nReset = itr->second.reset;
	}

	info.set_num(nNum);
	info.set_reset(nReset);
}

void InstanceMgr::InitCauchemarInstanceInfo(const PlayerInfo::CauchemarInstance& info)
{
	for(int i = 0; i < info.list_size(); i++)
	{
		map<int,sInstanceDiffculty>::iterator itr = m_InstanceCauchemar.find(info.list(i).mapid());
		if(itr != m_InstanceCauchemar.end())
		{
			itr->second.mapId =  info.list(i).mapid();
			itr->second.num =  info.list(i).num();
			itr->second.pass =  info.list(i).pass();
			itr->second.reset =  info.list(i).reset();
			itr->second.time =  info.list(i).time();
		}
		else
		{
			sInstanceDiffculty instanceDiffculty;
			instanceDiffculty.mapId =  info.list(i).mapid();
			instanceDiffculty.num =  info.list(i).num();
			instanceDiffculty.pass =  info.list(i).pass();
			instanceDiffculty.reset =  info.list(i).reset();
			instanceDiffculty.time =  info.list(i).time();

			m_InstanceCauchemar[info.list(i).mapid()] = instanceDiffculty;
		}
	}

	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();
	if(tempType != eDayResetToday)
	{
		if(eDayResetYesterday == tempType)//上一次登录是昨天，玩家今天第一次登录m_YesterdayInstanceSweep直接使用昨天保存的副本信息
		{
			for(int i = 0; i < info.list_size(); i++)
			{
				map<int,sInstanceDiffculty>::iterator it = m_YesterdayInstance.find(info.list(i).mapid());
				if(it != m_YesterdayInstance.end())
				{
					it->second.mapId =  info.list(i).mapid();
					it->second.num =  info.list(i).num();
					it->second.pass =  info.list(i).pass();
					it->second.reset =  info.list(i).reset();
					it->second.time =  info.list(i).time();
				}
				else
				{
					sInstanceDiffculty instanceDiffculty;
					instanceDiffculty.mapId =  info.list(i).mapid();
					instanceDiffculty.num =  info.list(i).num();
					instanceDiffculty.pass =  info.list(i).pass();
					instanceDiffculty.reset =  info.list(i).reset();
					instanceDiffculty.time =  info.list(i).time();

					m_YesterdayInstance[info.list(i).mapid()] = instanceDiffculty;
				}
			}
		}

		ResetCauchemarInstance();
	}

}

//保存恶梦副本信息
void InstanceMgr::SetCauchemarInstanceInfo(PlayerInfo::CauchemarInstance *info)
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceCauchemar.begin();
	for(; itr != m_InstanceCauchemar.end(); itr++)
	{
		PlayerInfo::InstanceCauchemarList* pList = info->add_list();
		pList->set_mapid(itr->second.mapId);
		pList->set_num(itr->second.num);
		pList->set_pass(itr->second.pass);
		pList->set_reset(itr->second.reset);
		pList->set_time(itr->second.time);
	}
}

void InstanceMgr::ResetCauchemarInstance()
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceCauchemar.begin();
	for(; itr != m_InstanceCauchemar.end(); ++itr)
	{
		itr->second.ReSet();
	}

	m_owner->SetDataFlag(eCauchemarInstanceInfo);
}

bool InstanceMgr::GetInstancePass(int nType,int sceneId)
{
	switch(nType)
	{
	case eGroupInstance:
		{
			map<int, GroupInstance>::iterator it = m_GroupInstance.find(sceneId);
			if(it != m_GroupInstance.end())
			{
				return it->second.pass;
			}

			break;
		}
	case eAdvancedInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
			if(it != m_InstanceAdvanced.end())
			{
				return it->second.pass;
			}

			break;
		}
	case eExpInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceExp.find(sceneId);
			if(it != m_InstanceExp.end())
			{
				return it->second.pass;
			}

			break;
		}
	case eMoneyInstance1:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceMoney.find(sceneId);
			if(it != m_InstanceMoney.end())
			{
				return it->second.pass;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
	}

	return false;
}

void InstanceMgr::SetInstancePass(int nType,int sceneId,int nPass)
{
	switch(nType)
	{
	case eGroupInstance:
		{
			map<int, GroupInstance>::iterator it = m_GroupInstance.find(sceneId);
			if(it != m_GroupInstance.end())
			{
				it->second.pass = nPass;
			}
			else
			{
				GroupInstance groupInstance;
				groupInstance.mapId = sceneId;
				groupInstance.pass = nPass;
				m_GroupInstance[sceneId] = groupInstance;
			}

			m_owner->SetDataFlag(eGroupInstanceInfo);
			break;
		}
	case eAdvancedInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
			if(it != m_InstanceAdvanced.end())
			{
				it->second.pass = nPass;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = nPass;
				m_InstanceAdvanced[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eAdvancedInstanceInfo);
			break;
		}
	case eExpInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceExp.find(sceneId);
			if(it != m_InstanceExp.end())
			{
				it->second.pass = nPass;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = nPass;
				m_InstanceExp[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
	case eMoneyInstance1:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceMoney.find(sceneId);
			if(it != m_InstanceMoney.end())
			{
				it->second.pass = nPass;
			}
			else
			{
				sInstanceDiffculty instanceDiffculty;
				instanceDiffculty.mapId = sceneId;
				instanceDiffculty.pass = nPass;
				m_InstanceMoney[sceneId] = instanceDiffculty;
			}

			m_owner->SetDataFlag(eInstanceInfo);
			break;
		}
	}
}


int InstanceMgr::CanNewStartSweep(int sceneId,InstanceType eType)
{
	switch(eType)
	{
		case ePersonalInstance:
		{
			map<int,sInstanceDiffculty>::iterator it = m_Instance.find(sceneId);
			if(it != m_Instance.end())
			{
				if(it->second.pass == 0)
				{
					return eNotPassInstance;
				}
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
		case eEliteInstance:
		{
			map<int,sInstanceDiffculty>::iterator it = m_InstanceElite.find(sceneId);
			if(it != m_InstanceElite.end())
			{
				if(it->second.pass == 0)
				{
					return eNotPassInstance;
				}
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
		case eCauchemarInstance:
		{
			map<int,sInstanceDiffculty>::iterator it = m_InstanceCauchemar.find(sceneId);
			if(it != m_InstanceCauchemar.end())
			{
				if(it->second.pass == 0)
				{
					return eNotPassInstance;
				}
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
		case eAdvancedInstance:
		{
			map<int,sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
			if(it != m_InstanceAdvanced.end())
			{
				if(it->second.pass == 0)
				{
					return eNotPassInstance;
				}
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
		case eExpInstance:
		{
			map<int,sInstanceDiffculty>::iterator it = m_InstanceExp.find(sceneId);
			if(it != m_InstanceExp.end())
			{
				if(it->second.pass == 0)
				{
					return eNotPassInstance;
				}
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
		case eMoneyInstance1:
		{
			map<int,sInstanceDiffculty>::iterator it = m_InstanceMoney.find(sceneId);
			if(it != m_InstanceMoney.end())
			{
				if(it->second.pass == 0)
				{
					return eNotPassInstance;
				}
			}
			else
			{
				return eNotPassInstance;
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return 0;
}
//获取进阶副本信息
void InstanceMgr::GetAdvancedInstanceInfo(PlayerInfo::AdvancedInstance& info)
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceAdvanced.begin();
	for(; itr != m_InstanceAdvanced.end(); itr++)
	{
		PlayerInfo::InstanceAdvancedList* pList = info.add_list();
		pList->set_mapid(itr->second.mapId);
		pList->set_num(itr->second.num);
		pList->set_pass(itr->second.pass);
		pList->set_reset(itr->second.reset);
		pList->set_time(itr->second.time);
	}
}

//初始化进阶副本信息
void InstanceMgr::InitAdvancedInstanceInfo(const PlayerInfo::AdvancedInstance& info)
{
	for(int i = 0; i < info.list_size(); i++)
	{
		map<int,sInstanceDiffculty>::iterator itr = m_InstanceAdvanced.find(info.list(i).mapid());
		if(itr != m_InstanceAdvanced.end())
		{
			itr->second.mapId =  info.list(i).mapid();
			itr->second.num =  info.list(i).num();
			itr->second.pass =  info.list(i).pass();
			itr->second.reset =  info.list(i).reset();
			itr->second.time =  info.list(i).time();
		}
		else
		{
			sInstanceDiffculty instanceDiffculty;
			instanceDiffculty.mapId =  info.list(i).mapid();
			instanceDiffculty.num =  info.list(i).num();
			instanceDiffculty.pass =  info.list(i).pass();
			instanceDiffculty.reset =  info.list(i).reset();
			instanceDiffculty.time =  info.list(i).time();

			m_InstanceAdvanced[info.list(i).mapid()] = instanceDiffculty;
		}
	}

	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();
	if(tempType != eDayResetToday)
	{
		if(eDayResetYesterday == tempType)//上一次登录是昨天，玩家今天第一次登录m_YesterdayInstanceSweep直接使用昨天保存的副本信息
		{
			for(int i = 0; i < info.list_size(); i++)
			{
				map<int,sInstanceDiffculty>::iterator it = m_YesterdayInstance.find(info.list(i).mapid());
				if(it != m_YesterdayInstance.end())
				{
					it->second.mapId =  info.list(i).mapid();
					it->second.num =  info.list(i).num();
					it->second.pass =  info.list(i).pass();
					it->second.reset =  info.list(i).reset();
					it->second.time =  info.list(i).time();
				}
				else
				{
					sInstanceDiffculty instanceDiffculty;
					instanceDiffculty.mapId =  info.list(i).mapid();
					instanceDiffculty.num =  info.list(i).num();
					instanceDiffculty.pass =  info.list(i).pass();
					instanceDiffculty.reset =  info.list(i).reset();
					instanceDiffculty.time =  info.list(i).time();

					m_YesterdayInstance[info.list(i).mapid()] = instanceDiffculty;
				}
			}
		}

		ResetAdvancedInstance();
	}
}

//保存进阶副本信息
void InstanceMgr::SetAdvancedInstanceInfo(PlayerInfo::AdvancedInstance *info)
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceAdvanced.begin();
	for(; itr != m_InstanceAdvanced.end(); ++itr)
	{
		PlayerInfo::InstanceAdvancedList * pAdvanced = info->add_list();
		pAdvanced->set_mapid(itr->second.mapId);
		pAdvanced->set_num(itr->second.num);
		pAdvanced->set_pass(itr->second.pass);
		pAdvanced->set_reset(itr->second.reset);
		pAdvanced->set_time(itr->second.time);
	}
}

void InstanceMgr::ResetAdvancedInstance()
{
	map<int,sInstanceDiffculty>::iterator itr = m_InstanceAdvanced.begin();
	for(; itr != m_InstanceAdvanced.end(); ++itr)
	{
		itr->second.ReSet();
	}

	m_owner->SetDataFlag(eAdvancedInstanceInfo);
}

int InstanceMgr::GetRateSuccessReward(int nType,int sceneId)
{
	int rate = 1;
	switch(nType)
	{
	case eAdvancedInstance:
		{
			map<int, sInstanceDiffculty>::iterator it = m_InstanceAdvanced.find(sceneId);
			if(it != m_InstanceAdvanced.end() && it->second.reset > 1)
			{
				rate = 2;
			}

			break;
		}
	default:
		rate = 1;
	}

	return rate;
}
