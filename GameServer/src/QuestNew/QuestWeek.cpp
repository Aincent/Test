#include "QuestWeek.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeCreature.h"
#include "../QuickMessage.h"
#include "Quest.h"
#include "FileLoader/GameParaLoader.h"

QuestWeek::QuestWeek(Player& owner) : m_Owner(owner),m_Time(0),m_Times(0),m_YesterdayTimes(MAX_WEEKTASK_NUM)
{

}

QuestWeek::~QuestWeek()
{

}

//数据库中设置成员
void QuestWeek::onRead(const PlayerInfo::QuestInfo &info)
{
	if(!m_Owner.IsOpenSystem(eStTaskWeek))
	{
		return ;
	}
	const PlayerInfo::WeekQuest& tQuest = info.weekquest();
	m_Time  = tQuest.time();
	m_Times = tQuest.times();

	eDayResetType tempType = m_Owner.GetCounterService()->getResetDayType();
	if(tempType  == eDayResetToday)
	{
		m_YesterdayTimes = info.yesterdayweektimes();
	}
	else if(tempType == eDayResetBeforeYesterDay)
	{
		m_YesterdayTimes = 0;
	}
	else
	{
		m_YesterdayTimes = tQuest.times();
	}

	return ;
}

bool QuestWeek::isSameWeek()
{
	struct tm tempTm;
	time_t tempCurrentTime = 0;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);

	tempTm.tm_hour = 0;
	tempTm.tm_min  = 0;
	tempTm.tm_sec  = 0;

	time_t tempTime = mktime(&tempTm);
	tempTime -= tempTm.tm_wday * 3600 * 24;

	if((DWORD64)tempTime < m_Time / 1000)
		return true;

	return false;
}

void QuestWeek::resetTask()
{
	if(!m_Owner.IsOpenSystem(eStTaskWeek))
	{
		return ;
	}

	DWORD tHaveQuestID = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuestByType(QuestType_Week);
//	DWORD tQuestID = makeQuestID(tHaveQuestID);
	DWORD tQuestID = makeQuestID(0);
	if(tQuestID == 0)
	{
		return ;
	}

	if(tHaveQuestID != 0)
	{
		m_Owner.GetQuestMgr()->getAcceptedQuests().removeQuest(tHaveQuestID);
	}

	int tError = eRetSuccess;
	m_Owner.GetQuestMgr()->accept(tQuestID, QuestState_Accepted, tError,eQuestOnline);

	m_Time  = CUtil::GetNowSecond();
	m_Times = 1;

	return ;
}

DWORD QuestWeek::makeQuestID(DWORD questID)
{
	vector<DWORD> tQuests;
	QuestLoader::GetInstance()->GetLvWeekTaskNum(m_Owner.getAttManage()->getValue<int>(eCharLv),tQuests,questID);
	if(tQuests.size() == 0)
	{
		return 0;
	}

	DWORD tQuestID = tQuests[rand() % tQuests.size()];

	return tQuestID;
}

ServerRet QuestWeek::onSubmit(DWORD questId)
{
	ServerRet tError = eRetSuccess;
	if(nextIsOver())
	{
		return tError;
	}

	GiveSubmitReward();
	m_Owner.GetQuestMgr()->getAcceptedQuests().removeQuest(questId);
	if(nextIsOver())
	{
		m_Owner.GetQuestMgr()->GiveCompleteReward(QuestType_Week);
	}

	DWORD tQuestID = makeQuestID(0);
	if(tQuestID == 0)
	{
		return tError;
	}

	++m_Times;

	int tErrorOne = eRetSuccess;
	m_Owner.GetQuestMgr()->accept(tQuestID, QuestState_Accepted, tErrorOne,eQuestOnline);

	return tError;
}

ServerRet QuestWeek::updateTask()
{
	int tGolden = GameParaLoader::GetInstance()->getWeekTask();
	if(!m_Owner.IsEnoughGolden(tGolden))
	{
		return eNotEnoughGold;
	}

	//扣除元宝
	int tempGolden 	   = 0;
	int tempBindGolden = 0;

	m_Owner.DecreaseGoldenCost(tGolden,tempGolden,tempBindGolden);

	vector<int> vecType;
	if(tempGolden > 0)
		vecType.push_back(eCharGolden);

	if(tempBindGolden > 0)
		vecType.push_back(eCharBindGolden);

	if(vecType.size() > 0)
	{
		m_Owner.SynCharAttribute(vecType);
	}

	DWORD tRemoveID = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuestByType(QuestType_Week);
//	DWORD tQuestID  = makeQuestID(tRemoveID);
	DWORD tQuestID = makeQuestID(0);
	if(tQuestID == 0)
	{
		return eSystemError;
	}

	m_Owner.GetQuestMgr()->SubmitTaskRemoveItem(tRemoveID);
	m_Owner.GetQuestMgr()->getAcceptedQuests().removeQuest(tRemoveID);

	int tError = eRetSuccess;
	m_Owner.GetQuestMgr()->accept(tQuestID, QuestState_Accepted, tError,eQuestOnline);

	updateToClient();

	return eRetSuccess;
}

