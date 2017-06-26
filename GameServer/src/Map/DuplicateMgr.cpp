/*
 * DuplicateMgr.cpp
 *
 *  Created on: 2014年10月14日
 *      Author: helh
 */

#include "DuplicateMgr.h"
#include "../MessageBuild.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "SceneManager.h"
#include "Timer/TimerInterface.h"
#include "../MapInstance/BeyondBattleMgr.h"


DuplicateMgr::DuplicateMgr()
{
	if(m_updateTime.IsValid())
	{
		m_updateTime.SetNull();
	}
	m_updateTime = TimerInterface::GetInstance()->RegRepeatedTime( &DuplicateMgr::UpdateTimer, this, 0, 0, 0, 0,  2 * 1000, 1 * 1000 );
}

DuplicateMgr::~DuplicateMgr()
{
	if(m_updateTime.IsValid())
	{
		m_updateTime.SetNull();
	}
}

bool DuplicateMgr::IsCreateInstance(int64 charid)
{
	INT64VINT64MAP::iterator it = m_instanceWill.find(charid);
	if(it != m_instanceWill.end())
	{
		vector<int64>::iterator vitr = it->second.begin();
		for(; vitr != it->second.end();)
		{
			if(CUtil::GetNowSecond() - *vitr > 30 * 1000)
			{
				vitr = it->second.erase(vitr);
			}
			else
			{
				++vitr;
			}
		}

		if(it->second.size() >= 5)
		{
			return false;
		}

	}

	return true;
}

void DuplicateMgr::UpdateTimer(void * arg)
{
	UpdateTimer();
}

void DuplicateMgr::UpdateTimer()
{
	vector<int64> vec;

	INT64GAMEDUPLICATEMAP::iterator it = m_duplicate.begin();
	for(; it !=m_duplicate.end(); ++it)
	{
		if(!it->second->Update())
		{

			LOG_DEBUG(FILEINFO,"m_duplicate destory instance map id = %ld",GET_MAP_ID(it->first));
			vec.push_back(it->first);
		}
	}

	for(uint i=0; i<vec.size(); ++i)
	{
		CSceneMgr::GetInstance()->DeleteScene(vec[i]);
	}
}

void DuplicateMgr::DeleteMap(int64 instance)
{
	INT64GAMEDUPLICATEMAP::iterator it = m_duplicate.find(instance);
	if(it == m_duplicate.end())
	{
		return;
	}
	else
	{
		m_duplicate.erase(it);
	}

	if(GET_MAP_TYPE(instance) == eBeyondMap)
	{
		BeyondBattleMgr::GetInstance()->DeleteFight(instance);
	}

	vector<Smart_Ptr<SvrItem> > gtSvr;
	MapInstanceProto::ReqDeleteInstance toGT;

	toGT.set_instanceid(instance);

	ServerConHandler::GetInstance()->GetAllGateServer(gtSvr);
	for(uint i=0; i<gtSvr.size(); ++i)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2GT_DELETEINSTANCE, &toGT, gtSvr[i].Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}



