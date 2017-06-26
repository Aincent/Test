/*
 * AcceptedQuest.cpp
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */


#include "AcceptedQuest.h"
#include "../Object/Player/Player.h"
#include "Log/Logger.h"
#include "../QuickMessage.h"
#include "../Attribute/AttributeCreature.h"

AcceptedQuests::AcceptedQuests(Player* owner)
	: mOwner(owner)
	, mIsDirty(false)
{
}

AcceptedQuests::~AcceptedQuests()
{
}

void AcceptedQuests::init()
{
	clear();
}

void AcceptedQuests::uninit()
{
	clear();
}

void AcceptedQuests::clear()
{
	mQuestList.clear();
}

Quest* AcceptedQuests::findQuest(uint questId) const
{
	QuestList::const_iterator it = mQuestList.begin();
	for( ;it != mQuestList.end(); ++it)
	{
		if ((*it)->getEntry()->m_nQuestId == questId)
			return const_cast<Quest*>((*it).Get());
	}

	return NULL;
}

Quest* AcceptedQuests::findDailyQuest() const
{
	QuestList::const_iterator it = mQuestList.begin();
	for(; it !=mQuestList.end(); ++it)
	{
		if ((*it)->getEntry()->m_nMainType == QuestType_Functional)
			return const_cast<Quest*>((*it).Get());
	}

	return NULL;
}

Quest* AcceptedQuests::findClanQuest() const
{
	QuestList::const_iterator it=mQuestList.begin();
	for( ; it != mQuestList.end(); ++it)
	{
		if ((*it)->getEntry()->m_nMainType == QuestType_Clan)
			return const_cast<Quest*>((*it).Get());
	}

	return NULL;
}

Quest* AcceptedQuests::findCampQuest() const
{
	QuestList::const_iterator it=mQuestList.begin();
	for( ; it != mQuestList.end(); ++it)
	{
		if ((*it)->getEntry()->m_nMainType != QuestType_Camp)
			continue;

		return const_cast<Quest*>((*it).Get());
	}

	return NULL;
}

uint AcceptedQuests::findMainQuest() const
{
	QuestList::const_iterator it = mQuestList.begin();
	for(; it != mQuestList.end(); ++it)
	{
		if ((*it)->getEntry()->m_nMainType == QuestType_Main)
			return (*it)->getEntry()->m_nQuestId;
	}

	return 0;
}

DWORD AcceptedQuests::findQuestByType(QuestType type)
{
	QuestList::const_iterator it = mQuestList.begin();
	for(; it != mQuestList.end(); ++it)
	{
		if ((*it)->getEntry()->m_nMainType == (DWORD)type)
			return (*it)->getEntry()->m_nQuestId;
	}

	return 0;
}

bool AcceptedQuests::hasMainQuest() const
{
	return findMainQuest() != 0;
}

