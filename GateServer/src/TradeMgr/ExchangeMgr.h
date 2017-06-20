/*
 * ExchangeMgr.h
 *
 *  Created on: 2014.5.5
 *  Descript:玩家与玩家之间交易 的消息
 *      Author: cz
 */

#ifndef _EXCHANGEMGR_H_
#define _EXCHANGEMGR_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Singleton.h"

using namespace CommBaseOut;


class ExchangeMgr : public Message_Handler, public Request_Handler, public Ack_Handler, public Singleton<ExchangeMgr>
{
public:
	ExchangeMgr();

	~ExchangeMgr();

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_EXCHANGETOOTHER);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_LOCKINFOTOOTHER);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2C_BUILDEXCHANGE);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_CANCELTOOTHER);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_VERIFYTOOTHER);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_INITEXCHANGE);		//请求交易
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_OTHERSTATUS);			//trader的反馈请求
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_CANCELEXCHANGE);		//取消交易
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_EXCHANGELOCK);		//锁定交易
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_VERIFYEXCHANGE);		//确认交易


	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_INITEXCHANGE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_OTHERSTATUS);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_CANCELEXCHANGE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_EXCHANGELOCK);		//锁定交易
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_VERIFYEXCHANGE);		//确认交易

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);


};


#endif /* _ExchangeMgr_H_ */
