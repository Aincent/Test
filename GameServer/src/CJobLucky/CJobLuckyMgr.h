/*
 * CJobLuckyMgr.h
 *
 *  Created on: 2014年8月15日
 *      Author: helh
 */

#ifndef CJOBLUCKYMGR_H_
#define CJOBLUCKYMGR_H_

#include <vector>
#include <map>
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "./CJobLuckyLoader/CJobLuckyLoader.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class CJobLuckyMgr
{
	struct CJobInfo
	{
		int id;
		int exp;

		CJobInfo():id(-1),exp(0)
		{

		}
	};

public:
	CJobLuckyMgr(Player * p);

	~CJobLuckyMgr();

	void ReInit();
	void ReleaseCJobLucky();

	void InitCJobLucky(const PlayerInfo::RandCJobLucky *info);

	//上线计算属性
	void onlineCalculateAtt();

	//计算战斗力
	int onlineCalculateFight();

	void getAtt(map<ECharAttrType,int64>& attList,int id);

	//初始化  比对 官印格子 是否 与开启向匹配
	void CompareJobLuckyPackage();

	void SetCJobLucky(PlayerInfo::RandCJobLucky * info);

	// 随机官印
	void RandJob(const int &index, int& err,int64& status);

	//卖出/拾取官印(hType操作类型，      packageType 背包类型      index 为序号)
	int HanlePackage(bool hType, bool packageType, int index, int param);

	//拾取到官印背包中去
	int PickedUpToPackage(int index, int param);

	//卖出官印
	int SaleCJobItem(bool packageType, int index, int param);

	//兑换官印
	int ExchangeCJob(int id);

	//吞噬
	void EatOtherCJob(int param, ServerReturn::ServerThreeInt &ret);

	//穿戴
	int WearCJob(const int64 &index);

	// 开启格子
	void OpenPackage(int newSectorSize, int& err);

	//交换身上的官印格子
	int ChangeCJobSector(int64 index);

	//卸下官印
	int UnWearCJob(int64 index);

	//加入到官印背包   false 背包满   true 加入成功
	bool AddPackage(int id);

	bool IsPackageFull();

	//加到身上
	int AddPlayer(const int &index, const int &id, const int &exp, const int64 &player);

	//加属性
	void AddAttr(const int &id);

	//删属性
	void DelAttr(const int &id);

	//玩家升级事件
	bool HandlePlayerAdvanceLevel(const EventArgs & e);

	void AddJobLuckyPackage();

	void SetRandIndex(const int &randIndex)	{m_randIndex = randIndex;}

	int GetRandIndex()const	{return m_randIndex;	}

	void gmAddJob(int id,int exp,int index);

	//获取身上几级以上官印的个数
	DWORD GetCjobCountByLv(DWORD lv);

protected:
	//身上是否有满级官运
	bool isHaveFullLevelJob();
	//考官随即
	int64 RandCJob(const int &index,int64& status);
	//开格子
	int OpenPackage(int newSectorSize);

private:
	Player * m_owner;
	//官印格子
	vector<CJobInfo> m_package;
	//科举格子
	vector<int> m_examPackage;
	//身上的官印
	map<int64, vector<CJobInfo> > m_rolePackage;
	//官勋
	int m_contri;
	//当前的考官序号
	int m_randIndex;
	//随机 官运次数  --- 为了  防止 玩家 随机完之后  立即领取
	int m_randJobCount;

	EventConnection m_lvEvent;
};



#endif /* CJOBLUCKYMGR_H_ */
