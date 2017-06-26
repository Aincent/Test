/*
 * GirlWarManager.h
 *
 *  Created on: 2014年12月6日
 *      Author: root
 */

#ifndef GIRLWARMANAGER_H_
#define GIRLWARMANAGER_H_

#include <map>
#include <vector>
#include "Smart_Ptr.h"
#include "define.h"
#include "GirlWar.h"
#include "MessageStruct/GirlWar/GirlWarMessage.pb.h"
#include "../../EventSet/EventDefine.h"

using namespace std;
using namespace CommBaseOut;


class GirlWarManager
{
public:
	GirlWarManager(Player * obj);
	~GirlWarManager();

	void Init();
	void ReInit();
	void ReleaseGirlWar();

	//初始化战姬
	void InitGirlWarInfo(const PlayerInfo::GirlWarInfo &info);

	void SetVitality();

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	//保存战姬
	void SetGirlWarInfo(PlayerInfo::GirlWarInfo *info);
	//添加战姬
	void AddGirlWar(Smart_Ptr<GirlWar>& girlWar);
	//查找战姬
	void GetGirlWar(int key,Smart_Ptr<GirlWar>& girlWar);
	void GetGirlWarById(int nId,Smart_Ptr<GirlWar>& girlWar);

	//获得战姬信息
	void GetAllGirlWarInfo(GirlWarMessageInfo::ToClientGirlWarInfo& info);
	//获得战姬达成条件
	void GetGirlWarCondition(int nId,GirlWarMessageInfo::ToClentNotHaveGirlWar* info);
	//战姬休战
	void ReseatGirlWar(int nId, GirlWarMessageInfo::ToClentFight& info);
	//战姬出战
	void FightGirlWar(int nId, GirlWarMessageInfo::ToClentFight& info);
	//战姬复活
	void ReviveGirlWar(int nId, GirlWarMessageInfo::ToClentFight& info);
	//创建战姬
	int GreateGirlWar(int nId);
	//void GreateGirlWar(int  nId);
	//创建限时战姬
//	void GreateGirlWar(int  nId , int64 time);
	//是否拥有nId的战姬
	bool IsHave(int nId);
	//标记已出战战姬
	void SetGirlWar(Smart_Ptr<GirlWar>& girlWar)
	{
		m_girlWar = girlWar;
		m_owner->SetDataFlag(eGirlWarInfo);
	}
	//获得已出战战姬
	Smart_Ptr<GirlWar>& GetGirlWar()
	{
		return m_girlWar;
	}

	bool EnterPetData(const EventArgs & e);

	bool LeavePetData(const EventArgs & e);
	//玩家下线时战姬处理
	 bool HandlePlayerOut(const EventArgs & e);
	 //玩家升级战姬处理
	 bool HandleCanLevelReward(const EventArgs & e);
	 //玩家签到战姬处理
	 bool HandleCanSignin(const EventArgs & e);
	 //玩家vip战姬处理
	 bool HandleUpdateVipLv(const EventArgs & e);
	 //玩家添加物品战姬处理
	 bool HandleAddItem(const EventArgs & e);
	 // 玩家通关副本处理
	 bool HandleInstancePass(const EventArgs& e);
	 // 玩家通关守护秦王最大节点
	 bool HandleGuardMaxNode(const EventArgs& e);

	//
//	void UpDateGirl(void * arg);

	//是否有战姬大过指定等级
	bool isMoreThanLevel(int level);
//	//激活战姬
//	int ActivatingGirlWar(int  nId);
//	//反激活战姬
//	int ActivatedGirlWar(int nId);
//	//战姬传功
//	int SwapExpGirlWar(int nId,int ngid,int type);
	//战姬升级
	void GirlWarUpLv(GirlWarMessageInfo::FromClentUplv& req,GirlWarMessageInfo::ToClentUplv& info);
    //穿戴装备
	int WareEquip(GirlWarMessageInfo::GirlWarEquip& req,GirlWarMessageInfo::RetGirlWarEquip& info);
	//一键穿戴装备
	int OneKeyWareEquip(int nId,GirlWarMessageInfo::RetGirlWarOneKeyEquip& info);
	//升阶
	int UpQuality(int nId,GirlWarMessageInfo::RetGirlWarUpQuality& info);
	//技能升级
	int UpSkill(GirlWarMessageInfo::GirlWarUpSkill& req,GirlWarMessageInfo::RetGirlWarUpSkill& info);
	//升星
	int UpStar(int nId,GirlWarMessageInfo::ToClientUpStar& info);

	void SetStarrankByOrder(int nId,int nStar);

	void SetQualityByOrder(int nId,int Quality);

	void SetLvByOrder(int nId,int nLv);

	//给某个战姬加或减属性
	void alterClanAtt(CreatureObj& object,bool isAdd);

	int GetGirlWarNum(){return m_collection.size();}

	int GetGirlWarQuality(int nQuality);
protected:
	//获得指定战姬緾绵所增加的属性
	void getDesignationLingeringAttList(Smart_Ptr<GirlWar> grilWar,map<ECharAttrType,int64>& attList);
	//获得战姬激活数量
	int GetStarrank();
	//穿戴装备
	void GetWareEquip(int nId,int nEquipId);
	//星阶加成
	void GetStarCollection();
	//是否可以获取此战姬
	int IsCanGetGirlWar(int nId);
private:
	Player *  m_owner;
	Smart_Ptr<GirlWar> m_girlWar;
	map<int64 ,Smart_Ptr<GirlWar> > m_collection;
	EventConnection m_outEvent;//玩家下线
	EventConnection m_logoutEvent;
	EventConnection m_enterEvent;
	EventConnection m_lvEvent;
	EventConnection m_signEvent;
	EventConnection m_vipEvent;
	EventConnection m_addItemEvent;
	EventConnection m_instancePassEvent;
	EventConnection m_guardPassNodeEvent; // 守护秦王

	TimerConnection	m_endTimer;
};


#endif /* GIRLWARMANAGER_H_ */
