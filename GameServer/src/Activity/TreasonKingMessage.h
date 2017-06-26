/*
 * TreasonKingMessage.h
 *
 *  Created on: 2014年11月25日
 *      Author: root
 */

#ifndef TREASONKINGMESSAGE_H_
#define TREASONKINGMESSAGE_H_

#include "define.h"
#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../QuickMessage.h"
#include "MessageStruct/Activity/TreasonKingInfo.pb.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class TreasonKingMessage : public Singleton<TreasonKingMessage>
{
public:
	TreasonKingMessage();
	~TreasonKingMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_TREASONKING_INFO);	//获取反王通关信息

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_TREASONKING_SOMETHING); //进行挖宝操作

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_TREASONKING_REWARD);	//获取奖励

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_TREASONKING_RESET);	//重置副本

	void DigTreasure(Smart_Ptr<Player>& player, TreasonKingInfo::FromClientDrop& fromClient, TreasonKingInfo::ToClientDrop& toClient); // 挖宝

};


#endif /* TREASONKINGMESSAGE_H_ */
