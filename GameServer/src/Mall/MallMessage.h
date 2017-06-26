/*
 * MailMessage.h
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#ifndef MALLMESSAGE_H_
#define MALLMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../QuickMessage.h"
#include "Singleton.h"
#include "../MsgCommonClass.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"


using namespace std;
using namespace CommBaseOut;

class MallMessageMgr:public Singleton<MallMessageMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MallMessageMgr();
public:
	~MallMessageMgr();

//	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_MALLINFO);		//请求商城信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_BUYGOODS);		//请求购买物品

//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_BUYLIMITGOODS);				//购买限时商品
//	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2GT_BUYLIMITGOODS);
//	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_BUYLIMITGOODS);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_MALL_LIMIT_BUY);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	int MallBuy(Smart_Ptr<Player>& player, int& want_id, int& want_num, int& buyType);
};


#endif /* MAILMESSAGE_H_ */
