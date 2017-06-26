/*
 * DanLuMgr.h
 *
 *  Created on: 2014年8月7日
 *      Author: root
 */

#ifndef DANLUMGR_H_
#define DANLUMGR_H_

#include "Singleton.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/DanLu.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"

#include "DanLuMessager.h"
#include "DanLuLoader.h"
#include "../EventSet/EventDefine.h"
#include <list>
#include "../Horse/HorseLoader/HorseLoader.h"

using namespace CommBaseOut;

class Player;

const int INITDANLUID = 1;   //最开始的丹炉ID

enum eDanLudvanceType
{
	eDanLuAdvanceNull   = 0,
	eDanLuAdvanceNormal = 1,
	eDanLuAdvanceAuto   = 2,
};

struct DanLuExpStatistics
{
	DanLuExpStatistics() : m_Counts(0),m_Exp(0){}
	int m_Counts;
	int m_Exp;
};

class DanLuMgr
{
public:
	DanLuMgr(Player * player);

	~DanLuMgr();

	void ReInit();
	void ReleaseDanLuMgr();

	void InitDanLuFromDB(const PlayerInfo::DanLuInfo &info);

	void SetDanLuToDB(PlayerInfo::DanLuInfo *info);

	// 初始化时熔炼值超过上限值的操作
	void HandlePointMoreCeil();

	void SetVitality();

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	void SetNowDanLuInfo(int id, int exp);

	void GetDanLuInfo(DanLuInfo::AckPlayerDanLuInfo &info);

	void GetNowDanLuInfo(DanLuInfo::AckPlayerDanLuInfo &info);

	void GetDanLuInfo(int *id, int *exp);

	int GetID()
	{
		return m_id;
	}

	//练炉
	void EatDanLu(DanLuInfo::AckPlayerDanLuInfo *info,DanLuInfo::ClientDanLuAddExp& req);

	ServerRet addExpResult(int addExp,int& currentID,int& currentExp,int& resultAddExp);

	ServerRet oneEatDanluBuckle(int& currentID,bool isBuy,bool isAuto,int& allExp,
			int& currentExp,map<int,int>& itemList,int& golden,int& money,map<eCommonRateType,DanLuExpStatistics>& resultExpList);

	//增加熔炼值
	void AddDanLuExp(int exp);

	//判定是否可以自动练炉
	bool IsCanFastDanLu();

	//判定是否已经开启了此功能
	bool IsOpenDanLuSystem();

	//本次练炉是否暴机
	eCommonRateType IsCrack(float bigRate,float smallRate);

	//是否自动炉练
	int CalDanLuFun(const LianLuInfo& info,eCommonRateType& type);

	int SetDanLuInfo(int *id, int *exp);

	void AddDanLuFun(int beforeID,int currentID);

	//炉练出 装备
	void DanLuEquip(DanLuInfo::RetClientDanLuEquip *info,const DanLuInfo::ClientDanLuEquip &req);

	//返还炉练物品
	void ReclaimDanLu(DanLuInfo::RetClientDanLuEquip *info,const DanLuInfo::ClientDanLuEquip &req);

	void DanluEquip_Exp(int exp);

	int DanluEquip_Point(const DanLuInfo::ClientDanLuEquip &req);

	//检查 数据
	int CheckDanluItem(const DanLuInfo::ClientDanLuEquip &info);

	//计算 能够 产出多少 点
	int CalHowPoint(const DanLuInfo::ClientDanLuEquip &info);

	//计算 能够 产出多少经验
	int CalHowExp(const DanLuInfo::ClientDanLuEquip &info);

	//随即 产生 能够 得到的 物品
	int RandItem(DanLuInfo::ProduceItem& item);

	//删除 用来 练物品
	void DeleteDanluItem(const DanLuInfo::ClientDanLuEquip &info);

	int GetDanluEquip(int& itemID, int& itemNum);

	//	进入丹道系统事件
	bool HandleCanEnterDanLu(const EventArgs & e);

	bool HandleDanLu(bool isOnline);

	bool IsDanLuMaxByLv();

	bool IsCanAdvance();

	bool IsCanSmelting();

	int GetProduceItemSize(){ return m_itemCntr.size();}

protected:

	int CheckAdvanceCond();
	void getDataAttList(int id,map<ECharAttrType,int64>& attList);

private:
	Player * m_owner;

	//炼炉id
	int m_id;
	//经验
	int m_exp;

	//当前 拥有的 点数
	int m_point;
//	//还没到 上限 产生物品 的次数
//	int m_produceNum;
	// 未领取的物品容器 key:位置索引  value:物品ID,物品数量
//	map<int, pair<int, int> > m_itemCntr;
	list<DanLuInfo::ProduceItem> m_itemCntr;

	EventConnection m_lvEvent;
};


#endif /* DANLUMGR_H_ */
