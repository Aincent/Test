/*
 * FindPathMgr.h
 *
 *  Created on: 2014年6月9日
 *      Author: helh
 */

#ifndef FINDPATHMGR_H_
#define FINDPATHMGR_H_

#include "Singleton.h"
#include "define.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "../ServerOtherDefine.h"

using namespace std;
using namespace CommBaseOut;

struct sReqPathInfo
{
	DWORD64 pos;
	int distance;

	sReqPathInfo():pos(0),distance(0)
	{

	}
};

class FindPathMgr : public Singleton<FindPathMgr>, public Message_Handler
{
public:
	FindPathMgr();
	~FindPathMgr();

	//寻路
	void FindPath(int64 mapid, int key, int startx, int starty, int endx, int endy, int distance = 0);

	//更新路径
	void UpdatePath();

	//发送寻路请求到pathserver
	void SendFindPath(PlayerSynch::ReqFindPath * path);

	DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_PS2GM_PATHPOS);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

private:

	INT64MREQPATHINFOMAP m_monsterPath;
};



#endif /* FINDPATHMGR_H_ */
