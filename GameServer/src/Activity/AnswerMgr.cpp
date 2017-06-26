/*
 * AnswerMgr.cpp
 *
 *  Created on: 2016年11月10日
 *      Author: root
 */

#include "AnswerMgr.h"
#include "FileLoader/AnswerLoader.h"
#include "MessageCommonRet.h"
#include "../Counter/Counter.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "PVPDefine.h"
#include "../Mail/MailMessage.h"
#include "FileLoader/InternationalLoader.h"

AnswerMgr::AnswerMgr(Player* obj):m_CurrentIndex(0),m_CurrentTitleID(0),m_RightCounts(0),m_WrongCounts(0),m_owner(obj)
{
	m_TitleList.clear();
}


AnswerMgr::~AnswerMgr()
{
	ReleaseAnswerMgr();
}

void AnswerMgr::InitAnswerInfo(const PlayerInfo::AnswerInfo& answerInfo)
{
	eDayResetType tempType = m_owner->GetCounterService()->getResetDayType();
	if(tempType != eDayResetToday)
	{
		ResetAnswerInfo();
		m_owner->SetDataFlag(eAnswerInfo);
	}
	else
	{
		m_CurrentIndex = answerInfo.currentindex();
		m_CurrentTitleID = answerInfo.currenttitleid();
		m_RightCounts = answerInfo.rightcounts();
		m_WrongCounts = answerInfo.wrongcounts();

		for(int i = 0; i < answerInfo.titleanswerids_size(); i++)
		{
			m_TitleList.push_back(answerInfo.titleanswerids(i));
		}
	}
}

void AnswerMgr::SetAnswerInfo(PlayerInfo::AnswerInfo *answerInfo)
{
	answerInfo->set_currentindex(m_CurrentIndex);
	answerInfo->set_currenttitleid(m_CurrentTitleID);
	answerInfo->set_rightcounts(m_RightCounts);
	answerInfo->set_wrongcounts(m_WrongCounts);

	for(size_t i = 0; i < m_TitleList.size(); i++)
	{
		answerInfo->add_titleanswerids(m_TitleList[i]);
	}
}

void AnswerMgr::ReleaseAnswerMgr()
{
	m_TitleList.clear();
}

void AnswerMgr::ResetAnswerInfo()
{
	m_CurrentIndex = 0;
	m_CurrentTitleID = 0;
	m_RightCounts = 0;
	m_WrongCounts = 0;

	FlushTitle();
}


void AnswerMgr::MakeOneTitle()
{
	if(m_CurrentIndex <= ANSWER_TITLE_SIZE)
		++m_CurrentIndex;

	if(m_TitleList.size() <= 0)
		return ;

	int tempIndex    = rand() % m_TitleList.size();
	m_CurrentTitleID = m_TitleList[tempIndex];

	m_TitleList.erase(m_TitleList.begin() + tempIndex);

	m_owner->SetDataFlag(eAnswerInfo);
}

void AnswerMgr::FlushTitle()
{
	m_TitleList.clear();

	const vector<int>& tempFileData = AnswerLoader::GetInstance()->GetFielData();

	vector<int>	tempTitleList;
	for(size_t j = 0;j < tempFileData.size();++j)
	{
		tempTitleList.push_back(tempFileData[j]);
	}

	for(size_t j = 0;j < (size_t)ANSWER_TITLE_SIZE && j < tempTitleList.size();++j)
	{
		int tempIndex = rand() % tempTitleList.size();

		m_TitleList.push_back(tempTitleList[tempIndex]);

		tempTitleList.erase(tempTitleList.begin() + tempIndex);
	}

	MakeOneTitle();
}


void AnswerMgr::GetAnswerInfo(AnswerProto::AckReqAnswerInfo& info)
{
	info.set_ret(0);
	if(m_CurrentIndex > ANSWER_TITLE_SIZE)
	{
		info.set_currentindex(ANSWER_TITLE_SIZE +1);
	}
	else
	{
		info.set_currentindex(m_CurrentIndex);
	}

	info.set_currenttitleid(m_CurrentTitleID);
	info.set_rightcounts(m_RightCounts);
}

