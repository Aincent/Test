/*
 * CreatMapTask.cpp
 *
 *  Created on: 2014年3月28日
 *      Author: helh
 */

#include "CreateMapTask.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "Network/NetWorkConfig.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageStruct/Map/GS2GTAllMap.pb.h"
#include "MessageStruct/ServerReturn4Int.pb.h"
#include "../ServerManager.h"
#include "./ConfigLoader/MapConfig.h"
#include "Thread/ThreadPool.h"
#include "SvrConfig.h"
#include "GameVillage.h"
#include "SceneManager.h"
#include "GameDuplicate.h"
#include "../MapInstance/BeyondBattleMgr.h"
#include "MessageStruct/Marry/MarryMessage.pb.h"
#include "../MapCarbon/BeyondCarbon.h"
#include "../MapCarbon/ClanCarbon.h"
#include "../MapCarbon/CountryBossCarbon.h"
#include "../MapCarbon/CountryWarCarbon.h"
#include "../MapCarbon/GroupCarbon.h"
#include "../MapCarbon/ProtectKingCarBon.h"
#include "../MapCarbon/TreasonKingCarbon.h"
#include "../MapCarbon/PVPCarbon.h"
#include "../MapCarbon/MoneyCarbon.h"
#include "../MapCarbon/MarryCarbon.h"
#include "../MapCarbon/GuideCarbon.h"
#include "../MapCarbon/SeabeachCarbon.h"
#include "../MapCarbon/RobberCarbon.h"
#include "../MapCarbon/WorldBossCarbon.h"
#include "../MapCarbon/ChallengeCarbon.h"
#include "../MapCarbon/HeroCarbon.h"
#include "../MapCarbon/ClanIntegralCarbon.h"
#include "../MapCarbon/AdvancedCarbon.h"

CCreateMapTask * CCreateMapTask::m_instance = 0;

CCreateMapTask::CCreateMapTask()
{
//	DEF_MSG_SIMPLE_REG_FUN(eWorldServer, MSG_SIM_GT2WS_PUSHGSMAP);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_CREATESCENE);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_CREATESCENE);
}

CCreateMapTask::~CCreateMapTask()
{
	m_createMap.clear();
	m_willCreate.clear();
}

void CCreateMapTask::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CREATESCENE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_CREATESCENE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void * CCreateMapTask::CreateMap(void * arg)
{
	CreateMapInfo * pInfo = static_cast<CreateMapInfo *>(arg);

	CCreateMapTask::GetInstance()->CreateMap(pInfo);

	return 0;
}

