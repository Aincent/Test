/*
 * WorldBoss.h
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#ifndef WORLDBOSS_H_
#define WORLDBOSS_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MessageBuild.h"
#include "Smart_Ptr.h"
#include <map>
#include "PVPDefine.h"

using namespace std;
using namespace CommBaseOut;


class Player;
class CreatureObj;
struct sAttackBossInfo;

struct WorldBossUpdate
{
	WorldBossUpdate() : m_BossID(0),m_Time(0),m_X(0),m_Y(0){}
	int m_BossID;
	int m_Time;
	int m_X;
	int m_Y;
};

enum eBossType
{
	eBossFieldType = 0,
	eBossWorldType = 1,
};

struct WorldBossConfig
{
	WorldBossConfig()
	{
		mapid = 0;
//		m_Type = eBossFieldType;
		countLimit = 0;
//		fitchTime = 0;
	}
	int mapid;				//地图id
//	eBossType m_Type;
	int countLimit;  //人数限制
//	int fitchTime;		//自动领取时间
	std::vector<PropItem> attackPrize;		//最后一击奖励
	std::vector<PVPSortPrize> sortPrize;    //排名奖励
	vector<PropItem> m_PartakePrize;		//参与奖励
	WorldBossUpdate m_UpdateTime;
};

class WorldBossMgr : public Singleton<WorldBossMgr>, public Message_Handler
{
public:
	WorldBossMgr();
public:
	~WorldBossMgr();

	void Handle_Message(Safe_Smart_Ptr<Message> &message);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_WORLDBOSS); //请求世界BOSS信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_WORLDBOSS_FITCH);//Boss副本领取奖励
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_WORLDBOSS_UIHLSTATE); // 世界bossUI高亮状态
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_NOTICEBOSS_OPENCNT); // 世界Boss开启数量
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_W2GM_WORLDBOSS_OVER); 
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_WORLDBOSS_DAMAGE_LIST); //客户端请求boss伤害排名

	//是否有存活的世界BOSS
	int WorldBossMgr_IsHaveAliveBoss(Smart_Ptr<Player>& player);

	bool HandleMonsterReflush(const EventArgs & e);

	bool HandleMonsterDead(const EventArgs & e);

	// 更新UI高亮的世界boss
	void UpdateWorldBossUIData(int monsterID, bool flag);

	//根据离开BOSS副本次数得到冷却时间
	int GetCoolTime(int count);

	//根据排名得到奖励
	void GetSortPrize(int bossID, int rank, std::vector<PropItem>& items);

	//根据最后一击得到奖励
	void GetLastAttackPrize(int bossID, std::vector<PropItem>& items);

	//得到某个副本的人数上限
	int GetMaxPlayer(int mapid);

	const map<int,WorldBossConfig>& getConfig(){return m_mapConfigInfo;}

	int getBossUpdateTime(int bossID);	

	const WorldBossConfig* getOneConfig(int bossID);	
	const vector<int>& getBossBloodPrompt(){return m_BossBloodPercents;}

private:
	void LoadConfig();
	void LoadPrize();

private:
	int m_nCoolTime;

	EventConnection m_monsterReflushEvent;

	EventConnection m_monsterDeadEvent;

	map<int,sAttackBossInfo> m_bossDemageInfo;
	//配置信息
	CLuaInterface m_pLua;
	map<int,WorldBossConfig> m_mapConfigInfo;
	vector<int> m_BossBloodPercents;
};

#endif /* WORLDBOSS_H_ */
