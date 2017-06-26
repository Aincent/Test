/*
 * QuestLoader.h
 *
 *  Created on: 2014年10月20日
 *      Author: root
 */

#ifndef QUESTLOADER_H_
#define QUESTLOADER_H_

#include "define.h"
#include "QuestDefine.h"
#include <string>
#include <vector>
#include <map>
#include "Singleton.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

#define DAILYTASK_START 	  20001
#define CLANTASK_START        30001
#define CAMPTASK_START 		  40001
#define GUIDETASK_START 	  50001
#define WEEKTASK_START 	  	  70001
#define MAX_DAILYCLANTASK_NUM 1000
#define MAIN_TASK_START 	  10001

struct Entry_Quest
{
	DWORD				m_nQuestId;                    		/// 任务编号
	string				m_sName;                      		/// 任务名称
	DWORD				m_nQuestChap;						/// 任务章节
	DWORD				m_nMainType;                  		/// 任务主类型
	DWORD				m_nMinorType;						/// 任务子类型
	string				Desc;                         		/// 任务描述
	DWORD				m_nTaskTarget;						/// 任务目标
	DWORD				m_nTaskTargetNum;					/// 任务目标数
	DWORD				m_nCollItemID;						/// 收集物品ID
	DWORD				m_nDropItemRate;					/// 掉落概率
	DWORD				m_nTaskMap;							/// 指定地点MapID
	DWORD				m_nTaskXpos;						/// 指定地点X坐标
	DWORD				m_nTaskYpos;						/// 指定地点Y坐标
	std::vector<DWORD> 	m_nTaskItemList;       				/// 接任务奖励物品
	std::vector<DWORD> 	m_nTaskItemListNum;    				/// 接任务奖励物品
	DWORD				m_nAcceptNpcId;                    	/// 接任务NPC
	DWORD				m_nSubmitNpcId;                    	/// 交任务NPC
	DWORD				m_nrewardExp; 						/// 奖励经验
	DWORD				m_nrewardMoney; 					/// 奖励金钱
	DWORD				m_nrewardForce; 					/// 奖励真气
	std::vector<DWORD>	m_vAwardItemList;     				/// 奖励物品
	std::vector<DWORD> 	m_vAwardItemNum;      				/// 奖励物品数量
	vector<DWORD>		m_vEquipId;							/// 对应装备ID
	vector<DWORD> 		m_vEquipNum; 						/// 装备数量
	DWORD				m_nLevelDown;						/// 接取任务的下限
	DWORD				m_nLevelUp;							/// 接取任务上限
	///这个数组并不是表里定义的，而是将表里完成任务需要的条件放到这个结构中
	CondCfg				SubmitConds[MAX_CONDITION_NUM];		/// 完成条件参数结构(这里实际才一组条件，为了便于扩展这里将完成条件定为2)
};

struct DailyTaskReward
{
	DWORD 			m_nId; 					//序号
	DWORD 			m_nType; 				//任务主类型
	DWORD 			m_nLv; 					//等级
	DWORD 			m_nrewardMoney; 		//奖励金钱
	DWORD				m_nrewardExp; 			// 奖励经验
	DWORD				m_nrewardForce; 		// 奖励真气
	DWORD				m_nClanContribution;	//帮贡
	DWORD				m_nCampContribution; //阵营贡献
	std::vector<DWORD> 	m_nTaskItemList;       				//任务奖励物品
	std::vector<DWORD> 	m_nTaskItemListNum;    				//任务奖励物品
	//20轮任务奖励
	DWORD 			m_nrewardMoney1; 		//奖励金钱
	DWORD				m_nrewardExp1; 			// 奖励经验
	DWORD				m_nrewardForce1; 		// 奖励真气
	DWORD				m_nClanContribution1;	//帮贡
	DWORD				m_nCampContribution1; //阵营贡献
	std::vector<DWORD> 	m_nTaskItemList1;       				//任务奖励物品
	std::vector<DWORD> 	m_nTaskItemListNum1;    				//任务奖励物品

