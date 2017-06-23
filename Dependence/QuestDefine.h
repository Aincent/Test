/*
 * QuestDefine.h
 *
 *  Created on: 2013年10月30日
 *      Author: helh
 */

#ifndef QUESTDEFINE_H_
#define QUESTDEFINE_H_

#include "define.h"

using namespace CommBaseOut;

enum eTaskState
{
	eStateUnAccept=0,
	eStateDoing,
	eStateFinish,
	eStateUpate
};

// 当前进行中的任务
struct SActiveQuestData
{
	BYTE	bState;			//任务状态
	WORD	TargetInfo[2]; //目标
	WORD	wQuestID;		//任务ID

	SActiveQuestData()
	{
//		memset(this, 0, sizeof(*this));
	}
};

enum eTaskFirstType
{
	eLineTask=1,  //主线
	eBranchTask, //支线
	eDailyTask, //日常
	eMaxFirstType,
};

enum eTaskSecondType
{
	eKillMonsterCount=1, //杀怪计数
	eTalkTask, //对话
	eCollectTask, //收集
	eUseItem, //使用道具
	eLetterTask, //送信任务
	eKillMonsterCollect, //杀怪收集
	ePassInstance, //通关副本任务类型
	eNpcUpdateMonster, //NPC刷新怪物
	ePassSingleInstance,	//通过单人副本
	ePassTeamInstance,	//通关组队副本
	eEnterInstance,		//进入副本
	eMaxSecondType,
};

#define QUEST_MAX_COUNT 9600

#define NPC_PLAYER_DISTANCE 8


#endif /* QUESTDEFINE_H_ */