Quest* AcceptedQuests::addQuest(uint questId, uint questState,eOnLineOrAccept type, DWORD numStar,DWORD rewardStar)
{
	const Entry_Quest* entry = Quest::getEntry(questId);
	if(!entry)
	{
		return NULL;
	}

	if(numStar ==0)
		numStar = 1;

	/*支线任务之外，其他任务 只能 存在 一个*/
	//查找 是否 已经 有 相同的
	if(entry->m_nMainType != QuestType_Branch)
	{
		QuestList::iterator it = mQuestList.begin();
		for (; it != mQuestList.end(); )
		{
			uint mainType = (*it)->getEntry()->m_nMainType;
			if(mainType == entry->m_nMainType)
			{
				mQuestList.erase(it++);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	Smart_Ptr<Quest> q = NULL;
	q = new Quest(mOwner, entry, numStar, rewardStar);
	if(q)
	{
		if(!q->create(questState,type))
		{
			return NULL;
		}

		mQuestList.push_back(q);
	}
	else
	{
		return NULL;
	}

	return q.Get();
}

void AcceptedQuests::removeQuest(uint questId)
{
	QuestList::iterator it = mQuestList.begin();
	for(; it != mQuestList.end(); ++it)
	{
		if((*it)->getEntry()->m_nQuestId == questId)
		{
			mQuestList.erase(it);
			mOwner->SetDataFlag(eQuestInfo);
			return ;
		}
	}
}

void AcceptedQuests::setDirty(bool setting)
{
	mIsDirty = setting;
}

bool questLess(const Quest* l, const Quest* r)
{
	return *l<*r;
}

Quest_CondStateInfo AcceptedQuests::updateToClient()
{
	Quest_CondStateInfo tempData;
	//这里更新主线任务和支线任务(类似)
	if(isDirty())
	{
		QuestProto::UpdateMainTask toClient;

		QuestList::const_iterator it = mQuestList.begin();
		for(; it !=mQuestList.end(); ++it)
		{
			const Quest* q = (*it).Get();
			if(!q)
			{
				continue;
			}

			const Entry_Quest *info = q->getEntry();
			if (!info)
			{
				continue;
			}

			if(info->m_nMainType == QuestType_Main)
			{
				toClient.set_state(q->getQuestState());
				toClient.set_svalue(info->m_nTaskTargetNum);
				toClient.set_questid(info->m_nQuestId);

				if(q->getQuestState() == QuestState_Accepted)
				{
					const QuestCondition* cond = q->getCondition(0);
					if(!cond)
					{
						continue;
					}
					toClient.set_fvalue(cond->getValue());
				}
				else
				{
					toClient.set_fvalue(info->m_nTaskTargetNum);			//
				}

				tempData.questID 	= toClient.questid();
				tempData.state 		= (QuestState)toClient.state();
				tempData.nowNum 	= toClient.fvalue();
				tempData.targeNum 	= toClient.svalue();
			}
			else if(info->m_nMainType == QuestType_Branch)
			{
				QuestProto::ClientBranchTask *branch = toClient.mutable_branch();

				if(q->getQuestState() == QuestState_Accepted)
				{
					const QuestCondition* cond = q->getCondition(0);
					if(!cond)
					{
						continue;
					}

					branch->add_fvalue(cond->getValue());
				}
				else
				{
					branch->add_fvalue(info->m_nTaskTargetNum);
				}

				branch->add_questid(info->m_nQuestId);
				branch->add_state(q->getQuestState());
				branch->add_svalue(info->m_nTaskTargetNum);
			}
		}

		if(toClient.questid() > 0 || toClient.has_branch())
		{
			if(toClient.questid() > 0 && tempData.state == QuestState_CanSubmit)
			{
				LOG_DEBUG(FILEINFO,"Player %lld CanSubmit Task,%d type,%d state,%d numStar,%d rewardStar,%d times,%d nowNum,%d targeNum,%d",
						mOwner->GetID(),
						tempData.questID,
						QuestType_Main,
						tempData.state,
						tempData.numStar,
						tempData.rewardStar,
						tempData.times,
						tempData.nowNum,
						tempData.targeNum);
			}

			QuickMessage::GetInstance()->AddSendMessage(mOwner->GetID(), mOwner->GetChannelID(),&toClient,MSG_SIM_GM2C_UPDATEMAINTASK);
		}
	}

	return tempData;
}


bool AcceptedQuests::onRead(const PlayerInfo::QuestInfo &info, bool todayFlag)
{
	for(int i=0; i<info.active_size(); ++i)
	{
		//ActiveQuest里面存储了所有正在进行任务的状态
		PlayerInfo::ActiveQuest newActive = info.active(i);

		InitAccptedQuest(newActive, todayFlag);
	}
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool AcceptedQuests::InitAccptedQuest(const PlayerInfo::ActiveQuest &newActive, bool todayFlag)
{
	uint questId = newActive.id();
	uint questState = newActive.state();
	int cond1Value = newActive.ftarget();
	//这个默认要是1
	float tempNumCoefficient = 1.0f;
//		int cond2Value = newActive.starget();  /*暂时没用*/

	const Entry_Quest* entry = Quest::getEntry(questId);
	if(!entry)
	{
		return false;
	}

	if(!mOwner->GetQuestMgr())
	{
		return false;
	}

	int numlevel = 1;
	int rewardlevel = 1;
	if(entry->m_nMainType == QuestType_Functional)
	{
		if(!mOwner->GetQuestMgr()->getDailyQuests())
		{
			return false;
		}

		if(todayFlag)
		{
			if(mOwner->GetQuestMgr()->getDailyQuests()->getQuestId() != questId)
			{
				return false;
			}
			if(mOwner->GetQuestMgr()->getDailyQuests()->getTimes() >=
													mOwner->GetQuestMgr()->getDailyQuests()->getMaxTimes() )
			{
				 return false;
			}
		}
		else
		{
			tempNumCoefficient = mOwner->GetQuestMgr()->getDailyQuests()->getCoefficient();
			questId = mOwner->GetQuestMgr()->getDailyQuests()->getQuestId();
			questState = QuestState_Accepted;
			cond1Value = 0;
		}

		numlevel = mOwner->GetQuestMgr()->getDailyQuests()->getNumStarLevel();
		rewardlevel = mOwner->GetQuestMgr()->getDailyQuests()->getRewardStarLevel();
	}
	else if(entry->m_nMainType == QuestType_Clan)
	{
		if(!mOwner->GetQuestMgr()->getClanQuests())
		{
			return false;  /*数据 不一致*/
		}
		if(todayFlag)
		{
			if(mOwner->GetQuestMgr()->getClanQuests()->getQuestId() != questId)
			{
				return false;
			}

			if(mOwner->GetQuestMgr()->getClanQuests()->getTimes() >=
															mOwner->GetQuestMgr()->getClanQuests()->getMaxTimes() )
			{
				 return false;
			}
		}
		else
		{
			tempNumCoefficient = mOwner->GetQuestMgr()->getClanQuests()->getCoefficient();
			questId = mOwner->GetQuestMgr()->getClanQuests()->getQuestId();
			questState = QuestState_Accepted;
			cond1Value = 0;
		}

		numlevel = mOwner->GetQuestMgr()->getClanQuests()->getNumStarLevel();
		rewardlevel = mOwner->GetQuestMgr()->getClanQuests()->getRewardStarLevel();
	}
	else if(entry->m_nMainType == QuestType_Camp)
	{
		if(!mOwner->GetQuestMgr()->getCampQuests())
		{
			return false;  /*数据 不一致*/
		}
		if(todayFlag)
		{
			if(mOwner->GetQuestMgr()->getCampQuests()->getQuestId() != questId)
			{
				return false;
			}

			if(mOwner->GetQuestMgr()->getCampQuests()->getTimes() >= mOwner->GetQuestMgr()->getCampQuests()->getMaxTimes())
			{
				 return false;
			}
		}
		else
		{
			questId    = mOwner->GetQuestMgr()->getCampQuests()->getQuestId();
			questState = QuestState_Accepted;
			cond1Value = 0;
		}

		tempNumCoefficient = mOwner->GetQuestMgr()->getCampQuests()->getCoefficient();
		numlevel    = mOwner->GetQuestMgr()->getCampQuests()->getNumStarLevel();
		rewardlevel = mOwner->GetQuestMgr()->getCampQuests()->getRewardStarLevel();
	}
	else if(entry->m_nMainType == QuestType_Week)
	{
		if(!mOwner->IsOpenSystem(eStTaskWeek))
		{
			return true;
		}
	}

	CheckNowQuest(questId, questState, numlevel, rewardlevel, cond1Value,tempNumCoefficient);

	return true;
}

bool AcceptedQuests::CheckNowQuest(uint questId, uint questState,DWORD numStar,DWORD rewardStar, int cond1Value,float numCoefficient)
{
	if(!findQuest(questId))
	{
		Quest* q = addQuest(questId, questState,eQuestOnline, numStar, rewardStar);
		if (q)
		{
			QuestCondition* qc = 0;
			size_t n = q->getConditionCount();
			if (n >= 1)
			{
				q->setCoefficient(numCoefficient);
				qc = q->getCondition(0);
				if (qc)
				{
					qc->initValue(cond1Value);
				}
			}
		}
	}
	return true;
}

bool AcceptedQuests::onWrite(PlayerInfo::QuestInfo *info, const bool & flag)
{
	//只  保存 主线任务 信息

	QuestList::const_iterator it=mQuestList.begin();
	for(; it !=	mQuestList.end(); ++it)
	{
		const Entry_Quest* entry = (*it)->getEntry();

		QuestState state = (*it)->getQuestState();
		int vals[2] = {0,0};

			//由于帮派和日常任务完成的数量是由星级控制，所以即使完成也需要保存条件指
		size_t cnt = (*it)->getConditionCount();
		for(size_t i = 0; i < cnt; i++)
		{
			const QuestCondition* qc = (*it)->getCondition(i);
			if(qc)
				vals[i] = qc->getValue();
		}

		PlayerInfo::ActiveQuest * newAct = info->add_active();

		newAct->set_id(entry->m_nQuestId);
		newAct->set_state(state);
		newAct->set_ftarget(vals[0]);
		newAct->set_starget(vals[1]);
	}

	return true;
}

void AcceptedQuests::ClearNpcFreshMonster()
{
	QuestList::const_iterator it=mQuestList.begin();
	for(; it != mQuestList.end(); ++it)
	{
		const Quest* q = (*it).Get();
		if(!q)
		{
			return;
		}

		const Entry_Quest* entry = q->getEntry();
		QuestState state = q->getQuestState();

		if(entry && (entry->m_nMinorType == QuestCondType_NpcFreMonster && state == QuestState_Accepted))
		{
			for(int j = 0; j < MAX_CONDITION_NUM;j++)
			{
				QuestCondition*cond = q->getCondition(j);
				int targetNum = 0;
				if(entry->m_nMainType == QuestType_Functional)
				{
					targetNum = 2 * entry->m_nTaskTargetNum / 10 ;
				}
				else if(entry->m_nMainType == QuestType_Clan)
				{
					targetNum = 2 * entry->m_nTaskTargetNum / 10 ;
				}
				else if(entry->m_nMainType == QuestType_Camp)
				{
					targetNum = 2 * entry->m_nTaskTargetNum / 10 ;
				}
				else
				{
					targetNum = entry->m_nTaskTargetNum;
				}

				if(cond)
				{
					cond->setValue(targetNum);
				}
			}
		}
	}
}

//void AcceptedQuests::DeleteBranchLoseTask()
//{
//	bool isUpdate = false;
//
//	QuestList::iterator it=mQuestList.begin();
//	for(; it!=	mQuestList.end(); )
//	{
//		const Entry_Quest* entry = (*it)->getEntry();
//
//		if(entry->m_nMainType == QuestType_Branch && entry->m_clearLevel <= mOwner->getAttManage()->getValue<int>(eCharLv))
//		{
//			int vals[2] = {0,0};
//
//			size_t cnt = (*it)->getConditionCount();
//			for(size_t i = 0; i < cnt; i++)
//			{
//				const QuestCondition* qc = (*it)->getCondition(i);
//				if(qc)
//					vals[i] = qc->getValue();
//			}
//
//			mOwner->GetQuestMgr()->ResetDeleteTaskItem(entry->m_nQuestId, vals[0]);
//			it = mQuestList.erase(it);
//			isUpdate = true;
//		}
//		else
//		{
//			++it;
//		}
//	}
//
//	if(isUpdate)
//	{
//		updateToClient();
//	}
//}