	DailyTaskReward() : m_nId(-1),m_nType(0),m_nLv(0),m_nrewardMoney(0),m_nrewardExp(0),m_nrewardForce(0),m_nClanContribution(0),m_nCampContribution(0),
			m_nrewardMoney1(0),m_nrewardExp1(0),m_nrewardForce1(0),m_nClanContribution1(0),m_nCampContribution1(0)
	{
		m_nTaskItemList.clear();
		m_nTaskItemListNum.clear();

		m_nTaskItemList1.clear();
		m_nTaskItemListNum1.clear();
	}

};

//struct LevelData
//{
//	LevelData() : m_DownLevel(0),m_UpLevel(0){}
//	bool operator < (const LevelData& other) const
//	{
//		return (m_DownLevel < other.m_DownLevel || (!(other.m_DownLevel < m_DownLevel) && (m_UpLevel < other.m_UpLevel)));
//	}
//
//	int m_DownLevel;
//	int m_UpLevel;
//};

//
//struct sTwentyReward
//{
//	QuestType m_Type;
//	LevelData m_Level;
//	vector<DWORD> m_vItemList;
//	vector<DWORD> m_vItemNum;
//	vector<WORD>  m_BindTypeList;
//};

//struct sSectionReward
//{
//	DWORD m_nSectID;
//	string m_sSectDesc;
//	DWORD m_nHp;			//加血
//	DWORD m_nMp;			//加魔法
//	DWORD m_npAtt;		//加物功
//	DWORD m_nmAtt;		//加法攻
//	DWORD m_npDefen;	//加物防
//	DWORD m_nmDefen;	//加法防
//};
//
////刷新消耗
//struct RefreshConsume
//{
//	RefreshConsume() : m_Type(QuestType_Clan),m_Starts(0),m_RewardMoney(0),m_RewardGolden(0),m_NumMoney(0),m_NumGolden(0){}
//	QuestType m_Type;
//	int m_Starts;
//	int m_RewardMoney;
//	int m_RewardGolden;
//	int m_NumMoney;
//	int m_NumGolden;
//};

//完成一次消耗
struct OneFinishConsume
{
	OneFinishConsume() : m_Type(QuestType_Clan),m_Golden(0){}
	QuestType m_Type;
	int m_Golden;
};

//刷新数据
//struct RefreshData
//{
//	RefreshData() :
//			m_Type(QuestType_Clan),
//			m_Starts(0),
//			m_RewardProbability(0),
//			m_RewardVipLevel(0),
//			m_RewardVipProBability(0),
//			m_NumConsume(1.0f),
//			m_NumProbability(0),
//			m_NumVipLevel(0),
//			m_NumVipProBability(0),
//			m_starFactor(0.0f){}
//	LevelData m_Level;
//	QuestType m_Type;
//	int m_Starts;
//	vector<DWORD> m_ItemList;
//	vector<DWORD> m_NumList;
//	vector<WORD>  m_BindTypeList;
//	int m_RewardProbability;
//	int m_RewardVipLevel;
//	int m_RewardVipProBability;
//	float m_NumConsume;
//	int m_NumProbability;
//	int m_NumVipLevel;
//	int m_NumVipProBability;
//	float m_starFactor;
//};

enum eWeekTaskType
{
	eWeekTaskNull 	= 0,
	eWeekTaskEnd    = 1,
	eWeekTaskMiddle = 2,
};

//struct WeekItems
//{
//	vector<DWORD> m_Items;
//	vector<DWORD> m_Num;
//	vector<WORD>  m_BindType;
//};
//
//struct WeekItemProbability
//{
//	WeekItemProbability() : m_ItemID(0),m_Probability(0),m_Num(0){}
//	DWORD m_ItemID;
//	int   m_Probability;
//	DWORD m_Num;
//};
//
//struct WeekTaskData
//{
//	WeekTaskData() : m_ID(0),m_LowLevel(0),m_UpLevel(0),m_Type(eWeekTaskNull),m_BindType(0){}
//	int m_ID;
//	int m_LowLevel;
//	int m_UpLevel;
//	eWeekTaskType 	  m_Type;
//	vector<WeekItemProbability>         m_FixReward;
//	vector<vector<WeekItemProbability> > m_RandReward;
//	WORD  m_BindType;
//};
//
//struct WeekTaskLuaData
//{
//	WeekTaskLuaData() : m_CountsReward(0),m_AllRing(10){}
//	short m_CountsReward;
//	short m_AllRing;
//};




