/*
 * PVPMgr.h
 *
 *  Created on: 2015年7月24日
 *      Author: root
 */

#ifndef PVPMGR_H_
#define PVPMGR_H_
#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"
#include "PVPDefine.h"
#include "Timer/TimerInterface.h"

class PVPMgr : public Singleton<PVPMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	PVPMgr();
	virtual ~PVPMgr();
public:

	//国际化表格序号
	int	getMailContentIndex(){return m_mailContentIndex;}

	//根据等级和副本层数得到奖励经验
	int getZoneExp(int lv,int layer);

	//得到副本经验奖励时间
	int getExpPrizeTime(){return m_expPrizeTime;}
	
	//采集时间
	int getGatherTime(){return m_gatherTime;}

	//楼层基础配置信息
	const PVPLayerInfo* getLayerInfo(int layer);

	//保护buffer
	int getProtectedBufferID(){return m_bufferID;}

	//通过楼层得到副本唯一id
	int64 getMapIDByLayer(int layer);

	int64 getEndTime(){ return m_beginTime; }
	void setEndTime(int64 time){ m_beginTime=time; }

	void GetBossDamageReward(int64 charid,int damagesort);

	void GetBossLastAttack(int64 charid,int layer);
protected:
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_PVP_GATHER);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_PVP_MAPS);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_ADD_ITEM);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_PVP_END);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_PVP_NOOPERATE);
		DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GS2WS_PVP_PLAYER_DATA);

		DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank);

		void loadConfig();
		void loadLua();
		void loadPrize();
		void loadLayerInfo();
		void loadmixInfo();
		void release();

		//消息处理
		virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:
		//－－－－－－－－－－－－－配置信息－－－－－－－－－－－－－
		CLuaInterface m_pLua;
		std::vector<PVPSortPrize> m_damagePrize; //boss伤害奖励
		std::vector<PVPSortPrize> m_killPrize;   //击杀排名奖励
		std::vector<PVPSortPrize> m_killNumsPrize;//击杀人数奖励
		std::map<int,PVPSortPrize> m_layerPrize; //楼层奖励
		std::vector<PVPSortPrize> m_lastAttckPrize;							 //boss最后一击奖励
		std::map<int,PVPLayerInfo> m_layerInfo;  //楼层信息

		int m_gatherTime;					//采集时间
		int m_expPrizeTime;			//经验奖励时间
		int m_mailContentIndex;	//邮件内容对应的国际化表格序号
		int m_bufferID;						//无敌保护buffer ID
		//------------------------------------------------------

		std::map<int,int64> m_pvpMaplist;		//所有的副本层数信息
		int64 m_beginTime;											//活动开始时间
};


#endif /* PVPMGR_H_ */
