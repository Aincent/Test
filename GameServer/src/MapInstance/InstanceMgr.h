/*
 * InstanceMgr.h
 *
 *  Created on: 2015年1月5日
 *      Author: root
 */

#ifndef INSTANCEMGR_H_
#define INSTANCEMGR_H_

#include <map>
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "Timer/TimerInterface.h"
#include "InstanceMapLoader.h"
#include "MessageStruct/Daily/Daily.pb.h"
#include "../Daily/Daily.h"
#include "CharDefine.h"
#include "../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;

#define CHALLEGEMAPFIRSTID 185
#define CHALLEGEMAPLEVEL 15

class Player;
class InstanceMgr
{
	struct MoneyInstanceData
	{
		MoneyInstanceData()
		{
			Reset();
		}

		void Reset()
		{
			mapId = 0;
			pass = 0;
			num = 0;
			buytimes = 0;
			reset = 0;
			updatetime = 0;
		}

		int mapId; //副本地图
		int pass;  //0表示未通关，1表示通关
		int num;   //本日已经完成的次数
		int buytimes; //本日金钱购买进入的次数
		int reset; 		//重置的次数
		int64 updatetime;//上次更新的时间
	};

	struct GroupInstance
	{
		int mapId; //副本地图
		int num;   //已经完成的次数
		int vipnum; //vip购买次数
		int buynum; //购买次数
		int pass;  //0表示未通关，1表示通关

		GroupInstance():mapId(0),num(0),vipnum(0),buynum(0),pass(0)
		{

		}

		GroupInstance(const PlayerInfo::InstanceGroupList& info)
		{
			mapId = info.mapid();
			num = info.num();
			vipnum = info.vipnum();
			buynum = info.buynum();
			pass = info.pass();
		}

		void Reset()
		{
			num = 0;
			vipnum = 0;
			buynum = 0;
		}
	};

	struct sInstanceDiffculty
	{
		int mapId;
		int pass;
		int num;
		int time;
		int reset;
		sInstanceDiffculty():mapId(0),pass(0),num(0),time(0),reset(0)
		{
		}
		sInstanceDiffculty(PlayerInfo::InstanceDiffculty *info)
		{
			mapId = info->mapid();
			pass = info->pass();
			num = info->num();
			time = info->time();
			reset = info->reset();
		}

		void SetProto(PlayerInfo::InstanceDiffculty &info)
		{
			info.set_mapid(mapId);
			info.set_pass(pass);
			info.set_num(num);
			info.set_time(time);
			info.set_reset(reset);
		}

		void ReSet()
		{
			num = 0;
			time = 0;
			reset = 0;
		}
	};
public:
	InstanceMgr(Player* player);
	~InstanceMgr();

	void ReleaseInstanceMgr();

	//保存铜钱副本数据
	void SetMoneyInstanceInfo(PlayerInfo::MoneyInstance *instanceInfo);
	//初始化铜钱副本数据
	void InitMoneyInstanceInfo(const PlayerInfo::MoneyInstance& instanceInfo);
	bool isMoneyInstanceUpdate(DWORD64 saveTime);
	void ResetMoneyInstance();
	int checkMoneyInstanceEnter(int& surplusNum);
	void EnterMoneyInstance();
	void getMoneyInstanceCount(int &maxCount, int &maxBuyCount, int &leftCount, int &buyCount);

	//铜钱副本本日购买次数
	int getMoneyInstanceBuyCount();

	//增加购买次数
	void addMoneyInstanceBuyCount();

	///////////////////////////
	//保存组队副本数据
	void SetGroupInstanceInfo(PlayerInfo::GroupInstance *instanceInfo);
	//初始化组队副本数据
	void InitGroupInstanceInfo(const PlayerInfo::GroupInstance& instanceInfo);
	void ResetGroupInstance();
	void GetGroupInstanceInfo(MapInstanceProto::GroupInstance& instanceInfo);

	int ShopGroupInstance(int mapId,MapInstanceProto::RetCostByGroupInstance& info);

	///////////////////////////////////////////

	//保存副本数据
	void SetInstanceInfo(PlayerInfo::InstanceInfoTwoDays *instanceInfo);
	//初始化副本数据
	void InitInstanceInfo(const PlayerInfo::InstanceInfoTwoDays &instanceInfo);
	//重置副本
	void ResetInstance();
	int ResetInstance(int sceneId);
	void ResetAllInstance(int type,int& code);

	//添加已完成副本
	void AddPassInstance(int sceneid, int num,int type, bool isPass = false);
	//更新已完成副本
	//获取剩余次数
	int GetLeftValidTimes(int sceneId);
	// 获取 所有副本剩余次数(个人,组队,挑战,vip)
	int GetAllInstSurplusNum();

