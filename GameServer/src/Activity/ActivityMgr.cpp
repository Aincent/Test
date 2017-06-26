/*
 * ActivityMgr.cpp
 *
 *  Created on: 2014年12月1日
 *      Author: root
 */

#ifndef ACTIVITYMGR_CPP_
#define ACTIVITYMGR_CPP_

#include "ActivityMgr.h"
#include "CounterDefine.h"
#include "CharDefine.h"
#include "../Counter/Counter.h"
#include "../Object/Player/Player.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "ActivityMessage.h"
#include <math.h>
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "FileLoader/YellowVipLoader.h"
#include "../Object/FileLoader/GirlWarLoader.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../ServerEventDefine.h"
#include "FileLoader/TreasonKingLoader.h"
#include "FileLoader/ActivitiesTimerLoader.h"
#include "../StatisticMgr/StatisticHandle.h"

ActivityMgr::ActivityMgr(Player* obj):m_owner(obj)
{
	ReInit();
}

ActivityMgr::~ActivityMgr()
{
	this->ReleaseActivity();
}

void ActivityMgr::ReInit()
{
	m_finishTime = 0;
	m_curNode = 0;
	m_reward = 0xFFFFFFFF;
	m_pastNode = 0;
	m_PKAwardGetIndex = 0;
	m_swapstime = 0;
	m_lefttime = 0;
	m_yellowVpInfo.clear();
	bzero(&m_TreasonBestTime,sizeof(m_TreasonBestTime));
}

void ActivityMgr::ReleaseActivity()
{
	if(m_finishTime.IsValid())
	{
		m_finishTime.SetNull();
	}

	for(int i=0; i<TREASONKINGMAPLEVEL; ++i)
	{
		m_TreasonBestTime[i] = 0;
	}

	m_yellowVpInfo.clear();
}

void ActivityMgr::AddCurNode()
{
	if(m_curNode >= ProtectedKingLoader::GetInstance()->GetMaxNode())
		return;
	SetCurNode(m_curNode + 1);
	if(m_curNode > m_pastNode)
	{
		m_pastNode = m_curNode;
//		ProtectedReward* pPR = ProtectedKingLoader::GetInstance()->GetProtectedReward(GetPKGetAwardIndex() + 1);
//		if (NULL != pPR && m_pastNode == pPR->node)
//		{
//		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndForceSyn, UIHLType(eUIHLBig_InstanceHall, eUIHLSmall_InstanceHall_GuardKingCanChallenge), 1, false);
//		}

		if (m_owner->GetMyself())
		{
			PlayerProtectKingPassMaxArgs tArgs(m_owner->GetMyself(), m_pastNode);
			m_owner->FireEvent(PLAYER_PROTECTKING_PASSMAXNODE, tArgs);
		}
	}

	m_owner->SetDataFlag(eActivityInfo);
}

void ActivityMgr::SetActivitytableInfo(PlayerInfo::Activity* info)
{
	info->set_curnode(m_curNode);
	info->set_preward(m_reward);
	info->set_pastnode(m_pastNode);
	info->set_swapstime(GetLeftTime());

	map<int,map<int,int> >::iterator itr = m_yellowVpInfo.begin();
	for(; itr != m_yellowVpInfo.end(); itr ++)
	{
		PlayerInfo::ActivityYellowVipReward* vipReward = info->add_vipreward();
		vipReward->set_activityid(itr->first);

		map<int,int>::iterator it = itr->second.begin();
		for(; it != itr->second.end(); ++it)
		{
			PlayerInfo::ActivityYellowVipRewardList* rewardList = vipReward->add_rewardlist();
			rewardList->set_value(it->first);
			rewardList->set_state(it->second);
		}
	}

	for(int i = 0; i < TREASONKINGMAPLEVEL; i++)
	{
		info->add_treasonkingbesttime(m_TreasonBestTime[i]);
	}
}

