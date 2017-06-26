/*
 * MeditationMgr.cpp
 *
 *  Created on: 2015年11月2日
 *      Author: root
 */
#include "MeditationMgr.h"
#include "MessageStruct/Map/ChangeMap.pb.h"
#include "PlayerMeditation.h"
#include "../Map/GameZone.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../Map/SceneManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../EventSet/EventDefine.h"
#include "CSVLoader.h"

#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"
#include "../ServerEventDefine.h"

MeditationMgr::MeditationMgr():m_InvitValidTime(30000),m_max_beInvite(5)
{
	DEF_MSG_QUICK_SIMPLE_REG_FUN(MeditationMgr, MSG_REQ_C2GM_MEDITATION);					//请求打坐
	DEF_MSG_QUICK_REQUEST_REG_FUN(MeditationMgr, MSG_REQ_C2GM_DOUBLE_MEDITATION);	//邀请双修
	DEF_MSG_QUICK_REQUEST_REG_FUN(MeditationMgr, MSG_REQ_REPLY_MEDITATION_INVIT);	//玩家同意或拒绝双修
	DEF_MSG_QUICK_REQUEST_REG_FUN(MeditationMgr, MSG_REQ_CANCCEL_DOUBLE_MEDITATION);	//玩家请求取消双修
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_GM2WS_PLAYER_NEXUS);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_WS2GS_NEXUS_CHANGE);

	LoadConfig();
}


MeditationMgr::	~MeditationMgr()
{
	if(m_MeditationCheckTimeID.IsValid())
	{
		m_MeditationCheckTimeID.SetNull();
	}

	std::map< pair<int64,int64>, DoubleMeditationData>::iterator it = m_timerList.begin();
	for(; it != m_timerList.end(); ++it)
	{
		if(it->second.param != NULL)
		{
			delete it->second.param;
			it->second.param = NULL;
		}

		if(it->second.timer.IsValid())
		{
			it->second.timer.SetNull();
		}
	}

}

const MeditationConfig* MeditationMgr::GetConfig(int type)
{
	std::map<int,MeditationConfig>::const_iterator it = m_config.find(type);
	if( it != m_config.end() )
	{
		return &it->second;
	}
	return NULL;
}

bool MeditationMgr::IsCanMeditation(CGameZone* pZone, int xpos, int ypos)
{
	if(pZone == NULL)
	{
		return false;
	}


	return pZone->IsCanMeditation(xpos, ypos);
	//return pZone->isCanDoing(eDoingMeditation, xpos, ypos);
}

void MeditationMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_WS2GS_NEXUS_CHANGE);

	DEF_SWITCH_TRY_DISPATCH_END
}

void MeditationMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{

}

void MeditationMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_GM2WS_PLAYER_NEXUS);

	DEF_SWITCH_TRY_DISPATCH_END
}

void MeditationMgr::LoadConfig()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return;
	}

	string strFile = strFileFullPath + "Data/PracticeTypeData.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return;
	}

	//ID	修炼类型	打坐加成	亲密度加成	亲密度加成时间
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;
		loader.GetInt(i, count++);

		MeditationConfig info;
		info.type = loader.GetInt(i, count++);
		info.rate = loader.GetFloat(i, count++) * 0.01f;
		info.closedValue = loader.GetInt(i, count++);
		info.closedTime = loader.GetInt(i, count++);
		m_config[info.type] = info;
	}

}

