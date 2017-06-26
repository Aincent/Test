/*
 * QuestClan.cpp
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */

#include "QuestClan.h"
#include "../Object/Player/Player.h"
#include "Quest.h"
#include "MessageCommonRet.h"
#include "QuestLoader.h"
#include "Log/Logger.h"
#include "../QuickMessage.h"
#include <math.h>
#include "../Attribute/AttributeCreature.h"
#include "AcceptedQuest.h"
#include "./FileLoader/GameParaLoader.h"
#include "./FileLoader/UpgradeLoader.h"


QuestClan::QuestClan(Player* owner)
	: mOwner(owner)
	, mIsDirty(false)
	, mBackExp(0)
	, mQuestId(0)
	, mNumStarLevel(1)
	, mRewardLevel(1)
	, mTimes(0)
	, mMaxTimes(MAX_CLANTASK_NUM)
	,m_NumCoefficient(1.0f)
	,m_YesterdayQuestIdAccepted(0)
	,m_YesterdayTimes(MAX_CLANTASK_NUM)
	,m_lastDoQuestID(0)
	,m_lastDoCondNum(0)
{
}

QuestClan::~QuestClan()
{
}

void QuestClan::init()
{

}

void QuestClan::uninit()
{
}

void QuestClan::setNeedUpdateToDB()
{
	if(mOwner)
		mOwner->SetDataFlag(eQuestInfo);
}

bool QuestClan::onSysActived()
{
	if(mOwner->getAttManage()->getValue<DWORD>(eCharFaction) > 0)
	{
		selectQuest();
		mIsDirty = true;
		return true;
	}

	return false;
}

int QuestClan::GetInitClanQuestId(void)
{
	// 读取任务表中本等级段可以接受的所有帮派任务
	int level = mOwner->getAttManage()->getValue<int>(eCharLv);

	std::vector<DWORD> questIdList;
	questIdList.clear();

	QuestLoader::GetInstance()->GetLvClanTaskNum(level,questIdList, this->getQuestId());

	if(questIdList.size() <= 0)
	{
		LOG_ERROR(FILEINFO,"Player Selet Clan Task,But Not Have Property Task");
		return 0;
	}

	return questIdList[rand()%(questIdList.size()-1)];
}

// 随机生成一个任务及其星级
void QuestClan::selectQuest()
{
	// 读取任务表中本等级段可以接受的所有帮派任务
	int level = mOwner->getAttManage()->getValue<int>(eCharLv);

	std::vector<DWORD> questIdList;
	questIdList.clear();

	QuestLoader::GetInstance()->GetLvClanTaskNum(level,questIdList, this->getQuestId());
	if(questIdList.size() <= 0)
	{
		return ;
	}

	uint new_questID = questIdList[rand()%questIdList.size()];

	m_NumCoefficient = 1.0f;

	this->setQuestId(new_questID);

	selectStarLevel(eRefreshStarTNum);

	selectStarLevel(eRefreshStarReward);

	setNeedUpdateToDB();
}

void QuestClan::selectStarLevel(int type)
{
	if (this->getQuestId() == 0)
		return;

	// 随机刷奖励和数量的星级

	if(eRefreshStarTNum == type)
	{
		this->setNumStarLevel(1);
	}
	else if(eRefreshStarReward == type)
	{
		this->setRewardStarLevel(1);
	}

	setNeedUpdateToDB();
	mIsDirty = true;
}

void QuestClan::clear()
{
}


bool QuestClan::canAccept(const Entry_Quest* entry, int error) const
{
	if (!entry)
	{
		error = eQuestNotExist;
		return false;
	}

	if(entry->m_nMainType != QuestType_Clan)
	{
		error = eNotClanTask;
		return false;
	}

	uint now_questID = this->getQuestId();

	if(now_questID > 0 && now_questID != entry->m_nQuestId)
	{
		error = eHaveClanTask;
		return false;
	}

	//是否加入帮派
	if(mOwner->getAttManage()->getValue<DWORD>(eCharFaction) <= 0 )
	{
		error = eClanIstExisted;
		return false;
	}

	//所需的等级
	if((int)entry->m_nLevelDown > mOwner->getAttManage()->getValue<int>(eCharLv))
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

int QuestClan::GiveSubmitReward(const BYTE i_RewardLevel)
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

	mOwner->GetContainer(ePackageType)->AddItem(tItemList,tItemNum,tempPosList,tBindTypeList,strengthLvVec,true);

	mOwner->SetDataFlag(eBaseInfo);
	mOwner->SetDataFlag(eBattleInfo);
	return 0;
}

