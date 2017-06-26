/*
 * BaseShopMessage.h
 *
 *  Created on: 2016年11月7日
 *      Author: root
 */

#ifndef BASESHOPMESSAGE_H_
#define BASESHOPMESSAGE_H_

#include "Network/MessageHandler.h"
#include "Singleton.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"

using namespace std;
using namespace CommBaseOut;

class BaseShopMessage : public Message_Handler, public Request_Handler, public Ack_Handler, public Singleton<BaseShopMessage>
{
public:
	BaseShopMessage();
	~BaseShopMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_BASESHOP_BUY); //请求商店购物
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_BASESHOP_MYSTER); // 玩家请求神秘商店数据
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_BASESHOP_FLUSH_MYSTER); // 玩家请求刷新神秘商店数据
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_BASESHOP_INFO);//玩家请求商店数据

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
};

#endif /* BASESHOPMESSAGE_H_ */
