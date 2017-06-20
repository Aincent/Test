/*
 * MonitorManager.h
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#ifndef MONITORMANAGER_H_
#define MONITORMANAGER_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;

class MonitorManager : public Singleton<MonitorManager>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MonitorManager();
	~MonitorManager();
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_MAPINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_MAPPLAYERINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_GETALLGAMESVRINFO);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

};

#endif /* MONITORMANAGER_H_ */
