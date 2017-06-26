/*
 * ShopMessage.h
 *
 *  Created on: 2014.5.8
 *      Author: cz
 */

/******************************************/
//说明：商店的通讯模块
/*****************************************/

#ifndef SHOPMESSAGE_H_
#define SHOPMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../Object/Player/Player.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class ShopMessage : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~ShopMessage();

	static ShopMessage * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new ShopMessage();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	//
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_BUYBACK);


	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_BUYNPCITEM);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_SELLITEM);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_BUYBACK);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	//是否可以 NPC交易   eVipRight VIP作用枚举
	int IsCanNPChandle(Smart_Ptr<Player> &player, int npcid);

private:
	ShopMessage();

private:
	static ShopMessage * m_instance;
};


#endif /* ShopMessage_H_ */
