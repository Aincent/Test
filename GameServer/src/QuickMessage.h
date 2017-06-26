/*
 * QuickMessage.h
 *
 *  Created on: 2014年4月22日
 *      Author: helh
 */

#ifndef QUICKMESSAGE_H_
#define QUICKMESSAGE_H_

#include "Singleton.h"
#include "define.h"
#include "./google/protobuf/message.h"
#include <map>
#include <vector>
#include "MessageStruct/QuickMessage.pb.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "./Object/Player/Player.h"

using namespace std;
using namespace CommBaseOut;


class SlotBase
{
public:
	virtual ~SlotBase() {};
	virtual void operator()(Safe_Smart_Ptr<Message> &message, string &content, Smart_Ptr<Player> &player) = 0;
};

template<typename T>
class MessageFunctionSlot : public SlotBase
{
public:
	typedef void (T::*MemberFunctionType)(Safe_Smart_Ptr<Message> &message, string &content, Smart_Ptr<Player> &player);

	MessageFunctionSlot(MemberFunctionType func, T* obj) :
	d_function(func),
		d_object(obj)
	{}

	virtual void operator()(Safe_Smart_Ptr<Message> &message, string &content, Smart_Ptr<Player> &player)
	{
		(d_object->*d_function)(message, content, player);
	}

private:
	MemberFunctionType d_function;
	T* d_object;
};

struct MessageBoundSlot
{
	Smart_Ptr<SlotBase> obj;

	MessageBoundSlot(Smart_Ptr<SlotBase> &tObj):obj(tObj)
	{

	}

	MessageBoundSlot()
	{

	}

	MessageBoundSlot & operator = (MessageBoundSlot & slot)
	{
		if(&slot == this)
			return *this;

		obj = slot.obj;

		return *this;
	}
};

class QuickMessage : public Singleton<QuickMessage>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	QuickMessage();
	~QuickMessage();

	//注册消息
	template<typename T>
	void RigsterMessage(T * obj, int msgid, void (T::*function)(Safe_Smart_Ptr<Message> &message, string &content, Smart_Ptr<Player> &player))
	{
		Smart_Ptr<SlotBase> tBase = new MessageFunctionSlot<T>(function, obj);
		MessageBoundSlot mSlot(tBase);

		m_messageSlot[msgid] = mSlot;
	}
	//回复消息
	void AckMessage(Safe_Smart_Ptr<CommBaseOut::Message> &message, google::protobuf::Message *content, int msgID);

	void AddSendMessage(int64 charid, int channel, google::protobuf::Message *content, int msgID);

	void UpdateMessage();

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_GT2GM_QUICKMESSAGE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_QUICKMESSAGE);
//	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2GM_ACCEPTQUEST);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:

	map<pair<int64, int>, vector<ServerMessage::ClientMessagePackage> > m_allMessage;
	map<int, MessageBoundSlot> m_messageSlot;
};

//o为类名 ，而且必须是单例类      t为消息id
#define DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(t) void On_MSG_QUICK_SIMPLE_##t(Safe_Smart_Ptr<CommBaseOut::Message> &message, string &content, Smart_Ptr<Player> &player)
#define DEF_MSG_QUICK_SIMPLE_REG_FUN(o, t) QuickMessage::GetInstance()->RigsterMessage(this, t, &o::On_MSG_QUICK_SIMPLE_##t);
#define DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(o,t) void o::On_MSG_QUICK_SIMPLE_##t(Safe_Smart_Ptr<CommBaseOut::Message> &message, string &content, Smart_Ptr<Player> &player)



#define DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(t) void On_MSG_QUICK_REQUEST_##t(Safe_Smart_Ptr<CommBaseOut::Message> &message, string &content, Smart_Ptr<Player> &player)
#define DEF_MSG_QUICK_REQUEST_REG_FUN(o, t) QuickMessage::GetInstance()->RigsterMessage(this, t, &o::On_MSG_QUICK_REQUEST_##t);
#define DEF_MSG_QUICK_REQUEST_DEFINE_FUN(o,t) void o::On_MSG_QUICK_REQUEST_##t(Safe_Smart_Ptr<CommBaseOut::Message> &message, string &content, Smart_Ptr<Player> &player)


//m为请求的消息        c为回复的protobuf指针     t为回给客户端的消息id
#define REQ_MSG_QUICK_ACK_FUN(m, c, t) QuickMessage::GetInstance()->AckMessage(m, c, t);


#endif /* QUICKMESSAGE_H_ */