void ActivityMgr::InitFromDB(const PlayerInfo::Activity& info)
{
	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();

	for(int i = 0; i < info.treasonkingbesttime_size(); i++)
	{
		m_TreasonBestTime[i] = info.treasonkingbesttime(i);
	}

	for(int i = 0; i < info.vipreward_size(); i++)
	{
		const PlayerInfo::ActivityYellowVipReward&  vipReward = info.vipreward(i);
		if(vipReward.activityid() == eDaily_Packs && tempType != eDayResetToday)
		{
			continue;
		}

		map<int,int> rList;
		for(int j = 0; j < vipReward.rewardlist_size(); j++)
		{
			const PlayerInfo::ActivityYellowVipRewardList& rewardList = vipReward.rewardlist(j);
			rList[rewardList.value()] = rewardList.state();
		}

		m_yellowVpInfo[vipReward.activityid()] =  rList;
	}

	SetCurNode(info.curnode());
	m_reward = info.preward();
	m_pastNode = info.pastnode();

	if(m_pastNode == 0)
	{
		m_reward = 0xFFFFFFFF;
	}
	else if(m_reward <= 0xFF) //兼容原有数据库存贮
	{
		m_reward |= 0xFFFFFF00;
	}

	m_owner->SetInitPartDataFlag(eActivityInfo);

	const int PROTECT_KING_SCENECID = GameParaLoader::GetInstance()->getProtectKingSceneId();

	if(info.swapstime() > 0)
	{
		if(!CMapConfig::GetInstance()->IsHaveMap(PROTECT_KING_SCENECID))
		{
			return;
		}

		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(PROTECT_KING_SCENECID);
		if (!entry)
		{
			LOG_DEBUG(FILEINFO,"InstanceSweep::startSweep: intanceId is invalid %d.",PROTECT_KING_SCENECID);
			return;
		}

		// 检查关卡限制
		if (entry->m_nLevel > 0 && (DWORD)m_owner->getAttManage()->getValue<int>(eCharLv) < entry->m_nLevel)
		{
			return;
		}

		Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
		if(!pPacket)
		{
			return;
		}

		if(m_finishTime.IsValid())
		{
			return;
		}

		if(m_pastNode == 0)
		{
			return;
		}

		m_swapstime = CUtil::GetNowSecond();

		//兼容以前的版本，以前info.swapstime()保存的时间是GetNowSeconds的时间
		int tAllTime = ProtectedKingLoader::GetInstance()->GetNodeSweepTime(0, ProtectedKingLoader::GetInstance()->GetMaxNode());
		if(tAllTime < info.swapstime())
			m_lefttime = tAllTime;
		else
			m_lefttime = info.swapstime();

		vector<int> vecType;
		vecType.push_back(ePower);
		m_owner->SynCharAttribute(vecType);

		m_owner->SetDataFlag(eActivityInfo);
		m_owner->SetDataFlag(eBattleInfo);

		if(m_finishTime.IsValid())
		{
			m_finishTime.SetNull();
		}
		m_finishTime = TimerInterface::GetInstance()->RegTimePoint(&ActivityMgr::EndSweep, this, 0, 0, 0, 0, m_lefttime);
	}

	int realNode = 0;
	SetPKGetAwardIndex(ProtectedKingLoader::GetInstance()->GetCanGetAwardNode(m_pastNode, realNode));
}

void ActivityMgr::GetProtectKingInfo(ActivityKing::AckClientKingInfo& info)
{
	info.set_maxnode(m_pastNode);
	info.set_node(m_curNode);
	info.set_reward(m_reward);

	Smart_Ptr<CounterService> & counter = m_owner->GetCounterService();
	if(counter)
	{
		int vipResetCnt = 0;//(int)m_owner->GetVipPart()->GetValueByRight(eVipGuardKingResetCnt);
		int resetCnt = ProtectedKingLoader::GetInstance()->GetTimes();
		if (0 < vipResetCnt)
		{
			resetCnt = resetCnt + vipResetCnt;
		}
		info.set_relush(resetCnt - counter->GetNumById(PROTECT_KING_COUNTER));
	}

	info.set_time(GetLeftTime());
}

