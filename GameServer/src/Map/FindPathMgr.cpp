/*
 * FindPathMgr.cpp
 *
 *  Created on: 2014年6月9日
 *      Author: helh
 */

#include "FindPathMgr.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "SvrConfig.h"
#include "SceneManager.h"


FindPathMgr::FindPathMgr()
{
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_PS2GM_PATHPOS);
}
FindPathMgr::~FindPathMgr()
{

}

void FindPathMgr::FindPath(int64 mapid, int key, int startx, int starty, int endx, int endy, int distance)
{
	INT64MREQPATHINFOMAP::iterator it = m_monsterPath.find(mapid);
	if(it == m_monsterPath.end())
	{
		map<int, sReqPathInfo> tMap;
		sReqPathInfo info;

		info.distance = distance;
		info.pos = ((DWORD64)startx << 48) | ((DWORD64)starty << 32) | ((DWORD64)endx << 16) | endy;
		tMap[key] = info;
		m_monsterPath[mapid] = tMap;
	}
	else
	{
		sReqPathInfo info;

		info.distance = distance;
		info.pos = ((DWORD64)startx << 48) | ((DWORD64)starty << 32) | ((DWORD64)endx << 16) | endy;
		it->second[key] = info;
	}
}

void FindPathMgr::UpdatePath()
{
	if(!ServerConHandler::GetInstance()->GetPathServer())
	{
		return;
	}

	PlayerSynch::ReqFindPath path;

	INT64MREQPATHINFOMAP::iterator it = m_monsterPath.begin();
	for(; it!=m_monsterPath.end(); ++it)
	{
		PlayerSynch::MapMonsterPos * monster = path.add_monster();

		path.add_mapid(it->first);

		map<int, sReqPathInfo>::iterator itMap = it->second.begin();
		for(; itMap!=it->second.end(); ++itMap)
		{
			PlayerSynch::SingleFindPos * pos = monster->add_monster();

			pos->set_key(itMap->first);
			pos->set_pos(itMap->second.pos);
			pos->set_distance(itMap->second.distance);

			if(path.ByteSize() > NORMAL_MSG_SEND_SIZE)
			{
				SendFindPath(&path);
				path.Clear();
				map<int, sReqPathInfo>::iterator tendIt = it->second.end();
				if(itMap != --tendIt)
				{
					monster = path.add_monster();

					path.add_mapid(it->first);
				}
			}
		}
	}

	if(path.mapid_size() > 0)
	{
		SendFindPath(&path);
	}

	m_monsterPath.clear();
}

void FindPathMgr::SendFindPath(PlayerSynch::ReqFindPath * path)
{
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GM2PS_FINDPATH, path, ServerConHandler::GetInstance()->GetPathServer().Get(), SimpleMessage);
	Message_Facade::Send(messRet);
}

void FindPathMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_PS2GM_PATHPOS);

	DEF_SWITCH_TRY_DISPATCH_END
}


DEF_MSG_SIMPLE_DEFINE_FUN(FindPathMgr, MSG_SIM_PS2GM_PATHPOS)
{
	PlayerSynch::AckPathPosition path;
	int len = 0;
	char *str = message->GetBuffer(len);

	path.ParseFromArray(str, len);

	int64 btime1 = CUtil::GetNowSecond();
	for(int i=0; i<path.mapid_size(); ++i)
	{
		const PlayerSynch::MapMonsterPath& mPath = path.path(i);

		int64 btime2 = CUtil::GetNowSecond();

		for(int j=0; j<mPath.path_size(); ++j)
		{
			const PlayerSynch::SingleMonsterPath& single = mPath.path(j);

			int64 btime3 = CUtil::GetNowSecond();
			CSceneMgr::GetInstance()->UpdatePath(path.mapid(i), &single);

			int64 etime3 = CUtil::GetNowSecond();
			if(etime3 - btime3 >= 1000)
			{
				LOG_FATAL(FILEINFO, "20 message id and to more time[%lld] mapid[%lld] size[%d] path size[%d] key[%d]", etime3 - btime3, path.mapid(i), path.mapid_size(), mPath.path_size(), single.key());
			}
		}

		int64 etime2 = CUtil::GetNowSecond();
		if(etime2 - btime2 >= 1000)
		{
			LOG_FATAL(FILEINFO, "20 message id and to more time[%lld]  mapid[%lld] size[%d]  path size[%d]", etime2 - btime2, path.mapid(i), path.mapid_size(), mPath.path_size());
		}
	}

	int64 etime1 = CUtil::GetNowSecond();
	if(etime1 - btime1 >= 1000)
	{
		LOG_FATAL(FILEINFO, "20 message id and to more time[%lld]  mapid size[%d]", etime1 - btime1, path.mapid_size());
	}
}


