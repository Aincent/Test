/*
 * CampQuest.cpp
 *
 *  Created on: 25/03/2015
 *      Author: root
 */

#include "CampQuest.h"
#include "QuestLoader.h"
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "QuestPart.h"
#include "QuestDefine.h"
#include "../Attribute/AttributeCreature.h"
#include "FileLoader/GameParaLoader.h"

#define CAMPTASK_FRESHNUMSTAR    1		//阵营条件个初始个数
#define CAMPTASK_FRESHREWARDSTAR 1		//阵营星级初始值

CampQuest::CampQuest(Player& owner) :
m_CurrentTimes(0),
m_NumTimes(0),
m_RewardStar(0),
m_QuestID(0),
m_NumCoefficient(1.0f),
m_Owner(owner),
m_YesterdayQuestIdAccepted(0),
m_YesterdayTimes(MAX_CAMPQUEST_NUM),
m_lastDoQuestID(0),
m_lastDoCondNum(0)
{

}

CampQuest::~CampQuest()
{

}

void CampQuest::onRead(const PlayerInfo::QuestInfo &info)
{
	int yesterCampSize = info.yesterdaycamp_size();
	if(0 == yesterCampSize)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_CAMPQUEST_NUM;
	}
	else
	{
		for(int i=0; i<yesterCampSize; ++i)
		{
			PlayerInfo::CampQuest campinfo = info.yesterdaycamp(i);
			m_YesterdayQuestIdAccepted = campinfo.questid();
			m_YesterdayTimes = campinfo.times();
		}
	}

	for(int i=0; i<info.ccampquest_size(); ++i)
	{
		const PlayerInfo::CampQuest& tempCampInfo = info.ccampquest(i);
		if(tempCampInfo.times() >= getMaxTimes())
		{
			setTimes(getMaxTimes());

			return ;
		}

		setQuestId(tempCampInfo.questid());
		setNumStarLevel(tempCampInfo.numlevel() > Max_Num_Start ? Max_Num_Start : tempCampInfo.numlevel());
		setRewardStarLevel(tempCampInfo.rewardlevel() > Max_Reward_Start ? Max_Reward_Start : tempCampInfo.rewardlevel());
		setTimes(tempCampInfo.times());
		setCoefficient(tempCampInfo.numcoefficient());
	}

	if(getQuestId() == 0)
		reset();

	return ;
}

// 随机生成一个任务及其星级
void CampQuest::selectQuest()
{
	std::vector<DWORD> tempQuestIdList;

	QuestLoader::GetInstance()->GetLvCampaskNum(m_Owner.getAttManage()->getValue<int>(eCharLv),tempQuestIdList,getQuestId());

	if(tempQuestIdList.size() <= 0)
		return ;

	int tempIndex = rand() % tempQuestIdList.size();
	DWORD tempQuestID = tempQuestIdList[tempIndex];

	//怪物系数被始为1
	m_NumCoefficient = 1.0f;

	setQuestId(tempQuestID);
	selectStarLevel(eRefreshStarTNum);
	selectStarLevel(eRefreshStarReward);

	m_Owner.SetDataFlag(eQuestInfo);

	return ;
}

void CampQuest::reset()
{
	setTimes(0);
	m_Owner.GetQuestMgr()->setCampDoTimes(0);
	selectQuest();

	return ;
}

bool CampQuest::nextIsOver()
{
	BYTE times = getTimes();

	times = times + 1;

	if(times >= getMaxTimes())
		return true;

	return false;
}

ServerRet CampQuest::GiveSubmitReward(const BYTE rewardLevel)
{
	vector<DWORD> tItemList;
	vector<DWORD> tItemNum;
	vector<WORD>  tBindTypeList;

	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Camp,m_Owner.getAttManage()->getValue<int>(eCharLv));
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

	return eRetSuccess;
}