void QuestWeek::updateToClient()
{
	QuestProto::WeekQuest toClient;

	if(!nextIsOver())
	{
		DWORD tQuestID = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuestByType(QuestType_Week);
		Quest* tQuestPtr = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuest(tQuestID);
		if(tQuestPtr != NULL)
		{
			const Entry_Quest *info = tQuestPtr->getEntry();
			if(info != NULL)
			{
				toClient.set_state(tQuestPtr->getQuestState());
				toClient.set_svalue(info->m_nTaskTargetNum);
				toClient.set_questid(info->m_nQuestId);

				if(tQuestPtr->getQuestState() == QuestState_Accepted)
				{
					const QuestCondition* cond = tQuestPtr->getCondition(0);
					if(cond != NULL)
						toClient.set_fvalue(cond->getValue());
				}
				else
				{
					toClient.set_fvalue(info->m_nTaskTargetNum);
				}
			}
		}

		toClient.set_times(m_Times);
	}

	QuickMessage::GetInstance()->AddSendMessage(m_Owner.GetID(),m_Owner.GetChannelID(),&toClient,MSG_SIM_GM2C_UPDATE_WEEK_TASK);

	return ;
}

void QuestWeek::onWrite(PlayerInfo::QuestInfo* info)
{
	info->set_yesterdayweektimes(m_YesterdayTimes);

	PlayerInfo::WeekQuest* tQuest = info->mutable_weekquest();
	tQuest->set_time(m_Time);
	tQuest->set_times(m_Times);

	return ;
}

void QuestWeek::levelUp(bool send)
{
	DWORD tQuestID = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuestByType(QuestType_Week);
	if(tQuestID != 0)
	{
		return ;
	}

	if(nextIsOver())
	{
		return ;
	}

    tQuestID = makeQuestID(0);
	if(tQuestID == 0)
	{
		return ;
	}

	++m_Times;

	int tError = eRetSuccess;
	m_Owner.GetQuestMgr()->accept(tQuestID, QuestState_Accepted, tError,eQuestOnline);

	if(send)
		updateToClient();

	return ;
}

ServerRet QuestWeek::finishOneTask(DWORD questID)
{
	Quest* tQuestPtr = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuest(questID);
	if(tQuestPtr == NULL)
	{
		return eQuestNotExist;
	}

	const Entry_Quest* tEntry = tQuestPtr->getEntry(); 
	if(tEntry == NULL)
	{
		return eQuestNotExist;
	}
	if(!m_Owner.GetQuestMgr()->isHaveEnoughPackage(*tEntry))
	{
		return ePackageIsFull;
	}

	const OneFinishConsume* tempData = QuestLoader::GetInstance()->GetOneFinishDeduction(QuestType_Week);
	if(tempData == NULL)
	{
		return eSystemError;
	}

	if(!m_Owner.IsEnoughGolden(tempData->m_Golden))
	{
		return eNotEnoughGold;
	}

	ServerRet tError = eRetSuccess;
	m_Owner.GetQuestMgr()->NewSubmitQeust(questID, tQuestPtr,0 , false,tError);

	//扣除元宝
	int tempGolden 	   = 0;
	int tempBindGolden = 0;

	m_Owner.DecreaseGoldenCost(tempData->m_Golden,tempGolden,tempBindGolden);

	vector<int> vecType;
	if(tempGolden > 0)
		vecType.push_back(eCharGolden);

	if(tempBindGolden > 0)
		vecType.push_back(eCharBindGolden);

	if(vecType.size() > 0)
	{
		m_Owner.SynCharAttribute(vecType);
	}

	updateToClient();

	return eRetSuccess;
}

