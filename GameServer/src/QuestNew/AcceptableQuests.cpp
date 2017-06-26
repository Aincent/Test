/*
 * AcceptableQuests.cpp
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */


#include "AcceptableQuests.h"
#include "../Object/Player/Player.h"
#include "Log/Logger.h"
#include "QuestLoader.h"
#include "MessageCommonRet.h"


AcceptableQuests::AcceptableQuests(Player* owner)
	: mOwner(owner)
	,	mLastMainTaskID(MAINTASK_STARTID)
	, mIsDirty(false)
{
}

AcceptableQuests::~AcceptableQuests()
{
}

void AcceptableQuests::init()
{
	//makeAcceptableQuestList(); // 延迟到读出任务数据之后执行
}

void AcceptableQuests::uninit()
{
}

//void AcceptableQuests::makeAcceptableQuestList(bool hasMainQuest, int &error)
//{
//	QuestPart* part = (QuestPart*)mOwner->GetQuestMgr().Get();
//
//	clear();
//
//	bool needAddNextLevelMainQuest = true;
//
//	if (hasMainQuest)
//		needAddNextLevelMainQuest = false;
//
//	// 先找下一个主线任务
//	if(needAddNextLevelMainQuest)
//	{
//		Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(mLastMainTaskID+1);
//		if(!entry)
//		{
//			error = eAllMainTaskHadComp;
//			return;
//		}
//
//		if (entry->m_nMainType == QuestType_Main)
//		{
//			if (part->canAccept(entry->m_nQuestId, error))
//			{
//				add(entry->m_nQuestId);
//				needAddNextLevelMainQuest = false;
//			}
//		}
//	}

	// 再找支线任务（没跟前面的主线任务合一起，是为了排序好看点，因为有的主线任务配置在支线任务后面）
//	for (DWORD i=0; i<cnt; i++)
//	{
//
//		if (entry->Type == QuestType_Branch)
//		{
//			if (part->canAccept(entry->QuestId, false))
//			{
//				add(entry->m_nQuestId);
//				if (entry->m_nMainType == QuestType_Main)
//					needAddNextLevelMainQuest = false;
//			}
//		}
//	}
//}

void AcceptableQuests::add(uint questId)
{
	mAcceptableQuestList.push_back(questId);
	mIsDirty = true;
}

void AcceptableQuests::remove(uint questId)
{
	size_t old = mAcceptableQuestList.size();
	mAcceptableQuestList.remove(questId);
	if (old > mAcceptableQuestList.size())
		mIsDirty = true;
}

void AcceptableQuests::clear()
{
	mIsDirty = (mAcceptableQuestList.empty() == false);
	mAcceptableQuestList.clear();
}

void AcceptableQuests::updateToClient()
{
	//现在可接任务都不在客户端显示
	if (mIsDirty)
	{
		//if (mAcceptableQuestList.size() > 0)
		{
		}

		mIsDirty = false;
	}
}