DEF_MSG_SIMPLE_DEFINE_FUN(MeditationMgr,MSG_WS2GS_NEXUS_CHANGE)
{
	ServerReturn::ServerThreeInt ret;

	int len = 0;
	char * content = message->GetBuffer(len);
	ret.ParseFromArray(content,len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, ret.retf());
	if(player.Get() == NULL)
	{
		return;
	}

	int64 min = ret.retf() < ret.rets() ? ret.retf() : ret.rets();
	int64 max = ret.retf() > ret.rets() ? ret.retf() : ret.rets();

	std::map< pair<int64,int64>, DoubleMeditationData>::iterator it = m_timerList.find(make_pair(min,max));
	if(it == m_timerList.end())
	{
		return;
	}

	switch(ret.rett())
	{
	case eNexusChange_Friend:
		it->second.param->type = eMeditationType_Friend;
		break;

	case eNexusChange_RemoveFriend:
		it->second.param->type = eMeditationType_Strange;
		break;

	case eNexusChange_Couple:
		it->second.param->type = eMeditationType_Couple;
		break;

	case eNexusChange_RemoveCouple:
		it->second.param->type = eMeditationType_Friend;
		break;
	}

	//通知客户端关系变化
	MeditationProto::NexusChange toClient;
	toClient.set_targetid(ret.rets());
	toClient.set_type(it->second.param->type);
	player->SendSynchMessage(&toClient, MSG_GS2C_NEXUS_CHANGE);

	Smart_Ptr<Player> target;
	PlayerMessage::GetInstance()->GetPlayerByCharId(target, ret.rets());
	if(target.Get() != NULL)
	{
		MeditationProto::NexusChange toClient;
		toClient.set_targetid(ret.retf());
		toClient.set_type(it->second.param->type);
		target->SendSynchMessage(&toClient, MSG_GS2C_NEXUS_CHANGE);
	}

}


DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(MeditationMgr,MSG_REQ_C2GM_MEDITATION)
{
		MeditationProto::ReqMeditation req;
		req.ParseFromString(content);

//		if(!player || !player->IsOpenSystem(eStMeditation))
//		{
//			player->SendToPlayerResultCodeMessage(eNotOpen);
//			return;
//		}

		//区域能否打坐双修
		if(!IsCanMeditation(player->GetGameZone(), player->GetXNewPos(), player->GetYNewPos()))
		{
			player->SendToPlayerResultCodeMessage(eMeditationPosLimit);
			return;
		}

		//请求打坐
		if (req.type() == REQMEDITATION)
		{
			int ret = player->GetMeditationMgr()->StartMeditationTimer();
			if(ret != 0)
			{
				player->SendToPlayerResultCodeMessage(ret);
				return;
			}
		}
		else if (req.type() == CANCELMEDITATION)
		{
			player->GetMeditationMgr()->DeleteMeditationTimer(true);
		}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MeditationMgr,MSG_REQ_C2GM_DOUBLE_MEDITATION)
{
	MeditationProto::ReqInvitMeditation req;
	MeditationProto::AckInvitMeditation ack;

	req.ParseFromString(content);
	int64 targetid = req.targetid();
	ack.set_targetid(targetid);

	Smart_Ptr<PlayerMeditation>& invit = player->GetMeditationMgr();

	//自己的当前位置能否有位置邀请双修
	int newxPos, newypos;
	if( !CalNewPos(player, newxPos, newypos) )
	{
		ack.set_code(eMeditationDoublePosLimit);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_DOUBLE_MEDITATION);
		return;
	}


	//自己的状态能否允许 没打坐或已经双修
	if(!player->IsInCreatureState(ERS_Meditation) || invit->GetTargetID()>0)
	{
		ack.set_code(eMeditationStateLimit);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_DOUBLE_MEDITATION);
		return;
	}

	if(player->GetGameZone() == NULL)
	{
		return;
	}

	//对方是否在同一张地图
	Smart_Ptr<Player> target = player->GetGameZone()->GetPlayer(targetid);
	if(target.Get() == NULL)
	{
		ack.set_code(eMeditationNotSameMap);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_DOUBLE_MEDITATION);
		return;
	}

	//对方等级状态是否允许双修
	if(target->IsInCreatureStateAny(ERS_Meditation)) //!target->IsOpenSystem(eStMeditation) ||
	{
		ack.set_code(eMeditationTargetStateLimit);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_DOUBLE_MEDITATION);
		return;
	}

	unsigned int state = ERS_Dead | ERS_Fight | ERS_Gather | ERS_Still | ERS_Ice | ERS_Stun | ERS_Distortion;
	if(target->IsInCreatureStateAny(state))
	{
		ack.set_code(eMeditationTargetStateLimit);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_DOUBLE_MEDITATION);
		return;
	}

	//发送邀请
	int ret = Invite(player, target);

	ack.set_code(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_DOUBLE_MEDITATION);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MeditationMgr,MSG_REQ_REPLY_MEDITATION_INVIT)
{
	MeditationProto::ReqReplyMeditation req;
	MeditationProto::AckReplyMeditation ack;

	req.ParseFromString(content);

	int ret = Reply(player, req.flag(), req.charid());
	if(ret != 0)
	{
		RemoveFromLists(req.charid(), player->GetID());

		ack.set_code(ret);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_REPLY_MEDITATION_INVIT);
		return;
	}

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
	if(worldSvr.Get() != NULL)
	{
		ServerReturn::ServerDoubleInt gstows;
		gstows.set_retf( player->GetID() );
		gstows.set_rets( req.charid() );

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_GM2WS_PLAYER_NEXUS, &gstows, worldSvr.Get(), Request);
		messRet->SetAct(new commonAct(message, player->GetID(), req.charid()));
		Message_Facade::Send(messRet);
	}

}

