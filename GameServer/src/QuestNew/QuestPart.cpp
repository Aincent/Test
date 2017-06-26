/*
 * QuestPart.cpp
 *
 *  Created on: 2014年10月22日
 *      Author: root
 */

#include "QuestDefine.h"
#include "QuestPart.h"
#include "../Object/Player/Player.h"
#include "Log/Logger.h"
#include "../ServerEventDefine.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Map/SceneManager.h"
#include "../Map/GameZone.h"
#include "../Object/FileLoader/NPCLoader.h"
#include "../Achievement/Achievement.h"
#include "../Achievement/AchievementLoader.h"
#include "StatisticBase/StatisticDefine.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "../QuickMessage.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Attribute/AttributeCreature.h"
#include <math.h>
#include "../Fighting/Fighting.h"
#include "FileLoader/GameParaLoader.h"
#include "../Container/ParcelMgr.h"
#include "../Attribute/AttributeManage.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "Goods/MakeProp.h"
#include "FileLoader/InternationalLoader.h"
#include "../Mail/MailMessage.h"
#include "../StatisticMgr/StatisticHandle.h"

#define TASK_MAIL_CONTENT 40
#define TASK_MAIL_THEME   41


QuestPart::QuestPart(Player* owner)
:mOwner(owner)
,mAcceptedQuests(owner)
,mDailyQuests(NULL)
,mClanQuests(NULL)
,m_clanDoTime(0)
,m_dailyDoTime(0)
,m_CampDoTime(0)
,m_allMainTaskHadCompFlag(false)
{
	this->Init();
}

QuestPart::~QuestPart()
{
	this->RelaseQuestPart();
}

bool QuestPart::IsCanDailyQuest()
{
	return mOwner->IsOpenSystem(eStDailyQuest);
}

bool QuestPart::IsCanClanQuest()
{
	if(mOwner->getAttManage()->getValue<DWORD>(eCharFaction) >0 && mOwner->IsOpenSystem(eStClan))
		return true;
	return false;
}

bool QuestPart::isCanCampQuest()
{
	if(mOwner->IsOpenSystem(eStCountry) && mOwner->getAttManage()->getValue<DWORD>(eCharCountry) != 0)
		return true;
	return false;
}

bool QuestPart::IsNeedSelectDailyQuest(const BYTE& time)
{
	if(time >=MAX_DAILYTASK_NUM || !this->IsCanDailyQuest())
		return false;
	return true;
}

bool QuestPart::IsNeedSelectClanQuest(const BYTE& time)
{
	if(time >=MAX_CLANTASK_NUM || !this->IsCanClanQuest())
		return false;
	return true;
}

bool QuestPart::IsNeedSelectCampQuest(const BYTE time)
{
	if(time >= MAX_CAMPQUEST_NUM || !this->isCanCampQuest())
		return false;
	return true;
}

void QuestPart::getDesignationChapterAttList(int chapterID,map<ECharAttrType,int64>& attList)
{
//	sSectionReward* tempData = QuestLoader::GetInstance()->GetSectReward(chapterID);
//	if(tempData == NULL)
//		return ;
//
//	attList[eCharHPMax] 	   += tempData->m_nHp;
//	attList[eCharMPMax] 	   += tempData->m_nMp;
//	attList[eCharPhyAttackMax] += tempData->m_npAtt;
//	attList[eCharPhyDefence]   += tempData->m_npDefen;

	return ;
}

//删除多余任务品
void QuestPart::removeSurplusTaskItem()
{
	map<int,int> tempTaskItemList;
	getAllTaskItem(tempTaskItemList);

	map<int,int> tempPackageList;
	Smart_Ptr<ArticleBaseMgr> tempPackageBasePtr = mOwner->GetContainer(ePackageType);
	ArticleParcelMgr* tempPackagePtr 			 = static_cast<ArticleParcelMgr*>(tempPackageBasePtr.Get());
	tempPackagePtr->getAllTaskItem(tempTaskItemList);

	map<int,int>::iterator tempIt = tempTaskItemList.begin();
	for(;tempIt != tempTaskItemList.end();++tempIt)
	{
		if(tempIt->second <= 0)
		{
			continue;
		}

		BaseArgs tempDeleteItem;
		tempDeleteItem.GoodsID = tempIt->first;
		tempDeleteItem.Num 	   = tempIt->second;
		tempDeleteItem.Pos 	   = -1;

		mOwner->GetContainer(ePackageType)->DeleteGoods(&tempDeleteItem,false);

		LOG_ERROR(FILEINFO,"removeSurplusTaskItem Player %lld itemID %d number %d",mOwner->GetID(),tempDeleteItem.GoodsID,tempDeleteItem.Num);
	}

	return ;
}

//获得所有任务物品ID对应的个数
void QuestPart::getAllTaskItem(map<int,int>& mapList)
{
	Quest* tempDaily = mAcceptedQuests.findDailyQuest();
	if(tempDaily != NULL && mDailyQuests)
	{
		const Entry_Quest* tempQuest = tempDaily->getEntry();
		if(tempQuest->m_nMinorType == QuestCondType_RemoveItem)
		{
			int tempCounts = 0;
			//任务状态为QuestState_CanSubmit时，上线时是没有创建mQuestConditionList，所有任务品个数不能从getCurrentValue获取
			if(tempDaily->getQuestState() == QuestState_CanSubmit)
			{
				tempCounts = mDailyQuests->GetTargetNum();
			}
			else
			{
				tempCounts = tempDaily->getCurrentValue();
			}

			if(tempCounts > 0)
			{
				//包裹中加，这里减，得到差值大于0，则算是多余的
				mapList[tempQuest->m_nCollItemID] -= tempCounts;
			}
		}
	}

	Quest* tempClan  = mAcceptedQuests.findClanQuest();
	if(tempClan != NULL && mClanQuests)
	{
		const Entry_Quest* tempQuest = tempClan->getEntry();
		if(tempQuest->m_nMinorType == QuestCondType_RemoveItem)
		{
			int tempCounts = 0;
			//任务状态为QuestState_CanSubmit时，上线时是没有创建mQuestConditionList，所有任务品个数不能从getCurrentValue获取
			if(tempClan->getQuestState() == QuestState_CanSubmit)
			{
				tempCounts = mClanQuests->GetTargetNum();
			}
			else
			{
				tempCounts = tempClan->getCurrentValue();
			}

			if(tempCounts > 0)
			{
				mapList[tempQuest->m_nCollItemID] -= tempCounts;
			}
		}
	}

	Quest* tempCamp  = mAcceptedQuests.findCampQuest();
	if(tempCamp != NULL && m_CampQuest)
	{
		const Entry_Quest* tempQuest = tempCamp->getEntry();
		if(tempQuest->m_nMinorType == QuestCondType_RemoveItem)
		{
			int tempCounts = 0;
			//任务状态为QuestState_CanSubmit时，上线时是没有创建mQuestConditionList，所有任务品个数不能从getCurrentValue获取
			if(tempCamp->getQuestState() == QuestState_CanSubmit)
			{
				tempCounts = m_CampQuest->GetTargetNum();
			}
			else
			{
				tempCounts = tempCamp->getCurrentValue();
			}

			if(tempCounts > 0)
			{
				mapList[tempQuest->m_nCollItemID] -= tempCounts;
			}
		}
	}

	DWORD tempMainTaskID  = mAcceptedQuests.findMainQuest();
	Quest* tempMain       = findQuest(tempMainTaskID);
	if(tempMain != NULL)
	{
		const Entry_Quest* tempQuest = tempMain->getEntry();
		if(tempQuest->m_nMinorType == QuestCondType_SendMail || tempQuest->m_nMinorType == QuestCondType_UseItem)
		{
			mapList[tempQuest->m_nTaskTarget] -= 1;
		}
		else if(tempQuest->m_nMinorType == QuestCondType_RemoveItem || tempQuest->m_nMinorType == QuestCondType_Mining)
		{
			int tempCounts = 0;
			if(tempMain->getQuestState() == QuestState_CanSubmit)
			{
				tempCounts = tempQuest->m_nTaskTargetNum;
			}
			else
			{
				tempCounts = tempMain->getCurrentValue();
			}

			if(tempCounts > 0)
			{
				mapList[tempQuest->m_nCollItemID] -= tempCounts;
			}
		}
	}

	return ;
}

ServerRet QuestPart::updateWeekTask()
{
	return m_WeekQuest->updateTask();
}

bool QuestPart::Init()
{
	mAcceptedQuests.init();

	// 升级事件
	if(!m_stopLvEvent.IsValid())
		m_stopLvEvent = RegEvent(mOwner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &QuestPart::onLvEvent);//玩家升级

	// 离开帮派事件
	if(!m_stopEnterClanEvent.IsValid())
		m_stopEnterClanEvent = RegEvent(mOwner, PLAYER_CLAN_ENTERCLAN, this, &QuestPart::onEnterClanEvent);//加入帮派

	if(!m_stopLeaveClanEvent.IsValid())
		m_stopLeaveClanEvent = RegEvent(mOwner, PLAYER_CLAN_LEAVECLAN, this, &QuestPart::onLeaveClanEvent);//离开帮派

	if(!m_stopAcceptTaskEvent.IsValid())
		m_stopAcceptTaskEvent = RegEvent(mOwner, PLAYER_QUEST_ACCEPTQUEST, this, &QuestPart::onAcceptTask);//接受主线任务

	if(!m_stopClearMonsterEvent.IsValid())
		m_stopClearMonsterEvent = RegEvent(mOwner,PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &QuestPart::ClearMosterTask);

	if(!m_stopInitPackageEvent.IsValid())
		m_stopInitPackageEvent = RegEvent(mOwner, PLAYER_INIT_PACKAGEINFO, this, &QuestPart::onInitPackageInfo);

	if(!m_JoinCampEvent.IsValid())
		m_JoinCampEvent = RegEvent(mOwner,PLAYER_JOIN_CAMP,this,&QuestPart::onJoinCamp);		//玩家加入阵营

	return true;
}

