/*
 * NPCLoader.cpp
 *
 *  Created on: 2013��10��21��
 *      Author: helh
 */

#include "NPCLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "Log/Logger.h"
#include "MessageCommonRet.h"
#include <math.h>
#include "../../QuestNew/QuestDefine.h"

NPCInfo * NPCInfo::m_instance = 0;

NPCInfo::NPCInfo()
{

}

NPCInfo::~NPCInfo()
{

}

int NPCInfo::Init(string &file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		NpcInfo info;
		int count = 0;

		info.npcID = loader.GetInt(i, count++, 0);
		count++;
		count++;
		count++;
		count++;
		count++;
		count++;

		info.mapid = loader.GetInt(i, count++);
		info.xpos = loader.GetInt(i, count++);
		info.ypos = loader.GetInt(i, count++);
		info.funID = loader.GetInt(i, count++);
		if(info.funID >eNPC_NOT_FUN)
		{
			NpcFunInfo funInfo;
			funInfo.mapID = info.mapid;

			SigFunInfo sig;
			sig.fun = (eNPCFunEnum)info.funID;
			sig.npcID = info.npcID;
			map<int , NpcFunInfo>::iterator it = m_mapFunNpc.find(funInfo.mapID);
			if(it != m_mapFunNpc.end())
			{
				it->second.m_nFun.push_back(sig);
			}
			else
			{
				funInfo.m_nFun.push_back(sig);

				m_mapFunNpc[funInfo.mapID] = funInfo;
			}
		}
//		string funStr = loader.GetString(i, count++, "");
//		StrArgPkg funPkg(">;", funStr);
//
//		info.funID = atoi(getArg(funPkg, 0).c_str());
//		info.fParam.param.push_back(atoi(getArg(funPkg, 1).c_str()));
//
//		for(int j=0; j<NPC_LIMIT_COUNT; j++)
//		{
//			string limitStr = loader.GetString(i, count++, "");
//			if(limitStr == "")
//			{
//				continue;
//			}
//
//			StrArgPkg limitPkg("|>;", limitStr);
//			for(unsigned int i=0; i<limitPkg.size(); ++i)
//			{
//				const IStrArgSeg& subStrSeg = limitPkg[i];
//				const IStrArgSeg& ssubStrSeg = subStrSeg[1];
//
//				info.limitID[j] = atoi(getArg(subStrSeg, 0).c_str());
//				info.lParam[j].signf.push_back(atoi(getArg(ssubStrSeg, 0).c_str()));
//				info.lParam[j].param.push_back(atoi(getArg(ssubStrSeg, 1).c_str()));
//			}
//		}

		m_npcInfo[info.npcID] = info;
	}

	return 0;
}

bool NPCInfo::IsExistedNpc(int id)
{
	map<int, NpcInfo>::iterator it = m_npcInfo.find(id);
	if(it == m_npcInfo.end())
	{
		return false;
	}

	return true;
}

NpcInfo & NPCInfo::GetNpcInfo(int id)
{
	map<int, NpcInfo>::iterator it = m_npcInfo.find(id);
	if(it == m_npcInfo.end())
	{
		LOG_ERROR(FILEINFO, "npc is not existed");
	}

	return it->second;
}

const NpcInfo* NPCInfo::GetInfo(int id)
{
	map<int, NpcInfo>::iterator it = m_npcInfo.find(id);
	if(it == m_npcInfo.end())
	{
		return NULL;
	}

	return &it->second;
}

void NPCInfo::GetAllMapNpc(int mapid, vector<NpcInfo> &info)
{
	map<int, NpcInfo>::iterator it = m_npcInfo.begin();
	for(; it!=m_npcInfo.end(); ++it)
	{
		if(it->second.mapid == mapid)
		{
			info.push_back(it->second);
		}
	}
}

int NPCInfo::ExamineDistance(int npcid, int xpos, int ypos)
{
	map<int, NpcInfo>::iterator it = m_npcInfo.find(npcid);
	if(it == m_npcInfo.end())
	{
		LOG_ERROR(FILEINFO, "examine npc distance but npc[%d] is not existed", npcid);

		return eRetNpcNotExist;
	}

	int xtemp = abs(it->second.xpos-xpos);
	int ytemp = abs(it->second.ypos-ypos);
	double distance = sqrt(xtemp * xtemp + ytemp * ytemp);

	if(distance >= NPC_PLAYER_DISTANCE)
	{
		return eBeyondDistance;
	}

	return 0;
}

int NPCInfo::GetNPCFunEnum(int npcid)
{
	map<int, NpcInfo>::iterator it = m_npcInfo.find(npcid);
	if(it != m_npcInfo.end())
		return it->second.funID;
	return eNPC_NOT_FUN;
}

int NPCInfo::GetFunNpcIDByMapId(int mapid, eNPCFunEnum funenum)
{
	map<int , NpcFunInfo>::iterator it = m_mapFunNpc.find(mapid);
	if(it != m_mapFunNpc.end())
	{
		for(unsigned int i=0; i<it->second.m_nFun.size(); ++i)
		{
			if(it->second.m_nFun[i].fun == funenum)
				return it->second.m_nFun[i].npcID;
		}
	}
	return 0;
}




