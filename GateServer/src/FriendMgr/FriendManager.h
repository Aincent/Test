/*
 * FriendManager.h
 *
 *  Created on: 2014年5月7日
 *      Author: root
 */

#ifndef FRIENDMANAGER_H_
#define FRIENDMANAGER_H_


#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;


class FriendManager : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~FriendManager();

	static FriendManager *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new FriendManager();
		}

		return m_instance;
	}

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_FINDFRIEND);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_OPENFRIEND);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_ADDFRIEND);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_DELETEFRIEND);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_SEEOTHERPLAYERINFO);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_RECOMMEND_FRIEND);


	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_CT2WS_OPENFRIEND);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_CT2WS_FINDFRIEND);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_RECOMMEND_FRIEND);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_CT2WS_ADDFRIEND);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_CT2WS_DELETEFRIEND);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_SEEOTHERPLAYERINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_ADDHATRED);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2CT_SYNUPDATE);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2C_SYNADDFRIEND);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GT_BLESSFRIEND);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2WS_SEEOTHERPLAYERINFO);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GM2GT_ADDHATRED);

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
	FriendManager();

private:
	static FriendManager * m_instance;

};

#endif /* FRIENDMANAGER_H_ */
