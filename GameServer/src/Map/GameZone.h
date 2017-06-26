/*
 * GameZone.h
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#ifndef GAMEZONE_H_
#define GAMEZONE_H_

#include <map>
#include "../Object/Player/Player.h"
#include "../Object/Monster/Monster.h"
#include "../Object/GirlWar/GirlWar.h"
#include "../Object/Spring/Spring.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "Sector.h"
#include "../Object/Object.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "../Object/NPC/NPC.h"
#include "../Object/Monster/Monster.h"
#include "MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include <vector>
#include "../MapInstance/InstanceMapLoader.h"
#include "../ServerOtherDefine.h"
#include "../Skill/Effect/EffectCommonData.h"

using namespace std;
using namespace CommBaseOut;
class ReflushManage;
class CreatureObj;

struct OldMapInfo
{
	int64 mapid;
	int xpos;
	int ypos;
};

class CGameZone
{
public:
	CGameZone(int64 sceneid);
	virtual ~CGameZone();

	virtual int Init();

	//初始化区块
	int InitField();

	//初始化npc
	int InitNpc();

	int InitRegion();

	void reflushMonster();

	//初始化monster
	int InitMonster();
	int CreateMonster(int monsterID,int counts,const vector<Pos>& posList,vector<int>* pKeyList = NULL);
	int CreateTypeMonster(MapMonster * aMonster);

	Smart_Ptr<Monster> CreateMonster(int monster, int posx, int posy);

	//初始化温泉
	int InitSpring();
	//用角色初始化monster
	int InitMonsterByPlayer(Smart_Ptr<Player> player, PlayerInfo::ArenaInfo &req);

	//生成坐标点范围内的格子
	bool GetSectorInPos(vector<Sector *> &mapSector, int xpos, int ypos, int distance);

	int64 GetSceneID()
	{
		return m_sceneID;
	}

	//计算所在的格子
	int GetSectorIndex(int xpos, int ypos);

	//更新场景
	void Update(int dwTick);
	//更新温泉
	void UpdateSpring(int dwTick);

	//计算所在的格子组
	int GetSectorGroupIndex(int xpos, int ypos);

	//计算2个点是否在同一9宫格内
	bool IsInSameNineGroup(int srcPosX, int srcPosY, int destPosX, int destPosY);

	//能否打坐
	bool IsCanMeditation(int xpos, int ypos);

	//获取坐标点的格子
	Sector * GetSector(int xpos, int ypos);

	//加入场景
	virtual int Join(Smart_Ptr<Object> & obj);
	//场景中移动
	virtual int Move(Smart_Ptr<Object> &obj);
	//场景中移动通知所有人
	virtual int MoveToAll(Smart_Ptr<Object> &obj);
	//离开场景，isDel是否广播移出
	virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);

	//广播通用接口
	void SendSynchMessage(::google::protobuf::Message *from, int msgid, int key, int xpos, int ypos, bool isMyself = false);
	//全图广播
	void SendMapAllSynchMessage(::google::protobuf::Message *from, int msgid, int key, bool isMyself = false);

	//同步玩家路径
	virtual void SendSynchPath(PlayerSynch::PathToAllPlayer &path, Smart_Ptr<Player> &player);

	//同步怪物路径
	virtual void SendSynchPath(PlayerSynch::MonsterPathToPlayer &path, Smart_Ptr<Monster> &monster);

	//同步宠物路径
	void SendSynchPetPath(PlayerSynch::GirlWarPathToPlayer &path, Smart_Ptr<GirlWar> &girlWar);

	//同步属性改变
	void SendSynchAttr(CharBase::SynchObjectAttr &attr, int xpos, int ypos, bool isMyself);
	//全图同步属性改变
	void SendAllMapAttr(CharBase::SynchObjectAttr &attr);

	//广播buff改变
	void SendSynBuffChange(CharBattle::BuffOperate *buf, int key, int xpos, int ypos);

	//同步动作改变
	void SendSynchAction(CharBattle::SynClientAttack &action, int xpos, int ypos);

	//获取npc
	Smart_Ptr<NPC> GetNpcByID(int npcid);

	const INTOBJECTMAP & GetAllObjects();

	//isMon判断是否需要添加采集怪的判断
	void GetObject(int key, Smart_Ptr<CreatureObj> &obj, bool isMon = false);
	//获取一定范围内的生物
	void GetObject(Smart_Ptr<CreatureObj> &owner, int xpos, int ypos, int type, int distance,int width, vector<Smart_Ptr<CreatureObj> > &obj);
	void GetObject(int xpos, int ypos, int distance, int country,eObjType eType, vector<Smart_Ptr<CreatureObj> > &obj);
	void GetPlayer(int xpos, int ypos, int distance, int country, vector<Smart_Ptr<CreatureObj> > &obj);

	//获取9宫格内的玩家
	void GetGroupObject(Smart_Ptr<CreatureObj> &owner, vector<Smart_Ptr<CreatureObj> > &obj);
	//获取旁边的格子
	void GetAroundPos(int xpos, int ypos, int count, vector<Sector *> &vec);
	//获取绕过某个点的没有怪物的路径点
	void GetAroundPos(int xpos, int ypos, int expos, int eypos, vector<Pos> &vec);
	//获取玩家
	Smart_Ptr<Player> GetPlayer(int64 charid)
	{
		INT64PLAYERMAP::iterator it = m_player.find(charid);
		if(it != m_player.end())
		{
			return it->second;
		}

		return 0;
	}

	//获取玩家的个数
	int GetPlayerCount()
	{
		return m_player.size();
	}

	int GetMonsterCount()
	{
		return m_allMonster.size();
	}

	int IsHaveMonster()
	{
		if(m_allMonster.size() > 0)
			return true;

		return false;
	}

	//获取本场景的所有玩家
	void GetWantPlayers(const uint &want_sz, std::vector<Smart_Ptr<Player> > &players);
	void GetAllPlayers(INT64PLAYERMAP &players);
	void GetAllPlayers(std::vector<Smart_Ptr<Player> > &players);

	//虚函数 ;
	virtual bool CanEnter(Smart_Ptr<Player> player) {return true;}

	//是否为副本
	bool isInstanced();

	//什么类型的地图
	EMapType GetInstancedType();

	//是否可以销毁地图
	virtual bool CanUnload(DWORD diff) {return false;}
	//可用于副本清除所有玩家
	virtual void UnloadAll() {};

	int64 GetInstanceID() { if(isInstanced()) return m_sceneID; return 0;}

	string GetMapName();

	//更新路径点
	void UpdatePath( const PlayerSynch::SingleMonsterPath * path );

	//旁断两点之间是否能直接通过
	bool IsValidPos(int bxpos, int bypos, int expos, int eypos);
	bool IsValidPos(PlayerSynch::SynchInfo &pos);
	bool IsValidPos(int xpos, int ypos);
	bool HaveValidPos(int xpos, int ypos, int expos, int eypos);
	bool RandGetValidPos(int xpos, int ypos, int &expos, int &eypos);


	void GetFinalPosLine(int bxpos, int bypos, int expos, int eypos, int &xpos, int &ypos, int distance);
	void GetBeginPosLine(int beginX,int beginY,int endX,int endY,int& resultX,int& resultY,int distance);

	bool RandGetPetValidPos(int xpos, int ypos, int &expos, int &eypos);

	//动态创建怪物
	Smart_Ptr<Monster> CreateMonsterDymic(DWORD monsterId,DWORD xPos,DWORD yPos, Smart_Ptr<Player> owner = NULL);
	Smart_Ptr<Monster> CreateMonsterDymic(DWORD monsterId,DWORD xPos,DWORD yPos, int hp, Smart_Ptr<Player> owner = NULL);

	//是否可以掉落
	bool IsAllocDrop(int sector)
	{
		return m_totalSector[sector].IsAllocDrop();
	}

	//判断某个格子是否有怪物
	bool IsHasMonster(int index)
	{
		if(m_totalSector[index].IsMonster())
		{
			return true;
		}

		return false;
	}

	//判断世界BOSS是否存活
	void IsActiveWorldBoss(int key, vector<pair<int,pair<int64,int> > > &isActiveList, int playerLevel = 0);

	int GetAllMonsterCount();

	bool IsAllMonsterDead();

	int SendObjectToGroup(Smart_Ptr<Object> & obj);

	Smart_Ptr<Object> GetObjectByKey(int key)
	{
		INTOBJECTMAP::iterator itr = m_allObject.find(key);
		if(itr != m_allObject.end())
		{
			return itr->second;
		}

		return 0;
	}

	void SetMonsterDead(int id);
	void SetAllMonsterDead();
	void AddObject(int nKey,Smart_Ptr<Object> object);
	void AddMonster(int id, vector<Smart_Ptr<Monster> > & vec);

	const INTOBJECTMAP& getAllObject() const
	{
		return m_allObject;
	}

	SectorGroup* getGroupByIndex(int index);

	bool isCanDoing(eObjectDoing doing,int x,int y);

	bool isCanAddState(EAttackState state,int x,int y);

	void afterLeave(Smart_Ptr<Player> player);

	//保存进入玩家的上次场景和坐标
	void SetOldMapPoint(int64 charid, int64 mapid, int xpos, int ypos);

	const OldMapInfo * GetOldMapPoint(int64 charid);

	void removeOldPoint(int64 charid);

protected:
	INTVMPNSTERMAP GetMapMonster() { return m_allMonster; }
	Smart_Ptr<Monster> GetMonsterByID(int id);

protected:

	INT64PLAYERMAP m_player; //此场景中的玩家       gs上的玩家唯一id   －    玩家
	INTNPCMAP m_npc; // 此场景中的npc   npcid - npc
	INTVMPNSTERMAP m_allMonster; // 此场景中的所有monster  monsterid - monster
	INTOBJECTMAP m_allObject; //此场景中的所有的对象  key - object
	vector<Smart_Ptr<Spring> > m_allSpring; //所有的加血点
	INTGRILWARMAP m_girlWar; //此场景中所有的战姬，key - object

	int64 m_sceneID;

	int m_totalGroupSize; // 格子组的个数

	Sector * m_totalSector;
	SectorGroup * m_totalGroup;
	int m_springTick;
	Smart_Ptr<ReflushManage> m_ReflushManage;	//该指针有可能为NULL
	map<int64,OldMapInfo> m_oldMapPoint; 		//玩家原来的点
};

#endif /* GAMEZONE_H_ */
