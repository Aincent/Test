/*
 * PlayerMessage.h
 *
 *  Created on: 2013��9��29��
 *      Author: helh
 */

#ifndef PLAYERMESSAGE_H_
#define PLAYERMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
//#include <map>
//#include "Player.h"
#include "Timer/TimerInterface.h"    //
#include "../../QuickMessage.h"
#include "EventSet/EventSet.h"
#include "../../ServerOtherDefine.h"
#include "MessageStruct/MSProtoc/MS_BranchPlayerOnlineInfo.pb.h"

using namespace std;
using namespace CommBaseOut;

//定义一个玩家缓存
struct PLayerCache
{
	bool bFlag;
	DWORD64 time;     //记录时间
	Smart_Ptr<Player>  playerCache;

	PLayerCache():bFlag(false),time(0),playerCache(NULL)
	{

	}
};

class Player;

class PlayerMessage :
		public Message_Handler,
		public Request_Handler,
		public Ack_Handler
{
public:
	~PlayerMessage();

	static PlayerMessage * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new PlayerMessage();
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

//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_CLIENTIN);
//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_SYNPATH);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_SYNCHPOS);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_SYNCHPOS);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_REVIVAL);
	//DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_UPDATE_CHANNEL);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_MONITOR);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_PLAYEREXIT);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_CLIENTIN);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_PLAYERINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_CHARATTR);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_CHANGEMAP);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_PLAYER_ONLINE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_CLIENTIN);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_PLAYERINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GS2DB_SAVE_PLAYERDATA);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2GT_CHANGEMAP);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_CONTAINERINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_SEEOTHERPLAYERINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2GT_PLAYER_ONLINE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_GET_WS_DATA);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_CHANGEPKTYPE);		//切换PK模式
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GATHER);		//开始采集
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_FIYING_SHOES);		//飞鞋

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GET_ATTR);//获取属性
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_REG_WS2GM_SYNCRANKINGREWARD); //同步排行领奖数据
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_SYNCHATTR);///WorldServer 同步玩家数到 GameServer
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_AD_SKILLBUFF); //帮派技能增删
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_MAKERT_INFO);//请求市场信息
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_CHANGE_EARNINGS);/// 增加收益
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_UP_CARRIAGES);//商品上架
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_SYSREWARD);//同步奖励
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_ACTIVITY_INVESTCOST);	///扣除投资金钱

//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_ISHAVECOMPENSATETOGET);//告诉玩家有补偿活动

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_ARENABECHALL); // 同步竞技场被挑战者的排名

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_SYNCHATTRCHAR);///WorldServer 同步玩家数到 GameServer

	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_UPDATEMARKET_UISTATE); // 更改市场UI状态

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_NOTICESELLERUI); // WS 通知GM出售者UI状态

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_JUMP_POINT); //请求跳点

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_DOWN_POINT); //请求下落

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_REFRESHARENA);

	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_REFRESHARENA);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_58PLAY_SYSREWARD);//同步奖励

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_CLICK_PLAYER_HEAD);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_SIM_C2GM_CHANGE_TASK_MAP); 

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);


	void addSendDataCach(int64 charID, SaveDataInfo& sendData);
	void dealWithSaveResult(SaveDataInfo& data);

	const INT64PLAYERMAP& getAllPlayer(){return m_allPlayer;}
	INT64PLAYERMAP& getAllPlayerRef(){return m_allPlayer;}

	inline void GetPlayerByCharid(const int64 &id, Smart_Ptr<Player> &player)
	{
		INT64PLAYERMAP::iterator it = m_allPlayer.find(id);
		if(it != m_allPlayer.end())
		{
			player = it->second;
		}
	}

	inline void GetPlayerCacheByCharid(const int64 &id, Smart_Ptr<Player> &player)
	{
		INT64PLAYERCACHEMAP::iterator it = m_PlayerCache.find(id);
		if(it != m_PlayerCache.end())
		{
			player = it->second.playerCache;
		}
	}

	void GetPlayerByKey(int key, Smart_Ptr<Player> &player)
	{
		INT64PLAYERMAP::iterator it = m_allPlayer.begin();
		for(;it!=m_allPlayer.end();++it)
		{
			if(it->second->GetKey() == key)
			{
				player = it->second;
				break;
			}
		}
	}

	template<class object>
	void foreach(object& obj)
	{
		INT64PLAYERMAP::iterator it = m_allPlayer.begin();
		for(;it != m_allPlayer.end();++it)
		{
			obj(it->second);
		}
	}

	//是否在相同9宫格内
	bool isInSameGroupSector(Smart_Ptr<Player>& player, int64 mapid, int xPos, int yPos);

	//当前地图是否可以小飞鞋传送
	int isCanFly(int mapID, int64 xPos, int64 yPos,int nLv);

	//玩家是否可以进入场景
	bool IsLoginON(Smart_Ptr<Player> &player,int flag);
	//玩家是否在线
	bool IsOnline(int64 charid);
	//玩家是否在线（判断真正在场景中）
	bool IsOnlineEx(int64 charid);

	//定时从玩家缓存中移除延时玩家
	void RemoveDelayPlayer(void* arg);
	//定时刷新玩家战斗状态
	void FlushPlayerFightState(void* arg);
	int64 GetCharidByName(string name);
	//全局    每分钟 定时操作
	void FlushPlayerOnlineState(void* arg);
	void FlushPlayerOnlineStateS(void* arg);
	void FlushPlayerOnlineState();
	//向管理服 报告在线人数
	void ReportPlayerOnlineCountToMS();
