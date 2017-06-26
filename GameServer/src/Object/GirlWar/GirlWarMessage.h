/*
 * GirlWarMessage.h
 *
 *  Created on: 2014年12月9日
 *      Author: root
 */

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../Player/Player.h"
#include "Timer/TimerInterface.h"    //
#include "../../QuickMessage.h"
#include "EventSet/EventSet.h"

using namespace std;
using namespace CommBaseOut;


#ifndef GIRLWARMESSAGE_H_
#define GIRLWARMESSAGE_H_

class GirlWarMessage
{
public:
	static GirlWarMessage* GetInstance();
	~GirlWarMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_SYNGIRLWARINFO);		//请求战姬信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWARUPLV);	//请求战姬升级
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWARTOFIGHT);  //请求出战或休战
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWARREVIVE);	//请求战姬复活
	DEF_MSG_QUICK_SIMPLE_DECLARE_FUN_H(MSG_SIM_C2GM_UPDATEGIRLWAR_UISTATE); // 更改战姬UI高亮状态
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWAR_WAREEQUIP);		//请求战姬加成
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWAR_ONEKEYWAREEQUIP);		//请求战姬一键加成
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWAR_UPQUALITY);		//请求战姬进阶
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ASKGIRLWAR_UPSKILL); //请求战姬技能升级
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GIRLWARUPSTAR);//请求战姬升星
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETGIRLWAR);//请求获取战姬

private:
	GirlWarMessage();
};


#endif /* GIRLWARMESSAGE_H_ */