DEF_MSG_ACK_DEFINE_FUN(MeditationMgr, MSG_GM2WS_PLAYER_NEXUS)
{
	  //请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_DEBUG(FILEINFO, "MSG_GM2WS_PLAYER_NEXUS ack timeout");
		return;
	}

	ServerReturn::ServerResultInt ack;

	int len = 0;
	char * content = message->GetBuffer(len);
	ack.ParseFromArray(content,len);

	Smart_Ptr<Player> invitor;
	Smart_Ptr<Player> target;

	PlayerMessage::GetInstance()->GetPlayerByCharId(invitor, static_cast<commonAct*>(act.Get())->mesDatas);
	PlayerMessage::GetInstance()->GetPlayerByCharId(target, static_cast<commonAct*>(act.Get())->mesDataf);

	if(invitor && target)
	{
		MeditationProto::AckReplyMeditation toClient;
		toClient.set_code(0);
		REQ_MSG_QUICK_ACK_FUN(static_cast<commonAct*>(act.Get())->mesReq, &toClient, MSG_REQ_REPLY_MEDITATION_INVIT);

		//开始双修
		StartDoubleMeditation(invitor, target, ack.ret());

		//从列表中移出
		RemoveFromLists(invitor->GetID(), target->GetID());
	}

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MeditationMgr,MSG_REQ_CANCCEL_DOUBLE_MEDITATION)
{
	MeditationProto::AckReqChangeMeditation ack;

	if(!player->IsInCreatureState(ERS_Meditation) || player->GetMeditationMgr()->GetTargetID() <= 0)
	{
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_CANCCEL_DOUBLE_MEDITATION);
		return;
	}

	player->GetMeditationMgr()->BroadCastSingleMeditation(true);

	player->GetMeditationMgr()->NoticeTarget();

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_CANCCEL_DOUBLE_MEDITATION);
}


int MeditationMgr::Invite(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target)
{
	std::map<int64,int64>& ref = m_InvitLists[invitor->GetID()];

	//已经在申请列表中了
	std::map<int64,int64>::iterator it = ref.find(target->GetID());
	if(it != ref.end())
	{
		return eMeditationInInvit;
	}

	int count = GetBeInvitCount(target->GetID());
	if( count > m_max_beInvite)
	{
		return eMeditationTargetStateLimit;
	}

	//加入列表
	ref[target->GetID()] = CUtil::GetNowSecond();

	UpdateBeInvitCount(target->GetID(),1);

	//通知目标玩家
	MeditationProto::SendInvitInfo toClient;
	toClient.set_charid(invitor->GetID());
	toClient.set_charname(invitor->GetName());
	toClient.set_viplevel(invitor->getAttManage()->getValue<int>(eVIPLv));

	target->SendSynchMessage(&toClient, MSG_GM2C_RECEIVE_MEDITATION_INVIT);

	//开启定时检测
	if(!m_MeditationCheckTimeID.IsValid())
	{
		m_MeditationCheckTimeID = TimerInterface::GetInstance()->RegRepeatedTime(&MeditationMgr::CheckTimer, this, 0, 0, 0, 0, 5000, 5000);
	}

	return 0;
}