//	// 向管理服 报告所有玩家在线时长
//	void ReportPlayerOnlineTimeToMs();
	//更新日常任务
	void updateDaily();

	bool PlayerLogout(Smart_Ptr<Player> pObj);

	//9宫格内传送
	void ChangeInSameGroup(Smart_Ptr<Player> &player, int id, int xPos, int yPos);

	//把玩家强制传送(必须在发送前验证x,y坐标和场景)
	void FastChangeMap(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player> &player, int id, int xPos, int yPos, bool activityNeed = false);

	//把玩家强制传送(必须在发送前验证x,y坐标和场景)--没有ACK返回
	int FastChangeMap(Smart_Ptr<Player> &player, int id, int xPos, int yPos, bool activityNeed = false);
	//只作为服务器强制客户端传送使用
	void ForceChangeMap(Smart_Ptr<Player> &player, int64 sceneid, int xpos, int ypos);
	//换线Paramf 场景ID或者地图ID Params 切图类型（或者包含坐标）列：(txpos << 48) | (typos << 32)) | eSceneType
	void ChangeMapLine(Safe_Smart_Ptr<CommBaseOut::Message> &message,Smart_Ptr<Player> &player,int64 Paramf,int64 Params);
	//得到所有玩家的ID
	void getAllCharId(vector<int64> &allCharId);
	void UpdateWorldBoss(int bossKey, int isActive=0);
	//离开场景
	void PlayerLeave(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player>& player, int64 sceneid, int xpos, int ypos,bool isOver = false);
	void PlayerLeaveTask(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player>& player, int64 sceneid, int xpos, int ypos);

	//更新 每次次数
	void UpdateDayCounter(vector<int> &vec);

	void GetPlayerByCharId(Smart_Ptr<Player> &player, int64 charId);
	//假如身上带有NPC刷出的任务怪，直接让玩家完成任务
//	bool ClearMosterTask(const EventArgs & e)
	uint GetOnlinePlayerCount(){	return m_allPlayer.size();}

	void MS_GetBranchOnlinePlayerInfo(const int &begin, const int&end, BranchPlayerInfo::AckBranchPlayerInfo *info);

	void InitServerTime();
	void ResetOnliePlayerInfo();
	void ResetOnLinePlayerInfoAfterResetTime();

	void onDateTime(void* arg);
	void eightHoursTime(void* arg);

	//模糊查询 在线 玩家
	void FuzzySelectPlayerInfo(const string &str, const uint &needNum, list<Smart_Ptr<Player> > &fuzzy_list);

	void AllPlayerExitByChannel(int channelID);
	//完成副本活跃度
	void FinshVitality(Smart_Ptr<Player> &player,int mapid);
	int64 GetEightHoursTime()
	{
		return m_eightHoursTime;
	}

	void FinishiInit(Smart_Ptr<Player> &player,Safe_Smart_Ptr<NullBase> &act);

	void getWBaseInfo(CharLogin::WBaseInfo& wBaseInfo,Smart_Ptr<Player> player,bool isOnLine);

//	int RefreshCostCheck(Smart_Ptr<Player> player, int& money, int& golden, int& bindGolden);

	void HandleWSAckData(Smart_Ptr<Player>& player, CharLogin::GMGetWSAckData& meContent, Safe_Smart_Ptr<NullBase> &act);
private:

	PlayerMessage();

	//普通地图切换
	void ChangeNormal(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, const MapStartPoint& info);

	//切入副本
	void ChangeInDuplicate(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, int64 secneid);

	//切出副本
	void ChangeOutDuplicate(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message);

	//开始换服换线
	void ChangeServer(Smart_Ptr<Player>& player, CommBaseOut::Safe_Smart_Ptr<CommBaseOut::Message> & message, int64 secneid);

	//能否进入副本
	bool IsCanEnterDuplicate(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<CGameDuplicate>& instance, const InstanceMapEntry* instanceInfo);

	//进入副本时离开当前地图
	void LeaveCurSecne(Smart_Ptr<Player>& player, Safe_Smart_Ptr<CommBaseOut::Message> &message, int64 secenid, Smart_Ptr<CGameDuplicate>& instance, const InstanceMapEntry* instanceInfo);

private:
	static PlayerMessage * m_instance;

	TimerConnection m_timerID;
	TimerConnection m_flushID;
	TimerConnection m_ervery_min_flushID;  //每分钟操作定时器
	TimerConnection m_dayTimerID;
	TimerConnection m_nextDaytime;
	TimerConnection m_eightHoursTimer;				//八小时定时器
	TimerConnection m_ervery10SecondFlush; // 每分钟定时器
	INT64PLAYERMAP m_allPlayer;
	INT64PLAYERCACHEMAP m_PlayerCache;
	std::map<int64,SaveDataInfo> m_saveDataList;//玩家信息保存临时缓存数据SaveDataInfo& sendData

	int64 m_eightHoursTime;		//八小时更新点
};


#endif /* PLAYERMESSAGE_H_ */