//void QuestWeek::getAllReward(vector<DWORD>& items,vector<DWORD>& nums)
//{
//	WeekItems tItems;
//	const WeekTaskLuaData& tWeekLua = QuestLoader::GetInstance()->getWeekLua();
//	if(tWeekLua.m_CountsReward != 0 && m_Times != tWeekLua.m_AllRing && m_Times % tWeekLua.m_CountsReward == 0)
//	{
//		getSpecifyCycleReward(tItems);
//	}
//
//	if(m_Times >= tWeekLua.m_AllRing)
//	{
//		getEndReward(tItems);
//	}
//
//	items.insert(items.end(),tItems.m_Items.begin(),tItems.m_Items.end());
//	nums.insert(nums.end(),tItems.m_Num.begin(),tItems.m_Num.end());
//
//	return ;
//}



ServerRet QuestWeek::oneComplete(DWORD questID)
{
	Quest* tQ = m_Owner.GetQuestMgr()->getAcceptedQuests().findQuest(questID);
	if(tQ == NULL)
	{
		return eSystemError;
	}

	DWORD tQuestID = questID;

	while(m_Times <= MAX_WEEKTASK_NUM)
	{
		GiveSubmitReward();
		tQuestID = makeQuestID(0);
		//tQuestID = makeQuestID(tQuestID);
		if(nextIsOver())
		{
			m_Owner.GetQuestMgr()->GiveCompleteReward(QuestType_Week);
		}

		++m_Times;
	}

	m_Owner.GetQuestMgr()->ResetDeleteTaskItem(questID,tQ->getCurrentValue());
	m_Owner.GetQuestMgr()->getAcceptedQuests().removeQuest(questID);

	updateToClient();

	return eRetSuccess; 
}

//int QuestWeek::getMaxRing()
//{
//	const WeekTaskLuaData& tWeekLua = QuestLoader::GetInstance()->getWeekLua();
//	return tWeekLua.m_AllRing;
//}

//void QuestWeek::getSpecifyCycleReward(WeekItems& items)
//{
//	QuestLoader::GetInstance()->getWeekTaskFixReward(m_Owner.getAttManage()->getValue<int>(eCharLv),items,eWeekTaskMiddle);
//	items.m_Items.insert(items.m_Items.end(),m_Rewards.m_Items.begin(),m_Rewards.m_Items.end());
//	items.m_Num.insert(items.m_Num.end(),m_Rewards.m_Num.begin(),m_Rewards.m_Num.end());
//	items.m_BindType.insert(items.m_BindType.end(),m_Rewards.m_BindType.begin(),m_Rewards.m_BindType.end());
//
//	return ;
//}

//void QuestWeek::getEndReward(WeekItems& items)
//{
//	QuestLoader::GetInstance()->getWeekTaskFixReward(m_Owner.getAttManage()->getValue<int>(eCharLv),items,eWeekTaskEnd);
//	items.m_Items.insert(items.m_Items.end(),m_EndRewards.m_Items.begin(),m_EndRewards.m_Items.end());
//	items.m_Num.insert(items.m_Num.end(),m_EndRewards.m_Num.begin(),m_EndRewards.m_Num.end());
//	items.m_BindType.insert(items.m_BindType.end(),m_EndRewards.m_BindType.begin(),m_EndRewards.m_BindType.end());
//
//	return ;
//}

//void QuestWeek::giveReward()
//{
//	WeekItems tItems;
//	const WeekTaskLuaData& tWeekLua = QuestLoader::GetInstance()->getWeekLua();
//	if(tWeekLua.m_CountsReward != 0 && m_Times != tWeekLua.m_AllRing && m_Times % tWeekLua.m_CountsReward == 0)
//	{
//		getSpecifyCycleReward(tItems);
//		makeSpecifyCycleReward();
//	}
//
//	if(m_Times >= tWeekLua.m_AllRing)
//	{
//		getEndReward(tItems);
//		m_EndRewards.m_Items.clear();
//		m_EndRewards.m_Num.clear();
//		m_EndRewards.m_BindType.clear();
//	}
//
//	vector<int> tPoss;
//	vector<DWORD> tStrenghts;
//
//	tStrenghts.resize(tItems.m_Items.size(),0);
//	m_Owner.GetContainer(ePackageType)->AddItem(tItems.m_Items,tItems.m_Num,tPoss,tItems.m_BindType,tStrenghts,true);
//
//	return ;
//}

