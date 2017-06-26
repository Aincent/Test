/*
 * SMagicMessage.h
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#ifndef SMAGICMESSAGE_H_
#define SMAGICMESSAGE_H_


#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "Singleton.h"

using namespace std;
using namespace CommBaseOut;

class SMagicMessage : public Singleton<SMagicMessage>
{
public:
	SMagicMessage();
	~SMagicMessage();

	//请求法器升阶信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_IMPOSE_MAGIC);
	//DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_FLUSH_MAGIC_SKILL);
	//DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_SIM_C2GM_REPLACE_MAGIC_SKILL);
};


#endif /* SMAGICMESSAGE_H_ */
