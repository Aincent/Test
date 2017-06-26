/*
 * MoneyInstanceMgr.h
 *
 *  Created on: 2015年9月11日
 *      Author: root
 */

#ifndef MONEYINSTANCEMGR_H_
#define MONEYINSTANCEMGR_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "MoneyInstanceDefine.h"

using namespace std;
using namespace CommBaseOut;

class MoneyInstanceMgr : public Singleton<MoneyInstanceMgr>, public Ack_Handler, public Request_Handler, public Message_Handler
{
public:
	MoneyInstanceMgr();
	~MoneyInstanceMgr();
public:
		int getMosterMoney(int type, int lv);
		int getMonsterAttack(int lv, int type, int node);
		int getMonsterHP(int lv, int type, int node);
		int getTowerScore(int monType, int killType);
		int getTowerCoolValue(int monType, int killType);

		//根据玩家等级得到塔的攻击 防御 生命
		int getTowerAttack(int lv, int param);
		int getTowerDefene(int lv, int param);
		int getTowerHP(int lv, int param);

		//根据玩家等级及塔的等级得到塔的属性
		void getTowerAttr(int lv, const TowerInfo* pInfo, int& attack, int& defene, int& hp);

		//得到塔的最大等级
		int getTowerMaxLevel();

		//得到刷怪波次及信息
		int getNodeType(int node);
		const FlushNodeInfo* getMonsterNodeInfo(int node)const;
		vector<Pos> GetPos(int index, int type);

		const TowerInfo* getTowerInfo(int lv)const;
		const AttrBuyInfo* getAttrBuyInfo(int count)const;
		const MoneyInstanceConfig& getConfig(){ return m_config;}

private:
		void loadFile();
		void loadTowerInfo();
		void loadAttrBuyInfo();
		void loadLimitInfo();
		void loadLua();

		virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_BUY_ITEM);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_USESKILL);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_BUY_ATTR);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_FITCHPRIZE);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_RANK);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_EXIT);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_MONEYINSTANCE_BUY_COUNT);

		DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GS2WS_MONEYINSTANCE_RANK);

private:
		std::map<int,TowerInfo> m_TowerInfo;
		std::map<int,AttrBuyInfo> m_BuyAtrrInfo;
		std::map<int,FlushNodeInfo> m_NodeInfo;
		vector<vector<Pos> > m_reflushPoint; 	//小怪刷怪点以及信走点
		vector<vector<Pos> > m_BossPoints;			//BOSS刷新点及行走路径
		MoneyInstanceConfig m_config;
		CLuaInterface m_pLua;
};



#endif /* MONEYINSTANCEMGR_H_ */
