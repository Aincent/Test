/*
 * Daily.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */
#include "Daily.h"
#include "math.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "DailyLoader.h"
#include "../Clan/ClanLoader/ClanLoader.h"
#include "../Map/SceneManager.h"
#include "../Activity/ActivityMgr.h"
#include "FileLoader/GameParaLoader.h"
#include <math.h>
#include "../Attribute/AttributeCreature.h"
#include "../MoneyShop/MoneyShopMgr.h"
#include "../EscortEx/EscortMgr.h"
#include "../LucklyTurntable/LucklyTurntableMgr.h"
#include "DailyMgr.h"
#include "../Arena/ArenaMgr.h"

DailyManager::DailyManager(Player* player):m_owner(player)
{
	ReInit();
}

DailyManager::~DailyManager()
{

}

void DailyManager::ReInit()
{
	m_nPracticeTime = 0;
	m_bIsGotPracticeRward = false;
	m_nPracticeExp = 0;
	m_RecoverResourceMap.clear();
}

void DailyManager::Release()
{
	m_RecoverResourceMap.clear();
}


void DailyManager::InitDailyPractice(int64 nPracticeTime)
{
	if(m_owner && m_owner->IsOpenSystem(eOfflineRewardLv) == false)
		return;

	SetPracticeTime(nPracticeTime);
	SetPracticeExp(GetExpByPractice(nPracticeTime));
	if(nPracticeTime > 0)
		m_bIsGotPracticeRward = false;
	else
		m_bIsGotPracticeRward = true;
}

int DailyManager::GetPracticeTime(void)
{
	return this->m_nPracticeTime;
}

void DailyManager::SetPracticeTime(int nPracticeTime)
{
	if(nPracticeTime <= 0 )
		return;
	m_nPracticeTime = nPracticeTime;
}

int DailyManager::GetExpByPractice(int nPracticeTime)
{
	int baseExp = DailyLoader::GetInstance()->GetParcticeParamExp(m_owner->getAttManage()->getValue<int>(eCharLv));
	//以分钟为单位
	float practiceTime = ceil((nPracticeTime)/(60.0f * 1000.0f));
	int exp = int(baseExp*practiceTime);
	return exp;
}

int DailyManager::GetExpReward(int ratio)
{
	return ratio * m_nPracticeExp;
}

void DailyManager::SetPracticeExp(int nPracticeExp)
{
	if(nPracticeExp < 0)
		return;
	m_nPracticeExp = nPracticeExp;
}

int DailyManager::GetDailyPracticeInfo(ServerReturn::ServerDoubleInt & toClient)
{
	if(this->m_bIsGotPracticeRward)
		toClient.set_retf(1);
	else
		toClient.set_retf(m_nPracticeExp);
	toClient.set_rets(m_nPracticeTime);

	return 0;
}

bool DailyManager::IsHaveDailyPracticeToGet(void)
{
	if(this->m_bIsGotPracticeRward)
		return false;
	else
		if(m_nPracticeExp > 0)
			return true;

	return false;
}

int DailyManager::GetDailyPracticeReward(int type)
{
	if(type < GetPracticeType_General || type > GetPracticeType_Treble)
		return eGetDailyPracticeRwardTypeError;

	if(this->m_bIsGotPracticeRward)
		return eDailyPracticeRwardHaveGotted;

	int ratio = type + 1;
	if(type == GetPracticeType_Double)
	{
		if (VIP_NOT_HAVE_THIS_RIGHT == (int)m_owner->GetVipPart()->GetValueByRight(eVipDoubleOfflineExp))
			return ePracticeVIPLvNotEnough;
	}
	else if(type == GetPracticeType_Treble)
	{
		if (VIP_NOT_HAVE_THIS_RIGHT == (int)m_owner->GetVipPart()->GetValueByRight(eVipTripleOfflineExp))
			return ePracticeVIPLvNotEnough;
	}
	int exp = this->GetExpReward(ratio);

	int ret = m_owner->AddExp(exp);
	if(0 == ret)
	{
		m_bIsGotPracticeRward = true;
		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Welfare, eUIHLSmall_Welfare_CanGetOffAward), 1, false);
	}

	return ret;
}

