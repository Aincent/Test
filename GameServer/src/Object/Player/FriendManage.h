#ifndef FRIENDMANAGE_H
#define FRIENDMANAGE_H

#include "Lua/LuaInterface.h"
#include "Network/MessageHandler.h"
#include "ServerMsgDefine.h"
#include "MsgDefineMacro.h"
#include "../../QuickMessage.h"

using namespace CommBaseOut;

#define INVALID_GOURD_VALUE (-1)		//葫芦属性为该值时表示已领取

class FriendManage : public Message_Handler,public Request_Handler,public Ack_Handler
{
public:
	~FriendManage();

public:
	static FriendManage * GetInstance();

	void DestroyInstance();

	int getExpValueByLevel(short level);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

public:
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_REQBLESSFRIEND);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_RECEIVE_GOURD_EXP);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GET_GOURD_EXP);

	int getLevel();

protected:
	bool loadLuaFile();



	int getMaxGourdExp();

private:
	FriendManage();

private:
	static FriendManage * m_instance;

private:
	CLuaInterface m_Lua;
};

#endif
