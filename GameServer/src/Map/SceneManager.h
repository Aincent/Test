/*
 * SceneManager.h
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <map>
#include "GameZone.h"
#include "Smart_Ptr.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "../Object/Player/Player.h"
#include "MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "Sector.h"
#include "GameDuplicate.h"
#include "MessageStruct/Daily/Daily.pb.h"

using namespace std;
using namespace CommBaseOut;

class CSceneMgr
{
public:
	~CSceneMgr();

	static CSceneMgr * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CSceneMgr();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	void AddMap(int64 id, Smart_Ptr<CGameZone> &newMap);

	void Update(int dwTick);

	//ɾ��
	void DestroySceneList();

	//֪ͨgateserver���й���ĵ�ͼ
	void SendAllMap(short int remoteid, unsigned char remoteType, int channelid, short int localid, unsigned char localType);

	// idΪgameserver��localid
	int InitMap(short int gsid, short int gtid);

	//ͬ�����Ա仯�����
	void SendSynchAttrChange(CharBase::SynchObjectAttr &attr, int64 sceneid, int xpos, int ypos, bool isMyself = false);
	//全图广播
	void SendAllMapAttr(CharBase::SynchObjectAttr &attr, int64 sceneid);

//	//广播属性和状态改变
//	void SendSynchAttrState(CharBattle::ObjectState &attr, int64 sceneid, int xpos, int ypos);

	//广播消息
	void SendSynchMessage(::google::protobuf::Message *from, int msgid, int64 sceneid, int key, int xpos, int ypos, bool isMyself = false);
	//全图广播
	void SendMapAllSynchMessage(::google::protobuf::Message *from, int msgid, int64 sceneid, int key, bool isMyself = false);

	//广播buff添加和删除
	void SendSynBuffChange(CharBattle::BuffOperate *buf, int key, int64 sceneid, int xpos, int ypos);

	//ͬ��ս�����������
	void SendSynchAttackAction(CharBattle::SynClientAttack &action, int64 sceneid, int xpos, int ypos);

	//��ȡ�Ź�����
	int GetSectorGroup(int64 mapid, int xpos, int ypos);

	//���볡��
	bool EnterScene(Smart_Ptr<Object> & obj);

	//�������ƶ�
	bool MoveTo(Smart_Ptr<Object> &obj);

	bool MoveToAll(Smart_Ptr<Object> &obj);

	//对象离开场景，isDel是否广播移出
	void LeaveScene(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);

	//��ȡ����
	Smart_Ptr<CGameZone> GetGameZone(DWORD64 sceneid);
	Smart_Ptr<CGameZone> GetGameZoneByMapID(int mapid);

	//获取格子
	Sector * GetSector(DWORD64 sceneid, int xpos, int ypos);

	//isMon判断是否需要添加采集怪的判断
	void GetObject(int64 mapid, int key, Smart_Ptr<CreatureObj> &obj, bool isMon = false);

	//得到技能攻击目标
	void GetSkillObject(Smart_Ptr<CreatureObj> &owner, int xpos, int ypos, Smart_Ptr<CreatureObj>& target, int targetType, int targetNUm, int type, int distance,int width, vector<Smart_Ptr<CreatureObj> > &obj);

	//获取一定范围内的生物
	void GetObject(Smart_Ptr<CreatureObj> &owner, int xpos, int ypos, int type, int distance,int width, vector<Smart_Ptr<CreatureObj> > &obj);
	//获取范围内的非同阵营的生物
	void GetObject(int64 mapid, int xpos, int ypos, int distance, int country,eObjType eType, vector<Smart_Ptr<CreatureObj> > &obj);

	//获取静态地图人数最少的场景id
	int64 GetSceneIDAboutPlayerCount(int mapid);

	int64 GetSceneIDAboutPlayerIsLoginOn(int mapid);
	//判断是否存在此场景
	bool IsHaveScene(int64 sceneid);
	//场景内两点的距离
	float GetDistance(int bXPos, int bYPos, int tXPos, int tYPos);
	//获取某个点旁边的点
	void GetAroundPos(int64 sceneid, int xpos, int ypos, int count, vector<Sector *> &vec);
	//获取绕过某个点的没有怪物的路径点
	void GetAroundPos(int64 sceneid, int xpos, int ypos, int expos, int eypos, vector<Pos> &vec);

	//获取9宫格内的玩家
	void GetGroupObject(Smart_Ptr<CreatureObj> &owner, vector<Smart_Ptr<CreatureObj> > &obj);

	//删除副本信息
	void DeleteInstance(int64 instanceid);
	//删除场景信息
	void DeleteScene(int64 sceneID);
	//更新路径点
	void UpdatePath( int64 sceneid, const PlayerSynch::SingleMonsterPath * path );

	//查看路径点是否有非法的
	bool IsValidPos(int64 sceneid, PlayerSynch::SynchInfo &pos);
	bool IsValidPos(int64 sceneid, int xpos, int ypos);
	//获取起点到终点是否有合法的路径
	bool HaveValidPos(int64 sceneid, int xpos, int ypos, int expos, int eypos);
	//	随机获得一个旁边可以走的点,true表示找到了，false没有找到
	bool RandGetValidPos(int64 sceneid, int xpos, int ypos, int &expos, int &eypos);
	//获取直线上能到达的最后一个点(从终点开始算)
	void GetFinalPosLine(int64 sceneid, int bxpos, int bypos, int expos, int eypos, int &xpos, int &ypos, int distance);
	//获取直线上能到达的最后一个点(从开始点开始算)
	void GetBeginPosLine(int64 sceneid, int bxpos, int bypos, int expos, int eypos, int &xpos, int &ypos, int distance);

	bool RandGetPetValidPos(int64 sceneid, int xpos, int ypos, int &expos, int &eypos);

	void AddWorldBossToZone(int key, DWORD64 sceneid, int monsterId);

	void RemoveBossFromZone(int key);

	void GetAllWorldBossInfo(vector<pair<int,pair<int64,int> > > &isActiveList, Smart_Ptr<Player>& player);

	Smart_Ptr<CGameZone> GetGameZoneByWorldBossKey(int key);

	void GetInstance(int64 instanceid,Smart_Ptr<CGameDuplicate>& instance);

	bool SendObjectToGroup(Smart_Ptr<Object> & obj);

	const WORLDBOSSZONEMAP& getWorldBossZone() const
	{
		return m_worldBossZone;
	}

private:
	CSceneMgr();

private:
	static CSceneMgr * m_instance;

	INT64CGAMEZONEMAP m_allZone;   //�� Ϊ����id

	WORLDBOSSZONEMAP m_worldBossZone; //世界BOSS出生所在的场景 boss的key -- CGameZone
};


#endif /* SCENEMANAGER_H_ */