int ActivityMgr::StartSweep(int sceneId)
{
	if(!CMapConfig::GetInstance()->IsHaveMap(sceneId))
	{
		return eInstanceInfoError;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceSweep::startSweep: intanceId is invalid %d.",sceneId);
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

	if(m_finishTime.IsValid())
	{
		return eInInstanceSweep;
	}
	if(m_pastNode == 0)
	{
		return eNotPassInstance;
	}

// 守护秦王扫荡取消次数限制   09－17
//	if(m_owner->GetCounterService()->GetNumById(PROTECT_KING_SWEEP) >= ProtectedKingLoader::GetInstance()->GetSweepTimes())
//	{
//		return eInstanceSweepTimeLimit;
//	}

	vector<int> vecType;
	vecType.push_back(ePower);
	m_owner->SynCharAttribute(vecType);

	m_swapstime = CUtil::GetNowSecond();
	m_lefttime = ProtectedKingLoader::GetInstance()->GetNodeSweepTime(m_curNode,m_pastNode);

	m_owner->SetDataFlag(eActivityInfo);
	m_owner->SetDataFlag(eBattleInfo);

	m_owner->GetCounterService()->IncreasePlayerCounter(PROTECT_KING_SWEEP,1);

	m_owner->GetVitalityMgr()->FinshMapInstanceVitality(sceneId);

	if(m_finishTime.IsValid())
	{
		m_finishTime.SetNull();
	}

	m_finishTime = TimerInterface::GetInstance()->RegTimePoint(&ActivityMgr::EndSweep, this, 0, 0, 0, 0,m_lefttime);

	return 0;
}



void ActivityMgr::EndSweep(void * arg)
{
	int exp = 0;
	int force = 0;
	int money = 0;
	vector<int> vec;

	BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eProtectKing, BehaviorType::Minor::eProtectKing_SweepOver, MONITOR);

	ProtectedKingLoader::GetInstance()->GetNodeReward(GetCurNode(),GetPastNode(), exp, force, money);

	m_owner->AddExp(exp);
	m_owner->ChangeMoney(money, true);
	m_owner->ChangeCurForce(force, true);

	vec.push_back(eCharMoney);
	vec.push_back(eCharForce);

	m_owner->SynCharAttribute(vec);

	SetCurNode(GetPastNode());
	SetSwapstime(0);
	SetLeftTime(0);
	m_owner->SetDataFlag(eActivityInfo);

//	TimerInterface::GetInstance()->DeleteTimer(pActivity->GetFinishTime());
//	pActivity->SetFinishTime(0);
	if(m_finishTime.IsValid())
	{
		m_finishTime.SetNull();
	}

//	StatisticInfo::CostAndGotInfo costAngGotInfo;
//	if(exp > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharExp,exp);
//	}
//
//	if(money > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,exp);
//	}
//
//	if(force > 0)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAngGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,exp);
//	}
//
//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_ProtectKing,eStaMinor_ProtectKing_RewardInCarbon,&costAngGotInfo);

	
}

int64 ActivityMgr::GetLeftTime()
{
	if(m_finishTime.IsValid())
	{
		m_owner->SetDataFlag(eActivityInfo);
		return m_swapstime + m_lefttime - CUtil::GetNowSecond();
	}

	return 0;
}

void ActivityMgr::SetCurNode(int curNode)
{
	m_curNode = curNode;
	Smart_Ptr<CounterService> & counter = m_owner->GetCounterService();
	if(counter)
		counter->SetPlayerCounter(PROTECT_KING_CURRENT_COUNTER,m_curNode);
}

bool ActivityMgr::IsCanEnterActive(int sceneId)
{
//	const int sceneId = GameParaLoader::GetInstance()->getProtectKingSceneId();

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(sceneId);
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"intanceId is invalid %d.",sceneId);
		return false;
	}

	// 检查关卡限制
	if (entry->m_nLevel > 0 && (DWORD)m_owner->getAttManage()->getValue<int>(eCharLv) < entry->m_nLevel)
	{
		return false;
	}

	return true;
}

//int ActivityMgr::RecoverResourceActive(int* num, sRecoverResource& resource)
//{
//	int vipCount = 0;
////	if(m_owner->GetVipPart())
////	{
////		vipCount = int(m_owner->GetVipPart()->GetValueByRight(eVipGuardKingResetCnt));
////		if(VIP_NOT_HAVE_THIS_RIGHT == vipCount)
////			vipCount = 0;
////	}
//
//	int restNum =	vipCount + ProtectedKingLoader::GetInstance()->GetTimes() - *num;
//	if(restNum <=0)
//		return eInstanceTimesNotEnough;
//
//	* num = restNum;
//
//	//昨天打到第几层
//	int maxLvYesterday = m_owner->GetCounterService()->GetYesterdayNumById(PROTECT_KING_CURRENT_COUNTER);
//
//	if(maxLvYesterday >= this->GetPastNode())//昨天已经打到了历史最高层数，就没有可追回的资源
//		return eDontHaveRcoverReward;
//
//	int exp = 0;
//	int money = 0;
//	int force = 0;
//
//	ProtectedKingLoader::GetInstance()->GetNodeReward(maxLvYesterday,GetPastNode(), exp, force, money);
//
//	resource.exp += exp * restNum;
//	resource.force += force * restNum;
//	resource.money += money * restNum;
//
//	return 0;
//}

