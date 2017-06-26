/*
 * HorseManager.h
 *
 *  Created on: 2014年4月30日
 *      Author: helh
 */

#ifndef HORSEMANAGER_H_
#define HORSEMANAGER_H_

#include "Singleton.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/Horse.pb.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "CharDefine.h"
#include "../Horse/HorseLoader/HorseLoader.h"
#include "MessageCommonRet.h"

using namespace CommBaseOut;

class Player;

struct hSkinInfo
{
	int skinid;
	int64 endTime;
	hSkinInfo():skinid(-1),endTime(0)
	{

	}
};
/*
 * m_faceID 为 默认 点击 骑坐骑 选中的
 *
 *  玩家 身上 有 标识 是否 骑和哪个
 *  注意点 ：  坐骑 进阶 直接 加属性  不管 坐骑的 状态
 *
 *
 * */

struct HorseExpStatistics
{
	HorseExpStatistics() : m_Counts(0),m_Exp(0){}
	int m_Counts;
	int m_Exp;
};

class HorseMgr
{
	enum
	{
		eHorseTypeNull 		= 0,
		eHorseTypeNormal    = 1,
		eHorseTypeAuto	    = 2,
	};

public:
	HorseMgr(Player * player);

	~HorseMgr();

	void ReInit();
	void ReleaseHorse();

	void InitHorseFromDB(const PlayerInfo::HorseInfo &info);

	void SetHorseToDB(PlayerInfo::HorseInfo *info);

	void SetVitality();

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	//获得一个坐骑
	void AddHorse();
	//上/下坐骑
	int HandleHorse(int id);

	int NewHandleHorse(const int& id, const bool &flag);

	//坐骑喂养
	void EatHorse(HorseInfo::HorseAdvance& requestMsg, HorseInfo::AddExpReturn *ret);

	int getEatExpByRate(const HorseStaticInfo& info,eCommonRateType& type);

	ServerRet oneSimulationData(int& currentId,bool isBuy,int& currentExp,int& allExp,
			map<eCommonRateType,HorseExpStatistics>& resultList,map<int,int>& itemList,int& golden,int& money);

	//坐骑 喂养  增加 属性
	void AddAttrWhenChangeHorse(const int &before_ID, const int &after_ID);

	//属性变化
	int AttrChangeByHorse(const int &beforeID, const int  &afterID);
	int MoveSpeedChangeByHorse(const int &beforeID, const int  &afterID);

	int GetId(){return m_id;}
	int GetEx(){return m_exp;}

	//玩家升级事件
	bool HandlePlayerAdvanceLevel(const EventArgs & e);

	//统计
//	void StatisticHorseData(int before_id, int after_id);

	void setModelID(const int& model_id);

	//够买坐骑 皮肤
	int BuyHorseSkin(const int& skinId);

	//使用道具获得皮肤
	int GetHorseSkin(const int& skinId , int64 time = 0,int num = 1);

	//增加皮肤
	int AddHorseSkin(const int& skinId ,int64 time = 0,int num = 1);

	//删除皮肤
	bool DeleteHorseSkin(const int& skinId);

	bool IsHaveThisSkin(const int&skinId);

	int GetHourseQuality();

	void GetHorseSkinInfo(const int& skinId,hSkinInfo & skinInfo);

	void UpDateSkin(void * arg);

	bool IsHorseMaxByLv();

	// 基本条件 是否能进阶
	bool IsCanAdvance();

protected:

	// 检查基本进阶条件 基本进阶条件 元宝不参与计算
	 int CheckAdvanceCond();

	void addSkinAtt(int id,bool isAdd,vector<int>& attList);

	//获得指定ID坐骑增加的属性
	void getDesignationHorseAttList(int id,map<ECharAttrType,int64>& attList);

	//获得指定ID皮肤增加的属性
	void getDesignationSkinAttList(int id,map<ECharAttrType,int64>& attList);

	// 是否达到自动进阶条件
	bool IsCanAutoAdvance();

private:
	Player * m_owner;

	int m_id; //坐骑id
	int m_faceID; //模型id
	int m_exp; //经验

	map<int,hSkinInfo > m_skin_vec; //皮肤
	EventConnection m_lvEvent;

	TimerConnection	m_endTimer;
};



#endif /* HORSEMANAGER_H_ */