void QuestPart::ReInit()
{
	m_clanDoTime = 0;
	m_dailyDoTime = 0;
	m_CampDoTime = 0;
	m_allMainTaskHadCompFlag = false;
	Init();
}

void QuestPart::RelaseQuestPart()
{
	//注销事件
	if(m_stopLvEvent.IsValid())
	{
		m_stopLvEvent.SetNull();
	}

	if(m_stopEnterClanEvent.IsValid())
	{
		m_stopEnterClanEvent.SetNull();
	}

	if(m_stopLeaveClanEvent.IsValid())
	{
		m_stopLeaveClanEvent.SetNull();
	}

	if(m_stopAcceptTaskEvent.IsValid())
	{
		m_stopAcceptTaskEvent.SetNull();
	}

	if(m_stopClearMonsterEvent.IsValid())
	{
		m_stopClearMonsterEvent.SetNull();
	}

	if(m_stopInitPackageEvent.IsValid())
	{
		m_stopInitPackageEvent.SetNull();
	}

	if(m_JoinCampEvent.IsValid())
	{
		m_JoinCampEvent.SetNull();
	}

	//清除各种任务列表
	mAcceptedQuests.uninit();

	if(mDailyQuests)
		mDailyQuests->uninit();

	if(mClanQuests)
		mClanQuests->uninit();

	mDailyQuests = NULL;
	mClanQuests = NULL;
	m_CampQuest = NULL;
	m_GuideQuest = NULL;
	m_WeekQuest = NULL;

}