//void QuestWeek::makeSpecifyCycleReward()
//{
//	m_Rewards.m_Items.clear();
//    m_Rewards.m_Num.clear();
//	m_Rewards.m_BindType.clear();
//
//	QuestLoader::GetInstance()->getWeekTaskRandReward(m_Owner.getAttManage()->getValue<int>(eCharLv),m_Rewards,eWeekTaskMiddle);
//	m_Flag |= eWeekTaskSendMiddle;
//
//	return ;
//}

//void QuestWeek::makeEndReward()
//{
//	m_EndRewards.m_Items.clear();
//    m_EndRewards.m_Num.clear();
//	m_EndRewards.m_BindType.clear();
//
//	QuestLoader::GetInstance()->getWeekTaskRandReward(m_Owner.getAttManage()->getValue<int>(eCharLv),m_EndRewards,eWeekTaskEnd);
//	m_Flag |= eWeekTaskSendEnd;
//
//	return ;
//}

bool QuestWeek::nextIsOver()
{
    return (m_Times) > MAX_WEEKTASK_NUM;
}

int QuestWeek::GiveSubmitReward()
{
	vector<DWORD> tItemList;
	vector<DWORD> tItemNum;
	vector<WORD>  tBindTypeList;

	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Clan,m_Owner.getAttManage()->getValue<int>(eCharLv));
	Smart_Ptr<ArticleBaseMgr> pPacket = m_Owner.GetContainer(ePackageType);
	if(!pPacket || !tinfo)
	{
		return eSystemError;
	}

	//判断背包是否能领取
	if(tinfo->m_nrewardMoney > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeMoneyItemId());
		tItemNum.push_back(tinfo->m_nrewardMoney);
	}

	if(tinfo->m_nrewardExp > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeExpItemId());
		tItemNum.push_back(tinfo->m_nrewardExp);
	}

	if(tinfo->m_nrewardForce > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeForceItemId());
		tItemNum.push_back(tinfo->m_nrewardForce);
	}

	if(tinfo->m_nClanContribution > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeContributionItemId());
		tItemNum.push_back(tinfo->m_nClanContribution);
	}

	if(tinfo->m_nCampContribution > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeCountryContributionItemId());
		tItemNum.push_back(tinfo->m_nCampContribution);
	}

	tItemList.insert(tItemList.end(),tinfo->m_nTaskItemList.begin(),tinfo->m_nTaskItemList.end());
	tItemNum.insert(tItemNum.end(),tinfo->m_nTaskItemListNum.begin(),tinfo->m_nTaskItemListNum.end());

	tBindTypeList.resize(tItemList.size(),1);

	vector<DWORD> strengthLvVec;
	strengthLvVec.resize(tItemList.size(), 0);

	if(pPacket->IsBagEnought(tItemList,tItemNum,tBindTypeList))
	{
		return ePackageFull;
	}

	vector<int>	  tempPosList;

	m_Owner.GetContainer(ePackageType)->AddItem(tItemList,tItemNum,tempPosList,tBindTypeList,strengthLvVec,true);

	m_Owner.SetDataFlag(eBaseInfo);
	m_Owner.SetDataFlag(eBattleInfo);

	return 0;
}

void QuestWeek::GetYesterdayWeekQuest(Daily::RecoverResourceInfoList * info)
{
	if(!m_Owner.IsOpenSystem(eStTaskWeek))
		return ;

	int restTimes = MAX_WEEKTASK_NUM - m_YesterdayTimes;
	if(restTimes <=0)
		return;
	Daily::RecoverResourceInfo * campQuestInfo = info->add_infolist();
	campQuestInfo->set_id(eRecorceSourceParamType_weekQuest);
	campQuestInfo->set_restcount(restTimes);
	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Week,m_Owner.getAttManage()->getValue<int>(eCharLv));
	if(tinfo)
	{
		Daily::RecoverResourceReward * reward = campQuestInfo->mutable_reward();
		reward->set_exp(tinfo->m_nrewardExp * restTimes);
		reward->set_money(tinfo->m_nrewardMoney * restTimes);
		reward->set_force(tinfo->m_nrewardForce * restTimes);
		reward->set_clancontri(tinfo->m_nClanContribution * restTimes);
		reward->set_campcontri(tinfo->m_nCampContribution * restTimes);

		for(size_t i = 0; i< tinfo->m_nTaskItemList.size(); i++)
		{
			Daily::RecoverResourceItemReward* pItem = reward->add_item();
			pItem->set_itemid(tinfo->m_nTaskItemList[i]);
			pItem->set_itemcount(tinfo->m_nTaskItemListNum[i] * restTimes);
		}
	}
}
