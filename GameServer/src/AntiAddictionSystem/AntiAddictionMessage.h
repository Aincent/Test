/*
 * AntiAddictionMessage.h
 *
 *  Created on: 2014年10月28日
 *      Author: root
 */
#ifndef ANTIADDICTIONMESSAGE_H_
#define ANTIADDICTIONMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "MessageStruct/AntiAddiction/AntiAddictionProto.pb.h"

using namespace std;
using namespace CommBaseOut;

class AntiAddictionMessage:public Singleton<AntiAddictionMessage>//, public Message_Handler, public Request_Handler
{
public:
	AntiAddictionMessage();
	~AntiAddictionMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_BIND_ANTI_ADDICTION_ID);
	void SendToDBSaveBindAntiAddictionInfo(AntiAddictionProto::ToDBBindAntiAddictionID & reqDB,Smart_Ptr<Player> & player);

//	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
//	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	//virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
};

#endif /* ANTIADDICTIONSYSTEM_H_ */