void ActivityMgr::GetYellowVipInfo(YellowVipInfo::ToClentYellowVipInfo& info)
{
	info.set_ret(0);
	info.set_is_yellow_vip(m_owner->GetYelloVip());
	info.set_is_yellow_year_vip(m_owner->GetYellowYearVip());
	info.set_yellow_vip_level(m_owner->GetYellowVipLevel());
	info.set_is_yellow_high_vip(m_owner->GetYellowHighVip());

	vector<DiamondTypes> vDiamon;
	YellowVipLoader::GetInstance()->GetDiamondTypes(vDiamon);

	for(size_t i = 0; i < vDiamon.size();i++)
	{
		switch(vDiamon[i].nType)
		{
		case eNovice_Packs:
			{
				if(vDiamon[i].nProfess == m_owner->getAttManage()->getValue<BYTE>(eCharProfession) || vDiamon[i].nProfess == 0)
				{
					YellowVipInfo::NovicePacks* novicePacks = info.mutable_novicepacks();
					for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
					{
						novicePacks->add_itemid(vDiamon[i].m_totalItem[j]);
						novicePacks->add_itemnum(vDiamon[i].m_totalNum[j]);
					}

					if(m_owner->GetYelloVip() && GetYellowVipState(eNovice_Packs,eNovice_Packs) == RewardState_DontHave)
					{
						SetYellowVipState(eNovice_Packs,eNovice_Packs,RewardState_NotGet);
					}

					novicePacks->set_state(GetYellowVipState(eNovice_Packs,eNovice_Packs));
				}

				break;
			}
		case eGrow_Packs:
			{
				YellowVipInfo::GrowPacks* growPacks = info.add_growpacks();
				for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
				{
					growPacks->add_itemid(vDiamon[i].m_totalItem[j]);
					growPacks->add_itemnum(vDiamon[i].m_totalNum[j]);
				}

				growPacks->set_value(vDiamon[i].nLv);

				if(m_owner->GetYelloVip() && GetYellowVipState(eGrow_Packs,vDiamon[i].nLv) == RewardState_DontHave && vDiamon[i].nLv <= m_owner->getAttManage()->getValue<BYTE>(eCharLv))
				{
					SetYellowVipState(eGrow_Packs,vDiamon[i].nLv,RewardState_NotGet);
				}

				growPacks->set_state(GetYellowVipState(eGrow_Packs,vDiamon[i].nLv));

				break;
			}
		case eDaily_Packs:
			{
				YellowVipInfo::DailyPacks* dailyPacks = info.add_dailypacks();
				for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
				{
					dailyPacks->add_itemid(vDiamon[i].m_totalItem[j]);
					dailyPacks->add_itemnum(vDiamon[i].m_totalNum[j]);
				}

				dailyPacks->set_value(vDiamon[i].nLv);

				if(m_owner->GetYelloVip() && GetYellowVipState(eDaily_Packs,vDiamon[i].nLv) == RewardState_DontHave)
				{
					SetYellowVipState(eDaily_Packs,vDiamon[i].nLv,RewardState_NotGet);
				}

				dailyPacks->set_state(GetYellowVipState(eDaily_Packs,vDiamon[i].nLv));

				break;
			}
		case eYearVipPacks:
			{
				YellowVipInfo::YearVipPacks* yearVipPacks = info.mutable_yearvippacks();
				for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
				{
					yearVipPacks->add_itemid(vDiamon[i].m_totalItem[j]);
					yearVipPacks->add_itemnum(vDiamon[i].m_totalNum[j]);
				}

				if(m_owner->GetYellowYearVip() && GetYellowVipState(eYearVipPacks,eYearVipPacks) == RewardState_DontHave)
				{
					SetYellowVipState(eYearVipPacks,eYearVipPacks,RewardState_NotGet);
				}

				yearVipPacks->set_state(GetYellowVipState(eYearVipPacks,eYearVipPacks));

				break;
			}
		case eGirlWarPacks:
			{
				YellowVipInfo::GirlWarPacks* girlwarpacks = info.mutable_girlwarpacks();
				for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
				{
					girlwarpacks->set_id(vDiamon[i].m_totalItem[j]);
				}

				if(m_owner->GetYelloVip() && GetYellowVipState(eGirlWarPacks,eGirlWarPacks) == RewardState_DontHave && vDiamon[i].nLv <= m_owner->GetYellowVipLevel())
				{
					SetYellowVipState(eGirlWarPacks,eGirlWarPacks,RewardState_NotGet);
				}

				girlwarpacks->set_state(GetYellowVipState(eGirlWarPacks,eGirlWarPacks));

				break;
			}
		default:
			{
				LOG_DEBUG(FILEINFO,"Yellow Vip types error");
			}
		}
	}
}

