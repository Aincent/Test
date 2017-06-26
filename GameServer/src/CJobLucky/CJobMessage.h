/*
 * CJobMessage.h
 *
 *  Created on: 2014年8月18日
 *      Author: helh
 */

#ifndef CJOBMESSAGE_H_
#define CJOBMESSAGE_H_


#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "Singleton.h"

using namespace std;
using namespace CommBaseOut;

class CJobMessage : public Singleton<CJobMessage>
{
public:
	CJobMessage();
	~CJobMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_CJOB_INFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_RAND_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_SALE_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_EXCHANGE_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_EAT_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_WEAR_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_OPEN_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_CHANGE_CJOB);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_UNWEAR_CJOB);

};





#endif /* CJOBMESSAGE_H_ */
