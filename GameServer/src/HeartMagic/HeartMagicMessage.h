/*
 * HEARTMagicMessage.h
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#ifndef HEARTMAGICMESSAGE_H_
#define HEARTMAGICMESSAGE_H_


#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "Singleton.h"

using namespace std;
using namespace CommBaseOut;

class HeartMagicMessage : public Singleton<HeartMagicMessage>
{
public:
	HeartMagicMessage();
	~HeartMagicMessage();

	//请求神兵升阶信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_IMPOSE_HEART_MAGIC);
	//DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_FLUSH_MAGIC_SKILL);
	//DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_SIM_C2GM_REPLACE_MAGIC_SKILL);
};


#endif /* HeartMagicMessage */