bool CampQuest::canAccept(const Entry_Quest* entry, int error) const
{
	if (!entry)
	{
		error = eQuestNotExist;
		return false;
	}

	if(entry->m_nMainType != QuestType_Camp)
	{
		error = eNotCampTask;
		return false;
	}

	DWORD now_questID = getQuestId();

	if(now_questID > 0 && now_questID != entry->m_nQuestId)
	{
		error = eHaveCampQuest;
		return false;
	}

	//是否加入阵营
	if(m_Owner.getAttManage()->getValue<DWORD>(eCharCountry) == 0 )
	{
		error = eCampIstExisted;
		return false;
	}

	//所需的等级
	if((int)entry->m_nLevelDown > m_Owner.getAttManage()->getValue<int>(eCharLv))
	{
		error = eLVNotEnough;
		return false;
	}

	//任务次数是否达到最大
	if(this->getTimes() > getMaxTimes())
	{
		error = eQuestMax;
		return false;
	}

	error = eRetSuccess;

	return true;
}

//玩家提交阵营任务
bool CampQuest::onSubmit(const DWORD questId)
{
	if(questId != this->getQuestId())
		return false;

	setTimes(getTimes() + 1);
	m_Owner.GetQuestMgr()->setCampDoTimes(getTimes());

	if(getTimes() < getMaxTimes())
	{
		selectQuest();
	}
	else
	{
		//次数不能重置
		setQuestId(0);
		setNumStarLevel(CAMPTASK_FRESHNUMSTAR);
		setRewardStarLevel(CAMPTASK_FRESHREWARDSTAR);

		m_NumCoefficient = 1.0f;

		//通知今日所有阵营任务已完成
		m_Owner.SendToPlayerResultCodeMessage(eAllCampTaskHadComp);
		m_Owner.SetDataFlag(eQuestInfo);

		return false;
	}

	m_Owner.SetDataFlag(eBaseInfo);

	return true;
}

Quest_CondStateInfo CampQuest::updateToClient()
{
	QuestProto::UpdateCampTask tempInfo;

	getClientData(tempInfo);

	//用于测试
	Quest_CondStateInfo tempData;

	tempData.questID 	= tempInfo.questid();
	tempData.state 		= (QuestState)tempInfo.state();
	tempData.numStar 	= tempInfo.numstar();
	tempData.rewardStar = tempInfo.rewardstar();
	tempData.times 		= tempInfo.times();
	tempData.nowNum 	= tempInfo.fvalue();
	tempData.targeNum 	= tempInfo.svalue();

	QuickMessage::GetInstance()->AddSendMessage(m_Owner.GetID(),m_Owner.GetChannelID(),&tempInfo,MSG_SIM_GM2C_UPDATECAMPASK);

	if(tempData.state == QuestState_CanSubmit)
	{
		LOG_DEBUG(FILEINFO,"Player %lld CanSubmit Task,%d type,%d state,%d numStar,%d rewardStar,%d times,%d nowNum,%d targeNum,%d",
				m_Owner.GetID(),
				tempData.questID,
				QuestType_Camp,
				tempData.state,
				tempData.numStar,
				tempData.rewardStar,
				tempData.times,
				tempData.nowNum,
				tempData.targeNum);
	}

	return tempData;
}

bool CampQuest::getClientData(QuestProto::UpdateCampTask& info)
{
	Quest* tempCamp = m_Owner.GetQuestMgr()->getAcceptedQuests().findCampQuest();
	if(tempCamp == NULL)
		return false;

	if(getQuestId() == 0)
		return false;

	if(getQuestId() != tempCamp->getEntry()->m_nQuestId)
		return false;

	if(getTimes() > getMaxTimes())
		return false;

	info.set_questid(getQuestId());
	info.set_state(tempCamp->getQuestState());
	info.set_numstar(getNumStarLevel());
	info.set_rewardstar(getRewardStarLevel());
	info.set_times(getTimes());

	if(tempCamp->getQuestState() == QuestState_Accepted)
	{
		QuestCondition* tempCond = tempCamp->getCondition(0);
		if(tempCond)
		{
			info.set_fvalue(tempCond->getValue());
			info.set_svalue(GetTargetNum());
		}
	}
	else if(tempCamp->getQuestState() == QuestState_CanSubmit)
	{
		info.set_fvalue(GetTargetNum());
		info.set_svalue(GetTargetNum());
	}

	return true;
}

void CampQuest::IncreaseDoMaxTime()
{
	setTimes(getMaxTimes());
	setQuestId(0);
	setNumStarLevel(CAMPTASK_FRESHNUMSTAR);
	setRewardStarLevel(CAMPTASK_FRESHREWARDSTAR);

	m_Owner.GetQuestMgr()->setCampDoTimes(getMaxTimes());
	m_Owner.SetDataFlag(eQuestInfo);

	return ;
}

