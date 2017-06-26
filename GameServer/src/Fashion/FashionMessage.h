/*
 * FashionMessage.h
 *
 *  Created on: 2015年3月2日
 *      Author: root
 */

#ifndef FASHIONMESSAGE_H_
#define FASHIONMESSAGE_H_

#include "Singleton.h"
#include "ServerMsgDefine.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "../QuickMessage.h"

using namespace CommBaseOut;

class FashionMessage : public Singleton<FashionMessage>{
public:
	FashionMessage();
	~FashionMessage();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_FASHIONINFO); // 请求时装信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_ACTIVATEFASHION); // 请求激活时装
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_WEARFASHION); // 请求穿戴或卸下时装
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_STRENTHFASHION); // 强化时装
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_SHOWHIDEFASHION); // 显示/隐藏时装

};

#endif /* FASHIONMESSAGE_H_ */
