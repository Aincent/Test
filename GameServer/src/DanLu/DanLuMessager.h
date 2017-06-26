/*
 * DanLuMessager.h
 *
 *  Created on: 2014年8月7日
 *      Author: root
 */

#ifndef DANLUMESSAGER_H_
#define DANLUMESSAGER_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../Object/Player/Player.h"

using namespace CommBaseOut;
using namespace std;



class Player;

class DanLuMessage : public Singleton<DanLuMessage>
{
public:
	DanLuMessage();
	~DanLuMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_DANLUINFO);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_EATDANLU);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_DANLUEQUIP);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_GETDANLUEQUIP);

};

#endif /* DANLUMESSAGER_H_ */
