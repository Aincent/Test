/*
 * MoneyShopMessage.h
 *
 *  Created on: 2014年8月9日
 *      Author: root
 */

#ifndef MONEYSHOPMESSAGE_H_
#define MONEYSHOPMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../Object/Player/Player.h"

using namespace CommBaseOut;
using namespace std;



class Player;

class MoneyShopMessage : public Singleton<MoneyShopMessage>
{
public:
	MoneyShopMessage();
	~MoneyShopMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_MONSYSHOPINFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GT_EXCHANGEMONEY);

};



#endif /* MONEYSHOPMESSAGE_H_ */