int MeditationMgr::Reply(Smart_Ptr<Player>& player, bool flag, int64 targetID)
{
	//拒绝
	if(!flag)
	{
		//从列表中移出
		RemoveFromLists(targetID, player->GetID());
		return 0;
	}

	if(player->GetGameZone() == NULL)
	{
		return eMeditationNotSameMap;
	}

	//对方是否还在线， 是否同一张地图
	Smart_Ptr<Player> target = player->GetGameZone()->GetPlayer(targetID);
	if(target.Get()==NULL)
	{
		return eMeditationNotSameMap;
	}

	//是否在申请列表
	if( !IsInInviteLists(targetID, player->GetID()) )
	{
		return eMeditationInvitValid;
	}

	//死亡状态
	if(player->IsDead())
	{
		return eCreatureDead;
	}

	//自己的状态
	if(player->IsInCreatureState(ERS_Distortion))
	{
		return eMeditationStateLimit;
	}

	//已经有对象了
	if(player->GetMeditationMgr()->GetTargetID() > 0)
	{
		return eMeditationStateLimit;
	}

	//状态是否已经允许
	if(target->GetMeditationMgr()->GetTargetID() > 0 || !target->IsInCreatureState(ERS_Meditation))
	{
		return eMeditationStateChange;
	}

	return 0;
}

//得到玩家被邀请的次数
int MeditationMgr::GetBeInvitCount(int64 charid)
{
	std::map<int64,int64>::const_iterator it = m_beInvits.find(charid);
	if(it != m_beInvits.end())
	{
		return it->second;
	}
	return 0;
}

//更新被邀请的次数
void MeditationMgr::UpdateBeInvitCount(int64 charid, int add)
{
	std::map<int64,int64>::iterator it = m_beInvits.find(charid);
	if(it != m_beInvits.end())
	{
		it->second += add;
		if(it->second < 0)
		{
			it->second = 0;
		}
	}
	else if(add > 0)
	{
		m_beInvits[charid] = add;
	}

}

void MeditationMgr::CheckTimer(void * obj)
{
	int64 curTime = CUtil::GetNowSecond();
	std::map<int64,std::map<int64,int64> >::iterator it = m_InvitLists.begin();
	for(; it != m_InvitLists.end(); )
	{
		std::map<int64,int64>::iterator temp = it->second.begin();
		for(; temp != it->second.end(); )
		{
			if(curTime - temp->second >= m_InvitValidTime)
			{
				it->second.erase(temp++);
			}
			else
			{
				++temp;
			}
		}

		if(it->second.size()==0)
		{
			m_InvitLists.erase(it++);
		}
		else
		{
			++it;
		}
	}

}

bool MeditationMgr::IsInInviteLists(int64 inviteID, int64 targetID)
{
	std::map<int64,std::map<int64,int64> >::const_iterator it = m_InvitLists.find(inviteID);
	if(it == m_InvitLists.end())
	{
		return false;
	}

	std::map<int64,int64>::const_iterator temp = it->second.find(targetID);
	if(temp == it->second.end())
	{
		return false;
	}

	return true;
}

void MeditationMgr::RemoveFromLists(int64 inviteID, int64 targetID)
{
	std::map<int64,std::map<int64,int64> >::iterator it = m_InvitLists.find(inviteID);
	if(it == m_InvitLists.end())
	{
		return;
	}

	std::map<int64,int64>::iterator temp = it->second.find(targetID);
	if(temp == it->second.end())
	{
		return;
	}

	it->second.erase(temp);

	if(it->second.size() == 0)
	{
		m_InvitLists.erase(it);
	}

	UpdateBeInvitCount(targetID, -1);

}

//从邀请列表中移出某个玩家的邀请数据
void MeditationMgr::RemovInvits(int64 charID)
{
	std::map<int64,std::map<int64,int64> >::iterator it = m_InvitLists.find(charID);
	if(it != m_InvitLists.end())
	{
		m_InvitLists.erase(it);
	}
}

