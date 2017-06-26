/*
 * ProtectedKingLoader.h
 *
 *  Created on: 2014年11月27日
 *      Author: helh
 */

#ifndef PROTECTEDKINGLOADER_H_
#define PROTECTEDKINGLOADER_H_

#include "Singleton.h"
#include "define.h"
#include <map>
#include <vector>
#include "Lua/LuaInterface.h"
#include "../../Map/ConfigLoader/FieldMonster.h"

using namespace std;
using namespace CommBaseOut;

struct ProReflushMonster
{
	int id;
	WORD count;
	//系数（以100为单位）
	WORD factor;

	ProReflushMonster():id(-1),count(0),factor(0)
	{

	}
};

//描述每关信息
struct ProtectKingMonster
{
	int id;
	vector<ProReflushMonster> monster; //一波的怪物
	DWORD exp;//累计经验
	DWORD money;//累计金钱
	DWORD force;//累计真气
	DWORD time; //扫荡时间
	int finish; //闯关时间

	ProtectKingMonster():id(-1),exp(0),money(0),force(0),time(0),finish(0)
	{

	}
};

//struct ProRewardItem
//{
//	int id;
//	int count;
//
//	ProRewardItem():id(-1),count(0)
//	{
//
//	}
//};

//描述关卡奖励
struct ProtectedReward
{
	int id;
	int node;//关卡等级
	//奖励绑定类型
	int bindType;
//	vector<ProRewardItem> item;//奖励的道具
	vector<DWORD> itemId;
	vector<DWORD> count;

	ProtectedReward():id(-1),node(0),bindType(0)
	{
		itemId.clear();
		count.clear();
	}
};

class ProtectedKingLoader : public Singleton<ProtectedKingLoader>
{
public:
	ProtectedKingLoader();
	~ProtectedKingLoader();

	int Init(string &path);
	int InitNodes(string &file);
	int InitReward(string &file);
	int InitOther(string &file);

	//得到最大的关数
	WORD GetMaxNode()
	{
		return m_nodeCount;
	}

	WORD GetCountDown()
	{
		return m_countDown;
	}

	//获得目标id
	int GetTargetID()
	{
		return m_targetID;
	}

	ProtectKingMonster * GetKingMonster(int node)
	{
		if((DWORD)node >= m_allNodes.size())
		{
			return 0;
		}

		return &m_allNodes[node];
	}

	int  GetNodeSweepTimes(int node)
	{
		if((DWORD)node >= m_allNodes.size())
		{
			return 0;
		}

		return m_allNodes[node].time;
	}

	vector<Pos> GetPos(int index)
	{
		vector<Pos> pos;

		if(m_reflushPoint.size() <= 0)
			return pos;

		index = index % m_reflushPoint.size();

		return m_reflushPoint[index];
	}

	int GetTimes()
	{
		return m_times;
	}

	int GetVipLv()
	{
		return m_vipLV;
	}

	ProtectedReward* GetProtectedReward(int node)
	{
		if((DWORD)node >= m_allReward.size())
		{
			return 0;
		}

		return &m_allReward[node];
	}

	//获得奖励
	void GetNodeReward(int bnode, int enode, int &exp, int &force, int &money);

	int GetNodeSweepTime(int bnode, int enode);

	int GetSweepTimes()
	{
		return m_nSweepTimes;
	}

	int GetAllRewardNum()
	{
		return m_allReward.size();
	}

	int GetCanGetAwardNode(int node,  int& realNode);


private:

	vector<ProtectKingMonster> m_allNodes; //所有关数据
	vector<ProtectedReward> m_allReward; //所有的奖励

	vector<vector<Pos> > m_reflushPoint; //刷怪点以及信走点
	WORD m_nodeCount; //所有的关数
	BYTE m_vipLV; //扫荡所需vip等级
	BYTE m_times; //每次重置的次数
	int m_targetID; //目标的怪物id
	WORD m_countDown; //倒计时
	WORD m_nSweepTimes;
};


#endif /* PROTECTEDKINGLOADER_H_ */