//s刷星, isFull表明是否全部刷满级，type即选择刷满的类型
//ServerRet CampQuest::refreshStarLevel(bool isFull,int type)
//{
//	if(isFull)
//	{
//		if(getNumStarLevel() >= MAX_CAMPQUEST_STARLEVEL && getRewardStarLevel() >= MAX_CAMPQUEST_STARLEVEL)
//		{
//			return eTaskMaxStart;
//		}
//	}
//	else
//	{
//		if(eRefreshStarTNum == type)
//		{
//			if(getNumStarLevel() >= MAX_CAMPQUEST_STARLEVEL)
//			{
//				return eTaskMaxStart;
//			}
//		}
//		else if(eRefreshStarReward == type)
//		{
//			if(getRewardStarLevel() >= MAX_CAMPQUEST_STARLEVEL)
//			{
//				return eTaskMaxStart;
//			}
//		}
//		else
//		{
//			return eClientSendDataError;
//		}
//	}
//
//	const RefreshConsume* tempRewardRefesh = NULL;
//	const RefreshConsume* tempNumRefesh    = NULL;
//
//	int tempMoney  = 0;
//	int tempGolden = 0;
//	ServerRet tempError  = eRetSuccess;
//	if(isFull)
//	{
//		tempRewardRefesh = QuestLoader::GetInstance()->GetRefleshFinishDeduction(QuestType_Functional,getRewardStarLevel());
//		if(tempRewardRefesh == NULL)
//		{
//			return eSystemError;
//		}
//		tempNumRefesh = QuestLoader::GetInstance()->GetRefleshFinishDeduction(QuestType_Functional,getNumStarLevel());
//		if(tempNumRefesh == NULL)
//		{
//			return eSystemError;
//		}
//
//		tempMoney = tempRewardRefesh->m_RewardMoney + tempNumRefesh->m_NumMoney;
//
//		if(m_Owner.getAttManage()->getValue<int>(eCharMoney) < tempMoney)
//		{
//			return eNotEnoghMoney;
//		}
//
//		tempGolden = tempRewardRefesh->m_NumGolden + tempNumRefesh->m_RewardGolden;
//
//		if(m_Owner.getAttManage()->getValue<int>(eCharGolden) < tempGolden)
//		{
//			return eNotEnogGolden;
//		}
//
//		float tempConsume = getCoefficient();
//		int tempNumStarts = QuestLoader::GetInstance()->refreshNumStarts(m_Owner.getAttManage()->getValue<int>(eCharLv)
//				,getNumStarLevel(),m_Owner.getAttManage()->getValue<int>(eVipCardLv),QuestType_Camp,tempConsume);
//
//		if(tempNumStarts == getNumStarLevel())
//		{
//			tempError = eRefreshFail;
//		}
//
//		this->setNumStarLevel(tempNumStarts);
//		setCoefficient(tempConsume);
//
//		int tempRewardStarts = QuestLoader::GetInstance()->refreshStarts(m_Owner.getAttManage()->getValue<int>(eCharLv)
//				,getRewardStarLevel(),m_Owner.getAttManage()->getValue<int>(eVipCardLv),QuestType_Camp);
//
//		//两种都失败，算为失败
//		if(tempRewardStarts == getRewardStarLevel())
//		{
//			if(tempError != eRefreshFail)
//			{
//				tempError = eRetSuccess;
//			}
//		}
//		else
//		{
//			tempError = eRetSuccess;
//		}
//
//		this->setRewardStarLevel(tempRewardStarts);
//	}
//	else
//	{
//		if(eRefreshStarTNum == type)
//		{
//			tempNumRefesh = QuestLoader::GetInstance()->GetRefleshFinishDeduction(QuestType_Functional,getNumStarLevel());
//			if(tempNumRefesh == NULL)
//			{
//				return eSystemError;
//			}
//			tempMoney = tempNumRefesh->m_NumMoney;
//			tempGolden = tempNumRefesh->m_NumGolden;
//		}
//		else if(eRefreshStarReward == type)
//		{
//			tempRewardRefesh = QuestLoader::GetInstance()->GetRefleshFinishDeduction(QuestType_Functional,getRewardStarLevel());
//			if(tempRewardRefesh == NULL)
//			{
//				return eSystemError;
//			}
//			tempMoney = tempRewardRefesh->m_RewardMoney;
//			tempGolden = tempRewardRefesh->m_RewardGolden;
//		}
//
//		if(m_Owner.getAttManage()->getValue<int>(eCharMoney) < tempMoney)
//		{
//			return eNotEnoghMoney;
//		}
//
//		if(m_Owner.getAttManage()->getValue<int>(eCharGolden) < tempGolden)
//		{
//			return eNotEnogGolden;
//		}
//
//		if(eRefreshStarTNum == type)
//		{
//			float tempConsume = getCoefficient();
//			int tempNumStarts = QuestLoader::GetInstance()->refreshNumStarts(m_Owner.getAttManage()->getValue<int>(eCharLv)
//					,getNumStarLevel(),m_Owner.getAttManage()->getValue<int>(eVipCardLv),QuestType_Camp,tempConsume);
//			if(getNumStarLevel() == tempNumStarts)
//			{
//				tempError = eRefreshFail;
//			}
//			this->setNumStarLevel(tempNumStarts);
//			setCoefficient(tempConsume);
//		}
//		else if(eRefreshStarReward == type)
//		{
//			int tempRewardStarts = QuestLoader::GetInstance()->refreshStarts(m_Owner.getAttManage()->getValue<int>(eCharLv)
//					,getRewardStarLevel(),m_Owner.getAttManage()->getValue<int>(eVipCardLv),QuestType_Camp);
//			if(tempRewardStarts == getRewardStarLevel())
//			{
//				tempError = eRefreshFail;
//			}
//			this->setRewardStarLevel(tempRewardStarts);
//		}
//	}
//
//	vector<int> vecType;
//
//	if(tempMoney > 0)
//	{
//		m_Owner.ChangeMoney(tempMoney,false);
//		vecType.push_back(eCharMoney);
//	}
//
//	if(tempGolden > 0)
//	{
//		m_Owner.ChangeGolden(tempGolden,false);
//		vecType.push_back(eCharGolden);
//	}
//
//	if(vecType.size() > 0)
//	{
//		m_Owner.SynCharAttribute(vecType);
//	}
//	m_Owner.SetDataFlag(eBaseInfo);
//	m_Owner.SetDataFlag(eQuestInfo);
//
//	return tempError;
//}

