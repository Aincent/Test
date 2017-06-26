/*
 * Quest.cpp
 *
 *  Created on: 2014年10月20日
 *      Author: root
 */


#include "define.h"
#include "Quest.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../Mail/MailMessage.h"
#include "FightDefined.h"
#include "./FileLoader/InternationalLoader.h"
#include "QuestGuide.h"
#include "../Attribute/AttributeCreature.h"
#include "./FileLoader/GameParaLoader.h"
#include "../StatisticMgr/StatisticHandle.h"

Quest::Quest(Player* owner, const Entry_Quest* entry, DWORD numStar, DWORD rewardStar)
	:mEntry(entry)
	,mOwner(owner)
	,mQuestState(QuestState_Accepted)
	,mNumStar(numStar)
	,mRewardStar(rewardStar)
	,m_NumCoefficient(1.0f)
{
	mQuestConditionList.clear();
}

Quest::~Quest()
{
	QuestConditionList::iterator it=mQuestConditionList.begin();
	for( ; it !=mQuestConditionList.end(); ++it)
	{
		QuestCondition* qc = *it;
		qc->setActive(false);

		if(qc)
		{
			delete qc;
			qc = NULL;
		}
	}
	mQuestConditionList.clear();
}

bool Quest::create(uint questState,eOnLineOrAccept type)
{
	setState((QuestState)questState);

	// 构建条件列表
	if (questState == QuestState_Accepted)
	{
		for (int i=0; i<MAX_CONDITION_NUM; i++)
		{
			const CondCfg& cfg = mEntry->SubmitConds[i];
			if (cfg.type > 0)
			{
				QuestCondition* qc = QuestCondition::createCondition(*this, i);
				if(!qc)
				{
					return false;
				}

				mQuestConditionList.push_back(qc);
//				cout<<endl<<"Quest Create Condition push to Container, quest id is "<<mEntry->m_nQuestId<<endl;
			}
		}

		for (QuestConditionList::iterator it=mQuestConditionList.begin(); it!=mQuestConditionList.end(); ++it)
		{
			QuestCondition* qc = *it;
			if (qc)
			{
				if(type != eQuestOnline)
				{
					qc->setActive(true);
				}
				else
					qc->initCondition();
			}
		}
	}

	return true;
}

const Entry_Quest* Quest::getEntry(uint questId)
{
	return (const Entry_Quest*)QuestLoader::GetInstance()->GetQuestInfo(questId);
}

bool Quest::canSubmit() const
{
	if(mQuestState == QuestState_CanSubmit)
		return true;
	return false;
}


/// 完成任务给予玩家奖励,rewardIdx仅仅用于多选一的奖励索引号
int Quest::submitReward(Player* player, int questID, bool byMail) 
{
	const Entry_Quest *tQuest = QuestLoader::GetInstance()->GetQuestInfo(questID);
	if(tQuest == NULL)
	{
		return eSystemError;
	}

	if(!player)
	{
		return eNull;
	}

	std::vector<DWORD> itemList;
	std::vector<DWORD> itemNum;

	Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
	if(!pPacket)
	{
		return eNull;
	}

	if(tQuest->m_vAwardItemList.size() > 0)
	{
		if(pPacket)
		{
			for(size_t i=0; i<tQuest->m_vAwardItemList.size(); i++)
			{
				uint itemId = tQuest->m_vAwardItemList[i];
				uint num = tQuest->m_vAwardItemNum[i];
				if(itemId > 0 && num > 0)
				{
					itemList.push_back(itemId);
					itemNum.push_back(num);
				}
			}
		}
	}

	//判断是否有武器奖励
	if(tQuest->m_vEquipId.size() > 0)
	{
		int index = player->GetSex() == false ? 0 : 3;
		index = index + player->getAttManage()->getValue<BYTE>(eCharProfession)-1;
		if(index < (int)tQuest->m_vEquipId.size() && index < (int)tQuest->m_vEquipNum.size())
		{
			uint equipId = tQuest->m_vEquipId[index];
			uint num = tQuest->m_vEquipNum[index];
			itemList.push_back(equipId);
			itemNum.push_back(num);
		}
	}

	vector<WORD> bindVec;

	bindVec.resize(itemList.size(),1);

	int res = player->GetContainer(ePackageType)->IsBagEnought(itemList, itemNum,bindVec);

	if(!res)
	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;

		if(!byMail)
		{
			vector<int> vecType;
			vector<int> vecBro;
			vector<int> vecPos;

			vector<DWORD> strengthLvVec;

			strengthLvVec.resize(itemList.size(), 0);

			player->GetContainer(ePackageType)->AddItem(itemList, itemNum, vecPos, bindVec,strengthLvVec,true,quest_fly_type);

//			for(uint i = 0;i < itemList.size();++i)
//			{
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,itemList[i],itemNum[i]);
//			}
		}

		if(byMail)
		{
			for(uint i=0; i<itemList.size(); i++)
			{
				vector<int64> goods;
				int64 info = itemNum[i];
				info = info << 32;	//物品数量
				DWORD itemid = itemList[i];
				info  = info | itemid;
				goods.push_back(info);
				string warning = InternationalLoader::GetInstance()->GetInterStr(11);
				MailMessageMgr::GetInstance()->SendSysMail(goods, player->GetName(),0,warning.c_str());
			}
		}

		// 经验、银两、元宝、精气、声望
		vector<int> vecType;

		int exp = tQuest->m_nrewardExp;
		int money = tQuest->m_nrewardMoney;
		int sp = tQuest->m_nrewardForce;

		if(!byMail)
		{
			if(money > 0)
			{
				player->ChangeMoney(money,	true);

				vecType.push_back(eCharMoney);

//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);

//				StatisticInfo::CostAndGotInfo costAndGotInfo1;
//				StatisticInfo::GotItemInfo * gotInfo1 = costAndGotInfo1.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo1,eCharMoney,money);
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Money,eStaMinor_Money_Quest,&costAndGotInfo1);
			}

			if(exp >0)
			{
//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharExp,exp);

				player->AddExp(exp);
			}

			if(sp >0)
			{
				player->ChangeCurForce(sp, true);
				vecType.push_back(eCharForce);

//				StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,sp);
			}
		}

		player->SetDataFlag(eBaseInfo);
		player->SetDataFlag(eBattleInfo);
		player->SynCharAttribute(vecType);