bool QuestPart::onLvEvent(const EventArgs & e)
{
	//日常任务激活方式现在由等级来激活
	if(mAcceptedQuests.findDailyQuest() == NULL && IsNeedSelectDailyQuest(m_dailyDoTime))
	{
		mDailyQuests = new DailyQuests(mOwner);  /**/

		int error = 0;

		mDailyQuests->initDailyQuestList();

		mDailyQuests->setDirty(true);

		if(mDailyQuests->getQuestId() <=0)
			return true;   /*开启等级 于  表格 配的  任务 没有 匹配的 */

		accept(mDailyQuests->getQuestId(), QuestState_Accepted, error,eQuestAccept);

		mDailyQuests->updateToClient();
	}

	if(mOwner->IsOpenSystem(eStTaskWeek))
	{
		m_WeekQuest->levelUp(true);
	}

	onJoinCamp(EventArgs());

	if(m_GuideQuest->isUpgradeIsAcceptQuest())
	{
		m_GuideQuest->acceptQuest();
		m_GuideQuest->updateToClient();
	}

	//升级会影响支线任务,得删除超出等级的任务
//	mAcceptedQuests.DeleteBranchLoseTask();

	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

bool QuestPart::onEnterClanEvent(const EventArgs & e)
{
	OneParmaEventArgs *tArgs = (OneParmaEventArgs *)(&e);
	Smart_Ptr<Player> player = tArgs->m_obj;
	if(!player)
		return true;

	if(player->GetID() == mOwner->GetID())
	{
		BYTE clantm = this->GetClanDoTime();

		if(!mClanQuests && IsNeedSelectClanQuest(clantm))
		{
			int error = 0;
			mClanQuests = new QuestClan(mOwner);

			mClanQuests->setTimes(clantm);

			mClanQuests->selectQuest();

			mClanQuests->setDirty(true);

			accept(mClanQuests->getQuestId(), QuestState_Accepted, error,eQuestAccept);

			mClanQuests->updateToClient();
		}
		else
		{
			LOG_ERROR(FILEINFO, "jion clan ok, but quest error, id[%lld] ", GET_PLAYER_CHARID(player->GetID()) );
		}
	}
	return true;
}

bool QuestPart::onLeaveClanEvent(const EventArgs & e)
{
	PlayerLeaveClanArgs *tArgs = (PlayerLeaveClanArgs *)(&e);
	Smart_Ptr<Player> player = tArgs->m_obj;
	if(!player)
		return true;

	if(player->GetID() == mOwner->GetID())
	{
		//离开帮派时删除帮派任务
		Quest* q = mAcceptedQuests.findClanQuest();
		if(q)
		{
			uint questID = q->getEntry()->m_nQuestId;

			int tempCounts = q->getCurrentValue();
			ResetDeleteTaskItem(questID,tempCounts);
			this->DiscardClanQuest(questID);
		}

		/*西沟  */
		mClanQuests = NULL;
	}

	return true;
}

//bool QuestPart::onSumitTask(int questID)
//{
//	Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(questID);
//	Entry_Quest *nextEntry = 0;
//
//	if(entry->m_nextQuest > 0)
//		nextEntry = QuestLoader::GetInstance()->GetQuestInfo(entry->m_nextQuest);
//
//	if(nextEntry && mOwner->getAttManage()->getValue<int>(eCharLv) < nextEntry->m_clearLevel)
//	{
//		int error = 0;
//
//		accept(entry->m_nextQuest, QuestState_Accepted,error,eQuestAccept);
//		if(error)
//		{
//			LOG_ERROR(FILEINFO, "player[%lld] accept branch quest error[%d]", mOwner->GetID(), error);
//		}
//	}
//
//	return true;
//}

bool QuestPart::onAcceptTask(const EventArgs & e)
{
	PlayerAcceptQuestEventArgs* data = (PlayerAcceptQuestEventArgs*)(&e);
	if(!data)
	{
		return false;
	}

	Smart_Ptr<Player> player = data->m_obj;
	Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(data->m_questid);

	MonsterInfo monsterInfo;
	MonsterLoader::GetInstance()->GetMonsterInfo(entry->m_nTaskTarget, monsterInfo);
	if(entry && player)
	{
		if(entry->m_nMinorType == QuestCondType_NpcFreMonster)
		{
			int64 sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerCount(monsterInfo.mapId);
			if(sceneid > 0)
			{
				CSceneMgr::GetInstance()->GetGameZone(sceneid)->CreateMonsterDymic(entry->m_nTaskTarget, monsterInfo.born_x, monsterInfo.born_y, player);
			}
		}
		return true;
	}

	return false;
}

bool QuestPart::SubmitTaskRemoveItem(int questID)
{
	Entry_Quest *entry = QuestLoader::GetInstance()->GetQuestInfo(questID);
	if(entry == NULL)
		return false;

	Smart_Ptr<ArticleBaseMgr> pack = mOwner->GetContainer(ePackageType);
	if(!pack)
		return false;

	uint targetnum = entry->m_nTaskTargetNum;
	if(entry->m_nMainType == QuestType_Functional)
	{
		if(!mDailyQuests)
			return false;

		targetnum = mDailyQuests->GetTargetNum();
	}
	else if(entry->m_nMainType == QuestType_Clan)
	{
		if(!mClanQuests)
			return false;

		targetnum = mClanQuests->GetTargetNum();
	}
	else if(entry->m_nMainType == QuestType_Camp)
	{
		if(!m_CampQuest)
			return false;

		targetnum = m_CampQuest->GetTargetNum();
	}
	else if(entry->m_nMainType == QuestType_Week)
	{
		Quest* tQuestPtr = getAcceptedQuests().findQuest(questID);
		if(tQuestPtr != NULL && !tQuestPtr->canSubmit())
		{
			targetnum = tQuestPtr->getCurrentValue();
		}
	}


	int ret = 0;

	if(entry->m_nMinorType == QuestCondType_RemoveItem || entry->m_nMinorType == QuestCondType_Mining )
	{
		BaseArgs deletArg;

		deletArg.GoodsID = entry->m_nCollItemID;
		deletArg.Num = targetnum;
		deletArg.Pos = -1;

		ret = pack->DeleteGoods(&deletArg);
	}

	if(entry->m_nMinorType == QuestCondType_SendMail)
	{
		BaseArgs deletArg;

		deletArg.GoodsID = entry->m_nTaskTarget;
		deletArg.Num = 1;
		deletArg.Pos = -1;

		ret = pack->DeleteGoods(&deletArg);
	}

	if(0 != ret)
		return false;

//	Statistic_Quest(entry->m_nMainType);

	return true;
}

int QuestPart::IsCanAcceptMainLineQeust(const uint& questId)
{
	// 已接则不可再接
	const Quest* q = mAcceptedQuests.findQuest(questId);
	if(q)
		return eQuestAccepted;

	// 接受任务的条件判断
	const Entry_Quest* entry = Quest::getEntry(questId);
	if (!entry)
		return eQuestNotExist;

	return 0;
}

bool QuestPart::canAccept(uint questId, int &error) const
{
	if(questId == 0)
	{
		error = eQuestNotExist;
		return false;
	}

	// 已接则不可再接
	const Quest* q = mAcceptedQuests.findQuest(questId);
	if (q)
	{
		error = eQuestAccepted;
		return false;
	}

	// 接受任务的条件判断
	const Entry_Quest* entry = Quest::getEntry(questId);
	if (!entry)
	{
		error = eQuestNotExist;
		return false;
	}

	if(entry->m_nMainType == QuestType_Functional)  // 日常任务需要关心重复次数
	{
		if(!mDailyQuests)
			return false;

			if (mAcceptedQuests.findDailyQuest() &&
					(mAcceptedQuests.findDailyQuest()->getEntry()->m_nQuestId == mDailyQuests->getQuestId())) // 已经接了日常任务，每次只能一个
			{
				error = eHaveDailyQuest;
				return false;
			}


		if(!mDailyQuests->canAccept(entry,error))
		{
			return false;
		}
	}
	else if (entry->m_nMainType == QuestType_Clan)
	{
		if(!mClanQuests)
			return false;

		if(mAcceptedQuests.findClanQuest() &&
				(mAcceptedQuests.findClanQuest()->getEntry()->m_nQuestId == mClanQuests->getQuestId())) // 已经接了帮派任务，每次只能一个
		{
			error = eHaveClanTask;
			return false;
		}


		if (!mClanQuests->canAccept(entry,error))
			return false;
	}
	else if(entry->m_nMainType == QuestType_Camp)
	{
		if(!m_CampQuest)
			return false;

		 //已经接了帮派任务，每次只能一个
		if(mAcceptedQuests.findCampQuest() && mAcceptedQuests.findCampQuest()->getEntry()->m_nQuestId == m_CampQuest->getQuestId())
		{
			error = eHaveCampQuest;

			return false;
		}

		if (!m_CampQuest->canAccept(entry,error))
			return false;
	}
	else if(entry->m_nMainType == QuestType_Guide)
	{
		if(mOwner->getAttManage()->getValue<int>(eCharLv) < (int)entry->m_nLevelDown)
		{
			error = eGuideLevelNotEnough;

			return false;
		}
	}

	return true;
}

bool QuestPart::canSubmit(const uint &questId, bool useMoney) const
{
	if(questId == 0)
		return false;

	// 只对已接任务进行判断
	const Quest* q = mAcceptedQuests.findQuest(questId);
	if(q)
	{
		if(q->canSubmit())
			return true;

		if(useMoney)
			return true;
	}

	return false;
}

// 接任务
bool QuestPart::accept(const uint &questId, const QuestState &state, int &error,eOnLineOrAccept type)
{
//	if(mAcceptedQuests.getCount() > MAX_ACCEPTED_QUEST_NUM) // 现在定义超过了3个任务，就不允许接了(主线任务，帮派任务，日常任务)
//		return false;

	if(!canAccept(questId, error))
	{
		return false;
	}

	const Entry_Quest* entry = Quest::getEntry(questId);
	if(!entry)
	{
		return false;  //任务 ID不对
	}

	int numlevel = 1;
	int rewardlevel = 1;
	if(entry->m_nMainType == QuestType_Functional)
	{
		if(!mDailyQuests)
			return false;

		numlevel = mDailyQuests->getNumStarLevel();
		rewardlevel = mDailyQuests->getRewardStarLevel();
	}
	else if(entry->m_nMainType == QuestType_Clan)
	{
		if(!mClanQuests)
			return false;

		numlevel = mClanQuests->getNumStarLevel();
		rewardlevel = mClanQuests->getRewardStarLevel();
	}
	else if(entry->m_nMainType == QuestType_Camp)
	{
		if(!m_CampQuest)
			return false;

		numlevel 	= m_CampQuest->getNumStarLevel();
		rewardlevel = m_CampQuest->getRewardStarLevel();
	}

	Quest* q = mAcceptedQuests.addQuest(questId,state,type,numlevel,rewardlevel);
	if(!q)
	{
		return false;
	}

	if(mOwner->GetMyself())
	{
		PlayerAcceptQuestEventArgs e(mOwner->GetMyself(),questId);
		mOwner->FireEvent(PLAYER_QUEST_ACCEPTQUEST,e);
	}

	mOwner->SetDataFlag(eQuestInfo);

	return true;
}

// 交任务
int QuestPart::NewSubmitQeust(const uint &questId, Quest* q,int rewardIdx, bool byMail,ServerRet& error)
{
	int ret = 0;

	//计算  完成 奖励
	sGiveQuestRewardNeedData re_data;

	re_data.questId = questId;
	re_data.questType = q->getEntry()->m_nMainType;

	if(q->getEntry()->m_nMainType == QuestType_Functional)
	{
		if(!mDailyQuests)
			return eSystemError;

		re_data.RewardLevel = mDailyQuests->getRewardStarLevel();
	}
	else if(q->getEntry()->m_nMainType == QuestType_Clan)
	{
		if(!mClanQuests)
			return eSystemError;

		re_data.RewardLevel = mClanQuests->getRewardStarLevel();
	}
	else if(q->getEntry()->m_nMainType == QuestType_Camp)
	{
		if(!m_CampQuest)
			return eSystemError;

		re_data.RewardLevel = m_CampQuest->getRewardStarLevel();
	}
	else if(q->getEntry()->m_nMainType == QuestType_Guide)
	{
		re_data.RewardLevel = rewardIdx;
		re_data.byMail 	    = byMail;
	}
	else
	{
		re_data.RewardLevel = rewardIdx;
		re_data.byMail 		= byMail;
	}

	if(!isHaveEnoughPackage(*q->getEntry()))
		return ePackageFull;

	this->CalQuestSubmitReward(q, re_data);

	SubmitTaskRemoveItem(questId);

	// 发送 配置表 中 要 给予 的 任务物品
	this->acceptConfigNeedItemReward(questId);

	if(q->getEntry()->m_nMainType == QuestType_Functional)
	{
		error = (ServerRet)this->UpdateDailyQeust(questId);
	}
	else if(q->getEntry()->m_nMainType == QuestType_Clan)
	{
		error = (ServerRet)this->UpdateClanQeust(questId);
	}
	else if(q->getEntry()->m_nMainType == QuestType_Camp)
	{
		error = (ServerRet)this->UpdateCampQeust(questId);
	}
	else if(q->getEntry()->m_nMainType == QuestType_Guide)
	{
		error = (ServerRet)this->UpdateGuideQeust(questId);
	}
	else if(q->getEntry()->m_nMainType == QuestType_Main)
	{
		error = (ServerRet)this->UpdateMainLineQeust(questId);
//		if(!error || error == eAllMainTaskHadComp)
//		{
//			onSumitTask(questId);
//		}
	}
	else if(q->getEntry()->m_nMainType == QuestType_Week)
	{
		error = (ServerRet)getWeekTask()->onSubmit(questId);	
	}
	else
	{
		error = (ServerRet)UpdateBranchQeust(questId);
//		if(!error)
//		{
//			onSumitTask(questId);
//		}
	}

	//出发  提交 任务  事件
	if(mOwner->GetMyself())
	{
		PlayerAcceptQuestEventArgs e(mOwner->GetMyself(),questId);
		mOwner->FireEvent(PLAYER_QUEST_SUBMITQUEST, e);
	}

	mOwner->SetDataFlag(eQuestInfo);

	return ret;
}

int QuestPart::UpdateBranchQeust(const uint &questId)
{
	int error = 0;

	// 删除任务
	mAcceptedQuests.removeQuest(questId);

	mOwner->SetDataFlag(eQuestInfo);

	return error;
}

int QuestPart::UpdateMainLineQeust(const uint &questId)
{
	int error = 0;

	if(QuestLoader::GetInstance()->IsMaxMainquestID(questId+1))
	{
		/*通知  已完成所有主线任务*/
		this->AllMainTaskHadComp(questId);

		return eAllMainTaskHadComp;
	}

	//自动接取下一个主线任务
	error = IsCanAcceptMainLineQeust(questId+1);
	if(error != 0)
		return error;

	// 删除任务
	mAcceptedQuests.removeQuest(questId);

	accept(questId+1, QuestState_Accepted,error,eQuestAccept);

	mOwner->SetDataFlag(eQuestInfo);
	return error;
}

int QuestPart::UpdateDailyQeust(const uint &questId)
{
	if(!mDailyQuests)
		return eDailyTimesFull;

	int error = 0;
	if(mDailyQuests->onSubmit(questId))
	{
		accept(mDailyQuests->getQuestId(), QuestState_Accepted, error,eQuestAccept);
	}
	else
	{
		error = eDailyTimesFull;
	}

	// 删除任务
	mAcceptedQuests.removeQuest(questId);
	mOwner->SetDataFlag(eQuestInfo);

	return error;
}

int QuestPart::UpdateClanQeust(const uint &questId)
{
	if(!mClanQuests)
		return eClanTimesFull;

	int error = 0;
	if(mClanQuests->onSubmit(questId))
	{
		accept(mClanQuests->getQuestId(), QuestState_Accepted,	error,eQuestAccept);
	}
	else
	{
		error = eClanTimesFull;
	}

	// 删除任务
	mAcceptedQuests.removeQuest(questId);
	mOwner->SetDataFlag(eQuestInfo);
	return error;
}

ServerRet QuestPart::UpdateCampQeust(DWORD questId)
{
	int tempRet = eRetSuccess;

	if(!m_CampQuest)
		return (ServerRet)tempRet;

	if(m_CampQuest->onSubmit(questId))
	{
		accept(m_CampQuest->getQuestId(),QuestState_Accepted,tempRet,eQuestAccept);
	}
	else
	{
		tempRet = eAllCampTaskHadComp;
	}

	// 删除任务
	mAcceptedQuests.removeQuest(questId);
	mOwner->SetDataFlag(eQuestInfo);

	return (ServerRet)tempRet;
}

ServerRet QuestPart::UpdateGuideQeust(DWORD questId)
{
	// 删除任务
	mAcceptedQuests.removeQuest(questId);
	mOwner->SetDataFlag(eQuestInfo);
	return m_GuideQuest->acceptQuest();
}

int QuestPart::CalQuestSubmitReward(Quest* q, const sGiveQuestRewardNeedData &data)
{
	int error = 0;

	if(data.questType == QuestType_Functional)
	{
		if(!mDailyQuests)
		{
			return eSystemError;
		}

		error = mDailyQuests->GiveSubmitReward(data.RewardLevel);
		if(mDailyQuests->nextIsOver())
		{
			error = GiveCompleteDailyReward();
		}
	}
	else if(data.questType == QuestType_Clan)
	{
		if(!mClanQuests)
		{
			return eSystemError;
		}

		error = mClanQuests->GiveSubmitReward(data.RewardLevel);
		if(mClanQuests->nextIsOver())
		{
			//20环结束获取完成环数奖励
			error = this->GiveCompleteReward(QuestType_Clan);
		}
	}
	else if(data.questType == QuestType_Camp)
	{
		if(!m_CampQuest)
		{
			return eSystemError;
		}

		error = m_CampQuest->GiveSubmitReward(data.RewardLevel);
		if(m_CampQuest->nextIsOver())
		{
			//20环结束获取完成环数奖励
			error = GiveCompleteReward(QuestType_Camp);
		}
	}
	else if(data.questType == QuestType_Main)
	{
		error = this->GiveSubmitMainLineReward(q, data);
	}
	else if(data.questType == QuestType_Branch)
	{
		error = this->GiveSubmitMainLineReward(q, data);
	}
	else if(data.questType == QuestType_Guide)
	{
		error = this->GiveSubmitMainLineReward(q, data);
	}

	return error;
}

//判断包裹是否足够装下一个任务有所给予的物品，包括装励物品和下一个任务发送的物品(下一个任务物品是在前一个任务给的)
bool QuestPart::isHaveEnoughPackage(const Entry_Quest& data)
{
	std::vector<DWORD> tempItemList;
	std::vector<DWORD> tempNumList;


	if(data.m_nMainType == QuestType_Functional)
	{
		if(!mDailyQuests)
		{
			return false;
		}

		const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)data.m_nMainType,
				mOwner->getAttManage()->getValue<int>(eCharLv));
		if(tinfo)
		{
			tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList.begin(),tinfo->m_nTaskItemList.end());
			tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum.begin(),tinfo->m_nTaskItemListNum.end());

			if(mDailyQuests->nextIsOver())
			{
				tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList1.begin(),tinfo->m_nTaskItemList1.end());
				tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum1.begin(),tinfo->m_nTaskItemListNum1.end());
			}
		}
	}
	else if(data.m_nMainType == QuestType_Clan)
	{
		if(!mClanQuests)
		{
			return false;
		}

		const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)data.m_nMainType,
				mOwner->getAttManage()->getValue<int>(eCharLv));
		if(tinfo)
		{
			tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList.begin(),tinfo->m_nTaskItemList.end());
			tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum.begin(),tinfo->m_nTaskItemListNum.end());

			if(mClanQuests->nextIsOver())
			{
				tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList1.begin(),tinfo->m_nTaskItemList1.end());
				tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum1.begin(),tinfo->m_nTaskItemListNum1.end());
			}
		}
	}
	else if(data.m_nMainType == QuestType_Camp)
	{
		if(!m_CampQuest)
		{
			return false;
		}

		const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)data.m_nMainType,
				mOwner->getAttManage()->getValue<int>(eCharLv));
		if(tinfo)
		{
			tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList.begin(),tinfo->m_nTaskItemList.end());
			tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum.begin(),tinfo->m_nTaskItemListNum.end());
			if(m_CampQuest->nextIsOver())
			{
				tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList1.begin(),tinfo->m_nTaskItemList1.end());
				tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum1.begin(),tinfo->m_nTaskItemListNum1.end());
			}
		}
	}
	else if(data.m_nMainType == QuestType_Week)
	{
		if(!m_WeekQuest)
		{
			return false;
		}

		const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)data.m_nMainType,
				mOwner->getAttManage()->getValue<int>(eCharLv));
		if(tinfo)
		{
			tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList.begin(),tinfo->m_nTaskItemList.end());
			tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum.begin(),tinfo->m_nTaskItemListNum.end());
			if(m_WeekQuest->nextIsOver())
			{
				tempItemList.insert(tempItemList.end(),tinfo->m_nTaskItemList1.begin(),tinfo->m_nTaskItemList1.end());
				tempNumList.insert(tempNumList.end(),tinfo->m_nTaskItemListNum1.begin(),tinfo->m_nTaskItemListNum1.end());
			}
		}
	}
	else
	{
		tempItemList.insert(tempItemList.end(),data.m_nTaskItemList.begin(),data.m_nTaskItemList.end());
		tempNumList.insert(tempNumList.end(),data.m_nTaskItemListNum.begin(),data.m_nTaskItemListNum.end());

		for(size_t i=0; i<data.m_vAwardItemList.size(); i++)
		{
			tempItemList.push_back(data.m_vAwardItemList[i]);
			tempNumList.push_back(data.m_vAwardItemNum[i]);
		}

		//判断是否有武器奖励
		if(data.m_vEquipId.size() > 0)
		{
			int index = mOwner->GetSex() == false ? 0 : 3;
			index = index + mOwner->getAttManage()->getValue<BYTE>(eCharProfession)-1;

			if(index < (int)data.m_vEquipId.size() && index < (int)data.m_vEquipNum.size())
			{
				uint equipId = data.m_vEquipId[index];
				uint tempNum = data.m_vEquipNum[index];
				tempItemList.push_back(equipId);
				tempNumList.push_back(tempNum);
			}
		}
	}

	vector<WORD> tempFlagList;
	tempFlagList.resize(tempItemList.size(),1);

	return (mOwner->GetContainer(ePackageType)->IsBagEnought(tempItemList, tempNumList,tempFlagList) == 0);
}