bool CampQuest::onWrite(PlayerInfo::QuestInfo* info)
{
	PlayerInfo::CampQuest *yesterdayCampinfo = info->add_yesterdaycamp();

	yesterdayCampinfo->set_questid(m_YesterdayQuestIdAccepted);
	yesterdayCampinfo->set_numlevel(this->getNumStarLevel());
	yesterdayCampinfo->set_rewardlevel(this->getRewardStarLevel());
	yesterdayCampinfo->set_times(m_YesterdayTimes);

	PlayerInfo::CampQuest* tempCampQuestData = info->add_ccampquest();

	tempCampQuestData->set_questid(getQuestId());
	tempCampQuestData->set_numlevel(getNumStarLevel());
	tempCampQuestData->set_rewardlevel(getRewardStarLevel());
	tempCampQuestData->set_times(getTimes());
	tempCampQuestData->set_numcoefficient(getCoefficient());

	return true;
}

void CampQuest::GetCampQuestConditionStateInfo(Quest_CondStateInfo* ptr)
{
	if(ptr == NULL)
		return ;

	Quest* tempQuest = m_Owner.GetQuestMgr()->getAcceptedQuests().findCampQuest();

	if(tempQuest == NULL)
		return ;

	ptr->questID = getQuestId();

	if(ptr->questID == 0)
		return ;

	ptr->state 		= tempQuest->getQuestState();
	ptr->numStar 	= getNumStarLevel();
	ptr->rewardStar = getRewardStarLevel();
	ptr->times 		= getTimes();

	if(ptr->state == QuestState_Accepted)
	{
		QuestCondition* tempCond = tempQuest->getCondition(0);
		if(tempCond)
		{
			ptr->nowNum   = tempCond->getValue();
			ptr->targeNum = GetTargetNum();
		}
	}
	else if(tempQuest->getQuestState() == QuestState_CanSubmit)
	{
		ptr->nowNum   = GetTargetNum();
		ptr->targeNum = GetTargetNum();
	}

	return ;
}