void DailyManager::InitRecoverResource()
{
	m_RecoverResourceMap.clear();

	Daily::RecoverResourceInfoList toClient;

	m_owner->GetCounterService()->GetYesterdayCounterInfo(&toClient);

	m_owner->GetInstanceMgr()->GetYesterdayInstanceInfo(&toClient);

	if(m_owner->GetQuestMgr()->getDailyQuests())
		m_owner->GetQuestMgr()->getDailyQuests()->GetYesterdayDailyQuest(&toClient);

	if(m_owner->GetQuestMgr()->getClanQuests())
		m_owner->GetQuestMgr()->getClanQuests()->GetYesterdayClanQuest(&toClient);

	if(m_owner->GetQuestMgr()->getCampQuests())
		m_owner->GetQuestMgr()->getCampQuests()->GetYesterdayCampQuest(&toClient);

	if(m_owner->GetQuestMgr()->getWeekTask())
		m_owner->GetQuestMgr()->getWeekTask()->GetYesterdayWeekQuest(&toClient);

	for(int i = 0;i < toClient.infolist_size();++i)
	{
		Daily::RecoverResourceInfo info = toClient.infolist().Get(i);
		sRecoverResourceInfo item = sRecoverResourceInfo(&info);
		m_RecoverResourceMap[info.id()] = item;
	}
}


void DailyManager::GetRecoverResourceInfo(Daily::RecoverResourceInfoList& info)
{
	int totalgolden = 0;
	map<int,sRecoverResourceInfo>::iterator itr = m_RecoverResourceMap.begin();
	for(; itr != m_RecoverResourceMap.end(); ++itr)
	{
		sRecorceSourceParam *pRecorce = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(itr->first);
		if(pRecorce)
		{
			totalgolden += pRecorce->golden;
		}

		itr->second.SetRecoverResourceInfo(info.add_infolist());
	}

	info.set_golden(totalgolden);

	info.set_ishaverecover(m_owner->GetCounterService()->GetNumById(RECOVER_RESOURCE_COUNT));
}

int DailyManager::RecoverResource(Daily::C2GMReqRecoverResource & req)
{
	int op = req.op();
	if(op < GetRecoverResourceType_Quick_Common || op > GetRecoverResourceType_Quick_Golden)
		return eRecoverResourceTypeError;

	if(m_RecoverResourceMap.size() == 0)
		return eDontHaveRcoverReward;

	if(m_owner->GetCounterService()->GetNumById(RECOVER_RESOURCE_COUNT) > 0)
		return eDontHaveRcoverReward;

	float radio = 0.0f;
	int totalgolden = 0;
	sRecoverResourceInfo info;
	std::map<int,sRecoverResourceInfo>::iterator it = m_RecoverResourceMap.begin();
	for(; it != m_RecoverResourceMap.end();++it)
	{
		sRecorceSourceParam *pRecorce = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(it->first);
		if(!pRecorce)
		{
			continue;
		}
		else
		{
			if(op == GetRecoverResourceType_Quick_Common)
			{
				radio = pRecorce->accerretireve;
			}
			else
			{
				radio = pRecorce->accerretireve;
				totalgolden += pRecorce->golden;
			}
		}

		sRecoverResourceInfo recoverResourceinfo = it->second;
		info.exp += recoverResourceinfo.exp * radio;
		info.money += recoverResourceinfo.money * radio;
		info.force += recoverResourceinfo.force * radio;
		info.clancontri += recoverResourceinfo.clancontri * radio;
		info.campcontri += recoverResourceinfo.campcontri * radio;
		std::map<int,int>::iterator it = recoverResourceinfo.item.begin();
		for(;it != recoverResourceinfo.item.end();it++)
		{
			std::map<int,int>::iterator itemIt = info.item.find(it->first);
			if(itemIt != info.item.end())
				itemIt->second += (it->second * radio);
			else
				info.item[it->first] = (it->second * radio);
		}
	}

	if(!m_owner->IsEnoughGolden(totalgolden))
		return eNotEnoughGold;

	m_owner->ChangeGolden(totalgolden,false);

	AddResource(info);

	m_owner->GetCounterService()->IncreasePlayerCounter(RECOVER_RESOURCE_COUNT,1);

	m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn,UIHLType(eUIHLBig_Welfare,eUIHLSmall_Welfare_CanResFound),false);
	return 0;
}

