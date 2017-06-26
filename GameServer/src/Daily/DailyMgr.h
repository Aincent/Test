/*
 * DailyMgr.h
 *
 *  Created on: 2014年6月22日
 *      Author: root
 */

#ifndef DAILYMGR_H_
#define DAILYMGR_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "Smart_Ptr.h"
#include "MessageStruct/Daily/Daily.pb.h"
#include "Daily.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class DailyMgr : public Singleton<DailyMgr>, public Ack_Handler
{
public:
	DailyMgr();
public:
	~DailyMgr();
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_GETCLANCOVERSOURCEINFO);//向WS请求昨日的城池奖励领取情况
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETDAILYINFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETYESTERDAYCOUNTERINFO);//请求昨天的活动完成次数
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_RECOVERRESOURCE);//资源追回
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETDAILYPRACTICEINFO);//请求闭关修炼信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETDAILYPRACTICEREWARD);//领取闭关修炼奖励

	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
};


#endif /* DAILYMGR_H_ */
