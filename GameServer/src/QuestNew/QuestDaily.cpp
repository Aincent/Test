/*
 * QuestDaily.cpp
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */


#include "QuestDaily.h"
#include "../Object/Player/Player.h"
#include "Quest.h"
#include "MessageCommonRet.h"
#include "QuestLoader.h"
#include "Log/Logger.h"
#include "../QuickMessage.h"
#include <math.h>
#include "../Attribute/AttributeCreature.h"
#include "./FileLoader/GameParaLoader.h"
#include "./FileLoader/UpgradeLoader.h"


DailyQuests::DailyQuests(Player* owner)
	: mOwner(owner)
	, mResetTimes(0)
	, mQuestIdAccepted(0)
	, mTimes(0)
	,	mNumStarLevel(1)
	, mRewardLevel(1)
	, mMaxTimes(MAX_DAILYTASK_NUM)
	, mIsDirty(false)
	,m_NumCoefficient(1.0f)
	,m_YesterdayQuestIdAccepted(0)
	,m_YesterdayTimes(MAX_DAILYTASK_NUM)
	,m_lastDoQuestID(0)
	,m_lastDoCondNum(0)
{
	init();
}

DailyQuests::~DailyQuests()
{

}

void DailyQuests::init()
{
	//m_sDailyCompleteEvent = RegEvent(mOwner, PLAYER_QUEST_ACCEPTQUEST, this, &DailyQuests::onDailyTaskComplete);//日常任务完成
}

void DailyQuests::uninit()
{
}

void DailyQuests::setNeedUpdateToDB()
{
	if(mOwner)
		mOwner->SetDataFlag(eQuestInfo);
}

int DailyQuests::GetInitDailyQuestId(void)
{
	int level = mOwner->getAttManage()->getValue<int>(eCharLv);
	std::vector<DWORD> idList;
	idList.clear();

	// 读取任务表中本等级段可以接受的所有日常任务
	QuestLoader::GetInstance()->GetLvDailyTaskNum(level,idList, this->getQuestId());
	if (idList.size() <= 0)
	{
		LOG_ERROR(FILEINFO,"Player Selet Daily Task,But Not Have Property Task");
		return 0;
	}
	return idList[rand()%idList.size()];
}

void DailyQuests::initDailyQuestList()
{
	int level = mOwner->getAttManage()->getValue<int>(eCharLv);

	std::vector<DWORD> idList;
	idList.clear();

	// 读取任务表中本等级段可以接受的所有日常任务
	QuestLoader::GetInstance()->GetLvDailyTaskNum(level,	idList, this->getQuestId());
	if(idList.size() <= 0)
	{
		LOG_ERROR(FILEINFO,"Player Selet Daily Task,But Not Have Property Task");
		return ;
	}

	DWORD newquestID = idList[rand() % idList.size()];
	this->setQuestId(newquestID);

	m_NumCoefficient = 1.0f;

	if(this->getQuestId() <=0)
		return ;

	Quest* daily = mOwner->GetQuestMgr()->getAcceptedQuests().findDailyQuest();
	if(daily && daily->getEntry() && (daily->getEntry()->m_nQuestId != this->getQuestId()))
	{
//		cout<<endl<<endl<<"Daily Quest id Change to "<<mQuestIdAccepted<<endl<<endl;
	}

	selectStarLevel(eRefreshStarTNum);

	selectStarLevel(eRefreshStarReward);

	setNeedUpdateToDB();
}

bool DailyQuests::onSysActived()
{
	/*判断是否  已经完成 每日 次数   ---  这是 由于  升等级之后的  触发事件*/
	if(this->getTimes() <mMaxTimes)
	{
		//判断是否达到最低等级标准
		Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(DAILYTASK_STARTID);
		if(info && mOwner->IsOpenSystem(eStDailyQuest))
		{
			initDailyQuestList();
			mIsDirty = true;
			return true;
		}
	}

	return false;
}

void DailyQuests::clear()
{
	//mDailyQuestInfoList.clear();
}

