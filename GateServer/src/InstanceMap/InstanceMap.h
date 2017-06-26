/*
 * InstanceMap.h
 *
 *  Created on: 2014年6月5日
 *      Author: root
 */

#ifndef INSTANCEMAP_H_
#define INSTANCEMAP_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;


class InstanceMap : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~InstanceMap();

	static InstanceMap *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new InstanceMap();
		}

		return m_instance;
	}

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_CREATEINSTANCE);

	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_CREATEINSTANCE);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_REQ_GM2GT_ENDINSTANCE);					//发送奖励
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_REQ_GM2GT_DELETEINSTANCE);			//场景服请求删除副本

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
	InstanceMap();

private:
	static InstanceMap * m_instance;

};



#endif /* InstanceMap */