void QuestClan::IncreaseDoTime()
{
	mTimes++;

	BYTE time = mOwner->GetQuestMgr()->GetClanDoTime();

	mOwner->GetQuestMgr()->SetClanDoTime(time + 1);
}

int QuestClan::IncreaseDoMaxTime()
{
	mOwner->GetQuestMgr()->SetClanDoTime(mMaxTimes);

	mTimes = mMaxTimes;
	this->setQuestId(0);
	mNumStarLevel = 1;
	mRewardLevel = 1;

	setNeedUpdateToDB();
	mIsDirty = true;
	return 0;
}

bool QuestClan::nextIsOver()
{
	BYTE times = mTimes;
	times = times +1;
	if(times == mMaxTimes)
		return true;
	return false;
}

/// 玩家提交帮派任务
bool QuestClan::onSubmit(const uint & questId)
{
	if(questId == this->getQuestId())
	{
		this->IncreaseDoTime();

		setNeedUpdateToDB();

		if(this->getTimes() >mMaxTimes)
			return false;

		if(this->getTimes() < mMaxTimes)
		{
			selectQuest();
		}
		else
		{
			this->setQuestId(0);
			mNumStarLevel = 1;
			mRewardLevel = 1;
			mIsDirty = true;

			m_NumCoefficient = 1.0f;

			/*通知  今日所有 帮派任务 已完成*/
			mOwner->SendToPlayerResultCodeMessage(eAllClanTaskHadComp);

			return false;
		}

		return true;
	}
		return false;
	}


// 重置玩家的帮派任务次数
void QuestClan::reset()
{
	this->setTimes(0);

	mOwner->GetQuestMgr()->SetClanDoTime(0);

	selectQuest();
}

/// 刷星, isFull表明是否全部刷满级，type即选择刷满的类型
//int QuestClan::refreshStarLevel(bool isFull,int type)
//{
//	if(!mOwner)
//	{
//		return eNull;
//	}
//
//	if(isFull)
//	{
//		if(this->getNumStarLevel() >= MAX_CLANQUEST_STARLEVEL &&
//				this->getRewardStarLevel() >= MAX_CLANQUEST_STARLEVEL)
//		{
//			return eTaskMaxStart;
//		}
//	}
//	else
//	{
//		if(eRefreshStarTNum == type)
//		{
//			if(this->getNumStarLevel() >= MAX_CLANQUEST_STARLEVEL)
//			{
//				return eTaskMaxStart;
//			}
//		}
//		else if(eRefreshStarReward == type)
//		{
//			if(this->getRewardStarLevel() >= MAX_CLANQUEST_STARLEVEL)
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
//		float tempConsume = getCoefficient();
//		int tempNumStarts = QuestLoader::GetInstance()->refreshNumStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//				,getNumStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Clan,tempConsume);
//
//		if(tempNumStarts == getNumStarLevel())
//		{
//			tempError = eRefreshFail;
//		}
//
//		this->setNumStarLevel(tempNumStarts);
//		setCoefficient(tempConsume);
//
//		int tempRewardStarts = QuestLoader::GetInstance()->refreshStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//				,getRewardStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Clan);
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
//					,getNumStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Clan,tempConsume);
//
//			if(getNumStarLevel() == tempNumStarts)
//			{
//				tempError = eRefreshFail;
//			}
//
//			this->setNumStarLevel(tempNumStarts);
//			setCoefficient(tempConsume);
//		}
//		else if(eRefreshStarReward == type)
//		{
//			int tempRewardStarts = QuestLoader::GetInstance()->refreshStarts(mOwner->getAttManage()->getValue<int>(eCharLv)
//					,getRewardStarLevel(),mOwner->getAttManage()->getValue<int>(eVipCardLv),QuestType_Clan);
//
//			if(tempRewardStarts == getRewardStarLevel())
//			{
//				tempError = eRefreshFail;
//			}
//
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
//	mOwner->SetDataFlag(eBaseInfo);
//	mOwner->SetDataFlag(eQuestInfo);
//
//	mIsDirty = true;
//
//	return tempError;
//}