int QuestPart::GiveSubmitMainLineReward(Quest* q, const sGiveQuestRewardNeedData &data)
{
	if(!q)
	{
		return eSystemError;
	}

	int error = 0;

	const Entry_Quest *tQuest = q->getEntry();
	if(tQuest == NULL)
	{
		return eSystemError;
	}
	error = q->submitReward(mOwner,tQuest->m_nQuestId, data.byMail);
	if(error != 0)
		return error;

	uint chap_num = -1;
	//是否章节发生变化
	if(!QuestLoader::GetInstance()->IsSameChap(q->getEntry()->m_nQuestId, &chap_num))
	{
		if(chap_num <=0)
			return eSystemError;

//		this->GetSectionReward(chap_num);

		//通知  章节 完成
		mOwner->SendToPlayerResultCodeMessage(eThisTaskChapOver);
		mOwner->SetDataFlag(eQuestInfo);
	}

	return error;
}

void QuestPart::AllMainTaskHadComp(const uint& questId)
{
	// 删除任务
	m_allMainTaskHadCompFlag = true;

	mAcceptedQuests.removeQuest(questId);

	this->SendAllMainTaskHadCompCmd();

	mOwner->SendToPlayerResultCodeMessage(eAllMainTaskHadComp);

	mOwner->SetDataFlag(eQuestInfo);
}

void QuestPart::SendAllMainTaskHadCompCmd()
{
	QuestProto::UpdateMainTask toClient;

	toClient.set_questid(0);
	toClient.set_fvalue(0);			//
	toClient.set_svalue(0);
	toClient.set_state(QuestState_CanAccept);

	QuickMessage::GetInstance()->AddSendMessage(mOwner->GetID(), mOwner->GetChannelID(), &toClient, MSG_SIM_GM2C_UPDATEMAINTASK);
}

DWORD QuestPart::findQuestByType(QuestType type)
{
	return mAcceptedQuests.findQuestByType(type);
}

Quest* QuestPart::findQuest(DWORD questID)
{
	return mAcceptedQuests.findQuest(questID);
}

ServerRet QuestPart::finishOneQuest(DWORD questID)
{
	Quest* tempQuest = mAcceptedQuests.findQuest(questID);
	if(tempQuest == NULL)
	{
		return eQuestNoAccepted;
	}

	const Entry_Quest* tempQuestData = tempQuest->getEntry();
	if(tempQuestData == NULL)
	{
		return eSystemError;
	}

	if(tempQuestData->m_nMainType == QuestType_Week)
	{
		return m_WeekQuest->finishOneTask(tempQuestData->m_nQuestId);
	}

	const OneFinishConsume* tempData = QuestLoader::GetInstance()->GetOneFinishDeduction((QuestType)tempQuestData->m_nMainType);
	if(tempData == NULL)
	{
		return eSystemError;
	}

	if(!mOwner->IsEnoughGolden(tempData->m_Golden))			//每一环为
		return eNotEnoughGold;

	tempQuest->setFinish();

	if(tempData->m_Golden > 0)
	{
		mOwner->ChangeGolden(tempData->m_Golden,false);
	}

	return eRetSuccess;
}

