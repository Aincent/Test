/*
 * Sector.h
 *
 *  Created on: 2013��10��15��
 *      Author: root
 */

#ifndef SECTOR_H_
#define SECTOR_H_

#include "../Object/Object.h"
//#include "../Object/Player/Player.h"

#include <map>
#include <vector>
#include "Smart_Ptr.h"
#include "define.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "../ServerOtherDefine.h"

using namespace std;
using namespace CommBaseOut;

class CreatureObj;
class Player;
class CGameZone;
class DropItemsAllot;
class GirlWar;
class Monster;
class RegionData;

class Sector
{
public:
	Sector(int index, CGameZone *pZone);
	Sector();
	~Sector();

	void Init(int index, CGameZone *pZone)
	{
		m_index = index;
		m_pZone = pZone;
	}

	void addRegion(int regionID,const RegionData* data);

	int GetIndex() { 	return m_index; }
	int GetGroupIndex() { 	return m_groupIndex; }

	void SetGroupIndex(int index) {	 m_groupIndex = index; }

	void SetSectorFlag(bool isvalid) { 	m_isValid = isvalid; }
	bool GetSectorFlag() { 	return m_isValid; }

	//格子是否已经存在掉落物了
	void SetHaveItem(bool isHaveItem) { m_isHaveItem = isHaveItem; }
	bool IsHaveItem(){ return m_isHaveItem; }

	//将对象加入格子
	int Join(Smart_Ptr<Object> & obj);

	void joinOrLeaveRegion(Smart_Ptr<Player>& player,Sector* leaveSector,bool isEnter);

	//将对象移出格子
	int Leave(Smart_Ptr<Object> & obj);

	//告诉所有的对象我来了
	void SendAllObject(Smart_Ptr<Object> & obj);
	//告诉所有的玩家我来了
	void SendAllPlayer(Smart_Ptr<Object> & obj);
	//告诉所有的怪物我来了
	void SendAllMonster(Smart_Ptr<Object> & obj);
	//告诉所有的掉落我来了
	void SendAllDrop(Smart_Ptr<Object> & obj);
	//告诉所有的战姬我来了
	void SendAllGirlWar(Smart_Ptr<Object> & obj);

	//告诉所有的对象我走了，side是否互发
	void SendObjectLeave(Smart_Ptr<Object> & obj, bool side = true);
	//告诉所有的玩家我走了，side是否互发
	void SendPlayerLeave(Smart_Ptr<Object> & obj, bool side = true);
	//告诉所有的玩家我走了，side是否互发
	void SendDropLeave(Smart_Ptr<Object> & obj);
	//告诉所有的怪物我来了
	void SendMonsterLeave(Smart_Ptr<Object> & obj);
	//告诉所有的战姬我走了
	void SendGirlWarLeave(Smart_Ptr<Object> & obj);
	//广播通用接口
	void SendSynchCommon(::google::protobuf::Message *from,int messageid,int key, bool isMyself);
	void SendSynchMessage(::google::protobuf::Message *from, int msgid, int key, bool isMyself = false);
	//通知所有的格子对象要移动了
	void SendSynchPath(PlayerSynch::PathToAllPlayer &path);
	void SendSynchPath(PlayerSynch::MonsterPathToPlayer &path);
	void SendSynchPath(PlayerSynch::GirlWarPathToPlayer &path);

	//通知所有的格子对象属性改变了
	void SendSynchAttr(CharBase::SynchObjectAttr &attr, bool isMyself);

	//广播buff改变
	void SendSynBuffChange(CharBattle::BuffOperate *buf, int key);

	//通知所有格子对象动作改变了
	void SendSynchAction(CharBattle::SynClientAttack &action);

	//获取所有玩家和怪物
	void GetCreatrueObj(vector<Smart_Ptr<CreatureObj> > &obj);
	//获取非同阵营的生物
	void GetCreatrueObj(int country,eObjType eType, vector<Smart_Ptr<CreatureObj> > &obj);
	//获取非同阵营的玩家
	void GetPlayer(int country, vector<Smart_Ptr<CreatureObj> > &obj);

	void GetPlayer(vector<Smart_Ptr<Player> > &obj);

	//获取坐标
	DWORD GetPosition();

	//是否可以掉落
	bool IsAllocDrop()
	{
		return IsHaveItem() ? false : true;
	}

	//判断是否有怪物
	bool IsMonster()
	{
		return ((m_monsterHashTable.size() > 0) ? true : false);
	}

	//获取格子里的某个怪物
	Smart_Ptr<Monster> GetSectorMonster(int64 id,int64 key);
	//获取格子里的某个玩家
	Smart_Ptr<Player> GetSectorPlayer(int64 id);

	template<class T>
	void foreach(T& operatorObj)
	{
		vector<Smart_Ptr<CreatureObj> > tCreatureList;
		GetCreatrueObj(tCreatureList);

		for(size_t i = 0;i < tCreatureList.size();++i)
		{
			operatorObj(tCreatureList[i]);
		}
	}

	CGameZone* getGameZone(){return m_pZone;}

	bool isCanDoing(eObjectDoing doing);

	bool isCanAddState(EAttackState state);

public:

	int m_index; // 格子编号
	WORD m_groupIndex; //格子组编号

	bool m_isValid; 		//是否是有效的格子
	bool m_isHaveItem;	//格子是否存在掉落物

	CGameZone * m_pZone;