Quest_CondStateInfo QuestClan::updateToClient()
{
	if(mIsDirty)
	{
		QuestProto::UpdateClanTask info;

		Quest_CondStateInfo stateInfo;

		this->GetClanQuestConditionStateInfo(&stateInfo);

		this->SetUpdateToClientCmd(&stateInfo, &info);

		QuickMessage::GetInstance()->AddSendMessage(mOwner->GetID(), mOwner->GetChannelID(),&info,MSG_SIM_GM2C_UPDATECLANTASK);

		if(stateInfo.state == QuestState_CanSubmit)
		{
			LOG_DEBUG(FILEINFO,"Player %lld CanSubmit Task,%d type,%d state,%d numStar,%d rewardStar,%d times,%d nowNum,%d targeNum,%d",
					mOwner->GetID(),
					stateInfo.questID,
					QuestType_Clan,
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

void QuestClan::GetClanQuestConditionStateInfo(Quest_CondStateInfo *ptr)
{
	if(!ptr)
	{
		return ;
	}

	Quest* clan = mOwner->GetQuestMgr()->getAcceptedQuests().findClanQuest();
	if(clan)
	{
		ptr->questID = this->getQuestId();
		if(ptr->questID == 0)
			return ;

		if(this->getQuestId() != clan->getEntry()->m_nQuestId)
			return;


		ptr->state = clan->getQuestState();

		ptr->numStar = this->getNumStarLevel();
		ptr->rewardStar = this->getRewardStarLevel();
		ptr->times = this->getTimes();

		if(this->getTimes() > mMaxTimes)
			return ;

		if(ptr->state == QuestState_Accepted)
		{
			QuestCondition *cond = clan->getCondition(0);
			if(cond)
			{
				ptr->nowNum = cond->getValue();
				ptr->targeNum = GetTargetNum();
			}
		}
		else if(clan->getQuestState() == QuestState_CanSubmit)
		{
			ptr->nowNum = GetTargetNum();
			ptr->targeNum = GetTargetNum();
		}
	}
}

void QuestClan::SetUpdateToClientCmd(const Quest_CondStateInfo *ptr,
		QuestProto::UpdateClanTask *info)
{
	info->set_state(ptr->state);
	info->set_questid(ptr->questID);
	info->set_numstar(ptr->numStar);
	info->set_rewardstar(ptr->rewardStar);
	info->set_times(ptr->times);
	info->set_fvalue(ptr->nowNum);
	info->set_svalue(ptr->targeNum);
}

bool QuestClan::evalAcceptedQuest()
{
	selectQuest();

	return false;
}

bool QuestClan::onRead(const PlayerInfo::QuestInfo &info)
{
	int yesterClanSize = info.yesterdayclan_size();
	if(0 == yesterClanSize)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_CLANTASK_NUM;
	}
	else
	{
		for(int i=0; i<yesterClanSize; ++i)
		{
			PlayerInfo::ClanQuest claninfo = info.yesterdayclan(i);
			m_YesterdayQuestIdAccepted = claninfo.questid();
			m_YesterdayTimes = claninfo.times();
		}
	}

	mIsDirty = true;

	for(int i=0; i<info.clan_size(); ++i)
	{
		PlayerInfo::ClanQuest claninfo = info.clan(i);
		if(claninfo.times() >= mMaxTimes)
		{
			this->setTimes(mMaxTimes);

			return true;
		}

		this->setQuestId(claninfo.questid());

		this->setNumStarLevel(claninfo.numlevel() > Max_Reward_Start ? Max_Reward_Start : claninfo.numlevel());

		this->setRewardStarLevel(claninfo.rewardlevel() > Max_Num_Start ? Max_Num_Start : claninfo.rewardlevel());

		this->setTimes(claninfo.times());

		setCoefficient(claninfo.numcoefficient());
	}
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool QuestClan::InitLastDailyQuest(const PlayerInfo::QuestInfo &info)
{
	for(int i=0; i<info.clan_size(); ++i)
	{
		PlayerInfo::ClanQuest claninfo = info.clan(i);
		m_lastDoQuestID = claninfo.questid();
	}

	for(int i = 0; i < info.active_size(); ++i)
	{
		//ActiveQuest里面存储了所有正在进行任务的状态
		PlayerInfo::ActiveQuest  newActive = info.active(i);

		Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(newActive.id());
		if(entry && (entry->m_nMainType == QuestType_Clan))
		{
			if(newActive.id() != (int)m_lastDoQuestID)
			{
				continue;
			}

			m_lastDoCondNum = info.active(i).ftarget();
		}
	}
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool QuestClan::InitYesterdayClanQuest(const PlayerInfo::QuestInfo &info)
{
	eDayResetType tempType = mOwner->GetCounterService()->getResetDayType();

	if(eDayResetBeforeYesterDay == tempType)
	{
		m_YesterdayQuestIdAccepted = GetInitClanQuestId();
		m_YesterdayTimes = 0;
		return true;
	}

	int questId = 0;
	int times = MAX_CLANTASK_NUM;
	for(int i=0; i<info.clan_size(); ++i)
	{
		PlayerInfo::ClanQuest claninfo = info.clan(i);
		questId = claninfo.questid();
		times = claninfo.times();
		if(claninfo.times() >= MAX_CLANTASK_NUM)
		{
			times = MAX_CLANTASK_NUM;
			break;
		}
	}

	if(eDayResetYesterday == tempType)
	{
		m_YesterdayQuestIdAccepted = questId;
		m_YesterdayTimes = times;
		return true;
	}

	int yesterClanSize = info.yesterdayclan_size();
	if(0 == yesterClanSize)
	{
		m_YesterdayQuestIdAccepted = 0;
		m_YesterdayTimes = MAX_CLANTASK_NUM;
		return true;
	}

	mIsDirty = true;
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool QuestClan::onWrite(PlayerInfo::QuestInfo *info)
{
	PlayerInfo::ClanQuest *yesterdayClaninfo = info->add_yesterdayclan();

	yesterdayClaninfo->set_questid(m_YesterdayQuestIdAccepted);
	yesterdayClaninfo->set_numlevel(this->getNumStarLevel());
	yesterdayClaninfo->set_rewardlevel(this->getRewardStarLevel());
	yesterdayClaninfo->set_times(m_YesterdayTimes);

	PlayerInfo::ClanQuest *claninfo = info->add_clan();

	claninfo->set_questid(this->getQuestId());
	claninfo->set_numlevel(this->getNumStarLevel());
	claninfo->set_rewardlevel(this->getRewardStarLevel());
	claninfo->set_times(this->getTimes());
	claninfo->set_numcoefficient(getCoefficient());

	return true;
}

int QuestClan::directComplete()
{

	Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(this->getQuestId());
	if(!info)
	{
		return eNull;
	}

	//消耗多少元宝，表没有配，这里先填值
	uint price = 10; //info->m
	uint hadMoney =  mOwner->GetTypeValue(eCharGolden);
	if (hadMoney < price)
	{
	    return eNotEnoughGold;
	}
	vector<int> vecType;

	mOwner->ChangeGolden(price, false);
	vecType.push_back(eCharGolden);
	mOwner->SynCharAttribute(vecType);

	return 0;
}

int QuestClan::GetTargetNum()
{
	Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(this->getQuestId());
	if(!info)
	{
		return 0;
	}

	int targetNum = (int)(m_NumCoefficient * info->m_nTaskTargetNum);

	if(targetNum <= 0)
	{
		targetNum = 1;
	}
	return targetNum;
}

void QuestClan::GetYesterdayClanQuest(Daily::RecoverResourceInfoList * info)
{
	DWORD faction = mOwner->getAttManage()->getValue<DWORD>(eCharFaction);
	if(faction <= 0)
		return;
	int restTimes = MAX_CLANTASK_NUM - m_YesterdayTimes;
	if(restTimes <=0)//已全部做完或者没有帮派任务
		return;
	Daily::RecoverResourceInfo * clanQuestInfo = info->add_infolist();
	clanQuestInfo->set_id(eRecorceSourceParamType_clanQuest);
	clanQuestInfo->set_restcount(restTimes);
	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)QuestType_Clan,mOwner->getAttManage()->getValue<int>(eCharLv));
	if(tinfo)
	{
		Daily::RecoverResourceReward * reward = clanQuestInfo->mutable_reward();
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

void QuestClan::GetClanQuest(Daily::DailyToDoInfoList * toClient)
{
	if(mOwner->getAttManage()->getValue<DWORD>(eCharFaction) <= 0)
		return;

	Daily::DailyToDoInfo * clanQuestInfo = toClient->add_infolist();
	clanQuestInfo->set_id(eDailyToDoType_clanQuest);
	clanQuestInfo->set_count(mTimes);
}

void QuestClan::setQuestId(const uint& questId)
{
	mQuestId = questId;
}

void QuestClan::SetClanQuestToYesterday()
{
	m_YesterdayQuestIdAccepted = mQuestId;
	m_YesterdayTimes = mTimes;
}

void QuestClan::initResetQuest()
{

	PlayerInfo::ActiveQuest info;

	info.set_id(getQuestId());

	mOwner->GetQuestMgr()->getAcceptedQuests().InitAccptedQuest(info,false);

	return ;
}


int QuestClan::GetUnCompleteNum()
{
	return getMaxTimes() - getTimes();
}