void CCreateMapTask::DeleteWillCreate(int64 id, vector<Smart_Ptr<CreateMapInfo> > &info)
{
	GUARD(CSimLock, obj, &m_willLock);
	vector<Smart_Ptr<CreateMapInfo> >::iterator it = m_willCreate.begin();
	for(;it!=m_willCreate.end(); )
	{
		if(id == (*it)->iOnlyID)
		{
			info.push_back(*it);
			it = m_willCreate.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CCreateMapTask::UpdateMap()
{
	GUARD(CSimLock, obj, &m_createLock);
	if(m_createMap.size() <= 0)
		return;

	vector<Smart_Ptr<CGameZone> > tMap(m_createMap);
	m_createMap.clear();
	obj.UnLock();

	vector<Smart_Ptr<CGameZone> >::iterator it = tMap.begin();
	for(; it!=tMap.end(); )
	{
		GameSvrToGate::CreateMapAck ack;
		vector<Smart_Ptr<CreateMapInfo> > info;

		CSceneMgr::GetInstance()->AddMap((*it)->GetSceneID(), *it);
		DeleteWillCreate((*it)->GetSceneID(), info);

		ack.set_ret((*it)->GetSceneID());
		ack.set_gsid(ServerConHandler::GetInstance()->GetLocalID());

		if(info.size() > 1 && info[0]->iMapType == eBeyondMap)
		{
			Smart_Ptr<CGameDuplicate> tDup = *it;

			BeyondBattleMgr::GetInstance()->AddFight(tDup->GetSceneID(), tDup->GetParamfValue(), tDup->GetParamsValue());
		}

		vector<Smart_Ptr<CreateMapInfo> >::iterator itVec = info.begin();
		for(; itVec!=info.end(); ++itVec)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_WS2GM_CREATESCENE, (*itVec)->sRequest, &ack, Ack);
			Message_Facade::Send(messRet);
		}

		it = tMap.erase(it);
	}
}

void CCreateMapTask::CreateMap(CreateMapInfo *info)
{
	int64 ret = -1;

	switch(info->iMapType)
	{
	case eOnlyMap:
	case eStaticMap:
		{
			Smart_Ptr<CGameZone> newZone = new CGameVillage(info->iOnlyID);

			ret = newZone->Init();
			if(ret == 0)
			{
				ret = info->iOnlyID;
			}

			GUARD(CSimLock, obj, &m_createLock);
			m_createMap.push_back(newZone);

			break;
		}

	case eBeyondMap:
	case ePVEMap://pve副本
		{
			ServerReturn::ServerFourInt param;
			Smart_Ptr<CGameZone> newZone;

			if(info->content.size()>0)
			{
				param.ParseFromArray(info->content.c_str(),info->content.size());
				cout<<endl<<"++++++++++++++++++++++++charid:"<<info->charid<<" static map:"<<info->iStaticID<<" onlyid:"<<info->iOnlyID<<" type"<<info->iMapType<<" paramf:"<<param.retf()<<" params:"<<param.rets()<<endl;
			}
			else
			{
				param.set_retf(0);
				param.set_rets(0);
			}

			InstanceMapEntry*  data = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(info->iOnlyID));
			if(data)
			{
				switch(data->m_eType)
				{
				case eChallenge:
				{
					newZone = new ChallengeCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}

				case eHeroInstance:
				{
					newZone = new HeroCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}

				case eGroupInstance:
					{
						newZone = new CGroupCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case	eClanBattle: //帮派战
					{
						newZone = new CClanCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case eTreasonKing: //剿灭反王
					{
						newZone = new CTreasonKingCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case eProtectKing: //守护秦王
					{
						newZone = new CProtectKingCarBon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case eBeyondSvrBattle: //夸服战
					{
						newZone = new CBeyondCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case eCountryBoss: //阵营boss战副本
					{
						newZone = new CCountryBossCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case eCountryWar: //阵营战
					{
						newZone = new CCountryWarCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
						break;
					}
				case ePVP://斗战重楼
				{
					newZone = new PVPCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}
				case eMoneyInstance:
				{
					newZone = new MoneyCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}
				case eMarryInstance:
				{
					newZone = new CMarryCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}
				
				case eGuideInstance:
				{
					newZone = new GuideCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}
				
				case eSeabeachInstance:
				{
					newZone = new SeabeachCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}

				case eClanRobber:
				{
					newZone = new RobberCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}

				case eWorldBossInstance:
				{
					newZone = new WorldBossCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}

				case eIntegralInstance:
				{
					newZone = new CClanIntegralCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}
				case eAdvancedInstance:
				{
					newZone = new CAdvancedCarbon(info->iOnlyID, info->charid, param.retf(),param.rets());
					break;
				}
//				case eCountryOverlord:
//				{
//					vector<int64> charId;
//					Safe_Smart_Ptr<Message> mess = static_cast<ThreeAct *>(info->sRequest->GetAct().Get())->mesReq;
//					if(mess)
//					{
//						ServerReturn::ServerRetIntArray meContent;
//						int len = 0;
//						char * content = mess->GetBuffer(len);
//						meContent.ParseFromArray(content, len);
//
//						for(size_t i = 0 ; i < meContent.ret_size(); i++)
//						{
//							charId.push_back(meContent.ret(i));
//						}
//					}
//
//					newZone = new CCountryOverlordCarbon(info->iOnlyID, info->charid, param.retf(),param.rets(),charId);
//					break;
//				}
				default:
				{
						newZone = new CGameDuplicate(info->iOnlyID, info->charid, param.retf(),param.rets());
				}
				}

				ret = newZone->Init();
				if(ret == 0)
				{
					ret = info->iOnlyID;
				}
				else
				{
					return;
				}

				GUARD(CSimLock, obj, &m_createLock);
				m_createMap.push_back(newZone);
			}

			break;
		}
	default:
		{
			LOG_ERROR(FILEINFO, "create map but map type is not exist");
		}
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(CCreateMapTask, MSG_REQ_WS2GM_CREATESCENE)
{
	LOG_DEBUG(FILEINFO, "gateserver request gameserver create map");

	GameSvrToGate::CreateMapInfo req;
	int len = 0;
	char *str = message->GetBuffer(len);
	int ret = 0;
	int64 targetMap = -1;

	req.ParseFromArray(str, len);

	Smart_Ptr<CreateMapInfo> pInfo = new CreateMapInfo();

//	printf("\n\n-----------------create map id [%d]---------------\n\n", req.staticid());



	if(req.maptype() == eBeyondMap)
	{
		ServerReturn::ServerDoubleInt param;

		if(req.content().size()>0)
		{
			param.ParseFromArray(req.content().c_str(),req.content().size());
		}
		else
		{
			param.set_retf(0);
			param.set_rets(0);
		}

		ret = BeyondBattleMgr::GetInstance()->AddMapIDToMatch(req.charid(), req.onlyid(), param.retf(), param.rets(), targetMap);
		if(ret == -1)
			return;
	}

	pInfo->charid = req.charid();
	pInfo->iOnlyID = req.onlyid();
	pInfo->iMapType = req.maptype();
	pInfo->iStaticID = req.staticid();
	pInfo->sRequest = message;
	pInfo->content = req.content();

	GUARD(CSimLock, obj, &m_willLock);
	if(targetMap > 0)
	{
		vector<Smart_Ptr<CreateMapInfo> >::iterator it = m_willCreate.begin();
		for(;it!=m_willCreate.end(); ++it)
		{
			if(targetMap == (*it)->iOnlyID)
			{
				(*it)->iOnlyID = req.onlyid();
			}
		}
	}

	m_willCreate.push_back(pInfo);
	obj.UnLock();

	if(ret == 0)
	{
		CThreadPool::GetInstance()->AddTask(CCreateMapTask::CreateMap, pInfo.Get());
	}
}



