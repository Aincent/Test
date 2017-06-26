/*
 * SeabeachMgr.h
 *
 *  Created on: 2015年12月7日
 *      Author: root
 */

#ifndef SEABEACHMGR_H_
#define SEABEACHMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"

using namespace std;
using namespace CommBaseOut;

struct PortalPoint
{
	PortalPoint():xpos(0),ypos(0){}
	int xpos;
	int ypos;
};

class SeabeachMgr : public Singleton<SeabeachMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	SeabeachMgr();
	~SeabeachMgr();
public:
	int GetBasePrizeTime(){ return m_expPrizeTime; }

	//根据等级得到基础奖励值
	int GetPrizeValue(int lv);

private:
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	//加载配置文件
	void LoadCofing();
	void LoadLua();

	//广播新对象
	void BroacastObject(Smart_Ptr<Player> &player, const std::vector<Smart_Ptr<CreatureObj> >& oldList, const std::vector<Smart_Ptr<CreatureObj> >& newList);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_SEABEACH_JUMP);			//请求跳跃
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_SEABEACH_GATHER);		//请求采集
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_SEABEACH_LEAVE_OBJECT);	//请求离开采集对象

private:
	CLuaInterface m_pLua;
	int m_expPrizeTime;	//场景奖励基础经验时间（妙为单位）

	std::map<int,PortalPoint> m_portalPoint;	//跳转点
};











#endif /* SEABEACHMGR_H_ */