int ActivityMgr::GetYellowVipState(int activityId,int value)
{
	map<int,map<int,int> >::iterator itr = m_yellowVpInfo.find(activityId);
	if(itr != m_yellowVpInfo.end())
	{
		map<int,int>::iterator it = itr->second.find(value);
		if(it != itr->second.end())
		{
			return it->second;
		}
	}

	return 0;
}

void ActivityMgr::SetYellowVipState(int activityId,int value,int state)
{
	map<int,map<int,int> >::iterator itr = m_yellowVpInfo.find(activityId);
	if(itr != m_yellowVpInfo.end())
	{
		map<int,int>::iterator it = itr->second.find(value);
		if(it != itr->second.end())
		{
			it->second =state;
		}
		else
		{
			itr->second[value] = state;
		}
	}
	else
	{
		map<int,int> mList;
		mList[value] = state;
		m_yellowVpInfo[activityId] = mList;
	}

	m_owner->SetDataFlag(eActivityInfo);
}

int ActivityMgr::GetYellowVipReward(int activityId,int value)
{
	int res  = 0;
	vector<int> vecPos;
	vector<DiamondTypes> vDiamon;
	YellowVipLoader::GetInstance()->GetDiamondTypes(vDiamon);

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	idAndCostAndGotInfo.set_beforeid(activityId);
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

	int money = 0;

	switch(activityId)
	{
	case eNovice_Packs:
		{
			for(size_t i = 0; i < vDiamon.size();i++)
			{
				if(vDiamon[i].nType == eNovice_Packs && (vDiamon[i].nProfess == m_owner->getAttManage()->getValue<BYTE>(eCharProfession) || vDiamon[i].nProfess == 0))
				{
					for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
					{
						const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(vDiamon[i].m_totalItem[j]);
						if(ginfo==NULL)
						{
							return eRewardHailConfigError;
						}

						if(vDiamon[i].m_totalItem[j] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()))
						{
							money = vDiamon[i].m_totalNum[j];
						}
					}

					vector<WORD> tempFlagsList;
					tempFlagsList.resize(vDiamon[i].m_totalItem.size(),1);

					res = m_owner->GetContainer(ePackageType)->IsBagEnought(vDiamon[i].m_totalItem, vDiamon[i].m_totalNum,tempFlagsList);
					if(!res)
					{
						vector<WORD> bindVec;

						bindVec.resize(vDiamon[i].m_totalItem.size(),1);

						m_owner->GetContainer(ePackageType)->AddItem(vDiamon[i].m_totalItem,vDiamon[i].m_totalNum,vecPos,bindVec,vDiamon[i].m_totalLv,true,npc_fly_type);

//						for(size_t j = 0;j < vDiamon[i].m_totalItem.size();++j)
//						{
//							StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo->add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotItemInfo,vDiamon[i].m_totalItem[j],vDiamon[i].m_totalNum[j]);
//						}

						SetYellowVipState(activityId,value,RewardState_HaveGet);

						m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_NoviceGiftBag),
								1, false);
					}

					break;
				}
			}

			break;
		}
	case eGrow_Packs:
		{
			for(size_t i = 0; i < vDiamon.size();i++)
			{
				if(vDiamon[i].nType == eGrow_Packs && vDiamon[i].nLv == value)
				{
					for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
					{
						const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(vDiamon[i].m_totalItem[j]);
						if(ginfo==NULL)
						{
							return eRewardHailConfigError;
						}

						if(vDiamon[i].m_totalItem[j] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()))
						{
							money = vDiamon[i].m_totalNum[j];
						}
					}

					vector<WORD> tempFlagsList;
					tempFlagsList.resize(vDiamon[i].m_totalItem.size(),1);

					res = m_owner->GetContainer(ePackageType)->IsBagEnought(vDiamon[i].m_totalItem, vDiamon[i].m_totalNum,tempFlagsList);
					if(!res)
					{
						vector<WORD> bindVec;

						bindVec.resize(vDiamon[i].m_totalItem.size(),1);

						m_owner->GetContainer(ePackageType)->AddItem(vDiamon[i].m_totalItem,vDiamon[i].m_totalNum,vecPos,bindVec,vDiamon[i].m_totalLv,true,npc_fly_type);

//						for(size_t j = 0;j < vDiamon[i].m_totalItem.size();++j)
//						{
//							StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo->add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotItemInfo,vDiamon[i].m_totalItem[j],vDiamon[i].m_totalNum[j]);
//						}

						SetYellowVipState(activityId,value,RewardState_HaveGet);

						m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_GrowthGiftBag),
								1, false);
					}

					break;
				}
			}

			break;
		}
	case eDaily_Packs:
		{
			for(size_t i = 0; i < vDiamon.size();i++)
			{
				if(vDiamon[i].nType == eDaily_Packs && vDiamon[i].nLv == value)
				{
					for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
					{
						const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(vDiamon[i].m_totalItem[j]);
						if(ginfo==NULL)
						{
							return eRewardHailConfigError;
						}

						if(vDiamon[i].m_totalItem[j] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()))
						{
							money = vDiamon[i].m_totalNum[j];
						}
					}

					vector<WORD> tempFlagsList;
					tempFlagsList.resize(vDiamon[i].m_totalItem.size(),1);

					res = m_owner->GetContainer(ePackageType)->IsBagEnought(vDiamon[i].m_totalItem, vDiamon[i].m_totalNum,tempFlagsList);
					if(!res)
					{
						vector<WORD> bindVec;

						bindVec.resize(vDiamon[i].m_totalItem.size(),1);

						m_owner->GetContainer(ePackageType)->AddItem(vDiamon[i].m_totalItem,vDiamon[i].m_totalNum,vecPos,bindVec,vDiamon[i].m_totalLv,true,npc_fly_type);

//						for(size_t j = 0;j < vDiamon[i].m_totalItem.size();++j)
//						{
//							StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo->add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotItemInfo,vDiamon[i].m_totalItem[j],vDiamon[i].m_totalNum[j]);
//						}

						SetYellowVipState(activityId,value,RewardState_HaveGet);

						m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_DailyGiftBag),
								1, false);
					}

					break;
				}
			}

			break;
		}
	case eYearVipPacks:
		{
			for(size_t i = 0; i < vDiamon.size();i++)
			{
				if(vDiamon[i].nType == eYearVipPacks)
				{
					for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
					{
						const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(vDiamon[i].m_totalItem[j]);
						if(ginfo==NULL)
						{
							return eRewardHailConfigError;
						}

						if(vDiamon[i].m_totalItem[j] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()))
						{
							money = vDiamon[i].m_totalNum[j];
						}
					}

					vector<WORD> tempFlagsList;
					tempFlagsList.resize(vDiamon[i].m_totalItem.size(),1);

					res = m_owner->GetContainer(ePackageType)->IsBagEnought(vDiamon[i].m_totalItem, vDiamon[i].m_totalNum,tempFlagsList);
					if(!res)
					{
						vector<WORD> bindVec;

						bindVec.resize(vDiamon[i].m_totalItem.size(),1);

						m_owner->GetContainer(ePackageType)->AddItem(vDiamon[i].m_totalItem,vDiamon[i].m_totalNum,vecPos,bindVec,vDiamon[i].m_totalLv,true,npc_fly_type);

//						for(size_t j = 0;j < vDiamon[i].m_totalItem.size();++j)
//						{
//							StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo->add_gotlist();
//							StatisticMgr::AddItemGotInfo(gotItemInfo,vDiamon[i].m_totalItem[j],vDiamon[i].m_totalNum[j]);
//						}

						SetYellowVipState(activityId,value,RewardState_HaveGet);

						m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_DailyGiftBag),
								1, false);
					}

					break;
				}
			}

			break;
		}
	case eGirlWarPacks:
		{
			for(size_t i = 0; i < vDiamon.size();i++)
			{
				if(vDiamon[i].nType == eGirlWarPacks)
				{
					for(size_t j = 0; j < vDiamon[i].m_totalItem.size(); j++)
					{
						const sGirlWarInfo *pInfo = GirlWarLoader::GetInstance()->GetGirlWarInfo(vDiamon[i].m_totalItem[j]);
						if(!pInfo)
						{
							return eRewardHailConfigError;
						}

						m_owner->GetGirlWarManager()->GreateGirlWar(vDiamon[i].m_totalItem[j]);

//						StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo->add_gotlist();
//						StatisticMgr::AddItemGotInfo(gotItemInfo,vDiamon[i].m_totalItem[j],vDiamon[i].m_totalNum[j]);

						SetYellowVipState(activityId,value,RewardState_HaveGet);

						m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_YellowVip, eUIHLSmall_YellowVip_ExclusiveGirl),
								1, false);
					}
				}
			}

			break;
		}
	}

