/*
 * ArenaMessage.h
 *
 *  Created on: 2016年5月4日
 *      Author: root
 */

#ifndef ARENAMESSAGE_H_
#define ARENAMESSAGE_H_

#include "Singleton.h"
#include "Common.h"
#include "MsgDefineMacro.h"
#include "../QuickMessage.h"

using namespace CommBaseOut;

class ArenaMessage : public Singleton<ArenaMessage>, public Request_Handler, public Ack_Handler, public Message_Handler
{
public:
	ArenaMessage();
	~ArenaMessage();

	void Handle_Request(Safe_Smart_Ptr<Message> &message);
	void Handle_Ack(Safe_Smart_Ptr<Message>& ack);
	void Handle_Message(Safe_Smart_Ptr<Message> &message);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ARENAINFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_CHANCECDTIME);	//取消竞技场冷脚时间
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ADDCHALLENGETIMES);	//增加挑战次数
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_RANKINGREWARD);	//领取排名奖励

	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REG_GM2WS_SYNCRANKINGREWARD);	//同步排行领奖数据
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_ARENAINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_SYNCARENA);

};

#endif /* ARENAMESSAGE_H_ */
