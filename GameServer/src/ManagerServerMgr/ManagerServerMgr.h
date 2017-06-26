/*
 * ManagerServerMgr.h
 *
 *  Created on: 2015年2月5日
 *      Author: root
 */

#ifndef MANAGERSERVERMGR_H_
#define MANAGERSERVERMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include <regex.h>

using namespace CommBaseOut;
using namespace std;



class ManagerMessage : public Singleton<ManagerMessage>,
														public Request_Handler,
														public Message_Handler
{
public:
	ManagerMessage();
	~ManagerMessage();
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_FORBIDDEN);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_RECHARGE);
//	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_EXCHANGE_GIFT);
	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_EXCHANGE_GIFT);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_EXCHANGE_GIFT);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
private:
	void InitRegex(void);
private:
	regex_t reg; //先声明一个正则表达式体
};



#endif /* MANAGERSERVERMGR_H_ */