int QuestPart::acceptConfigNeedItemReward(const uint & questId)
{
	Entry_Quest* entry_ptr = QuestLoader::GetInstance()->GetQuestInfo(questId);
	if(!entry_ptr)
	{
		return eNull;
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = mOwner->GetContainer(ePackageType);
	if(!pPacket)
	{
		return eNull;
	}

	// 奖励物品
	if(entry_ptr->m_nTaskItemList.size() > 0)
	{
		vector<int> vecPos;
		vector<WORD> bindVec;

		bindVec.resize(entry_ptr->m_nTaskItemList.size(),1);

		//判断背包是否能领取
		if(pPacket->IsBagEnought(entry_ptr->m_nTaskItemList,entry_ptr->m_nTaskItemListNum,bindVec))
		{
			return ePackageFull;
		}

		vector<DWORD> strengthLvVec;

		strengthLvVec.resize(entry_ptr->m_nTaskItemList.size(), 0);

		pPacket->AddItem(entry_ptr->m_nTaskItemList, entry_ptr->m_nTaskItemListNum, vecPos, bindVec,strengthLvVec,true);
	}

	return 0;
}

// 交任务
int QuestPart::submit(const uint &questId, uint *questType,ServerRet& error, bool useMoney, int rewardIdx, bool byMail)
{
	LOG_DEBUG(FILEINFO,"Player %lld Submit Task %d",mOwner->GetID(),questId);

	Quest* q = mAcceptedQuests.findQuest(questId);
	if (!q)
		return eQuestNoAccepted;

	const Entry_Quest* entry = Quest::getEntry(questId);
	if(!entry)
	{
		return eSystemError;
	}

	*questType = entry->m_nMainType;

	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(mOwner->GetMapID());
	if(!zone)
	{
		LOG_ERROR(FILEINFO, "client submit quest but scene is not existed");

		return eRetZoneNotExist;
	}

	Smart_Ptr<NPC> npc = zone->GetNpcByID(entry->m_nSubmitNpcId);
	if(npc)
	{
		int res = NPCInfo::GetInstance()->ExamineDistance(entry->m_nSubmitNpcId, mOwner->GetXNewPos(), mOwner->GetYNewPos());
		if(res)
		{
			return res;
		}
	}

	if(!canSubmit(questId, useMoney))
		return eQuestNoFinished;

	BEHAVIOR_MONI(mOwner->GetMyself(), BehaviorType::Main::eTask, BehaviorType::Minor::eTask_Submit, MONITOR);

	int res = this->NewSubmitQeust(questId, q, rewardIdx, byMail,error);

	

	return res;
}


// 没有 帮派任务
void QuestPart::DiscardClanQuest(uint& questId)
{
	mAcceptedQuests.removeQuest(questId);

	QuestProto::UpdateClanTask info;

	info.set_questid(0);
	QuickMessage::GetInstance()->AddSendMessage(mOwner->GetID(), mOwner->GetChannelID(),&info, MSG_SIM_GM2C_UPDATECLANTASK);

	mOwner->SetDataFlag(eQuestInfo);
}

/// 日常任务升星, isFull表明是否直接升级到最高级
//int QuestPart::upgradeQuestsStarLevel(uint questId, int type)
//{
//	int error = 0;
//	Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(questId);
//	if(!info)
//	{
//		return error = eQuestNotExist;
//	}
//
//	bool isFull = false;
//	if(type == 0)
//	{
//		isFull = true;
//	}
//
//	if(info->m_nMainType == QuestType_Functional)
//	{
//		Quest* q = mAcceptedQuests.findDailyQuest();
//		if(q)
//		{
//			if(!mDailyQuests)
//				return eSystemError;
//
//			//当前个数
//			int tempCounts = q->getCurrentValue();
//
//			error = mDailyQuests->refreshStarLevel(isFull, type);
//
//			if(error == 0)
//			{
//				q->setNumStar(mDailyQuests->getNumStarLevel());
//				q->setRewardStar(mDailyQuests->getRewardStarLevel());
//				q->setCoefficient(mDailyQuests->getCoefficient());
//				q->updateCondition();
//
//				int tempMaxCounts = mDailyQuests->GetTargetNum();
//				if(tempCounts > tempMaxCounts)
//					ResetDeleteTaskItem(questId,tempCounts - tempMaxCounts);
//
//				mOwner->SetDataFlag(eQuestInfo);
//			}
//		}
//	}
//	else if(info->m_nMainType == QuestType_Clan)
//	{
//		Quest* q = mAcceptedQuests.findClanQuest();
//		if(q)
//		{
//			if(!mClanQuests)
//				return eSystemError;
//
//			error = mClanQuests->refreshStarLevel(isFull, type);
//
//			//当前个数
//			int tempCounts = q->getCurrentValue();
//
//			if(error == 0)
//			{
//				q->setNumStar(mClanQuests->getNumStarLevel());
//				q->setRewardStar(mClanQuests->getRewardStarLevel());
//				q->setCoefficient(mClanQuests->getCoefficient());
//				q->updateCondition();
//
//				int tempMaxCounts = mClanQuests->GetTargetNum();
//				if(tempCounts > tempMaxCounts)
//					ResetDeleteTaskItem(questId,tempCounts - tempMaxCounts);
//
//				mOwner->SetDataFlag(eQuestInfo);
//			}
//		}
//	}
//	else if(info->m_nMainType == QuestType_Camp)
//	{
//		Quest* tempQuest = mAcceptedQuests.findCampQuest();
//
//		if(tempQuest == NULL)
//			return eSystemError;
//
//		if(!m_CampQuest)
//			return eSystemError;
//
//		error = m_CampQuest->refreshStarLevel(isFull, type);
//
//		if(error != eRetSuccess)
//			return error;
//
//		//当前个数
//		int tempCounts = tempQuest->getCurrentValue();
//
//		tempQuest->setNumStar(m_CampQuest->getNumStarLevel());
//		tempQuest->setRewardStar(m_CampQuest->getRewardStarLevel());
//		tempQuest->setCoefficient(m_CampQuest->getCoefficient());
//		tempQuest->updateCondition();
//
//		int tempMaxCounts = m_CampQuest->GetTargetNum();
//		if(tempCounts > tempMaxCounts)
//			ResetDeleteTaskItem(questId,tempCounts - tempMaxCounts);
//
//		mOwner->SetDataFlag(eQuestInfo);
//	}
//
//	return error;
//}

bool QuestPart::ResetDeleteTaskItem(const uint& questID, const int& condtionNum,bool isSend)
{
	const Entry_Quest* entry_ptr = Quest::getEntry(questID);
	if(!entry_ptr || condtionNum<=0)
	{
		return false;
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = mOwner->GetContainer(ePackageType);
	if(!pPacket)
		return false;

	int ret = 0;

	if(entry_ptr->m_nMinorType == QuestCondType_RemoveItem || entry_ptr->m_nMinorType == QuestCondType_Mining )
	{
		BaseArgs deletArg;

		deletArg.GoodsID = entry_ptr->m_nCollItemID;
		deletArg.Num = condtionNum;
		deletArg.Pos = -1;

		ret = pPacket->DeleteGoods(&deletArg,isSend);
	}

	if(entry_ptr->m_nMinorType == QuestCondType_SendMail)
	{
		BaseArgs deletArg;

		deletArg.GoodsID = entry_ptr->m_nTaskTarget;
		deletArg.Num = 1;
		deletArg.Pos = -1;

		ret = pPacket->DeleteGoods(&deletArg,isSend);
	}

	if(0 != ret)
		return false;

	return true;
}

void QuestPart::ResetQuest()
{
	//到 点  重置 在线玩家 任务 信息步奏
	//先  删除 ( 1物品 2任务)---重置  ----- 再 加入已接列表
	int error = 0;
	Quest_CondStateInfo condInfo;

	if(mDailyQuests)
	{
		mDailyQuests->GetDailyQuestConditionStateInfo(&condInfo);

		if(condInfo.questID >0)
		{
			this->ResetDeleteTaskItem(condInfo.questID, condInfo.nowNum);

			mAcceptedQuests.removeQuest(condInfo.questID);
		}

		mDailyQuests->reset();

		accept(mDailyQuests->getQuestId(), QuestState_Accepted, error,eQuestAccept);
	}

	if(mClanQuests)
	{
		mClanQuests->GetClanQuestConditionStateInfo(&condInfo);
		if(condInfo.questID >0)
		{
			this->ResetDeleteTaskItem(condInfo.questID, condInfo.nowNum);

			mAcceptedQuests.removeQuest(condInfo.questID );
		}

		mClanQuests->reset();

		accept(mClanQuests->getQuestId(), QuestState_Accepted,	error,eQuestAccept);
	}

	if(m_CampQuest)
	{
		m_CampQuest->GetCampQuestConditionStateInfo(&condInfo);
		if(condInfo.questID > 0)
		{
			this->ResetDeleteTaskItem(condInfo.questID, condInfo.nowNum);

			mAcceptedQuests.removeQuest(condInfo.questID );
		}

		m_CampQuest->reset();

		accept(m_CampQuest->getQuestId(), QuestState_Accepted,	error,eQuestAccept);
	}

	if(mDailyQuests)
	{
		mDailyQuests->setDirty(true);

		mDailyQuests->updateToClient();
	}

	if(mClanQuests)
	{
		mClanQuests->setDirty(true);

		mClanQuests->updateToClient();
	}

	if(m_CampQuest)
	{
		m_CampQuest->updateToClient();
	}

	m_WeekQuest->resetTask();
	m_WeekQuest->updateToClient();

	mOwner->SetDataFlag(eQuestInfo);
}

void QuestPart::InitQuestInfoFromDB(const PlayerInfo::QuestInfo &info)
{
	int needNum = 0;
	/* 1。判定 玩家 是否 有 日常 帮派任务
	 *  如 有 超过 1天 重置 ----  重置  步奏 a 保存 上一次 正在 做的任务 完成次数 --以免是 收集物品类的 任务 需要 做删除操作(由于 背包信息 是分步初始化 故才 保存)
	 *  																		可以 查看  bool onInitPackageInfo(const EventArgs &e);
	 *                          b 保存 资源 找回相关
	 *                          	c  重置 相关类型 的 任务
	 *               在同一天内 －－－－－正常 读取  操作
	 *               注意点 :  由于  帮派可能 同一天 内 多次 操作 所以 要保存 完成次数  －－－－m_clanDoTime
	 *                              为了 与 帮派 一致  故 加入 的  m_dailyDoTime
	 *                             为了 标识   玩家 所有 主线 任务 都 已经 完成  ----- m_allMainTaskHadCompFlag
	 *								由于 之前的 任务 数据 错误  为了 包容 之前的 引入 InitRightQuestData() 现在  可以去除
	 *		最后 就是 通知 客户端 相关任务信息  		:setDirty(true);  与 updateToClient();.....要一起 使用  现在 我也明白 为什么 要 引入 标识位 无关 痛痒 就 没改
	 * */

	m_allMainTaskHadCompFlag = info.allmaintaskhadcompflag();
	if(!m_allMainTaskHadCompFlag)
		needNum++;

	if(IsCanDailyQuest())
	{
		mDailyQuests = new DailyQuests(mOwner);
		needNum++;
	}

	mClanQuests = new QuestClan(mOwner);
	needNum++;

	if(isCanCampQuest())
	{
		m_CampQuest = new CampQuest(*mOwner);
		++needNum;
	}

	m_GuideQuest = new QuestGuide(mOwner);
	++needNum;

	m_WeekQuest = new QuestWeek(*mOwner);

	if(mOwner->GetCounterService()->getResetDayType() == eDayResetToday)
	{
		BYTE doclantm     = info.doclantime();
		BYTE dailytime    = info.dailytime();
		BYTE tempCampTime = info.docamptime();

		this->SetClanDoTime(doclantm);
		this->SetDailyDoTime(dailytime);
		this->setCampDoTimes(tempCampTime);

		if(mDailyQuests)
		{
			mDailyQuests->onRead(info);
		}

		if(mClanQuests)
		{
			mClanQuests->onRead(info);
		}

		if(m_CampQuest)
		{
			m_CampQuest->onRead(info);
		}

		m_WeekQuest->onRead(info);

		//不同一天引导任务也要读
		m_GuideQuest->onRead(info);

		mAcceptedQuests.onRead(info, true); //同一天
	}
	else
	{
		if(mDailyQuests)
		{
			mDailyQuests->InitLastDailyQuest(info);

			mDailyQuests->InitYesterdayDailyQuest(info);

			mDailyQuests->reset();
		}

		if(mClanQuests)
		{
			mClanQuests->InitLastDailyQuest(info);

			mClanQuests->InitYesterdayClanQuest(info);

			mClanQuests->reset();
		}

		if(m_CampQuest)
		{
			m_CampQuest->InitLastDailyQuest(info);

			m_CampQuest->InitYesterdayDailyQuest(info);

			m_CampQuest->reset();
		}

		m_GuideQuest->onRead(info);
		mAcceptedQuests.onRead(info, false);

		m_WeekQuest->resetTask();
	}

	//InitRightQuestData用于第二天重置时，重新设置一次今天刷的任务
	if(info.active_size() != needNum)
	{
		this->InitRightQuestData();
	}
	mOwner->SetDataFlag(eQuestInfo);
}

//上线计算属性
void QuestPart::onlineCalculateAtt()
{
	DWORD tempQuestID = 0;
	if(m_allMainTaskHadCompFlag)
	{
		tempQuestID = QuestLoader::GetInstance()->getMaxMainQuestID();
	}
	else
	{
		tempQuestID = findQuestByType(QuestType_Main);
	}

//	map<int,int> tempChapterList;
//
//	QuestLoader::GetInstance()->getBeforeChapterIDList(tempQuestID,tempChapterList);
//
//	map<ECharAttrType,int64> tempAttList;
//	map<int,int>::iterator tempIt = tempChapterList.begin();
//
//	for(;tempIt != tempChapterList.end();++tempIt)
//	{
//		getDesignationChapterAttList(tempIt->first,tempAttList);
//	}
//
//	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttQust);
//
//	mOwner->calculateResultAddAtt(tempAttList,tempValueType);
//
//	calculateFight();

	//如果还不能做该任务，在这里要再次清空，因为代码中很多地方用mClanQuests是否为空做过很多判断，如：第二天上线时如果mClanQuests不为空就重置任务，而该玩家还没有该任务
	if(!IsCanClanQuest())
	{
		Quest* q = mAcceptedQuests.findClanQuest();
		if(q)
		{
			mAcceptedQuests.removeQuest(q->getEntry()->m_nQuestId);
		}
		mClanQuests = NULL;
	}
	//今天次数没有完成，并且又没有帮派任务的
	else if(m_clanDoTime < MAX_CLANTASK_NUM && mClanQuests->getQuestId() <= 0)
	{
		mClanQuests->setTimes(m_clanDoTime);
		mClanQuests->selectQuest();

		InitRightQuestData();
	}

	//删除多余任务品
	removeSurplusTaskItem();

	return ;
}

//计算一次战斗力
int QuestPart::calculateFight()
{
	DWORD tempQuestID = 0;
	if(m_allMainTaskHadCompFlag)
	{
		tempQuestID = QuestLoader::GetInstance()->getMaxMainQuestID();
	}
	else
	{
		tempQuestID = findQuestByType(QuestType_Main);
	}

	map<int,int> tempChapterList;

	QuestLoader::GetInstance()->getBeforeChapterIDList(tempQuestID,tempChapterList);

	map<ECharAttrType,int64> tempAttList;
	map<int,int>::iterator tempIt = tempChapterList.begin();

	for(;tempIt != tempChapterList.end();++tempIt)
	{
		getDesignationChapterAttList(tempIt->first,tempAttList);
	}

	int tempResult = mOwner->getFightingManage()->CalculateFighting(tempAttList);
	return mOwner->getFightingManage()->setFighting(eFightingQuest,tempResult);
}

//玩家信息  初始化 完成 之后 主动 推送 主线  日常  帮派任务
void QuestPart::FillInQuestInfoCmd()
{
	if(!m_allMainTaskHadCompFlag)
	{
		mAcceptedQuests.setDirty(true);

		mAcceptedQuests.updateToClient();
	}
	else
	{
		//发送 主线 任务 完成 信息
		this->SendAllMainTaskHadCompCmd();
	}

	if(mDailyQuests)
	{
		mDailyQuests->setDirty(true);
		mDailyQuests->updateToClient();
	}

	if(mClanQuests)
	{
		mClanQuests->setDirty(true);
		mClanQuests->updateToClient();
	}

	if(m_CampQuest)
	{
		m_CampQuest->updateToClient();
	}

	if(m_GuideQuest)
	{
		m_GuideQuest->updateToClient();
	}
	
	if(mAcceptedQuests.findQuestByType(QuestType_Week) != 0)
	{
		m_WeekQuest->updateToClient();
	}

	mOwner->SetDataFlag(eQuestInfo);

	return ;
}

void QuestPart::InitRightQuestData()
{
	bool flag = false;
	Quest* q = NULL;

	q = mAcceptedQuests.findDailyQuest();
	if(mDailyQuests && mDailyQuests->getQuestId() && !q)
	{
		PlayerInfo::ActiveQuest info;

		info.set_id(mDailyQuests->getQuestId());

		mAcceptedQuests.InitAccptedQuest(info, flag);
	}

	q	= mAcceptedQuests.findClanQuest();
	if(mClanQuests && mClanQuests->getQuestId() && !q)
	{
		PlayerInfo::ActiveQuest info;

		info.set_id(mClanQuests->getQuestId());

		mAcceptedQuests.InitAccptedQuest(info, flag);
	}

	q = mAcceptedQuests.findCampQuest();
	if(m_CampQuest && m_CampQuest->getQuestId() && q == NULL)
	{
		PlayerInfo::ActiveQuest tempInfo;

		tempInfo.set_id(m_CampQuest->getQuestId());
		mAcceptedQuests.InitAccptedQuest(tempInfo,flag);
	}
	uint tQuestID = mAcceptedQuests.findMainQuest();
	if(tQuestID == 0)
	{
		tQuestID = QuestLoader::GetInstance()->getMainTask(mOwner->getAttManage()->getValue<int>(eCharLv));
		PlayerInfo::ActiveQuest tempInfo;
		tempInfo.set_id(tQuestID);
		tempInfo.set_state(QuestState_Accepted);
		tempInfo.set_ftarget(0);
		mAcceptedQuests.InitAccptedQuest(tempInfo,flag);
	}
	mOwner->SetDataFlag(eQuestInfo);
}

void QuestPart::SetQuestInfoToDB(PlayerInfo::QuestInfo *info)
{
	BYTE clantm   	   = this->GetClanDoTime();
	BYTE dailytm  	   = this->GetDailyDoTime();
	BYTE tempCampTimes = GetCampDoTimes();

	info->set_doclantime(clantm);
	info->set_dailytime(dailytm);
	info->set_docamptime(tempCampTimes);

	info->set_allmaintaskhadcompflag(m_allMainTaskHadCompFlag);

	mAcceptedQuests.onWrite(info, m_allMainTaskHadCompFlag);

	if(mDailyQuests)
	{
		mDailyQuests->onWrite(info);
	}

	if(mClanQuests)
	{
		mClanQuests->onWrite(info);
	}

	if(m_CampQuest)
	{
		m_CampQuest->onWrite(info);
	}

	if(m_GuideQuest)
	{
		m_GuideQuest->onWrite(info);
	}

	m_WeekQuest->onWrite(info);

	return ;
}

int QuestPart::oneKeyComplete(const uint& questid)
{
	int error = 0;
	int times = 0;
	int rewardTime = 0;
	vector<int> vecType;

	Quest* q = mAcceptedQuests.findQuest(questid);
	if(!q)
		return eQuestNoAccepted;

	Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(questid);
	if(!info)
		return error = eQuestNotExist;

	if(info->m_nMainType == QuestType_Functional)
	{
		if(!mDailyQuests)
			return eSystemError;

		// 判断有没有VIP 一键完成权限
		if (VIP_NOT_HAVE_THIS_RIGHT == mOwner->GetVipPart()->GetValueByRight(eVipDailyTaskImmediate))
		{  // 有权限
			return eNotOneKeyComplete;
		}


		times = mDailyQuests->getTimes();
		rewardTime = mDailyQuests->getMaxTimes() - times;
	}
	else if(info->m_nMainType == QuestType_Clan)
	{
		if(!mClanQuests)
			return eSystemError;

		// 判断有没有VIP 一键完成权限
		if (VIP_NOT_HAVE_THIS_RIGHT == mOwner->GetVipPart()->GetValueByRight(eVipClanTaskImmediate))
		{  // 有权限
			return eNotOneKeyComplete;
		}

		times = mClanQuests->getTimes();
		rewardTime = mClanQuests->getMaxTimes() - times;
	}
	else if(info->m_nMainType == QuestType_Camp)
	{
		if(!m_CampQuest)
			return eSystemError;

		// 判断有没有VIP 一键完成权限
		if (VIP_NOT_HAVE_THIS_RIGHT == mOwner->GetVipPart()->GetValueByRight(eVipCampTaskImmediate))
		{  // 有权限
			return eNotOneKeyComplete;
		}

		times 	   = m_CampQuest->getTimes();
		rewardTime = m_CampQuest->getMaxTimes() - times;
	}
	else if(info->m_nMainType == QuestType_Week)
	{
		times = m_WeekQuest->getCurTimes() - 1;	
	}
	else
	{
		return eClientSendDataError;
	}

	int quest_maxNum = 0;

	if(info->m_nMainType == QuestType_Functional )
		quest_maxNum =  MAX_DAILYTASK_NUM;
	else if(info->m_nMainType == QuestType_Clan)
		quest_maxNum =  MAX_CLANTASK_NUM;
	else if(info->m_nMainType == QuestType_Camp)
		quest_maxNum = MAX_CAMPQUEST_NUM;
	else 
		quest_maxNum = MAX_WEEKTASK_NUM;

	if(times >= quest_maxNum)
		return eQuestFinished;

	times = quest_maxNum - times;
	if(times <=0)
		return eSystemError;

	int needGold = 0; // 需要付出的元宝

	 // 没权限
	int singleCost = QuestLoader::GetInstance()->GetOneFinishSginleCost((QuestType)info->m_nMainType);
	if (singleCost < 0)
	{
		return eSystemError;
	}
	needGold = times * singleCost;

	if(info->m_nMainType == QuestType_Functional)
	{
		if(mOwner->getAttManage()->getValue<int>(eCharGolden) < needGold) 
			return eNotEnoughGold;
	}
	else
	{
		if(!mOwner->IsEnoughGolden(needGold))			//每一环为
			return eNotEnoughGold;
	}

	int retcode = 0;
	int ret = 0;
	if(info->m_nMainType == QuestType_Functional)
	{
		retcode = eAllDailyTaskHadComp;

		ret = this->GiveCompleteDailyReward();
		if(ret != 0)
			return ret;

		//把剩下的 所有 都一 最大值给予 奖励
		for(int i=0; i<rewardTime; ++i)
		{
			ret = mDailyQuests->GiveSubmitReward(Max_Reward_Start);

			//完成活跃次数
//			mOwner->GetVitalityMgr()->FinshVitality(eViDailyQuest);

			if(ret != 0)
				return ret;
		}

		ResetDeleteTaskItem(questid,q->getCurrentValue());

		mAcceptedQuests.removeQuest(mDailyQuests->getQuestId());

		mDailyQuests->IncreaseDoMaxTime();

		mDailyQuests->updateToClient();
	}
	else if(info->m_nMainType == QuestType_Camp)
	{
		retcode = eAllCampTaskHadComp;

		ret = GiveCompleteReward(info->m_nMainType);

		if(ret != eRetSuccess)
			return ret;

		for(int i = 0;i < rewardTime;++i)
		{
			ret = m_CampQuest->GiveSubmitReward(Max_Reward_Start);

			if(ret != eRetSuccess)
				return ret;
		}

		ResetDeleteTaskItem(questid,q->getCurrentValue());
		mAcceptedQuests.removeQuest(m_CampQuest->getQuestId());
		m_CampQuest->IncreaseDoMaxTime();
		m_CampQuest->updateToClient();
	}
	else if(info->m_nMainType == QuestType_Clan)
	{
		retcode = eAllClanTaskHadComp;

		ret = this->GiveCompleteReward(info->m_nMainType);
		if(ret != eRetSuccess)
			return ret;

		for(int i=0; i<rewardTime; ++i)
		{
			ret = mClanQuests->GiveSubmitReward(Max_Reward_Start);
			if(ret != 0)
				return ret;
		}

		ResetDeleteTaskItem(questid,q->getCurrentValue());
		mAcceptedQuests.removeQuest(mClanQuests->getQuestId());

		mClanQuests->IncreaseDoMaxTime();

		mClanQuests->updateToClient();

	}
	else if(info->m_nMainType == QuestType_Week)
	{
		m_WeekQuest->oneComplete(questid);		
	}

	OneKeyCompleteArgs e(mOwner->GetMyself(),questid,times);
	mOwner->FireEvent(PLAYER_ONE_KEY_COMPLETE,e);

	if (0 != needGold)
	{
		//扣除元宝
		int tempGolden 	   = 0;
		int tempBindGolden = 0;


		if(info->m_nMainType != QuestType_Functional)
		{
			mOwner->DecreaseGoldenCost(needGold,tempGolden,tempBindGolden);		
		}
		else
		{
			mOwner->ChangeGolden(needGold, false);
			tempGolden = needGold;
		}

		if(tempGolden > 0)
			vecType.push_back(eCharGolden);

		if(tempBindGolden > 0)
			vecType.push_back(eCharBindGolden);

		if(vecType.size() > 0)
			mOwner->SynCharAttribute(vecType);
	}

	/*通知  已完成今日 相关任务*/
	mOwner->SendToPlayerResultCodeMessage(retcode);

	mOwner->SetDataFlag(eBaseInfo);
	mOwner->SetDataFlag(eQuestInfo);


	return 0;
}

int QuestPart::GiveCompleteDailyReward()
{
	vector<DWORD> tItemList;
	vector<DWORD> tItemNum;
	vector<WORD>  tBindTypeList;

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
	// 奖励物品
	vector<int> vecPos;

	Smart_Ptr<ArticleBaseMgr> pPacket = mOwner->GetContainer(ePackageType);
	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo(QuestType_Functional,mOwner->getAttManage()->getValue<int>(eCharLv));
	if(!pPacket || !tinfo)
	{
		return eNull;
	}

	//判断背包是否能领取
	if(tinfo->m_nrewardMoney1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeMoneyItemId());
		tItemNum.push_back(tinfo->m_nrewardMoney1);
	}

	if(tinfo->m_nrewardExp1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeExpItemId());
		tItemNum.push_back(tinfo->m_nrewardExp1);
	}

	if(tinfo->m_nrewardForce1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeForceItemId());
		tItemNum.push_back(tinfo->m_nrewardForce1);
	}

	if(tinfo->m_nClanContribution1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeContributionItemId());
		tItemNum.push_back(tinfo->m_nClanContribution1);
	}

	if(tinfo->m_nCampContribution1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeCountryContributionItemId());
		tItemNum.push_back(tinfo->m_nCampContribution1);
	}

	tItemList.insert(tItemList.end(),tinfo->m_nTaskItemList1.begin(),tinfo->m_nTaskItemList1.end());
	tItemNum.insert(tItemNum.end(),tinfo->m_nTaskItemListNum1.begin(),tinfo->m_nTaskItemListNum1.end());

	tBindTypeList.resize(tItemList.size(),1);

	if(pPacket->IsBagEnought(tItemList,tItemNum,tBindTypeList))
	{
		return ePackageFull;
	}

	vector<DWORD> strengthLvVec;
	strengthLvVec.resize(tItemList.size(), 0);

	pPacket->AddItem(tItemList,tItemNum, vecPos, tBindTypeList,strengthLvVec,true);