void AnswerMgr::ChoiceAnswer(int nIndex,AnswerProto::AckReqAnswerInfo& info)
{
	float rate = 0.0f;
	if(nIndex < eTitleTypeA || nIndex > eTitleTypeC)
	{
		info.set_ret(eClientSendDataError);
		return;
	}

	if(m_CurrentIndex > ANSWER_TITLE_SIZE)
	{
		info.set_ret(eClientSendDataError);
		return;
	}


	MakeOneTitle();
	if(nIndex == ANSWER_RIGHT_INDEX)
	{
		++m_RightCounts;
		info.set_state(1);
		rate = 1.0f;
	}
	else
	{
		++m_WrongCounts;
		info.set_state(2);
		rate = 0.5f;
	}

	GetAnswerInfo(info);

	const AnswerReward* pReward = AnswerLoader::GetInstance()->GetAnswerReward(eConditionAnswer);
	if(pReward)
	{
		m_owner->MoneyChange(pReward->m_Limit * rate);
		int tempForce = m_owner->ChangeCurForce(pReward->m_Value * rate);
		m_owner->SynCharAttribute(eCharMoney, tempForce);
	}

	StatisticsReward();

	m_owner->GetVitalityMgr()->FinshVitality(eViAnswer);
	m_owner->GetCounterService()->GetNumById(ANSWER_ENTER_COUNT);
}

void AnswerMgr::StatisticsReward()
{
	vector<int> vec;
	vector<DWORD> idList;
	vector<DWORD> numList;
	vector<WORD> tempFlagList;
	vector<DWORD> tempStreghtList;
	vector<PropItem> items;

	const AnswerReward* pReward = AnswerLoader::GetInstance()->GetAnswerReward(eConditionStatistics,m_RightCounts);
	if(pReward)
	{
		vector<AnswerItem>::const_iterator itr = pReward->m_ItemList.begin();
		for(; itr != pReward->m_ItemList.end(); ++itr)
		{
			idList.push_back(itr->m_ItemID);
			numList.push_back(itr->m_Counts);
		}

		if(idList.size() > 0)
		{
			tempFlagList.resize(idList.size(),1);
			tempStreghtList.resize(numList.size(),0);

			Smart_Ptr<ArticleBaseMgr> pPacket = m_owner->GetContainer(ePackageType);
			if(!pPacket)
			{
				return;
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
					char warning1[128] = {0};
					sprintf(warning1,InternationalLoader::GetInstance()->GetInterStr(70).c_str(),m_RightCounts);
					MailMessageMgr::GetInstance()->SendMail(m_owner->GetID(),items,warning1,InternationalLoader::GetInstance()->GetInterStr(35));
				}
			}
		}
	}
}

int AnswerMgr::RecoverResource(int num, Daily::RecoverResourceInfo *info)
{
	if(!m_owner->IsOpenSystem(eStAnswer))
	{
		return eLVNotEnough;
	}

	int haveNum = ANSWER_TITLE_SIZE - num;
	if (haveNum <=0)
		return eAnswerActivityEnd;

	sRecorceSourceParam *ptr = DailyLoader::GetInstance()->GetRecorceSourceParamPtr(eRecorceSourceParamType_answer);
	if (!ptr)
	{
		return eNull;
	}

	const AnswerReward* pReward = AnswerLoader::GetInstance()->GetAnswerReward(eConditionAnswer);
	if(pReward)
	{
		Daily::RecoverResourceReward * reward = info->mutable_reward();
		reward->set_money(pReward->m_Limit * haveNum);
		reward->set_force(pReward->m_Value * haveNum);
		info->set_restcount(haveNum);

		info->set_id(eRecorceSourceParamType_answer);
	}

	return 0;
}