	// 获取单人副本已挑战次数，返回单人副本所有次数
	int GetSingleInstUnChall(int& unChallenge);
	int GetChallInstUnChall(int& unChallenge);
	//得到类型副本的重置次数
	int getResetTime(int type);
	// 获得铜钱副本已挑战次数 返回总次数
	int GetMoneyInstHasChall(int& hasChallenge);
	// 获取精英副本已挑战次数，返回精英副本所有次数
	int GetEliteInstUnChall(int& unChallenge);
	// 获取噩梦副本已挑战次数，返回噩梦副本所有次数
	int GetCauchemarInstUnChall(int& unChallenge);
	// 获取进阶副本已挑战次数，返回进阶副本所有次数
	int GetAdvancedInstUnChall(int& unChallenge);
private:
	// 只支持 单人/精英/噩梦 副本  获取已挑战次数，返回副本所有次数
	int GetSomeInstUnChall(InstanceType instType, ESysSettingType lvLimitType, int& unChallenge);
	// 只支持 单人/精英/噩梦 副本
public:
	int GetLvEnough(int sceneId,int nLv);
	//获取VIP副本信息
	void GetVipInstanceInfo(MapInstanceProto::VipInstance & list);
	//获取普通副本信息
	void GetCommonInstanceInfo(MapInstanceProto::CommonInstanceInfo & info);
	//获取经验副本信息
	void GetExpInstanceInfo(MapInstanceProto::ExpInstanceInfo & info);
	//获取铜钱副本信息
	void GetMoneyInstanceInfo(MapInstanceProto::MoneyInstanceInfo & info);
	//发放奖励
	int SendAwardToPlayer(bool isSuccess, DWORD sceneId, DWORD startTime = 0, DWORD endTime = 0,DWORD fight = 0);
	void GetYesterdayInstanceInfo(int recorceSourceParamType,int instanceType,Daily::RecoverResourceInfoList * info);
	void GetYesterdayInstanceInfo(Daily::RecoverResourceInfoList * info);
	int RecoverResourceArena(int num, Daily::RecoverResourceInfo *info);

	bool CanEnterInstance(int instanceType,int specialType);

	//新扫荡功能 取消CD
	int NewStartSweep(int sceneId,MapInstanceProto::SimSweepInstanceAward& toClient,bool flag = false);

	int CanSweep();

	// 获取竞技场未挑战次数 返回总次数
//	int GetArenaUnChallenge(int& unChallenge);

	////////////////////////////////////////////////////////////////////////////////
	//获取精英副本信息
	void GetEliteInstanceInfo(MapInstanceProto::EliteInstance& info);
	//初始化精英副本信息
	void InitEliteInstanceInfo(const PlayerInfo::EliteInstance& info);
	//保存精英副本信息
	void SetEliteInstanceInfo(PlayerInfo::EliteInstance *info);
	//重置精英副本
	void ResetEliteInstance();

	//获取恶梦副本信息
	void GetCauchemarInstanceInfo(MapInstanceProto::CauchemarInstance& info);
	//初始化恶梦副本信息
	void InitCauchemarInstanceInfo(const PlayerInfo::CauchemarInstance& info);
	//保存恶梦副本信息
	void SetCauchemarInstanceInfo(PlayerInfo::CauchemarInstance *info);
	//重置梦副副本
	void ResetCauchemarInstance();

	//获取进阶副本信息
	void GetAdvancedInstanceInfo(PlayerInfo::AdvancedInstance& info);
	//初始化进阶副本信息
	void InitAdvancedInstanceInfo(const PlayerInfo::AdvancedInstance& info);
	//保存进阶副本信息
	void SetAdvancedInstanceInfo(PlayerInfo::AdvancedInstance *info);
	//重置进阶副本
	void ResetAdvancedInstance();

private:
	bool GetInstancePass(int nType,int sceneId);

	void SetInstancePass(int nType,int sceneId,int nPass);

	int CanNewStartSweep(int sceneId,InstanceType eType);

	int GetRateSuccessReward(int nType,int sceneId);
private:
	Player * m_owner;
	map<int,sInstanceDiffculty> m_Instance;
	map<int, sInstanceDiffculty> m_YesterdayInstance;
	map<int, PlayerInfo::InstanceVip> m_InstanceVip;
	MoneyInstanceData m_MoneyInstance;//金钱副本
	map<int,GroupInstance> m_GroupInstance;
	map<int,sInstanceDiffculty> m_InstanceElite;
	map<int,sInstanceDiffculty> m_InstanceCauchemar;
	map<int,sInstanceDiffculty> m_InstanceAdvanced;
	map<int,sInstanceDiffculty> m_InstanceExp;
	map<int,sInstanceDiffculty> m_InstanceMoney;
};

#endif /* INSTANCEMGR_H_ */