//	if(0 == res && m_owner && eAccountType_User == m_owner->GetAccountType())
//	{
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_PackageGift,eStaMinor_PackageGift_YellowRewardEvent,&idAndCostAndGotInfo);

//		if(money > 0)
//		{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_YellowDiamond,&costAndGotInfo);
//		}
//	}

	return res;
}
int ActivityMgr::YNoviceRewardNotGet()
{
	int count = 0;
	YellowVipInfo::ToClentYellowVipInfo info;
	GetYellowVipInfo(info);

	if (info.has_novicepacks())
	{
		if (RewardState_NotGet == info.novicepacks().state())
		{
			 count++;
		}
	}

	return count;
}
int ActivityMgr::YGrowthRewardNotGet()
{
	int count = 0;
	YellowVipInfo::ToClentYellowVipInfo info;
	GetYellowVipInfo(info);

	for (int i = 0; i < info.growpacks_size(); ++i)
	{
		if (RewardState_NotGet == info.growpacks(i).state())
		{
			count++;
		}
	}
	return count;
}
int ActivityMgr::YDailyRewardNotGet()
{
	int count = 0;
	YellowVipInfo::ToClentYellowVipInfo info;
	GetYellowVipInfo(info);

	for (int i = 0; i < info.dailypacks_size(); ++i)
	{
		if (RewardState_NotGet == info.dailypacks(i).state())
		{
			count++;
		}
	}
	if (info.has_yearvippacks())
	{
		if (RewardState_NotGet == info.yearvippacks().state())
		{
			count++;
		}
	}

	return count;
}
int ActivityMgr::YExclusiveGirlNotGet()
{
	int count = 0;
	YellowVipInfo::ToClentYellowVipInfo info;
	GetYellowVipInfo(info);

	if (info.has_girlwarpacks())
	{
		if (RewardState_NotGet == info.girlwarpacks().state())
		{
			count++;
		}
	}
	return count;
}