int DailyManager::AddResource(const sRecoverResourceInfo& info)
{
	const GoodsInfo *	ptr =	NULL;
	vector<DWORD> idList;
	vector<DWORD> numList;
	int ret = 0;

	std::map<int,int>::const_iterator it = info.item.begin();
	for(; it != info.item.end(); ++it)
	{
		ptr = GoodsLoader::GetInstance()->GetItemDataByID(it->first);
		if(!ptr)
			continue; /*数据有误*/

		idList.push_back(it->first);
		numList.push_back(it->second);
	}

	vector<WORD> flagVec;
	flagVec.resize(idList.size(), 0);
	ret = m_owner->GetContainer(ePackageType)->IsBagEnought(idList, numList,flagVec);

	if(!ret)
	{
		vector<int> vec;
		vector<int> vecPos;
		vector<DWORD> strengthLvVec;

		strengthLvVec.resize(idList.size(), 0);

		ret = m_owner->GetContainer(ePackageType)->AddItem(idList, numList, vecPos, flagVec,strengthLvVec);

		if(info.exp >0)
		{
			m_owner->AddExp(info.exp);
		}

		if(info.clancontri >0)
		{
			m_owner->ChangeFContri(info.clancontri, true);
			vec.push_back(eFaContribution);

			m_owner->SynchCharAttributeToWS(eFaContribution, info.clancontri);
		}

		if(info.campcontri > 0)
		{
			m_owner->ChangeCountryContribution(info.campcontri,true);
		}

		if(info.force >0)
		{
			m_owner->ChangeCurForce(info.force, true);
			vec.push_back(eCharForce);
		}

		if(info.money >0)
		{
			m_owner->ChangeMoney(info.money, true);
			vec.push_back(eCharMoney);
		}

		m_owner->SetDataFlag(eBaseInfo);
		if (vec.size() > 0)
			m_owner->SynCharAttribute(vec);
	}

	return ret;
}

int DailyManager::GetDailyInfo(Daily::DailyToDoInfoList * toClient)
{
	m_owner->GetCounterService()->GetDailyCounterInfo(toClient);

	int protect = m_owner->GetVitalityMgr()->GetVitality(eViProtectKing);
	Daily::DailyToDoInfo * protectInfo = toClient->add_infolist();
	protectInfo->set_id(eDailyToDoType_protectKing);
	protectInfo->set_count(protect);

	int challenge = m_owner->GetVitalityMgr()->GetVitality(eViThreatInstance);
	if(challenge > 1)
		challenge = 1;
	Daily::DailyToDoInfo * challengeInfo = toClient->add_infolist();
	challengeInfo->set_id(eDailyToDoType_challengeInstance);
	challengeInfo->set_count(challenge);

	int single = m_owner->GetVitalityMgr()->GetVitality(eViPersonalInstance);
	Daily::DailyToDoInfo * singleInfo = toClient->add_infolist();
	singleInfo->set_id(eDailyToDoType_singleInstance);
	singleInfo->set_count(single);

//	int group = m_owner->GetVitalityMgr()->GetVitality(eViGroupInstance);
//	Daily::DailyToDoInfo * groupInfo = toClient->add_infolist();
//	groupInfo->set_id(eDailyToDoType_groupInstance);
//	groupInfo->set_count(group);

	if(m_owner->GetQuestMgr()->getDailyQuests())
		m_owner->GetQuestMgr()->getDailyQuests()->GetDailyQuest(toClient);

	if(m_owner->GetQuestMgr()->getClanQuests())
		m_owner->GetQuestMgr()->getClanQuests()->GetClanQuest(toClient);

	int killWorldBoss = m_owner->GetVitalityMgr()->GetVitality(eViBoss);
	Daily::DailyToDoInfo * bossInfo = toClient->add_infolist();
	bossInfo->set_id(eDailyToDoType_worldBoss);
	bossInfo->set_count(killWorldBoss);

	int answer = m_owner->GetVitalityMgr()->GetVitality(eViAnswer);
	Daily::DailyToDoInfo * answerInfo = toClient->add_infolist();
	answerInfo->set_id(eDailyToDoType_answer);
	answerInfo->set_count(answer);

	return 0;
}

