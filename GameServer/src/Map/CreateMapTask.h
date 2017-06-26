/*
 * CreateMapTask.h
 *
 *  Created on: 2014年3月28日
 *      Author: root
 */

#ifndef CREATEMAPTASK_H_
#define CREATEMAPTASK_H_

#include "define.h"
#include <map>
#include <vector>
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "GameZone.h"

using namespace std;
using namespace CommBaseOut;

struct CreateMapInfo
{
	int64 charid;
	int64 iOnlyID;
	int iMapType;
	int iStaticID;
	string content;
	Safe_Smart_Ptr<Message> sRequest;

	CreateMapInfo():charid(-1),iOnlyID(-1),iMapType(-1),iStaticID(-1),content("")
	{

	}
};

class CCreateMapTask : public Request_Handler
{
public:
	~CCreateMapTask();


	static CCreateMapTask * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CCreateMapTask();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

//		DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_CREATESCENE);

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_CREATESCENE);

	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	static void * CreateMap(void * arg);
	void CreateMap(CreateMapInfo *info);
	void DeleteWillCreate(int64 id, vector<Smart_Ptr<CreateMapInfo> > &info);

	void UpdateMap();

private:
	CCreateMapTask();

private:

	static CCreateMapTask * m_instance;

	CSimLock m_willLock;
	vector<Smart_Ptr<CreateMapInfo> > m_willCreate; //将要创建的场景信息

	CSimLock m_createLock;
	vector<Smart_Ptr<CGameZone> > m_createMap; //已经创建的场景
};




#endif /* CREATEMAPTASK_H_ */
