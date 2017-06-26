/*
 * QuestGuide.cpp
 *
 *  Created on: 23/04/2015
 *      Author: root
 */
#include "QuestGuide.h"
#include "QuestLoader.h"
#include "../Object/Player/Player.h"
#include "Quest.h"
#include "QuestPart.h"
#include "../QuickMessage.h"



QuestGuide::QuestGuide(Player* owner) : m_Owner(owner),m_QuestID(0)
{

}

QuestGuide::~QuestGuide()
{

}

ServerRet QuestGuide::acceptQuest()
{
	int tempRet 	= eRetSuccess;
	DWORD tempQuest = getNextGuideQuestID();
	if(tempQuest == 0)
		return eGuideLevelNotEnough;

	m_Owner->GetQuestMgr()->accept(tempQuest,QuestState_Accepted,tempRet,eQuestAccept);

	if((ServerRet)tempRet != eRetSuccess)
		return (ServerRet)tempRet;

	m_QuestID = tempQuest;

	return (ServerRet)tempRet;
}

//玩家升级时是否要接收一个任务
bool QuestGuide::isUpgradeIsAcceptQuest()
{
	//玩家身上已有任务，则不升级时不用再检测
	if(m_Owner->GetQuestMgr()->findQuestByType(QuestType_Guide) != 0)
		return false;

	int tempError;
	return m_Owner->GetQuestMgr()->canAccept(getNextGuideQuestID(),tempError);
}

bool QuestGuide::onWrite(PlayerInfo::QuestInfo *info)
{
	info->set_questguideid(m_QuestID);

	return true;
}

void QuestGuide::onRead(const PlayerInfo::QuestInfo &info)
{
	m_QuestID = info.questguideid();

	return ;
}

void QuestGuide::updateToClient()
{
	DWORD tempQuestID    = m_Owner->GetQuestMgr()->findQuestByType(QuestType_Guide);
	DWORD tempAllNum     = 0;
	QuestState tempState = QuestState_Accepted;

	Quest* tempQuest = m_Owner->GetQuestMgr()->findQuest(tempQuestID);
	if(tempQuest != NULL)
	{
		tempState = tempQuest->getQuestState();
		const Entry_Quest* tempInfo = tempQuest->getEntry();
		if(tempInfo != NULL)
			tempAllNum = tempInfo->m_nTaskTargetNum;
	}

	QuestProto::UpdateMainTask tempClient;

	tempClient.set_state(tempState);
	tempClient.set_questid(tempQuestID);
	tempClient.set_svalue(tempAllNum);

	if(tempQuest != NULL)
	{
		if(tempQuest->getQuestState() == QuestState_Accepted)
		{
			const QuestCondition* tempCondition = tempQuest->getCondition(0);
			if(tempCondition != NULL)
				tempClient.set_fvalue(tempCondition->getValue());
		}
		else
		{
			tempClient.set_fvalue(tempAllNum);
		}
	}

	QuickMessage::GetInstance()->AddSendMessage(m_Owner->GetID(), m_Owner->GetChannelID(),&tempClient,MSG_SIM_GM2C_UPDATEGUIDE_TASK);

	return ;
}

DWORD QuestGuide::getNextGuideQuestID()
{
	//玩家身上有任务时m_Owner->GetQuestMgr()->findQuestByType(QuestType_Guide)返回的值必须和m_QuestID一样。
	const Entry_Quest* tempData = NULL;
	//1.第一次领取引导任务
	if(m_QuestID == 0)
	{
		return GUIDETASK_START;
	}

	//2.玩家不是第一次领任务
	DWORD tempQuestID = QuestLoader::GetInstance()->getNextQuestID(m_QuestID,QuestType_Guide);
	tempData 		  = QuestLoader::GetInstance()->GetQuestInfo(tempQuestID);
	if(tempData == NULL)
		return 0;

	return tempQuestID;
}