int DailyManager::GetEveryDayMustWork()
{
	int count = 0;
	// 日常任务 1
	Smart_Ptr<QuestPart> quest = m_owner->GetQuestMgr();
	if ((bool)quest)
	{
		if ((bool)(quest->getDailyQuests()))
		{
			count += quest->getDailyQuests()->GetUnCompleteNum();
		}
	}
	// 世界Boss 1
	count += m_owner->GetVitalityMgr()->GetUnCompleteByID(eViBoss);
	// 个人副本 1
//	count += m_owner->GetVitalityMgr()->GetVitality(eViPersonalInstance);
	count += m_owner->GetVitalityMgr()->GetUnCompleteByID(eViPersonalInstance);
	// 组队副本 1
	count += m_owner->GetVitalityMgr()->GetUnCompleteByID(eViGroupInstance);
	// 剿灭反王 1
	count += m_owner->GetVitalityMgr()->GetUnCompleteByID(eViTreasonKing);
	// 守护秦王 1
	count += m_owner->GetVitalityMgr()->GetUnCompleteByID(eViProtectKing);
	// 帮派任务 1
	if ((bool)quest)
	{
		if ((bool)(quest->getClanQuests()))
		{
			count += quest->getClanQuests()->GetUnCompleteNum();
		}
	}
	// 钱庄 1
	count += (bool)m_owner->GetMyself() ? MoneyShopMgr::GetInstance()->MoneyShop_IsHaveFreeNum(m_owner->GetMyself()) : 0;
	// 挑战副本 1
	int challengeNum = m_owner->GetVitalityMgr()->GetVitality(eViThreatInstance);
	if (1 < challengeNum)
	{
		challengeNum = 0;
	}
	else
	{
		challengeNum = 1;
	}
	count += challengeNum;
	// 护送黄冈 1
	Smart_Ptr<EscortMgr>& escortMgr = m_owner->GetEscortMgr();
	if ((bool)escortMgr)
	{
		count += escortMgr->GetEscortLeftTime();
	}
	// 转盘 1
	Smart_Ptr<LucklyTurntableMgr>& LTMgr = m_owner->GetLucklyTableMgr();
	if ((bool)LTMgr)
	{
		 count += LTMgr->LucklyTurntable_IsHaveFreeNum();
	}


	// 竞技场 1
	count += m_owner->GetArenaMgr()->GetCanChallCnt();
//	const int Arena_MapID = GameParaLoader::GetInstance()->getArenaMapId();
//	InstanceMapEntry *mapptr = InstanceLoader::GetInstance()->GetEntry(Arena_MapID);
//	if (mapptr != NULL)
//	{
//		Smart_Ptr<CounterService>& counterMgr = m_owner->GetCounterService();
//		if ((bool)counterMgr)
//		{
//			count += (mapptr->m_nReaptTimes - counterMgr->GetNumById(COMPETE_INSTANCE_COUNTER));
//		}
//	}

	return count;
}


void DailyManager::ResetDaily()
{
	if(m_owner->GetCounterService())
		m_owner->GetCounterService()->SetCounterInfoToYesterday();

	if(m_owner->GetQuestMgr()->getDailyQuests())
		m_owner->GetQuestMgr()->getDailyQuests()->SetDailyQuesToYesterday();

	if(m_owner->GetQuestMgr()->getClanQuests())
		m_owner->GetQuestMgr()->getClanQuests()->SetClanQuestToYesterday();
}

int DailyManager::GetRecoverResourceCount(void)
{
	Daily::RecoverResourceInfoList toClient;

	if(m_owner->GetCounterService())
		m_owner->GetCounterService()->GetYesterdayCounterInfo(&toClient);

	if(m_owner->GetInstanceMgr())
		m_owner->GetInstanceMgr()->GetYesterdayInstanceInfo(&toClient);

	if(m_owner->GetQuestMgr()->getDailyQuests())
		m_owner->GetQuestMgr()->getDailyQuests()->GetYesterdayDailyQuest(&toClient);

	if(m_owner->GetQuestMgr()->getClanQuests())
		m_owner->GetQuestMgr()->getClanQuests()->GetYesterdayClanQuest(&toClient);

	int count = 0;
	for (int i = 0; i < toClient.infolist_size(); ++i)
	{
		if (0 < toClient.infolist(i).restcount())
		{
			++count;
		}
//		count += toClient.infolist(i).restcount();
	}

	if(m_owner->GetCityRewardList().size() > 0)
		count++;

	return count;
}