void DailyQuests::IncreaseDoTime()
{
	this->setTimes(this->getTimes() +1);

	BYTE time = mOwner->GetQuestMgr()->GetDailyDoTime();

	mOwner->GetQuestMgr()->SetDailyDoTime(time +1);
	mOwner->SetDataFlag(eQuestInfo);
}

void DailyQuests::IncreaseDoMaxTime()
{
	mOwner->GetQuestMgr()->SetDailyDoTime(mMaxTimes);

	this->setTimes(mMaxTimes);

	this->setQuestId(0);

	this->SetNumStarLevel(1);

	this->setRewardStarLevel(1);

	setNeedUpdateToDB();

	mIsDirty = true;
}

void DailyQuests::selectStarLevel(int type)
{
	if(this->getQuestId() == 0)
		return;

	// 随机刷奖励和数量的星级

	if(eRefreshStarTNum == type)
	{
		this->SetNumStarLevel(1);
	}
	else if(eRefreshStarReward == type)
	{
		this->setRewardStarLevel(1);
	}

	setNeedUpdateToDB();
	mIsDirty = true;
}

bool DailyQuests::canAccept(const Entry_Quest* entry, int &error) const
{
	if (!entry)
	{
		error = eQuestNotExist;
		return false;
	}

	if(entry->m_nMainType != QuestType_Functional)
	{
		error = eNotDailyTask;
		return false;
	}

	uint now_questID = this->getQuestId();
	if(now_questID > 0 && now_questID != entry->m_nQuestId)
	{
		error = eHaveDailyQuest;
		return false;
	}

	//所需的等级
	if(mOwner->IsOpenSystem(eStDailyQuest)==false)
	{
		error = eLVNotEnough;
		return false;
	}

	//任务次数是否达到最大
	if(this->getTimes() > mMaxTimes)
	{
		error = eQuestMax;

		return false;
	}

	error = 0;
	return true;
}

/// 接受一个日常任务
void DailyQuests::onAccept(uint questId)
{

}


int DailyQuests::GiveSubmitReward(const BYTE& i_RewardLevel)
{
	vector<DWORD> tItemList;
	vector<DWORD> tItemNum;
	vector<WORD>  tBindTypeList;

	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Clan,mOwner->getAttManage()->getValue<int>(eCharLv));
	Smart_Ptr<ArticleBaseMgr> pPacket = mOwner->GetContainer(ePackageType);
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

	tBindTypeList.resize(tItemList.size(),1);

	vector<DWORD> strengthLvVec;
	strengthLvVec.resize(tItemList.size(), 0);

	if(pPacket->IsBagEnought(tItemList,tItemNum,tBindTypeList))
	{
		return ePackageFull;
	}

	vector<int>	  tempPosList;

	mOwner->GetContainer(ePackageType)->AddItem(tItemList,tItemNum,tempPosList,tBindTypeList,strengthLvVec,true);

	mOwner->SetDataFlag(eBaseInfo);
	mOwner->SetDataFlag(eBattleInfo);

	return 0;
}

bool DailyQuests::nextIsOver()
{
	BYTE times = this->getTimes();
	times = times +1;
	if(times == mMaxTimes)
		return true;
	return false;
}

/// 玩家接日常任务
bool DailyQuests::onSubmit(uint questId)
{
	if(questId == this->getQuestId())
	{
		mIsDirty = true;

		this->IncreaseDoTime();

		if(this->getTimes() >mMaxTimes)
			return false;

		if(this->getTimes() < mMaxTimes)
		{
			initDailyQuestList();
		}
		else
		{
			//20环结束获取完成环数奖励
			this->setQuestId(0);

			this->SetNumStarLevel(1);

			this->setRewardStarLevel(1);

			m_NumCoefficient = 1.0f;

			setNeedUpdateToDB();

			/*通知  已完成今日所有日常任务*/
			mOwner->SendToPlayerResultCodeMessage(eAllDailyTaskHadComp);
			return false;
		}
		mOwner->SetDataFlag(eQuestInfo);
		return true;
	}
	return false;
}

// 重置玩家的日常任务次数
void DailyQuests::reset()
{
	this->setTimes(0);

	mOwner->GetQuestMgr()->SetDailyDoTime(0);

	initDailyQuestList();
}