bool CampQuest::InitLastDailyQuest(const PlayerInfo::QuestInfo &info)
{
	for(int i=0; i<info.ccampquest_size(); ++i)
	{
		const PlayerInfo::CampQuest& tempCamp = info.ccampquest(i);
		m_lastDoQuestID = tempCamp.questid();
	}

	for(int i = 0; i < info.active_size(); ++i)
	{
		//ActiveQuest里面存储了所有正在进行任务的状态
		const PlayerInfo::ActiveQuest&  newActive = info.active(i);

		Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(newActive.id());
		if(entry && (entry->m_nMainType == QuestType_Camp))
		{
			if(newActive.id() != (int)m_lastDoQuestID)
			{
				continue;
			}

			m_lastDoCondNum = info.active(i).ftarget();
		}
	}
	m_Owner.SetDataFlag(eQuestInfo);

	return true;
}

bool CampQuest::InitYesterdayDailyQuest(const PlayerInfo::QuestInfo &info)
{
	eDayResetType tempType = m_Owner.GetCounterService()->getResetDayType();

	if(eDayResetBeforeYesterDay == tempType)
	{
		m_YesterdayQuestIdAccepted = GetInitCampQuestId();
		m_YesterdayTimes = 0;
		return true;
	}

	int questId = 0;
	int times = MAX_CAMPQUEST_NUM;
	for(int i=0; i<info.ccampquest_size(); ++i)
	{
		PlayerInfo::CampQuest campinfo = info.ccampquest(i);
		questId = campinfo.questid();
		times = campinfo.times();
		if(campinfo.times() >= MAX_CAMPQUEST_NUM)
		{
			times = MAX_CAMPQUEST_NUM;
			break;
		}
	}

	if(eDayResetYesterday == tempType)
	{
		m_YesterdayQuestIdAccepted = questId;
		m_YesterdayTimes = times;
		return true;
	}

	int yesterCampSize = info.yesterdaycamp_size();
	if(0 == yesterCampSize)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_CAMPQUEST_NUM;
		return true;
	}

	m_Owner.SetDataFlag(eQuestInfo);

	return true;
}

int CampQuest::GetTargetNum()
{
	Entry_Quest* tempInfo = QuestLoader::GetInstance()->GetQuestInfo(getQuestId());
	if(tempInfo == NULL)
	{
		return 0;
	}

	int tempTargetNum = (int)(m_NumCoefficient * tempInfo->m_nTaskTargetNum);

	if(tempTargetNum <= 0)
	{
		tempTargetNum = 1;
	}

	return tempTargetNum;
}

int CampQuest::GetUnCompleteNum()
{
	return getMaxTimes() - getTimes();
}

void CampQuest::selectStarLevel(eRefreshStarType type)
{
	if (getQuestId() == 0)
		return;

	// 随机刷奖励和数量的星级
	if(eRefreshStarTNum == type)
	{
		setNumStarLevel(1);
	}
	else if(eRefreshStarReward == type)
	{
		setRewardStarLevel(1);
	}
	m_Owner.SetDataFlag(eQuestInfo);
	return ;
}

int CampQuest::GetInitCampQuestId(void)
{
	int level = m_Owner.getAttManage()->getValue<int>(eCharLv);
	std::vector<DWORD> idList;
	idList.clear();

	QuestLoader::GetInstance()->GetLvCampaskNum(level,idList, this->getQuestId());
	if (idList.size() <= 0)
	{
		LOG_ERROR(FILEINFO,"Player Selet Daily Task,But Not Have Property Task");
		return 0;
	}

	return idList[rand()%idList.size()];
}


void CampQuest::GetYesterdayCampQuest(Daily::RecoverResourceInfoList * info)
{
	DWORD country = m_Owner.getAttManage()->getValue<DWORD>(eCharCountry);
	if(country <= 0)
		return;
	int restTimes = MAX_CAMPQUEST_NUM - m_YesterdayTimes;
	if(restTimes <=0)//已全部做完或者没有帮派任务
		return;
	Daily::RecoverResourceInfo * campQuestInfo = info->add_infolist();
	campQuestInfo->set_id(eRecorceSourceParamType_campQuest);
	campQuestInfo->set_restcount(restTimes);
	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Camp,m_Owner.getAttManage()->getValue<int>(eCharLv));
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
