/*
 * MSOpreationMessage.h
 *
 *  Created on: 2014年11月3日
 *      Author: root
 */

#ifndef MSOPREATIONMESSAGE_H_
#define MSOPREATIONMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;

/*
 * 此文件 为管理服 相关操作 影响 在线玩家
 * 目前  为
 * */

class MSopreationManager : public Singleton<MSopreationManager>,
							public Message_Handler,
							public Request_Handler,
							public Ack_Handler
{
public:
	MSopreationManager();
	~MSopreationManager();

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GT_ABOUTTALKING);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

};



#endif /* MSOPREATIONMESSAGE_H_ */