/// 刷星, isFull表明是否全部刷满级，type即选择刷满的类型
//int DailyQuests::refreshStarLevel(bool isFull,int type)
//{
//	if(!mOwner)
//	{
//		return eNull;
//	}
//
//	if(isFull)
//	{
//		if(this->getNumStarLevel() >= MAX_DAILYQUEST_STARLEVEL &&
//				this->getRewardStarLevel() >= MAX_DAILYQUEST_STARLEVEL)
//		{
//			return eTaskMaxStart;
//		}
//	}
//	else
//	{
//		if(eRefreshStarTNum == type)
//		{
//			if(this->getNumStarLevel() >= MAX_DAILYQUEST_STARLEVEL)
//			{
//				return eTaskMaxStart;
//			}
//		}
//		else if(eRefreshStarReward == type)
//		{
//			if(this->getRewardStarLevel() >= MAX_DAILYQUEST_STARLEVEL)
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
//		if(mOwner->getAttManage()->getValue<int>(eCharMoney) < tempMoney)
//		{
//			return eNotEnoghMoney;
//		}
//
//		tempGolden = tempRewardRefesh->m_NumGolden + tempNumRefesh->m_RewardGolden;
//
//		if(mOwner->getAttManage()->getValue<int>(eCharGolden) < tempGolden)
//		{
//			return eNotEnogGolden;
//		}
//
//		float tempConsume = getCoefficient();
//		int tempNumStarts = QuestLoader::GetInstance()->refreshNumStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//				,getNumStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Functional,tempConsume);
//
//		if(tempNumStarts == getNumStarLevel())
//		{
//			tempError = eRefreshFail;
//		}
//
//		this->SetNumStarLevel(tempNumStarts);
//		setCoefficient(tempConsume);
//
//		int tempRewardStarts = QuestLoader::GetInstance()->refreshStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//				,getRewardStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Functional);
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
//		if(mOwner->getAttManage()->getValue<int>(eCharMoney) < tempMoney)
//		{
//			return eNotEnoghMoney;
//		}
//
//		if(mOwner->getAttManage()->getValue<int>(eCharGolden) < tempGolden)
//		{
//			return eNotEnogGolden;
//		}
//
//		if(eRefreshStarTNum == type)
//		{
//			float tempConsume = getCoefficient();
//			int tempNumStarts = QuestLoader::GetInstance()->refreshNumStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//					,getNumStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Functional,tempConsume);
//
//			if(getNumStarLevel() == tempNumStarts)
//			{
//				tempError = eRefreshFail;
//			}
//			this->SetNumStarLevel(tempNumStarts);
//			setCoefficient(tempConsume);
//		}
//		else if(eRefreshStarReward == type)
//		{
//			int tempRewardStarts = QuestLoader::GetInstance()->refreshStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//					,getRewardStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Functional);
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
//		mOwner->ChangeMoney(tempMoney,false);
//		vecType.push_back(eCharMoney);
//	}
//
//	if(tempGolden > 0)
//	{
//		mOwner->ChangeGolden(tempGolden,false);
//		vecType.push_back(eCharGolden);
//	}
//
//	if(vecType.size() > 0)
//	{
//		mOwner->SynCharAttribute(vecType);
//	}
//
//	mOwner->SetDataFlag(eBaseInfo);
//	mOwner->SetDataFlag(eQuestInfo);
//
//	mIsDirty = true;
//
//	return tempError;
//}


Quest_CondStateInfo DailyQuests::updateToClient()
{
	if(mIsDirty)
	{
		QuestProto::UpdateDailyTask info;

		Quest_CondStateInfo stateInfo;

		this->GetDailyQuestConditionStateInfo(&stateInfo);

		this->SetUpdateToClientCmd(&stateInfo, &info);

		QuickMessage::GetInstance()->AddSendMessage(mOwner->GetID(), mOwner->GetChannelID(),&info,MSG_SIM_GM2C_UPDATEDAILYTASK);

		if(stateInfo.state == QuestState_CanSubmit)
		{
			LOG_DEBUG(FILEINFO,"Player %lld CanSubmit Task,%d type,%d state,%d numStar,%d rewardStar,%d times,%d nowNum,%d targeNum,%d",
					mOwner->GetID(),
					stateInfo.questID,
					QuestType_Functional,
					stateInfo.state,
					stateInfo.numStar,
					stateInfo.rewardStar,
					stateInfo.times,
					stateInfo.nowNum,
					stateInfo.targeNum);
		}

		return stateInfo;
	}

	return Quest_CondStateInfo();
}

