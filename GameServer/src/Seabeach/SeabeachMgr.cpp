/*
 * SeabeachMgr.cpp
 *
 *  Created on: 2015年12月7日
 *      Author: root
 */
#include "SeabeachMgr.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../EventSet/EventDefine.h"
#include "CSVLoader.h"

#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "MessageCommonRet.h"
#include "../QuickMessage.h"

#include "MessageStruct/Seabeach/Seabeach.pb.h"
#include "MessageStruct/Map/ChangeMap.pb.h"
#include "../MapCarbon/SeabeachCarbon.h"
#include "../Meditation/PlayerMeditation.h"
#include "./FileLoader/UpgradeLoader.h"


SeabeachMgr::SeabeachMgr():m_expPrizeTime(0)
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(SeabeachMgr,MSG_REQ_C2GS_SEABEACH_GATHER);
	DEF_MSG_QUICK_REQUEST_REG_FUN(SeabeachMgr, MSG_REQ_C2GS_SEABEACH_JUMP);
	DEF_MSG_QUICK_REQUEST_REG_FUN(SeabeachMgr, MSG_REQ_SEABEACH_LEAVE_OBJECT);

	m_pLua.OpenLua();
	LoadCofing();

}

SeabeachMgr::~SeabeachMgr()
{
	m_pLua.CloseLua();
}

void SeabeachMgr::LoadCofing()
{
	LoadLua();
}

void SeabeachMgr::LoadLua()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return;
	}

	string strConfig = strFileFullPath + "Data/Seabeach.lua";
	if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
	{
		return;
	}

	//奖励时间
	m_pLua.SetLuaFunction("GetTime");
	if(m_pLua.Call(0, 1))
	{
		return;
	}
	m_expPrizeTime = m_pLua.GetInt();

}

int SeabeachMgr::GetPrizeValue(int lv)
{
	UpgradeInfo nowInfo;

	UpgradeLoader::GetInstance()->GetUpgradeByLv(lv, nowInfo);
	if(nowInfo.lv == 0)
	{
		LOG_ERROR(FILEINFO, "get upgrade info error and upgrade failed");
		return 0;
	}

	m_pLua.SetLuaFunction("GetExp");
	m_pLua.SetFloat(nowInfo.factoryExp);

	if(m_pLua.Call(1,1))
	{
		return 0;
	}

	return m_pLua.GetInt();
}

void SeabeachMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{

}

void SeabeachMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{

}

void SeabeachMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SeabeachMgr,MSG_REQ_C2GS_SEABEACH_JUMP)
{
	SeabeachProto::AckJump ack;
	SeabeachProto::ReqJump req;
	req.ParseFromString(content);

	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(pInstance==NULL || pInstance->m_eType != eSeabeachInstance)
	{
		return;
	}

	MapStartPoint info;
	CMapConfig::GetInstance()->GetTransferMapByStartPoint(req.id(), info);
	if(info.ID <= 0)
	{
		return;
	}

	if(player->GetGameZone() == NULL)
	{
		return;
	}

	Sector *pTargetSector = player->GetGameZone()->GetSector(info.tXPos, info.tYPos);
	Sector *pSelfSector = player->GetSector();

	if(pTargetSector == NULL || pSelfSector == NULL || pTargetSector == pSelfSector)
	{
		return;
	}

	//如果打坐状态则取消
	player->CanccelMeditation();

	//原有的九宫格对象
	Smart_Ptr<CreatureObj> playerTemp = player;
	vector<Smart_Ptr<CreatureObj> > objTemp;
	player->GetGameZone()->GetGroupObject(playerTemp, objTemp);

	SeabeachProto::JumpInfo Brocast;
	Brocast.set_key(player->GetKey());
	Brocast.set_xpos(info.tXPos);
	Brocast.set_ypos(info.tYPos);
	player->SendToAround(&Brocast, MSG_GS2C_SEABEACH_JUMP_INFOL, true);

	Smart_Ptr<Object> obj = player;
	pSelfSector->Leave(obj);

	player->SetXNewPos(info.tXPos);
	player->SetYNewPos(info.tYPos);
	player->SetXPos(info.tXPos);
	player->SetYPos(info.tYPos);
	player->SetDataFlag(eBaseInfo);
	player->ClearPath();

	pTargetSector->Join(obj);

	//新的目标九宫格对象
	Smart_Ptr<CreatureObj> playerNewTemp = player;
	vector<Smart_Ptr<CreatureObj> > objNewTemp;
	player->GetGameZone()->GetGroupObject(playerNewTemp, objNewTemp);

	BroacastObject(player, objTemp, objNewTemp);

	ack.set_code(0);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_SEABEACH_JUMP);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SeabeachMgr,MSG_REQ_SEABEACH_LEAVE_OBJECT)
{
	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(pInstance==NULL || pInstance->m_eType != eSeabeachInstance)
	{
		return;
	}

	CGameZone *pZone = player->GetGameZone();
	if(pZone==NULL)
	{
		return;
	}

	if(!player->IsInCreatureState(ERS_Distortion))
	{
		return;
	}

	SeabeachProto::AckLeaveObject ack;

	int ret = ((SeabeachCarbon*)(pZone))->LeaveObject(player);

	ack.set_code(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_SEABEACH_LEAVE_OBJECT);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SeabeachMgr,MSG_REQ_C2GS_SEABEACH_GATHER)
{
	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(pInstance==NULL || pInstance->m_eType != eSeabeachInstance)
	{
		return;
	}

	CGameZone *pZone = player->GetGameZone();
	if(pZone==NULL)
	{
		return;
	}

	SeabeachProto::ReqGather req;
	req.ParseFromString(content);

	int ret = ((SeabeachCarbon*)(pZone))->Gather(player,req.objkey());

	SeabeachProto::AckGather ack;
	ack.set_code(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_SEABEACH_GATHER);
}

void SeabeachMgr::BroacastObject(Smart_Ptr<Player> &player, const std::vector<Smart_Ptr<CreatureObj> >& oldList, const std::vector<Smart_Ptr<CreatureObj> >& newList)
{
	for(size_t i=0; i<newList.size(); ++i)
	{
		if(newList[i].Get() == NULL)
		{
			continue;
		}

		int key = newList[i]->GetKey();
		if(key == player->GetKey())
		{
			continue;
		}

		bool isSame = false;

		//过滤相同的对象
		for(size_t j=0; j<oldList.size(); ++j)
		{
			if(key == oldList[j]->GetKey())
			{
				isSame = true;
				break;
			}
		}

		if(isSame)
		{
			continue;
		}

		//通知不同的对象
		int type = newList[i]->GetType();
		if(type == ePlayer)
		{
			Smart_Ptr<Player> target = newList[i];
			Smart_Ptr<Object> object1 = player;
			Smart_Ptr<Object> object2 = target;

			target->SendObjectSynch(object1);
			player->SendObjectSynch(object2);
		}
		else if(type == eMonster)
		{
			Smart_Ptr<Object> object = newList[i];
			player->SendObjectSynch(object);
		}

	}
}