//	 if(costAndGotInfo.gotlist_size() > 0 && !byMail && eAccountType_User == mOwner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(mOwner->GetID(),eStatic_Task,eStaMinor_Task_GetReward,&costAndGotInfo);


		return 0;
	}

	return ePackageFull;
}

QuestCondition* Quest::getCondition(int idx) const
{
	if(idx >=0 && idx <(int)mQuestConditionList.size())
		return mQuestConditionList[idx];
	return 0;
}

// 当条件的值发生改变时，调用该方法
void Quest::updateCondition()
{
	bool canSubmit = true;
	for(QuestConditionList::iterator it=mQuestConditionList.begin(); it!=mQuestConditionList.end(); ++it)
	{
		QuestCondition* qc = *it;
		if(qc->isFinished())
		{
			qc->setActive(false);
		}
		else
		{
			canSubmit = false;
			break;
		}
	}

	if(canSubmit)
	{
		setState(QuestState_CanSubmit);
	}
	else
	{
		onStateOrConditionChanged();
	}
}

void Quest::setState(QuestState state)
{
	if(mQuestState != state)
	{
		mQuestState = state;
		onStateOrConditionChanged();
	}
	mOwner->SetDataFlag(eQuestInfo);
}

void Quest::onStateOrConditionChanged()
{
	//任务状态改变，数据设脏
	if(mOwner->GetQuestMgr())
	{
		if(mEntry->m_nMainType == QuestType_Functional)
		{
			mOwner->GetQuestMgr()->getDailyQuests()->setDirty(true);
			mOwner->GetQuestMgr()->getDailyQuests()->updateToClient();
		}
		else if(mEntry->m_nMainType == QuestType_Clan)
		{
			mOwner->GetQuestMgr()->getClanQuests()->setDirty(true);
			mOwner->GetQuestMgr()->getClanQuests()->updateToClient();
		}
		else if(mEntry->m_nMainType == QuestType_Main)
		{
			mOwner->GetQuestMgr()->getAcceptedQuests().setDirty(true);
			mOwner->GetQuestMgr()->getAcceptedQuests().updateToClient();
		}
		else if(mEntry->m_nMainType == QuestType_Branch)
		{
			mOwner->GetQuestMgr()->getAcceptedQuests().setDirty(true);
			mOwner->GetQuestMgr()->getAcceptedQuests().updateToClient();
		}
		else if(mEntry->m_nMainType == QuestType_Camp)
		{
			mOwner->GetQuestMgr()->getCampQuests()->updateToClient();
		}
		else if(mEntry->m_nMainType == QuestType_Guide)
		{
			mOwner->GetQuestMgr()->getGuideQuests()->updateToClient();
		}
		else if(mEntry->m_nMainType == QuestType_Week)
		{
			mOwner->GetQuestMgr()->getWeekTask()->updateToClient();
		}

		mOwner->SetDataFlag(eQuestInfo);
	}
}

bool Quest::operator < (const Quest& rhs) const
{
	if (mEntry->m_nMinorType != rhs.getEntry()->m_nMinorType)
	{
		return mEntry->m_nMinorType < rhs.getEntry()->m_nMinorType;
	}

	return mQuestState > rhs.getQuestState();
}

int Quest::getCurrentValue()
{
	int tempResult = 0;
	for(size_t i = 0;i < mQuestConditionList.size();++i)
	{
		tempResult += mQuestConditionList[i]->getValue();
	}

	return tempResult;
}

int Quest::getAllNum()
{
	return (int)(mEntry->m_nTaskTargetNum * m_NumCoefficient);
}

void Quest::setFinish()
{
	for(size_t i = 0;i < mQuestConditionList.size();++i)
	{
		mQuestConditionList[i]->setFinish();
	}
	return ;
}