void DailyQuests::GetDailyQuestConditionStateInfo(Quest_CondStateInfo *ptr)
{
	if(!ptr)
	{
		return ;
	}

	Quest* daily = mOwner->GetQuestMgr()->getAcceptedQuests().findDailyQuest();
	if(daily)
	{
		ptr->questID = this->getQuestId();
		if(ptr->questID == 0)
			return ;

		if(ptr->questID != daily->getEntry()->m_nQuestId)
			return ;

		ptr->state = daily->getQuestState();

		ptr->numStar = this->getNumStarLevel();
		ptr->rewardStar = this->getRewardStarLevel();
		ptr->times = this->getTimes();

		if(this->getTimes() > mMaxTimes)
			return ;

		if(ptr->state == QuestState_Accepted)
		{
			QuestCondition *cond = daily->getCondition(0);
			if(cond)
			{
				ptr->nowNum = cond->getValue();
				ptr->targeNum = GetTargetNum();
			}
		}
		else if(daily->getQuestState() == QuestState_CanSubmit)
		{
			ptr->nowNum = GetTargetNum();
			ptr->targeNum = GetTargetNum();
		}
	}
}

void DailyQuests::SetUpdateToClientCmd(const Quest_CondStateInfo *ptr,QuestProto::UpdateDailyTask *info)
{
	if(!ptr || !info)
	{
		return ;
	}

	info->set_state(ptr->state);
	info->set_questid(ptr->questID);
	info->set_numstar(ptr->numStar);
	info->set_rewardstar(ptr->rewardStar);
	info->set_times(ptr->times);
	info->set_fvalue(ptr->nowNum);
	info->set_svalue(ptr->targeNum);
}
bool DailyQuests::onRead(const PlayerInfo::QuestInfo &info)
{
	int yesterDailySize = info.yesterdaydaily_size();
	if(0 == yesterDailySize)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_DAILYTASK_NUM;
	}
	else
	{
		for(int i=0; i<yesterDailySize; ++i)
		{
			PlayerInfo::DailyQuest dailyinfo = info.yesterdaydaily(i);
			m_YesterdayQuestIdAccepted = dailyinfo.questid();
			m_YesterdayTimes = dailyinfo.times();
		}
	}

	mIsDirty = true;

	for(int i=0; i<info.daily_size(); ++i)
	{
		PlayerInfo::DailyQuest dailyinfo = info.daily(i);
		if(dailyinfo.times() >= mMaxTimes)
		{
			this->setTimes(mMaxTimes);
			return true;
		}

		this->setQuestId(dailyinfo.questid());

		this->SetNumStarLevel(dailyinfo.numlevel() > Max_Num_Start ? Max_Num_Start : dailyinfo.numlevel());

		this->setRewardStarLevel(dailyinfo.rewardlevel() > Max_Reward_Start ? Max_Reward_Start : dailyinfo.rewardlevel());

		this->setTimes(dailyinfo.times());

		setCoefficient(dailyinfo.numcoefficient());
	}

	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool DailyQuests::InitLastDailyQuest(const PlayerInfo::QuestInfo &info)
{
	for(int i=0; i<info.daily_size(); ++i)
	{
		PlayerInfo::DailyQuest dailyinfo = info.daily(i);
		m_lastDoQuestID = dailyinfo.questid();
	}

	for(int i=0; i<info.active_size(); ++i)
	{
		//ActiveQuest里面存储了所有正在进行任务的状态
		Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(info.active(i).id());
		if(entry && (entry->m_nMainType == QuestType_Functional))
		{
			if(info.active(i).id() != (int)m_lastDoQuestID)
			{
				continue;
			}

			m_lastDoCondNum = info.active(i).ftarget();
		}
	}
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool DailyQuests::InitYesterdayDailyQuest(const PlayerInfo::QuestInfo &info)
{
	eDayResetType tempType = mOwner->GetCounterService()->getResetDayType();
	if(eDayResetBeforeYesterDay == tempType)
	{
		m_YesterdayQuestIdAccepted = GetInitDailyQuestId();
		m_YesterdayTimes = 0;
		return true;
	}

	int questIdAccepted = 0;
	int times = MAX_DAILYTASK_NUM;
	for(int i=0; i<info.daily_size(); ++i)
	{
		PlayerInfo::DailyQuest dailyinfo = info.daily(i);
		questIdAccepted = dailyinfo.questid();
		times = dailyinfo.times();
		if(times >= MAX_DAILYTASK_NUM)
		{
			times = MAX_DAILYTASK_NUM;
			break;
		}
	}

	if(eDayResetYesterday == tempType)
	{
		m_YesterdayQuestIdAccepted = questIdAccepted;
		m_YesterdayTimes = times;
		return true;
	}

	int yesterDailySize = info.yesterdaydaily_size();
	if(0 == yesterDailySize)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_DAILYTASK_NUM;
		return true;
	}

	mIsDirty = true;
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool DailyQuests::onWrite(PlayerInfo::QuestInfo *info)
{
	if(this->getTimes() > mMaxTimes)
		return false;

	if(info)
	{
		PlayerInfo::DailyQuest *dailyinfo = info->add_daily();

		dailyinfo->set_questid(this->getQuestId());
		dailyinfo->set_numlevel(this->getNumStarLevel());
		dailyinfo->set_rewardlevel(this->getRewardStarLevel());
		dailyinfo->set_times(this->getTimes());
		dailyinfo->set_numcoefficient(getCoefficient());

		PlayerInfo::DailyQuest *yesterdayDailyinfo = info->add_yesterdaydaily();

		yesterdayDailyinfo->set_questid(m_YesterdayQuestIdAccepted);
		yesterdayDailyinfo->set_numlevel(this->getNumStarLevel());
		yesterdayDailyinfo->set_rewardlevel(this->getRewardStarLevel());
		yesterdayDailyinfo->set_times(m_YesterdayTimes);

	}

	return true;
}


int DailyQuests::GetTargetNum()
{
	Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(this->getQuestId());
	if(!info)
	{
		return 0;
	}

	if(this->getNumStarLevel() == 0)
		this->SetNumStarLevel(1);

	int targetNum = (int)(m_NumCoefficient * info->m_nTaskTargetNum);

	if(targetNum <= 0)
	{
		targetNum = 1;
	}
	return targetNum;
}

void DailyQuests::GetYesterdayDailyQuest(Daily::RecoverResourceInfoList * info)
{
	int restTimes = MAX_DAILYTASK_NUM - m_YesterdayTimes;
	if(restTimes <= 0)
		return;

	Daily::RecoverResourceInfo * dailyQuestInfo = info->add_infolist();
	dailyQuestInfo->set_id(eRecorceSourceParamType_dailyQuest);
	dailyQuestInfo->set_restcount(restTimes);

	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Functional,mOwner->getAttManage()->getValue<int>(eCharLv));
	if(tinfo)
	{
		Daily::RecoverResourceReward * reward = dailyQuestInfo->mutable_reward();
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

void DailyQuests::GetDailyQuest(Daily::DailyToDoInfoList * toClient)
{
	Daily::DailyToDoInfo * clanQuestInfo = toClient->add_infolist();
	clanQuestInfo->set_id(eDailyToDoType_dailyQuest);
	clanQuestInfo->set_count(this->getTimes());
}

void DailyQuests::setQuestId(const uint& questId)
{
		mQuestIdAccepted = questId;
}

void DailyQuests::SetDailyQuesToYesterday()
{
	m_YesterdayQuestIdAccepted = mQuestIdAccepted;
	m_YesterdayTimes = mTimes;
}

int DailyQuests::GetUnCompleteNum()
{
	return getMaxTimes() - getTimes();
}
