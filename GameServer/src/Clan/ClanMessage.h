/*
 * ClanMessage.h
 *
 *  Created on: 2014年9月30日
 *      Author: helh
 */

#ifndef CLANMESSAGE_H_
#define CLANMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../QuickMessage.h"
#include "MessageStruct/Clan/Clan.pb.h"
#include "../MsgCommonClass.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class ClanMessage : public Singleton<ClanMessage>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	ClanMessage();
	~ClanMessage();

	//加属性
	void AddAttr(const Smart_Ptr<Player> & player, int blv, int elv,bool isSendEvend = true);
	//剪掉属性
	void DeleteAttr(const Smart_Ptr<Player> & player, int lv);

	void onlineCalculateAtt(Smart_Ptr<Player>& player,BYTE currentLevel);

	//计算等级加的战斗力
	int calculateClanFight(BYTE level,const Smart_Ptr<Player>& player);

	void clanDataAlter(Smart_Ptr<Player>& player,ClanPackage::SynchAttrToGM& data,vector<int>& selfAtt,vector<int>& broadcastAtt,bool isSend);

	void clearClanData(Smart_Ptr<Player>& player,vector<int>& selfAtt,vector<int>& broadcastAtt,bool isSend);

	void KickOutClanMap(Smart_Ptr<Player> &player);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_TACK_OFF);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_CHANGE_ATTR);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_COST_CONTRIBUTION);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_SHOP_GOODS);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_SHOP_SUCCESS);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_CLANWARBEGIN);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_REQ_WS2GM_SYNWILLJOIN);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_INTEGRAL_SYSRESINGUP);//积分战补报扣元宝

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_INTEGRAL_KILLRANK_INFO); //查看击杀排名

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_CLANWELFARECOST);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_TAKECLANWELFARE);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_DESTROY_CLAN_MAP);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_ACTIVE_START_END);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_MAKE_GODTREE_ITEM);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_W2GM_BUCKLE_ITEM);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_OCCUPY_FLAG);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GS2WS_FACTION_NAME);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	int CreateClanByItem(const Smart_Ptr<Player> &player);

	void makeItemTest(Smart_Ptr<Player> player,int level);

};




#endif /* CLANMESSAGE_H_ */