//int ActivityMgr::YellowRewardNotGet()
//{
//	int count = 0;
//	YellowVipInfo::ToClentYellowVipInfo info;
//	GetYellowVipInfo(info);
//
//	if (info.has_novicepacks())
//	{
//		if (RewardState_NotGet == info.novicepacks().state())
//		{
//			 count++;
//		}
//	}
//
//	for (int i = 0; i < info.growpacks_size(); ++i)
//	{
//		if (RewardState_NotGet == info.growpacks(i).state())
//		{
//			count++;
//		}
//	}
//
//	for (int i = 0; i < info.dailypacks_size(); ++i)
//	{
//		if (RewardState_NotGet == info.dailypacks(i).state())
//		{
//			count++;
//		}
//	}
//
//	if (info.has_yearvippacks())
//	{
//		if (RewardState_NotGet == info.yearvippacks().state())
//		{
//			count++;
//		}
//	}
//
//	if (info.has_girlwarpacks())
//	{
//		if (RewardState_NotGet == info.girlwarpacks().state())
//		{
//			count++;
//		}
//	}
//
//	return count;
//}

void ActivityMgr::ReSetYellowVipState()
{
	vector<DiamondTypes> vDiamon;
	YellowVipLoader::GetInstance()->GetDiamondTypes(vDiamon);

	for(size_t i = 0; i < vDiamon.size();i++)
	{
		switch(vDiamon[i].nType)
		{
		case eDaily_Packs:
			{
				if(m_owner->GetYelloVip())
				{
					SetYellowVipState(eDaily_Packs,vDiamon[i].nLv,RewardState_NotGet);
				}

				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void ActivityMgr::SetPKGetAwardIndex(int index)
{
	m_PKAwardGetIndex = index;
}
int ActivityMgr::GetPKGetAwardIndex()
{
	return m_PKAwardGetIndex;
}

void ActivityMgr::AddPKGetAwardIndex()
{
	++m_PKAwardGetIndex;
}

void ActivityMgr::DecPKGetAwardIndex()
{
	--m_PKAwardGetIndex;
}

bool ActivityMgr::AddTreasonBestTime(int mapid,int elapseTime)
{
	bool bResult = false;
	int nPos = mapid - TREASONKINGMAPFIRSTID;
	if(nPos >= 0 && nPos < TREASONKINGMAPLEVEL)
	{
			if(m_TreasonBestTime[nPos] > elapseTime || m_TreasonBestTime[nPos] == 0)
			{
				m_TreasonBestTime[nPos] = elapseTime;
				bResult = true;
			}
	}

	return bResult;
}

void ActivityMgr::GetTreasonBestTime(TreasonKingInfo::ToClentTreasonInfo& info)
{
	for(int i = 0; i < TREASONKINGMAPLEVEL; i++)
	{
		info.add_treasonkingbesttime(m_TreasonBestTime[i]);
	}
}

int ActivityMgr::GetTreasonBestTime(int mapid)
{
	int nPos = mapid - TREASONKINGMAPFIRSTID;
	if(nPos >= 0 && nPos < TREASONKINGMAPLEVEL)
	{
			return 	m_TreasonBestTime[nPos];
	}

	return 0;
}

int ActivityMgr::GetPKUnResetNum()
{
	// 是否是VIP 增加重置次数
	if (!m_owner->GetVipPart())
	{
		return 0;
	}

//	int vipResetCnt = (int)m_owner->GetVipPart()->GetValueByRight(eVipGuardKingResetCnt);
	int resetCnt = ProtectedKingLoader::GetInstance()->GetTimes();
//	if (0 < vipResetCnt)
//	{
//		resetCnt = resetCnt + vipResetCnt;
//	}

	resetCnt -= m_owner->GetCounterService()->GetNumById(PROTECT_KING_COUNTER);
	return resetCnt < 0 ? 0 : resetCnt;
}

// 获取守护秦王剩余关卡
int ActivityMgr::GetPKSuplusNode()
{
	return ProtectedKingLoader::GetInstance()->GetMaxNode() - m_curNode;
}

int ActivityMgr::GetTreasonSurplusNum()
{
//	if (!m_owner->GetVipPart())
//	{
//		return 0;
//	}
	int num = 0;
	InstanceMapEntry* instanceInfo = InstanceLoader::GetInstance()->GetEntry(GameParaLoader::GetInstance()->getTreasonKingMapId());
//	if (NULL != instanceInfo && m_owner->IsOpenSystem(eTreasonKingLv))
	{
//		int vipAddCnt = (int)m_owner->GetVipPart()->GetValueByRight(eVipTreasonKingCnt);
		int reaptTimes = instanceInfo->m_nReaptTimes; // 重复次数;
//		int reaptTimes = 0;
//		if (0 < vipAddCnt)
//		{
//			reaptTimes = reaptTimes + vipAddCnt;
//		}
		num += reaptTimes;
		if (m_owner->GetCounterService())
		{
			num -= m_owner->GetCounterService()->GetNumById(TREASON_KING_ENTER);
		}
	}
	return num < 0 ? 0 : num;
}

int ActivityMgr::GetTreansonSuplusPass()
{
	return TreasonKingLoader::GetInstance()->GetMaxLevel() - m_owner->GetCounterService()->GetNumById(TREASON_KING_LEVELS);

}
int ActivityMgr::GetTotalChallengeTrean()
{
	int num = 0;
	InstanceMapEntry* instanceInfo = InstanceLoader::GetInstance()->GetEntry(GameParaLoader::GetInstance()->getTreasonKingMapId());
	if (NULL != instanceInfo) //&& m_owner->IsOpenSystem(eTreasonKingLv))
	{
		num = instanceInfo->m_nReaptTimes; // 重复次数;
	}
	return num;
}

// 获取守护秦王总挑战次数
int ActivityMgr::GetTotalResetPK()
{
	int total = 0;

	total += ProtectedKingLoader::GetInstance()->GetTimes();
//	int vipResetCnt = int(m_owner->GetVipPart()->GetValueByRight(eVipGuardKingResetCnt));
//
//	if (0 < vipResetCnt)
//	{
//		total +=vipResetCnt;
//	}

	return total;



}

int ActivityMgr::GetTreasonSurplusResetCnt(int& surplusReset)
{
//	if (!m_owner->GetVipPart() || !m_owner->GetCounterService() || !m_owner->IsOpenSystem(eTreasonKingLv))
//	{
//		return 0;
//	}

//	int vipAddCnt = (int)m_owner->GetVipPart()->GetValueByRight(eVipTreasonKingCnt);
	int resetTimes = 0;
//	if (0 < vipAddCnt)
//	{
//		resetTimes = vipAddCnt;
//	}

	surplusReset = resetTimes - m_owner->GetCounterService()->GetNumById(TREASON_KING_RESET);

	return resetTimes;
}

#endif /* ACTIVITYMGR_CPP_ */