//	for(size_t i = 0;i < tItemList.size();++i)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,tItemList[i],tItemNum[i]);
//	}
//
//	if(costAndGotInfo.gotlist_size() > 0 && eAccountType_User == mOwner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(mOwner->GetID(),eStatic_Task,eStaMinor_Task_GetReward,&costAndGotInfo);

	return 0;
}

int QuestPart::GiveCompleteReward(int type)
{	
	vector<DWORD> tItemList;
	vector<DWORD> tItemNum;
	vector<WORD>  tBindTypeList;

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
	// 奖励物品
	vector<int> vecPos;

	Smart_Ptr<ArticleBaseMgr> pPacket = mOwner->GetContainer(ePackageType);
	const DailyTaskReward *tinfo = QuestLoader::GetInstance()->GetTwentyInfo((QuestType)type,mOwner->getAttManage()->getValue<int>(eCharLv));
	if(!pPacket || !tinfo)
	{
		return eNull;
	}

	//判断背包是否能领取
	if(tinfo->m_nrewardMoney1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeMoneyItemId());
		tItemNum.push_back(tinfo->m_nrewardMoney1);
	}

	if(tinfo->m_nrewardExp1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeExpItemId());
		tItemNum.push_back(tinfo->m_nrewardExp1);
	}

	if(tinfo->m_nrewardForce1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeForceItemId());
		tItemNum.push_back(tinfo->m_nrewardForce1);
	}

	if(tinfo->m_nClanContribution1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeContributionItemId());
		tItemNum.push_back(tinfo->m_nClanContribution1);
	}

	if(tinfo->m_nCampContribution1 > 0)
	{
		tItemList.push_back(GameParaLoader::GetInstance()->getExchangeCountryContributionItemId());
		tItemNum.push_back(tinfo->m_nCampContribution1);
	}

	tItemList.insert(tItemList.end(),tinfo->m_nTaskItemList1.begin(),tinfo->m_nTaskItemList1.end());
	tItemNum.insert(tItemNum.end(),tinfo->m_nTaskItemListNum1.begin(),tinfo->m_nTaskItemListNum1.end());

	tBindTypeList.resize(tItemList.size(),1);

	vector<DWORD> strengthLvVec;
	strengthLvVec.resize(tItemList.size(), 0);

	if(pPacket->IsBagEnought(tItemList,tItemNum,tBindTypeList))
	{
		return ePackageFull;
	}

	pPacket->AddItem(tItemList,tItemNum, vecPos, tBindTypeList,strengthLvVec,true);
