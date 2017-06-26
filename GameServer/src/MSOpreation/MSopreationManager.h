/*
 * MailManager.h
 *
 *  Created on: 2014年6月4日
 *      Author: root
 */

#ifndef MAILMANAGER_H_
#define MAILMANAGER_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "MessageStruct/MSProtoc/MS_OpreationSych.pb.h"

class Player;

using namespace CommBaseOut;

/*
 * 此文件 为管理服 相关操作 影响 在线玩家
 * 目前  为
 * */
class MSopreationManager : public Singleton<MSopreationManager>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MSopreationManager();
	~MSopreationManager();
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_ABOUTTALKING);


	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_MS2GM_BRANCHONLINECOUNT);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_MS2GM_BRANCHONLINEPLAYERINFO);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	int MSDoSomeThing(const Smart_Ptr<Player> &player, MS_SychProto::SychGMThig &info);
};


#endif /* MAILMANAGER_H_ */
