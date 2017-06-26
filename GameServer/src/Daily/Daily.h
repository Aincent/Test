/*
 * Daily.h
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */

#ifndef DAILY_H_
#define DAILY_H_

#include "define.h"
#include <map>
#include <vector>
#include "DailyLoader.h"
#include "MessageStruct/Daily/Daily.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "../Counter/Counter.h"

using namespace std;
using namespace CommBaseOut;

class Player;

enum GetPracticeType
{
	GetPracticeType_General=0,//普通
	GetPracticeType_Double,//双倍
	GetPracticeType_Treble,//3倍
};

enum GetRecoverResourceType
{
	GetRecoverResourceType_Quick_Common = 1,//普通找回
	GetRecoverResourceType_Quick_Golden,//自尊追回
};

struct sRecoverResourceInfo
{
	int id;
	int restcount;
	int exp;
	int money;
	int force;
	int clancontri;
	int campcontri;
	std::map<int,int> item;
	sRecoverResourceInfo():id(0),restcount(0),exp(0),money(0),force(0),clancontri(0),campcontri(0)
	{
		item.clear();
	}
	sRecoverResourceInfo(Daily::RecoverResourceInfo *info)
	{
		id = info->id();
		restcount = info->restcount();
		exp = info->reward().exp();
		money = info->reward().money();
		force = info->reward().force();
		clancontri = info->reward().clancontri();
		campcontri = info->reward().campcontri();
		for(int i = 0 ; i < info->reward().item_size();i++)
		{
			item[info->reward().item(i).itemid()] = info->reward().item(i).itemcount();
		}
	}

	void SetRecoverResourceInfo(Daily::RecoverResourceInfo *info)
	{
		info->set_id(id);
		info->set_restcount(restcount);
		info->mutable_reward()->set_exp(exp);
		info->mutable_reward()->set_money(money);
		info->mutable_reward()->set_force(force);
		info->mutable_reward()->set_clancontri(clancontri);
		info->mutable_reward()->set_campcontri(campcontri);
		std::map<int,int>::iterator it = item.begin();
		for(;it!=item.end();it++)
		{
			Daily::RecoverResourceItemReward *temp = info->mutable_reward()->add_item();
			temp->set_itemid(it->first);
			temp->set_itemcount(it->second);
		}
	}
};

class DailyManager
{
public:
	DailyManager(Player * player);
	~DailyManager();

	void ReInit();
	void Release();

	void InitDailyPractice(int64 nPracticeTime);
	int GetPracticeTime(void);
	void SetPracticeTime(int nPracticeTime);
	int GetExpByPractice(int nPracticeTime);
	//普通领取 双倍领取 三倍领取
	int GetExpReward(int ratio);
	void SetPracticeExp(int nPracticeExp);
	bool GetIsGotPracticeRward()
	{
		return m_bIsGotPracticeRward;
	}
	//资源追回
	void InitRecoverResource();
	void GetRecoverResourceInfo(Daily::RecoverResourceInfoList& info);
	int RecoverResource(Daily::C2GMReqRecoverResource & req);
	int GetDailyInfo(Daily::DailyToDoInfoList * toClient);

	int GetEveryDayMustWork(); // 获取每日必做的剩余
public:
	int GetDailyPracticeInfo(ServerReturn::ServerDoubleInt & toClient);

	bool IsHaveDailyPracticeToGet(void);

	int GetDailyPracticeReward(int type);

	int AddResource(const sRecoverResourceInfo &info);
	//玩家在刷新点也在线必须把当前的数据copy一份作为能回收的资源
	void ResetDaily();
	//能追回的资源项目个数
	int GetRecoverResourceCount(void);
private:
	Player* m_owner;
	//闭关修炼时间
	int m_nPracticeTime;
	//是否已经领取闭关修炼经验奖励
	bool m_bIsGotPracticeRward;
	//闭关修炼能领取的基础经验
	int m_nPracticeExp;

	std::map<int,sRecoverResourceInfo> m_RecoverResourceMap;
};

#endif /* DAILY_H_ */
