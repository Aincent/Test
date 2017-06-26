///*
// * WorshipMessage.h
// *
// *  Created on: 2015年3月25日
// *      Author: root
// */
//
//#ifndef WORSHIPMESSAGE_H_
//#define WORSHIPMESSAGE_H_
//
//#include "Singleton.h"
//#include "ServerMsgDefine.h"
//#include "Network/MessageHandler.h"
//#include "MsgDefineMacro.h"
//#include "../QuickMessage.h"
//
//using namespace CommBaseOut;
//
//class WorshipMessage: public Singleton<WorshipMessage>, public Request_Handler, public Ack_Handler
//{
//public:
//	WorshipMessage();
//	~WorshipMessage();
//
////	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_WORSHIP);
//
//	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GB_ADDBYWORSHIP);
//
//	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_CHECKWORSHIP);
//
//
//	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
//	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
//
//};
//
//#endif /* WORSHIPMESSAGE_H_ */