//	for(size_t i = 0;i < m_vItemList.size();++i)
//	{
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,m_vItemList[i],m_vItemNum[i]);
//	}

//	if(costAndGotInfo.gotlist_size() > 0 && eAccountType_User == mOwner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(mOwner->GetID(),eStatic_Task,eStaMinor_Task_GetReward,&costAndGotInfo);

	return 0;
}

int QuestPart::CalCompleteRewardNum(const int& lv, const int & parm_one,
		const int &parm_two, const int& parm_three)
{
	double needChen = (double)parm_one/100;

	int ret = 0;
	double res = 0;

	res = pow(lv, needChen);

	ret =  (int)res  +  lv * parm_two + parm_three;
	return ret;
}

//void QuestPart::GetSectionReward(const uint& id)
//{
//	vector<int> vecType;
//	vector<int> vecBro;
//	sSectionReward * chapterReward = QuestLoader::GetInstance()->GetSectReward(id);
//
//	if(chapterReward)
//	{
//		eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttQust);
//
//		mOwner->ChangeHPMax(chapterReward->m_nHp,true,tempValueType);
//		mOwner->ChangeMPMax(chapterReward->m_nMp,true,tempValueType);
//		mOwner->ChangePhyAttackMax(chapterReward->m_npAtt,true,tempValueType);
//		mOwner->ChangePDefence(chapterReward->m_npDefen,true,tempValueType);
//
//		calculateFight();
//
//		vecBro.push_back(eCharHPMax);
//		vecBro.push_back(eCharMPMax);
//
//		vecType.push_back(eCharPhyAttackMax);
//		vecType.push_back(eCharPhyDefence);
//		vecType.push_back(eFightPower);
//	}
//
//	mOwner->SetDataFlag(eBaseInfo);
//	mOwner->SetDataFlag(eBattleInfo);
//
//	mOwner->SynCharAttribute(vecType);
//	mOwner->BroadcastCharAttribute(vecBro);
//}

