/*
 * QuestPart.h
 *
 *  Created on: 2014年10月21日
 *      Author: root
 */

#ifndef QUESTPART_H_
#define QUESTPART_H_

#include "Quest.h"
#include "AcceptedQuest.h"
#include "QuestDaily.h"
#include "QuestClan.h"
#include "AcceptableQuests.h"
#include "../EventSet/EventDefine.h"
#include "EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "Timer/TimerInterface.h"
#include "../Daily/Daily.h"
#include "Smart_Ptr.h"
#include "MessageCommonRet.h"
#include "CampQuest.h"
#include "QuestGuide.h"
#include "QuestWeek.h"

using namespace std;
using namespace CommBaseOut;

class Player;

const int Max_Reward_Start = 1;
const int Max_Num_Start = 1;


//所有 任务 奖励 所需  信息
struct sGiveQuestRewardNeedData
{
	uint questId;
	uint questType;
	BYTE RewardLevel;  //奖励等级    日常  帮派 计算 所需
	bool byMail;
	sGiveQuestRewardNeedData():questId(0),questType(0),RewardLevel(0),byMail(false)
	{

	}
};

/*
 * 此文件 为  玩家 任务  相关的
 * 给  客户端  提供  的 唯一 接口  只有 submit()
 * 其他  情况 下 都是  只要 任务 完成   出发事件  主动 推送  可以 查看 condition中的  相关 Quest::onStateOrConditionChanged()函数
 * 一键 完成  任务  给予 剩下 所有 的 奖励 且 以最大 奖励  星级 计算
 * 任务 获得 的 所有 物品  都是  帮定的
 *
 * 要 注意的 是  如果 玩家没有   帮派 任务  的 话  是没有 mClanQuests组件  可以 查看  加入帮派  和  退出 帮派 事件
 * 而  日常 任务  是 以  玩家 升级 事件 来判定 的  等级   Open_DailyQuest_Lv(可调整 )
 *为了  标识  玩家  完成 所有 主线  任务  m_allMainTaskHadCompFlag
 *为了  由于 帮派 可以  无限制的  退 ，加  所以 加入 m_clanDoTime记录 当天 完成的 次数
 *为了  与  日常 形式 上 保持 一致  加入 m_dailyDoTime(其实 现在 没有什么 特殊 作用 )
 *还可 查看  InitQuestInfoFromDB函数的   注释 (在 .cpp文件中  )
 * */


class QuestPart
{

public:
	QuestPart(Player* owner);
	~QuestPart();

	/// 各个人物
public:
	AcceptedQuests& getAcceptedQuests()						{ return mAcceptedQuests; }
	Smart_Ptr<DailyQuests>	& getDailyQuests()				{ return mDailyQuests; }
	Smart_Ptr<QuestClan>& getClanQuests()					{ return mClanQuests; }
	Smart_Ptr<CampQuest>& getCampQuests()					{ return m_CampQuest; }
	Smart_Ptr<QuestGuide>& getGuideQuests()					{ return m_GuideQuest; }
	Smart_Ptr<QuestWeek>& getWeekTask()						{ return m_WeekQuest; }

public:
	bool Init();

	void ReInit();
	void RelaseQuestPart();

	Player* getOwner() const { return mOwner; }

public:
	bool canAccept(uint questId, int &error) const;
	bool canSubmit(const uint &questId, bool useMoney = false) const;

	int IsCanAcceptMainLineQeust(const uint& questId);

	/// 接受任务
	bool accept(const uint &questId, const QuestState &state, int &error,eOnLineOrAccept type);

	/// 提交任务,useMoney为true表明花元宝立即完成,rewardIdx为有多个奖励时的奖励索引, byMail表明是否将任务奖励至于邮件中（一般用于系统自动完成任务）
	int submit(const uint &questId, uint *questType,ServerRet& error, bool useMoney = false, int rewardIdx = 0, bool byMail = false);

	int NewSubmitQeust(const uint &questId, Quest* q, int rewardIdx, bool byMail,ServerRet& error);
	int GiveCompleteDailyReward();

	//提交  各种类型任务
	int UpdateMainLineQeust(const uint &questId);
	int UpdateBranchQeust(const uint &questId);
	int UpdateDailyQeust(const uint &questId);
	int UpdateClanQeust(const uint &questId);
	ServerRet UpdateCampQeust(DWORD questId);
	ServerRet UpdateGuideQeust(DWORD questId);

	int GiveSubmitMainLineReward( Quest* q, const sGiveQuestRewardNeedData &data);

	int acceptConfigNeedItemReward(const uint & questId);

	//计算 奖励
	int CalQuestSubmitReward(Quest* q, const sGiveQuestRewardNeedData &data);

	//判断包裹是否足够装下一个任务有所给予的物品，包括装励物品和下一个任务发送的物品(下一个任务物品是在前一个任务给的)
	bool isHaveEnoughPackage(const Entry_Quest& data);

	//没有了帮派 －－－导致没有帮派任务
	void DiscardClanQuest(uint& questId);