void MeditationMgr::RemoveBeInvites(int64 charID)
{
	std::map<int64,int64>::iterator it = m_beInvits.find(charID);
	if(it != m_beInvits.end())
	{
		m_beInvits.erase(it);
	}

}

void MeditationMgr::PlayerFightGirlWar(Smart_Ptr<Player>& player)
{
	if(!player->IsInCreatureState(ERS_Meditation))
	{
		return;
	}
}

void MeditationMgr::StartDoubleMeditation(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target, int type)
{
	//通知取消采集
	Smart_Ptr<Player> temp = target;
	OneParmaEventArgs args(temp);
	FireGlobalEvent(PLAYER_MEDITATION_START, args);

	//死亡状态处理
	if(invitor->IsDead() || target->IsDead())
	{
		return;
	}

	//通过邀请者的位置移动目标的位置
	MovePlayerPos(invitor, target);

	//更新玩家的双修信息
	UpdateDoubleMeditation(invitor, target, type);

	//广播
	MeditationProto::BroadcastMeditation broad;
	broad.set_charkey( invitor->GetKey() );
	broad.set_targetkey( target->GetKey() );

	Smart_Ptr<GirlWar> girl1 = invitor->GetGirlWarManager()->GetGirlWar();
	Smart_Ptr<GirlWar> girl2 = target->GetGirlWarManager()->GetGirlWar();

	broad.set_girlwarkey( girl1 ? girl1->GetKey() : 0 );
	broad.set_targetgirlwarkey( girl2 ?  girl2->GetKey() : 0);

	target->SendToAround(&broad, MSG_GM2C_BROADCAST_DOUBLE_MEDITATINO);

}

void MeditationMgr::UpdateDoubleMeditation(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target, int type)
{
	//取消单修
	invitor->GetMeditationMgr()->SetTargetID( target->GetID() );
	invitor->GetMeditationMgr()->CanccelTimer();

	target->GetMeditationMgr()->SetTargetID( invitor->GetID() );
	target->GetMeditationMgr()->UpdateSelfDoubleState();

	UpdateBeInvitCount(invitor->GetID(),-m_max_beInvite);
	UpdateBeInvitCount(target->GetID(),-m_max_beInvite);

	//开启双修公共定时器
	StartDoubleTimer(invitor, target, type);

}

void MeditationMgr::StartDoubleTimer(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target, int type)
{
	int64 min = invitor->GetID() < target->GetID() ? invitor->GetID() : target->GetID();
	int64 max = invitor->GetID() > target->GetID() ? invitor->GetID() : target->GetID();

	DoubleMeditationDataParam* p = new DoubleMeditationDataParam();
	p->invitor = invitor->GetID();
	p->target = target->GetID();
	p->type = type;
	p->beginTime = CUtil::GetNowSecond();

	DoubleMeditationData& ref = m_timerList[make_pair(min,max)];
	ref.param = p;
	ref.timer = TimerInterface::GetInstance()->RegRepeatedTime(&MeditationMgr::OnDoubleMeditation, this, (void*)p, 0, 0, 0, 30000, 30000);

}

void MeditationMgr::MovePlayerPos(Smart_Ptr<Player>& invitor, Smart_Ptr<Player>& target)
{
	int xNewPos = 0;
	int yNewPos = 0;

	CalNewPos(invitor, xNewPos, yNewPos);

	target->SetXNewPos(xNewPos);
	target->SetYNewPos(yNewPos);
	target->SetDataFlag(eBaseInfo);
	target->ClearPath();

	Smart_Ptr<Object> obj = target;

	if(!CSceneMgr::GetInstance()->MoveToAll(obj))
	{
		LOG_ERROR(FILEINFO, "player[gs charid=%lld] move  failed ", GET_PLAYER_CHARID(target->GetID()));
	}

	ChangeMap::GMForceClientChangeMap toClient;

	toClient.set_mapid(GET_MAP_ID(target->GetMapID()));
	toClient.set_xpos(xNewPos);
	toClient.set_ypos(yNewPos);

	MapStaticInfo *info = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(target->GetMapID()));
	if(info != NULL)
	{
		toClient.set_pktype(target->GetPkTypeByMap(info->pktype));
	}

	QuickMessage::GetInstance()->AddSendMessage( target->GetID(), target->GetChannelID(), &toClient, MSG_SIM_GM2C_FORCE_CHANGEMAP);

	Smart_Ptr<GirlWar> girlWar =target->GetGirlWarManager()->GetGirlWar();
	if(girlWar)
	{
		girlWar->RelocatePosToAll(xNewPos,yNewPos);
	}

}


