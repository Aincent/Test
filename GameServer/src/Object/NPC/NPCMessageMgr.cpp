/*
 * NPCMessageMgr.cpp
 *
 *  Created on: 2013��10��21��
 *      Author: helh
 */

#include "NPCMessageMgr.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../../MessageBuild.h"
#include "NPCMgr.h"

NPCMessageMgr::NPCMessageMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(NPCMessageMgr, MSG_REG_C2GM_NPCHOUSEINFO);

}

NPCMessageMgr::~NPCMessageMgr()
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(NPCMessageMgr, MSG_REG_C2GM_NPCHOUSEINFO)
{
	ServerMessage::ClientSingleMessage toClient;


	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_GT2GM_QUICKMESSAGE);
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_QUICKMESSAGE, message, &toClient);
//	Message_Facade::Send(messRet);
}

