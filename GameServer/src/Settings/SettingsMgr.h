/*
 * SettingsMgr.h
 *
 *  Created on: 2014年6月13日
 *      Author: root
 */

#ifndef SETTINGSMGR_H_
#define SETTINGSMGR_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "../MessageBuild.h"
#include "Smart_Ptr.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class SettingsMgr : public Singleton<SettingsMgr>, public Ack_Handler
{
public:
	SettingsMgr();
public:
	~SettingsMgr();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_SETTINGS);		//请求修改系统设置
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_BUG_SUBMIT);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_BUG_SUBMIT);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
};

#endif /* SETTINGSMGR_H_ */