bool MeditationMgr::CalNewPos(Smart_Ptr<Player>& invitor, int &newxPos, int &newypos)
{
	int distance = 4;
	int xpos = invitor->GetXNewPos();
	int ypos = invitor->GetYNewPos();

	newxPos = xpos;
	newypos = ypos;

	std::vector< std::pair<int,int> > vPos;

	//上
	vPos.push_back( make_pair(xpos, ypos-distance) );

	//右上
	//vPos.push_back( make_pair(xpos+distance, ypos-distance) );

	//右
	vPos.push_back( make_pair(xpos+distance, ypos) );

	//右下
	//vPos.push_back( make_pair(xpos+distance, ypos+distance) );

	//下
	vPos.push_back( make_pair(xpos, ypos+distance) );

	//左下
	//vPos.push_back( make_pair(xpos-distance, ypos+distance) );

	//左
	vPos.push_back( make_pair(xpos-distance, ypos) );

	//左上
	//vPos.push_back( make_pair(xpos-distance, ypos-distance) );

	bool isValid = false;

	CGameZone * pZone = invitor->GetGameZone();
	if(pZone == NULL)
	{
		return isValid;
	}

	for(size_t i=0; i<vPos.size(); ++i)
	{
		if(pZone->IsValidPos(vPos[i].first, vPos[i].second) && IsCanMeditation(pZone, vPos[i].first, vPos[i].second))
		{
			newxPos = vPos[i].first;
			newypos = vPos[i].second;
			isValid = true;
			break;
		}
	}

	return isValid;
}

void MeditationMgr::OnDoubleMeditation(void* args)
{
	DoubleMeditationDataParam* pParam = (DoubleMeditationDataParam*)args;
	pParam->count++;

	Smart_Ptr<Player> invitor;
	Smart_Ptr<Player> target;

	PlayerMessage::GetInstance()->GetPlayerByCharId(invitor, pParam->invitor);
	PlayerMessage::GetInstance()->GetPlayerByCharId(target, pParam->target);

	//计算加经验 真气 亲密度
	int closeValue = 0;

	const MeditationConfig* pConfig = GetConfig(pParam->type);
	if(pConfig == NULL)
	{
		return;
	}

	if( (30*pParam->count) % (pConfig->closedTime/1000) == 0 )
	{
		closeValue = pConfig->closedValue;
	}

	//奖励
	if(invitor && target)
	{
		invitor->GetMeditationMgr()->Prize(pConfig->rate, closeValue);
		target->GetMeditationMgr()->Prize(pConfig->rate, closeValue);
	}

	//通知世界服更新亲密度
	if(closeValue > 0)
	{
		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(invitor->GetID()),worldSvr);
		if(worldSvr.Get() != NULL)
		{
			ServerReturn::ServerThreeInt gstows;
			gstows.set_retf( invitor->GetID() );
			gstows.set_rets( target->GetID() );
			gstows.set_rett( closeValue );

			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_GM2WS_ADD_CLOSENESS, &gstows, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}

}

void MeditationMgr::CanccelDoubleMeditation(int64 invitor, int64 target)
{
	int64 min = invitor < target ? invitor : target;
	int64 max = invitor > target ? invitor : target;

	std::map< pair<int64,int64>, DoubleMeditationData>::iterator it = m_timerList.find( make_pair(min,max) );
	if(it == m_timerList.end())
	{
		return;
	}

	if(it->second.param != NULL)
	{
		delete it->second.param;
		it->second.param = NULL;
	}

	if(it->second.timer.IsValid())
	{
		it->second.timer.SetNull();
	}

	m_timerList.erase(it);
}