void QuestPart::gmFinishTask(QuestType type)
{
	Quest* tempQuest = NULL;
	int error		 = 0;
	const Entry_Quest* tempQuestData = NULL;
	ServerRet tempAcceptSuccess;
	switch(type)
	{
	case QuestType_Main:
		{
			this->AllMainTaskHadComp(mAcceptedQuests.findMainQuest());

			return;
		}
	case QuestType_Functional:
		{
			tempQuest = mAcceptedQuests.findDailyQuest();
			if(tempQuest == NULL)
			{
				error = eQuestNoAccepted;
				break;
			}

			tempQuestData = tempQuest->getEntry();
			if(tempQuestData == NULL || tempQuestData->m_nMainType != QuestType_Functional)
			{
				error = eSystemError;
				break;
			}

			NewSubmitQeust(tempQuestData->m_nQuestId,tempQuest,0,false,tempAcceptSuccess);

			break;
		}
	case QuestType_Clan:
		{
			tempQuest = mAcceptedQuests.findClanQuest();
			if(tempQuest == NULL)
			{
				error = eQuestNoAccepted;
				break;
			}

			tempQuestData = tempQuest->getEntry();
			if(tempQuestData == NULL || tempQuestData->m_nMainType != QuestType_Clan)
			{
				error = eSystemError;
				break;
			}

			NewSubmitQeust(tempQuestData->m_nQuestId,tempQuest,0,false,tempAcceptSuccess);

			break;
		}
	case QuestType_Camp:
		{
			tempQuest = mAcceptedQuests.findCampQuest();
			if(tempQuest == NULL)
			{
				error = eQuestNoAccepted;
				break;
			}

			tempQuestData = tempQuest->getEntry();
			if(tempQuestData == NULL || tempQuestData->m_nMainType != QuestType_Camp)
			{
				error = eSystemError;
				break;
			}

			NewSubmitQeust(tempQuestData->m_nQuestId,tempQuest,0,false,tempAcceptSuccess);

			break;
		}
	case QuestType_Guide:
		{
			DWORD tempQuestID = mAcceptedQuests.findQuestByType(QuestType_Guide);
			if(tempQuestID == 0)
			{
				error = eQuestNoAccepted;
				break;
			}

			tempQuest = mAcceptedQuests.findQuest(tempQuestID);
			if(tempQuest == NULL)
			{
				error = eQuestNoAccepted;
				break;
			}

			tempQuestData = tempQuest->getEntry();
			if(tempQuestData == NULL || tempQuestData->m_nMainType != QuestType_Guide)
			{
				error = eSystemError;
				break;
			}

			NewSubmitQeust(tempQuestData->m_nQuestId,tempQuest,0,false,tempAcceptSuccess);

			break;
		}
	default:
		{
			error = eSystemError;
			return ;
		}
	}

	mOwner->SetDataFlag(eQuestInfo);
	QuestProto::ToSubmitTask toClient;
	if(tempQuestData != NULL)
		toClient.set_questid(tempQuestData->m_nQuestId);
	toClient.set_error(error);

	QuickMessage::GetInstance()->AddSendMessage(mOwner->GetID(), mOwner->GetChannelID(), &toClient, MSG_REQ_C2GM_UPDATEQUEST);

	switch(type)
	{
	case QuestType_Functional:
		{

			mOwner->GetQuestMgr()->getDailyQuests()->setDirty(true);

			mOwner->GetQuestMgr()->getDailyQuests()->updateToClient();

			break;
		}
	case QuestType_Clan:
		{

			mOwner->GetQuestMgr()->getClanQuests()->setDirty(true);

			mOwner->GetQuestMgr()->getClanQuests()->updateToClient();

			break;
		}
	case QuestType_Camp:
		{
			mOwner->GetQuestMgr()->getCampQuests()->updateToClient();
			break;
		}
	case QuestType_Guide:
		{
			m_GuideQuest->updateToClient();

			break;
		}
	default:
		{
			break;
		}
	}

	return ;
}

void QuestPart::gmReceiveTask(uint questID)
{
	const Entry_Quest* tempQuest = Quest::getEntry(questID);
	if(tempQuest == NULL)
		return ;

	// 删除任务
	mAcceptedQuests.removeQuest(mAcceptedQuests.findMainQuest());
	m_allMainTaskHadCompFlag = false;

	int tempError;
	accept(questID, QuestState_Accepted,tempError,eQuestAccept);

	mAcceptedQuests.setDirty(true);
	mAcceptedQuests.updateToClient();

	mOwner->SetDataFlag(eQuestInfo);
	return ;
}

void QuestPart::sendMail(vector<DWORD> itemList,vector<DWORD> numList,vector<WORD> flagList)
{
//	MailInfo::GoodsInfo tempGoods;
	MailInfo::SpecialMailItemList tempSpecial;
	for(size_t i = 0;i < itemList.size();++i)
	{
//		int tempType = GoodsLoader::GetInstance()->GetSpecliItemFunPlayerAttr(itemList[i]);
		//物品
//		if(tempType == 0)
//		{
//			const GoodsInfo* tempGoodsInfo = GoodsLoader::GetInstance()->GetItemDataByID(itemList[i]);
//			if(tempGoodsInfo == NULL)
//				continue;
//
//			Smart_Ptr<PropBase> tempProp = makePropByType(itemList[i],(eGoodsType)tempGoodsInfo->type,0);
//			if(!tempProp)
//				continue ;
//
//			tempProp->SetFlag(flagList[i]);
//			tempProp->SetAmount(numList[i]);
//
//			CharLogin::ClientItemInfo* tempItem = tempGoods.add_item();
//
//			tempProp->SetClientInfo(tempItem);
//
//		}
//		else
		{
			MailInfo::SpecialItem* tempSpecialItem = tempSpecial.add_itemlist();

			tempSpecialItem->set_itemid(itemList[i]);
			tempSpecialItem->set_num(numList[i]);
		}
	}

	string tempContent = InternationalLoader::GetInstance()->GetInterStr(TASK_MAIL_CONTENT);
	string tempTheme   = InternationalLoader::GetInstance()->GetInterStr(TASK_MAIL_THEME);

	MailMessageMgr::GetInstance()->SendMailByCharId(mOwner->GetID(),tempContent,tempTheme, &tempSpecial);

	return ;
}

bool QuestPart::ClearMosterTask(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);
	Smart_Ptr<Monster> tempMon = pArgs->m_pObj->GetTaskMon();
	if(tempMon)
	{
		if(!tempMon->IsDead())
		{
			tempMon->SetDead();
		}

		Smart_Ptr<Monster> temp;
		pArgs->m_pObj->AddTaskMonster(temp);
	}
	else
	{
		//可能由于异常情况导致服务器没有将NPC怪物刷出，导致无法完成任务
		mAcceptedQuests.ClearNpcFreshMonster();
		//pArgs->m_pObj->SetDataFlag(eQuestInfo);
	}
	return true;
}

bool QuestPart::onInitPackageInfo(const EventArgs &e)
{
	//用于  每日 重置 任务  中 有 采集物品 相关 需要 删除
	//由于  配置表里 规定 了 不能 超过 一个 格子的  最大 叠加数
	//故可以 只超过一次 就 直接 设置 为0
	// 防止 删除 过多的
	if(mDailyQuests && mDailyQuests->GetDoLastQuestID()>0)
	{
		if(this->ResetDeleteTaskItem(mDailyQuests->GetDoLastQuestID(), mDailyQuests->GetDoLastCondNum(),false))
		{
			mDailyQuests->SetDoLastCondNum(0);
		}
	}

	if(mClanQuests && mClanQuests->GetDoLastQuestID())
	{
		if(this->ResetDeleteTaskItem(mClanQuests->GetDoLastQuestID(), mClanQuests->GetDoLastCondNum(),false))
		{
			mClanQuests->SetDoLastCondNum(0);
		}
	}

	if(m_CampQuest && m_CampQuest->GetDoLastQuestID())
	{
		if(this->ResetDeleteTaskItem(m_CampQuest->GetDoLastQuestID(), m_CampQuest->GetDoLastCondNum(),false))
		{
			m_CampQuest->SetDoLastCondNum(0);
		}
	}

	mOwner->SetDataFlag(eQuestInfo);

	return true;
}

//玩家加入阵营事件
bool QuestPart::onJoinCamp(const EventArgs& e)
{
	if(mAcceptedQuests.findCampQuest() != NULL)
		return true;

	BYTE tempNum = GetCampDoTimes();

	if(!IsNeedSelectCampQuest(tempNum))
		return true;

	if(!(bool)m_CampQuest)
		m_CampQuest = new CampQuest(*mOwner);

	m_CampQuest->setTimes(tempNum);
	m_CampQuest->selectQuest();

	int error = 0;

	accept(m_CampQuest->getQuestId(), QuestState_Accepted, error,eQuestAccept);
	m_CampQuest->updateToClient();
	mOwner->SetDataFlag(eQuestInfo);
	return true;
}

//void QuestPart::Statistic_Quest(BYTE type)
//{
//	eLogicRelevant logic_enum = eStaMinor_Task_FinishMainLines;
//
//	if(type == QuestType_Clan)
//		logic_enum = eStaMinor_Task_FinishFactions;
//	else if(type == QuestType_Functional)
//		logic_enum = eStaMinor_Task_FinishDailys;
//	else
//		return ;

//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(mOwner->GetMyself(), eStatic_Task,	logic_enum);
//}

void QuestPart::ResetYesterdayQuest(int recorceSourceParamType)
{
	if(eRecorceSourceParamType_dailyQuest == recorceSourceParamType)
	{
		if(!mDailyQuests)
			return ;

		mDailyQuests->ResetYesterdayDailyQuest();
	}
	else if(eRecorceSourceParamType_clanQuest == recorceSourceParamType)
	{
		if(!mClanQuests)
			return ;

		mClanQuests->ResetYesterdayClanQuest();
	}
	else if(eRecorceSourceParamType_campQuest == recorceSourceParamType)
	{
		if(!m_CampQuest)
			return ;

		m_CampQuest->ResetYesterdayCampQuest();
	}
	else if(eRecorceSourceParamType_weekQuest == recorceSourceParamType)
	{
		if(!m_WeekQuest)
			return ;

		m_WeekQuest->ResetYesterdayWeekQuest();
	}

	mOwner->SetDataFlag(eQuestInfo);
}