class QuestLoader : public Singleton<QuestLoader>
{
public:
	QuestLoader();
	~QuestLoader();
public:
	int Init(string & path);
	int InitQuest(string &file);
	int InitDailyTaskReward(string &file);
	int InitOneFinishConsume(string &file);
//	int InitTwentyReward(string &file);
//	int InitSection(string &file);
//	int InitRefreshData(string &file);
//	int InitRefreshConsume(string &file);
//	int InitOneFinishConsume(string &file);
//	int InitLua(string& file);

	Entry_Quest * GetQuestInfo(int questID);

	const DailyTaskReward *GetTwentyInfo(QuestType type,int level);
//	sTwentyReward *GetTwentyInfo(QuestType type,int level);

	//获取对应等级日常任务数量
	void GetLvDailyTaskNum(const DWORD &lv, std::vector<DWORD> &quests, const DWORD & needPassID);
	//获取对应等级帮派任务数量
	void GetLvClanTaskNum(const DWORD & lv, std::vector<DWORD> &quests, const DWORD & needPassID);
	//获取对应等级阵营任务数量
	void GetLvCampaskNum(const DWORD & lv, std::vector<DWORD> &quests, const DWORD & needPassID);
	void GetLvWeekTaskNum(DWORD lv, std::vector<DWORD> &quests,DWORD needPassID);

	DWORD getNextQuestID(DWORD currentQuestID,QuestType type);

//	int refreshStarts(int level,int currentStarts,int vipLevel,QuestType type);
//
//	int refreshNumStarts(int level,int currentStarts,int vipLevel,QuestType type,float& consume);
//
//	const RefreshData* getOneFinishRewart(int level,int currentStarts,QuestType type);
//
//	//获取对应等级日常任务奖励
//	sSectionReward *GetSectReward(DWORD id);

	bool IsMaxMainquestID(const DWORD& id)
	{
		if(m_maxMainquestID == id)
			return true;
		return false;
	}

	uint getMaxMainQuestID(){return m_maxMainquestID;}

	//是否 是 同一 章节
	bool IsSameChap(const uint &questID, uint *chap);

	//获得指定任务之前的所有章节，不包含当任务前章节
	void getBeforeChapterIDList(DWORD questID,map<int,int>& chapterIDList);

//	const RefreshConsume* GetRefleshFinishDeduction(QuestType type,int starts);

	const OneFinishConsume* GetOneFinishDeduction(QuestType type);

//	const OneFinishConsume* GetOneFinishDeduction(QuestType type,int counts);
//
	int GetOneFinishSginleCost(QuestType type);
//
//	int GetTaskExpReward(int lv,float starFactor);
//
//	int GetExtraTaskExpReward(int lv);
//
//	int GetTaskMoneyReward(QuestType type,int lv,float starFactor);
//
//	int GetTaskForceReward(QuestType type,int lv,float starFactor);
//
//	bool GetExtraTaskReward(QuestType type,int lv,int& nItemId, int& nItemNum);
//
//	int initWeekTask(string& fileName);

//	void getWeekTaskFixReward(int lv,WeekItems& rewards,eWeekTaskType type);
//
//	void getWeekTaskRandReward(int lv,WeekItems& rewards,eWeekTaskType type);

	int getMainTask(int level);

//	const WeekTaskLuaData& getWeekLua(){return m_WeekLua;}

private:
	map<DWORD,Entry_Quest> m_allQuest;
	vector<DailyTaskReward> m_DailyTaskReward;
	map<QuestType,OneFinishConsume> m_OneFinishConsumeList;
//	map<QuestType,map<LevelData,sTwentyReward> > m_twenty;
//	map<int,sSectionReward> m_section;
	uint m_maxMainquestID;  												//最大主线 ID
//	map<QuestType,map<int,vector<RefreshData> > > m_RefreshDataList;
//	map<QuestType,map<int,RefreshConsume> > m_RefreshConsumeList;
//	map<QuestType,map<int,OneFinishConsume> > m_OneFinishConsumeList;
//	map<int,WeekTaskData> m_WeekTasks;
//	WeekTaskLuaData		  m_WeekLua;

	CLuaInterface tlua;
};


#endif /* QUESTLOADER_H_ */
