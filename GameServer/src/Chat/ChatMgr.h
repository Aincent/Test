/*
 * ChatMgr.h
 *
 *  Created on: 2014年5月20日
 *      Author: root
 */

#ifndef CHATMGR_H_
#define CHATMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "EventSet/EventSet.h"
#include "../EventSet/EventDefine.h"
#include "CharDefine.h"
#include "../Dependence/MessageStruct/CharLogin/Chat.pb.h"
#include "../Object/Player/Player.h"

using namespace CommBaseOut;

class ChatMgr : public Singleton<ChatMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	ChatMgr();
	~ChatMgr();
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_CHAT);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_CHAT);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_WORLDCHATVITALITY);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	//跑马灯
	void Marquee(EMarqueeChannel type,string paramf, string params ,string  paramt);
	//组队系统消息
	void chatRanksSystem(const string &str , const int64 &RanksId);
	//int转化string
	string GetStringByInt(int value);

	bool KillMonster(const EventArgs & e);

	// CD时间是否到了
	bool IsCDEnd(Smart_Ptr<Player> player, int chatType);

	void OrdinaryChat(Smart_Ptr<Player>& player, ChatInfo::EndChatInfo &ret);

	void WorldChat(int64 charid,ChatInfo::EndChatInfo &end);

	bool ChatCostGolden(Smart_Ptr<Player> player, int propID);

private:
	EventConnection m_killMonsterEvent;
};


#endif /* CHATMGR_H_ */
