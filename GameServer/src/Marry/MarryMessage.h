/*
 * MarryMessage.h
 *
 *  Created on: 2015年3月19日
 *      Author: root
 */

#include "define.h"
#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "MessageStruct/Marry/MarryMessage.pb.h"
#include "MessageStruct/Marry/MarryServer.pb.h"

using namespace std;
using namespace CommBaseOut;


class MarryMessage : public Singleton<MarryMessage>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MarryMessage();

	~MarryMessage();

public:
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

public:
	ServerRet checkSelfChangeMap(Smart_Ptr<Player>& player,Smart_Ptr<Player>& otherPlayer);

	void dealwithOtherChangeMap(int64 playerID,MarryServer::MarryChangeMap& returnMsg);

	//调整坐标点，防止两个玩家位置一样
	void adjustPos(CGameZone& gameZone,int& xpos,int& ypos);

public:
	//获取结婚奖励领取情况
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_GET_MARRY_REWARD);

	//送花扣除物品
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_BUCKLE_ITEM);

	//强制离婚扣钱
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_FORCE_DIVORCE_BUCKLE_GLODEN);

	//不在一条线上请求结婚对方地图信息
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS_GM_MARRY_GET_OTHER_MAP_INFO);

	//成功结婚
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_MARRY);

	//强制离婚给钱
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_FORCE_DIVORCE_GIVE_GLODEN);

	//离婚成功，清除数据
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_DIVORCE_SUCCESS);

	//请求婚戒信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_REQUEST_RING_INFO);

	//升阶
	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_RING_UP_STEP);

	//换图
	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_MARRY_CHANGE_MAP);

	//领取结婚奖励
	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_RECEIVE_REWARD);

	//换图返回
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_MARRY_CHANGE_MAP);
};





