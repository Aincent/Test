/*
 * CountryRedEnvelopeMessage.h
 *
 *  Created on: 2015年4月9日
 *      Author: root
 */

#ifndef COUNTRYREDENVELOPEMESSAGE_H_
#define COUNTRYREDENVELOPEMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"

using namespace std;
using namespace CommBaseOut;

class CountryRedEnvelopeMessage : public Singleton<CountryRedEnvelopeMessage>,
public Ack_Handler,public Request_Handler,public Message_Handler
{
public:
	CountryRedEnvelopeMessage();
	~CountryRedEnvelopeMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GET_REDENVELOPE);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_SEND_REDENVELOPE);

	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_GET_REDENVELOPE);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_SEND_REDENVELOPE);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_GET_REWARD);

//	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_ISHAVEENVELOPETOGET);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_AllGMADDENVELOPE);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_PLAYERADDENVELOPE);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_ENVSENDERBYGET);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
};


#endif /* COUNTRYREDENVELOPEMESSAGE_H_ */
