/*
 * TradeMgr.h
 *
 *  Created on: 2014.5.5
 *  Descript:定义玩家与NPC还有玩家与玩家之间交易 的消息
 *      Author: cz
 */

#ifndef _TradeMgr_H_
#define _TradeMgr_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;


class TradeMgr : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~TradeMgr();

	static TradeMgr *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new TradeMgr();
		}

		return m_instance;
	}

//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_UPDATE_QUESTTARGET);
//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_SYNQUEST);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_BUYNPCITEM);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_SELLITEM);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_BUYBACK);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_BUYBACK);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

private:
	TradeMgr();

private:
	static TradeMgr * m_instance;

};


#endif /* _TradeMgr_H_ */
