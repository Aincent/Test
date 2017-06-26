/*
 * ContainerMessage.h
 *
 *  Created on: 2014年3月31日
 *      Author: root
 */

/******************************************/
//说明：容器的通讯模块
/*****************************************/

#ifndef CONTAINERMESSAGE_H_
#define CONTAINERMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../Object/Player/Player.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"

using namespace std;
using namespace CommBaseOut;

const int Comm_Pos_Num = -1;

class ContainerMessage : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~ContainerMessage();

	static ContainerMessage * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new ContainerMessage();
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

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_MOVEGOODS);
//	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_ADDBACKPACKSIZE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_DISCARDGOODS);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_SPLITGOODS);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_MOVEQUICKSECTOR);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_USEGOODS);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_WEAREQUIP);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_RECOVEREQUIP);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_SORTBACKPACK);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_ADDBACKPACKSIZE);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	inline bool CalContainerType(int type)
	{
		if(type < eArticleMax && type >=ePackageType)
			return true;
		else
			return false;
	}

private:
	ContainerMessage();
	void AddBackpackSize(Smart_Ptr<Player> &player,Goods::GTAckToCOpenGoodsRet &toClient,Goods::CReqToGTOpenGoodsRet &reqGM);
private:
	static ContainerMessage * m_instance;
};

#define GM_ContainerMessage ContainerMessage::GetInstance()

#endif /* CONTAINERMESSAGE_H_ */
