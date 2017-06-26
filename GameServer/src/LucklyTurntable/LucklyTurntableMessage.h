/*
 * LucklyTurntableMessage.h
 *
 *  Created on: 2014年8月20日
 *      Author: root
 */

#ifndef LUCKLYTURNTABLEMESSAGE_H_
#define LUCKLYTURNTABLEMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../Object/Player/Player.h"

using namespace CommBaseOut;
using namespace std;



class Player;

class LucklyTurntableMessage : public Singleton<LucklyTurntableMessage>
{
public:
	LucklyTurntableMessage();
	~LucklyTurntableMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_LUCKLYTURNTABLEINFO);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_REFULSH_LUCKLYTURNTABL_EREAW);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_ROLL_LUCKLYTURNTABL);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_GET_LUCKLYTURNTABL_EREAW);

};



#endif /* LUCKLYTURNTABLEMESSAGE_H_ */