	/// 日常任务升星;type为选择日常还是帮派任务，isFull是不是两个直接刷到最大星级；两个参数只有一个会生效
//	int upgradeQuestsStarLevel(uint questId, int type);

	//一键完成
	int oneKeyComplete(const uint& questid);

	int GiveCompleteReward(int type);

	int CalCompleteRewardNum(const int& lv, const int & parm_one, const int &parm_two, const int& parm_three);

	void SetClanDoTime(const BYTE& time)
	{
		if(time >20)
			return ;
		m_clanDoTime = time;
	}

	BYTE GetClanDoTime()const {	return m_clanDoTime; }

	void SetDailyDoTime(const BYTE& time)
	{
		if(time >20)
			return ;

		m_dailyDoTime = time;
	}

	BYTE GetDailyDoTime()const {	return m_dailyDoTime; }

	void setCampDoTimes(BYTE campDoTimes){m_CampDoTime = campDoTimes;}

	BYTE GetCampDoTimes() const {return m_CampDoTime;}

	//初始化任务数据
	void InitQuestInfoFromDB(const PlayerInfo::QuestInfo &info);

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	void InitRightQuestData();

	//通知 客户端 任务信息
	void FillInQuestInfoCmd();

	// 数据库相关
	void SetQuestInfoToDB(PlayerInfo::QuestInfo *info);

	void ResetYesterdayQuest(int recorceSourceParamType);

	//所有主线任务 都完成
	void AllMainTaskHadComp(const uint& questId);

	//发送所有主线任务 都完成 指令
	void SendAllMainTaskHadCompCmd();

	DWORD findQuestByType(QuestType type);

	Quest* findQuest(DWORD questID);

	ServerRet finishOneQuest(DWORD questID);

public:
	void ResetQuest();

	//重置 日常 帮派任务 时  删除已经 收集到的 物品
	bool ResetDeleteTaskItem(const uint& questID, const int& condtionNum,bool isSend = true);

	void gmFinishTask(QuestType type);

	void gmReceiveTask(uint questID);

	void sendMail(vector<DWORD> itemList,vector<DWORD> numList,vector<WORD> flagList);

	ServerRet updateWeekTask();

	bool SubmitTaskRemoveItem(int questID);								    ///响应提交任务

private:
//	void GetSectionReward(const uint& id);					//领取章节奖励

//	void Statistic_Quest(BYTE type);

	/*此项任务 是否 开启*/
	bool IsCanDailyQuest();
	bool IsCanClanQuest();
	bool isCanCampQuest();

	/*是否 需要 重新 选择 一个 新 任务*/
	bool IsNeedSelectDailyQuest(const BYTE &time);
	bool IsNeedSelectClanQuest(const BYTE &time);
	bool IsNeedSelectCampQuest(const BYTE time);

	void getDesignationChapterAttList(int chapterID,map<ECharAttrType,int64>& attList);

	//删除多余任务品
	void removeSurplusTaskItem();

	//获得所有任务物品ID对应的个数
	void getAllTaskItem(map<int,int>& mapList);

private:
	bool onLvEvent(const EventArgs & e);							///相应等级升级事件
	bool onEnterClanEvent(const EventArgs & e);						///响应进入帮派事件
	bool onLeaveClanEvent(const EventArgs & e);						///响应退出帮派事件
	bool onAcceptTask(const EventArgs & e);							///响应提交任务
//	bool onSumitTask(int questID);							///响应提交任务
	bool ClearMosterTask(const EventArgs & e);						/// 下线自动完成NPC刷怪
	bool onInitPackageInfo(const EventArgs &e); 					//响应 玩家 初始化 背包 信息
	bool onJoinCamp(const EventArgs& e);							//玩家加入阵营事件

	Player*				mOwner;
	AcceptedQuests		mAcceptedQuests;	/// 已接任务.
	Smart_Ptr<DailyQuests>			mDailyQuests;		/// 日常任务
	Smart_Ptr<QuestClan>			mClanQuests;		/// 帮派任务
	Smart_Ptr<CampQuest>			m_CampQuest;		/// 阵营任务
	Smart_Ptr<QuestGuide>			m_GuideQuest;		//  引导任务
	Smart_Ptr<QuestWeek>			m_WeekQuest;

	EventConnection m_stopLvEvent;
	EventConnection m_stopEnterClanEvent;
	EventConnection m_stopLeaveClanEvent;
	EventConnection m_stopAcceptTaskEvent;
	EventConnection m_stopClearMonsterEvent;
	EventConnection m_stopInitPackageEvent;
	EventConnection m_JoinCampEvent;				//加入阵营

	BYTE m_clanDoTime;  //帮派任务 完成次数   //防止多次 退帮派 导致 重复刷新
	BYTE m_dailyDoTime;  //帮派任务 完成次数   //防止多次 退帮派 导致 重复刷新
	BYTE m_CampDoTime;  //
	bool m_allMainTaskHadCompFlag;  //是否 所有 主线任务 都完成
};



#endif /* QUESTPART_H_ */