	INT64PLAYERMAP   m_playerHashTable;
	INTVMPNSTERMAP   m_monsterHashTable;
	Smart_Ptr<DropItemsAllot> m_dropItem; //掉落
	INTGRILWARMAP m_girlWarHashTable;
	map<int,const RegionData*> m_RegionList;
};

class SectorGroup
{
public:
	SectorGroup(int index);
	SectorGroup();
	~SectorGroup();

	int GetGroupSectors(){ return m_groupSector.size(); }

	void AddNearbyGroup(SectorGroup * group);
	void AddGroupSector(Sector * pSector);

	int GetGroupIndex() { 	return m_index; }
	void SetGroupIndex(int index, CGameZone *pZone)
	{
		m_index = index;
		m_pZone = pZone;
	}

	//通知所有九宫格对象来了（只更新与group不相邻格子的地方）
	void SendAllObjectToGroup(Smart_Ptr<Object> & obj, SectorGroup *group);
	//通知所有的九宫格玩家新的玩家来了
	void SendAllPlayerToGroup(Smart_Ptr<Object> & obj, SectorGroup *group);
	//通知所有的九宫格玩家新的掉落来了
	void SendAllDropToGroup(Smart_Ptr<Object> & obj, SectorGroup *group);
	//通知所有的九宫格对象走了（只更新与group不相邻格子的地方）
	void SendObjLeaveToGroup(Smart_Ptr<Object> & obj, SectorGroup *group);
	//通知所有的九宫格对象走了
	void SendObjLeaveToGroup(Smart_Ptr<Object> & obj);
	//通知所有的九宫格玩家走了（只更新与group不相邻格子的地方）
	void SendPlayerLeaveToGroup(Smart_Ptr<Object> & obj, SectorGroup *group);
	//通知所有的九宫格掉落走了（只更新与group不相邻格子的地方）
	void SendDropLeaveToGroup(Smart_Ptr<Object> & obj, SectorGroup *group);

	//通知自己组的格子对象来了
	void SendAllObjectToMyself(Smart_Ptr<Object> & obj);
	//通知自己组的格子玩家来了
	void SendAllPlayerToMyself(Smart_Ptr<Object> & obj);
	//通知自己组的格子掉落来了
	void SendAllDropToMyself(Smart_Ptr<Object> & obj);
	//通知自己组的格子对象走了，side是否互相发送
	void SendObjLeaveToMyself(Smart_Ptr<Object> & obj, bool side = true);
	//通知自己组的格子玩家走了，side是否互相发送
	void SendPlayerLeaveToMyself(Smart_Ptr<Object> & obj, bool side = true);
	//通知自己组的格子掉落走了，side是否互相发送
	void SendDropLeaveToMyself(Smart_Ptr<Object> & obj);

	//广播通用接口
	void SendSynchCommon(::google::protobuf::Message *from,int messageid, int key, bool isMyself);
	void SendSynchCommonToMyself(::google::protobuf::Message *from, int messageid, int key, bool isMyself);

	void SendSynchMessage(::google::protobuf::Message *from, int msgid, int key, bool isMyself = false);
	void SendSynchMessageToMyself(::google::protobuf::Message *from, int msgid, int key, bool isMyself = false);

	//通知所有格子对象要移动了
	void SendSynchPath(PlayerSynch::PathToAllPlayer &path);
	void SendSynchPath(PlayerSynch::MonsterPathToPlayer &path);
	void SendSynchPath(PlayerSynch::GirlWarPathToPlayer &path);

	void SendSynchPathToMyself(PlayerSynch::PathToAllPlayer &path);
	void SendSynchPathToMyself(PlayerSynch::MonsterPathToPlayer &path);
	void SendSynchPathToMyself(PlayerSynch::GirlWarPathToPlayer &path);

	//通知所有格子对象的属性变化了
	void SendSynchAttr(CharBase::SynchObjectAttr &attr, bool isMyself);
	void SendSynchAttrToMyself(CharBase::SynchObjectAttr &attr, bool isMyself);

	//广播buff改变
	void SendSynBuffChange(CharBattle::BuffOperate *buf, int key);
	void SendSynBuffChangeToMyself(CharBattle::BuffOperate *buf, int key);

	//广播属性和状态
//	void SendSynchAttrState(CharBattle::ObjectState &attr);
//	void SendSynchAttrStateToMyself(CharBattle::ObjectState &attr);

	//通知格子对象的动作
	void SendSynchAction(CharBattle::SynClientAttack &action);
	void SendSynchActionToMyself(CharBattle::SynClientAttack &action);

	//是否是周围的组
	bool IsNearbyGroup(SectorGroup * group);

	void GetGroupObject(vector<Smart_Ptr<CreatureObj> > &obj);
	void GetGroupObjectToMyself(vector<Smart_Ptr<CreatureObj> > &obj);

	template<class T>
	void foreachAll(T& operatorObj)
	{
		for(size_t i = 0;i < m_nearbyGroup.size();++i)
		{
			m_nearbyGroup[i]->foreach(operatorObj);
		}
	}

	template<class T>
	void foreach(T& operatorObj)
	{
		for(size_t i = 0;i < m_groupSector.size();++i)
		{
			m_groupSector[i]->foreach(operatorObj);
		}
	}

private:

	WORD m_index; //格子组编号
	CGameZone *m_pZone;
	vector<Sector *> m_groupSector;//组内的格子
	vector<SectorGroup *> m_nearbyGroup; //周围的组
};



#endif /* SECTOR_H_ */
