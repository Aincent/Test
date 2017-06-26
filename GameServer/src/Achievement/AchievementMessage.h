/*
 * AchievementMessage.h
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#ifndef ACHIEVEMENTMESSAGE_H_
#define ACHIEVEMENTMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../Object/Player/Player.h"
#include "../QuickMessage.h"
#include "MessageStruct/AchievementProto.pb.h"

using namespace std;
using namespace CommBaseOut;

class AchievementMessage : public Singleton<AchievementMessage>, public Ack_Handler,public Message_Handler
{
public:
	AchievementMessage();
public:
	~AchievementMessage();
	//void Init();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETRECENTPRESTIGE);		//成就总览

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETTITLEINOF);		//查看称号
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETPRESTIGETYPE);		//查看某个成就类型
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETPRESTIGEPRIZE);		//获取成就奖励
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_ACTIVETITLE);		//激活称号
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_GETACHSHOPINFO);	//获取成就商店信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_BUGACHSHOPGOODS);	//购买成就商品
//	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2GT_GLORY);
	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_UPDATATITLEID);//WS推送给GM更新称号ID
	//DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_PRESTIGECOMPLETE);		   //某成就完成
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2WS_GLORY);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
};


#endif /* ACHIEVEMENTMESSAGE_H_ */
