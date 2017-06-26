/*
 * FestivalActive.cpp
 *
 *  Created on: 2015年4月30日
 *      Author: root
 */
#include "FestivalActive.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../QuickMessage.h"
#include "../EventSet/EventDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/FestivalActive/FestivalActive.pb.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "FestivalAttr.h"
#include "FileLoader/GameParaLoader.h"

FestivalActiveMgr::FestivalActiveMgr()
{
	loadConfig();
	loadPrize();

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_START_END_ACTIVE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalActiveMgr,MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(FestivalActiveMgr,MSG_REQ_C2GS_FESTIVAL_EXCHAGE_INFO);

	m_monsterKilledEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this,&FestivalActiveMgr::handleMonsterDead);
	initTimer();
}

FestivalActiveMgr::~FestivalActiveMgr()
{
	if(m_stime.IsValid())
	{
			m_stime.SetNull();
	}
}


void FestivalActiveMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
			DEF_SWITCH_TRY_DISPATCH_BEGIN
			DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_START_END_ACTIVE);
			DEF_SWITCH_TRY_DISPATCH_END
}

void FestivalActiveMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
			DEF_SWITCH_TRY_DISPATCH_BEGIN

			DEF_SWITCH_TRY_DISPATCH_END
}

void FestivalActiveMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
			DEF_SWITCH_TRY_DISPATCH_BEGIN

			DEF_SWITCH_TRY_DISPATCH_END
}

bool FestivalActiveMgr::isActive(int id)
{
	return true;
}

int64 FestivalActiveMgr::timeconvert(int year,int month,int day,int hour,int minute,int seconds)
{
		struct tm local;
		time_t t = 0;
		time(&t);
		localtime_r(&t,&local);
		local.tm_year = year-1900;
		local.tm_mon = month-1;
		local.tm_mday = day;
		local.tm_hour = hour;
		local.tm_min = minute;
		local.tm_sec = seconds;
		t = mktime(&local);

		int64 ntime = t;

	return ntime;
}

bool FestivalActiveMgr::getfestivalTime(int id,int64& begin, int64& end)
{
	std::map<int,FestivalActiveInfo>::const_iterator it=m_festivalInfo.find(id);
	if(it==m_festivalInfo.end())
		return false;
	begin = it->second.startTime;
	end = it->second.endTime;
	return true;
}

void	FestivalActiveMgr::FestivalCmd(Smart_Ptr<Player>& player)
{
		FestivalActivePackage::FestivalActiveStartEnd cmd;
		std::map<int,int>::const_iterator it=m_curActiveFestival.begin();
		for(; it!=m_curActiveFestival.end(); ++it)
		{
				int64 begin=0;
				int64 end=0;
				if(!getfestivalTime(it->first, begin, end))
					continue;
				cmd.set_id(it->first);
				cmd.set_flag(0);
				cmd.set_begintime(begin);
				cmd.set_endtime(end);
				QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(), &cmd, MSG_SIM_GS2C_START_END_ACTIVE);
		}
}

//得到掉落的物品
	int FestivalActiveMgr::getDropItem()
	{
			if(m_curActiveFestival.size()==0)
				return 0;

			//取当前进行中的第一个活动
			std::map<int,int>::const_iterator it=m_curActiveFestival.begin();
			if(it==m_curActiveFestival.end())
				return 0;

			std::map<int,FestivalActiveInfo>::const_iterator itInfo=m_festivalInfo.find(it->second);
			if(itInfo==m_festivalInfo.end())
				return 0;

			if(itInfo->second.dropInfo.size()==0)
				return 0;

			int seed = rand()%1000;
			if(seed > itInfo->second.rate)
				return 0;

			//如果只有一个不需随机
			if(itInfo->second.dropInfo.size()==1)
			{
					return itInfo->second.dropInfo[0].first;
			}
			//再随机一个种子
			seed = rand()%100;
			float value=0;
			for(size_t i=0; i<itInfo->second.dropInfo.size(); ++i)
			{
				 value += itInfo->second.dropInfo[i].second;
				 if(value >= seed)
				  {
					 	 return itInfo->second.dropInfo[i].first;
				   }
			}

			return 0;
	}

	const FestivalActiveInfo* FestivalActiveMgr::getFestivalInfo(int id)
	{
			std::map<int,FestivalActiveInfo>::const_iterator it=m_festivalInfo.find(id);
			if(it!=m_festivalInfo.end())
				return &it->second;
			return NULL;
	}

	const FestivalActiveInfo* FestivalActiveMgr::getCurFestivalInfo()
	{
			//取当前进行中的第一个活动
			std::map<int,int>::const_iterator it=m_curActiveFestival.begin();
			if(it==m_curActiveFestival.end())
					return NULL;
			return getFestivalInfo(it->first);
	}

void FestivalActiveMgr::loadConfig()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return;
		}

		string strFile = strFileFullPath + "Data/FestivalActive.csv";
		CSVLoader loader;
		if(!loader.OpenFromFile(strFile.c_str()))
		{
				return;
		}

		//活动id	活动开始时间(年月日小时分钟)	活动结束时间	怪物与玩家掉落等级差	怪物死亡掉落物品几率(单位百分比为1000)	物品掉落ID及权重(权重之和为100)
		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
				FestivalActiveInfo info;
				int count = 0;
				//id
				info.id = loader.GetInt(i, count++);

				//活动开始时间(年月日小时分钟)
				std::string strTime=loader.GetString(i, count++);
				StrArgPkg strTimePkg("-", strTime);
				if(strTimePkg.size() != 5)
						return;

				int year=atoi(getArg(strTimePkg,0).c_str());
				int month=atoi(getArg(strTimePkg,1).c_str());
				int day=atoi(getArg(strTimePkg,2).c_str());
				int hour=atoi(getArg(strTimePkg,3).c_str());
				int minute=atoi(getArg(strTimePkg,4).c_str());
				info.startTime = timeconvert(year,month,day,hour,minute,0);

				//活动结束时间
				strTime = loader.GetString(i,count++);
				StrArgPkg strPkg2("-", strTime);
				if(strPkg2.size() != 5)
						return;

				year=atoi(getArg(strPkg2,0).c_str());
				month=atoi(getArg(strPkg2,1).c_str());
				day=atoi(getArg(strPkg2,2).c_str());
				hour=atoi(getArg(strPkg2,3).c_str());
				minute=atoi(getArg(strPkg2,4).c_str());
				info.endTime = timeconvert(year,month,day,hour,minute,0);

				//等级差
				info.lv = loader.GetInt(i, count++);

				//怪物死亡掉落物品几率(单位百分比为1000)
				info.rate = loader.GetInt(i, count++);

				//物品掉落ID及权重(权重之和为100)
				std::string strDrop = loader.GetString(i, count++);
				StrArgPkg strPkg("|", strDrop);
				for(size_t n=0; n<strPkg.size(); ++n)
				{
					 std::string strProp = getArg(strPkg,n);
					 StrArgPkg strPropPkg(":", strProp);
					 if(strPropPkg.size()!=2)
						 continue;
					 int dropID = atoi(getArg(strPropPkg,0).c_str());
					 float dropRate = atof(getArg(strPropPkg,1).c_str());
					 	info.dropInfo.push_back(make_pair(dropID,dropRate));
				}
				m_festivalInfo[info.id] = info;
			}
}

void FestivalActiveMgr::loadPrize()
{
			std::string strFileFullPath;
			if(CUtil::GetModulePath(strFileFullPath) != 0)
			{
					return;
			}

			string strFile = strFileFullPath + "Data/FestivalPrize.csv";
			CSVLoader loader;
			if(!loader.OpenFromFile(strFile.c_str()))
			{
					return;
			}

			//奖励兑换id	所属活动id	兑换需要的物品id及数量	兑换得到的物品id及数量
			for(unsigned int i=0; i<loader.GetRowCount(); ++i)
			{
					FestivalPrize info;
					int count = 0;
					//id
					info.id = loader.GetInt(i, count++);
					info.activeID = loader.GetInt(i, count++);

					//消耗物品
					std::string strCost = loader.GetString(i, count++);
					StrArgPkg strCostPkg("|", strCost);
					for(size_t n=0; n<strCostPkg.size(); ++n)
					{
						std::string strProp = getArg(strCostPkg,n);
						StrArgPkg strPropPkg(":", strProp);
						if(strPropPkg.size()!=2)
							 continue;
						int id = atoi(getArg(strPropPkg,0).c_str());
						int num = atoi(getArg(strPropPkg,1).c_str());
						info.costProp.push_back(make_pair(id,num));
					}

					//得到物品
					std::string strGet = loader.GetString(i, count++);
					StrArgPkg strGetPkg("|",strGet);
					for(size_t n=0; n<strGetPkg.size(); ++n)
					{
						std::string strProp = getArg(strGetPkg,n);
						StrArgPkg strPropPkg(":", strProp);
						if(strPropPkg.size()!=2)
							 continue;
						int id = atoi(getArg(strPropPkg,0).c_str());
						int num = atoi(getArg(strPropPkg,1).c_str());
						info.getProp.push_back(make_pair(id,num));
					}

					//
					info.count = loader.GetInt(i, count++);
					m_festivalPrizeInfo[info.id] = info;
			}
}

bool FestivalActiveMgr::handleMonsterDead(const EventArgs& e)
{
	 TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
		if(!data)
		{
			return true;
		}

		if(false == bool(data->m_other))
			return true;

		if(data->m_other->GetType() != ePlayer && data->m_other->GetType() != ePet)
			return true;

		Smart_Ptr<Player>  player;
		Smart_Ptr<GirlWar>  girlwar;

		if(data->m_other->GetType() == ePet)
		{
			girlwar = data->m_other;
			player  = girlwar->GetOwner();
		}
		else
		{
			player = data->m_other;
		}

		Smart_Ptr<Monster> mon = data->m_myself;
		if(player.Get() && mon.Get() && mon->GetQuality()!=eGatherMonster && mon->GetQuality()!= eEscortCar
				&& mon->GetQuality()!=eBoxMonster)
		{
			InstanceMapEntry* pInfo = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
			if(pInfo!=NULL)
				return true;

			const FestivalActiveInfo* pActive = getCurFestivalInfo();
			if(pActive==NULL)
				return true;
			int tempPlayerLevel  = player->getAttManage()->getValue<int>(eCharLv);
			int tempMonsterLevel = mon->getAttManage()->getValue<int>(eCharLv);
			if(tempPlayerLevel > tempMonsterLevel && tempPlayerLevel-tempMonsterLevel > pActive->lv)
			{
				return true;
			}

			int propID=getDropItem();
			if(propID==0)
				return true;

			int64 timeOut = pActive->endTime;
			DropItemsManager::GetInstance()->festivalActiveDrop(mon,player,propID, timeOut);
		}

		return true;
}

DEF_MSG_SIMPLE_DEFINE_FUN(FestivalActiveMgr,MSG_SIM_WS2GS_START_END_ACTIVE)
{
	//更新内存
	FestivalActivePackage::FestivalActiveStartEnd req;
	int len=0;
	char* content = message->GetBuffer(len);
	req.ParseFromArray(content,len);

	//0标识表示开始;1表示结束
	if(req.flag()==0)
	{
		m_curActiveFestival[req.id()] = req.id();
	}
	else
	{
		std::map<int,int>::iterator it=m_curActiveFestival.find(req.id());
		if(it!=m_curActiveFestival.end())
		{
			m_curActiveFestival.erase(it);
		}
	}

	//通知在线玩家
	int64 begin=0;
	int64 end=0;
	if(!getfestivalTime(req.id(), begin, end))
			return;

	FestivalActivePackage::FestivalActiveStartEnd cmd;
	cmd.set_id(req.id());
	cmd.set_flag(req.flag());
	cmd.set_begintime(begin);
	cmd.set_endtime(end);

	INT64PLAYERMAP &ref = PlayerMessage::GetInstance()->getAllPlayerRef();
	INT64PLAYERMAP::const_iterator it=ref.begin();
	for(; it!=ref.end(); ++it)
	{
			if(it->second.Get()==NULL)
				continue;
			QuickMessage::GetInstance()->AddSendMessage(it->second->GetID(), it->second->GetChannelID(), &cmd, MSG_SIM_GS2C_START_END_ACTIVE);
	}

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalActiveMgr,MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE)
{
		FestivalActivePackage::ReqFestivalPrize req;
		//FestivalActivePackage::AckFestivalPrize ret;
		ServerReturn::ServerDoubleRetInt ret;
		req.ParseFromString(content);

		std::map<int,FestivalPrize>::const_iterator itInfo=m_festivalPrizeInfo.find(req.id());
		if(itInfo==m_festivalPrizeInfo.end())
			return;

		//当前活动是否进行中
		std::map<int,int>::iterator it=m_curActiveFestival.find(itInfo->second.activeID);
		if(it==m_curActiveFestival.end())
		{
				ret.set_retf(eActivityNotExist);
				REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE);
				return;
		}

		//
		int count = player->getFestivalMgr()->getAttr(req.id());
		if(count >= itInfo->second.count && itInfo->second.count > 0)
		{
			ret.set_retf(eFestivalCountLimit);
			REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE);
			return;
		}

		std::vector<DWORD> idList;
		std::vector<DWORD> numList;
		std::vector<int> vecPos;
		std::vector<DWORD> strengthLv;
		std::vector<WORD> flagList;

		//物品是否足够
		for(size_t i=0; i<itInfo->second.costProp.size(); ++i)
		{
				int nums=player->GetContainer(ePackageType)->GetGoodsNum(itInfo->second.costProp[i].first);
				if(nums<itInfo->second.costProp[i].second)
						return;
		}

		//包裹空间是否足够
		for(size_t i=0; i<itInfo->second.getProp.size(); ++i)
		{
				idList.push_back(itInfo->second.getProp[i].first);
				numList.push_back(itInfo->second.getProp[i].second);
		}

		flagList.resize(idList.size(),eGetBind);
		if(player->GetContainer(ePackageType)->IsBagEnought(idList, numList,flagList) != 0)
		{
				ret.set_retf(ePackageIsFull);
				REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE);
				return;
		}

		//扣除物品
		for(size_t i=0; i<itInfo->second.costProp.size(); ++i)
		{
			player->GetContainer(ePackageType)->DeleteGoodsById(itInfo->second.costProp[i].first,
				itInfo->second.costProp[i].second, true);
		}

		//添加物品
		strengthLv.resize(idList.size(), 0);
		player->GetContainer(ePackageType)->AddItem(idList,numList,vecPos,flagList,strengthLv,true);

		player->getFestivalMgr()->attrAdd(req.id());
		int leftCount = itInfo->second.count - player->getFestivalMgr()->getAttr(req.id());

		//无次数限制时，保证剩余次数不为0
		if(leftCount <=0 && itInfo->second.count==0)
		{
			leftCount = 1;
		}

		int64 code = req.id();
		code = code<<32|leftCount;

		ret.set_retf(0);
		ret.set_rets(code);
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FestivalActiveMgr,MSG_REQ_C2GS_FESTIVAL_EXCHAGE_INFO)
{
	Smart_Ptr<PlayerFestiveAttr> pFestival = player->getFestivalMgr();
	if(!pFestival)
		return;

	ServerReturn::ServerDoubleUint32Array ret;
	std::map<int,FestivalPrize>::iterator it=m_festivalPrizeInfo.begin();
	for(; it!=m_festivalPrizeInfo.end(); ++it)
	{
		const FestivalActiveInfo* pInfo = getCurFestivalInfo();
		if(pInfo==NULL || pInfo->id != it->second.activeID)
			continue;

		int count = pFestival->getAttr(it->first);
		count = it->second.count-count;
		if(count<0)
			count = 0;

		//无限制时保证次数不为0
		if(it->second.count==0 && count <=0 )
		{
			count = 1;
		}

		ServerReturn::ServerDoubleUint32* pItem = ret.add_ret();
		pItem->set_retf(it->first);
		pItem->set_rets(count);
	}
	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_FESTIVAL_EXCHAGE_INFO);
}

void FestivalActiveMgr::initTimer()
{
		if(m_stime.IsValid())
		{
				m_stime.SetNull();
		}
		int hour=0,minute=0,seconds=0;
		GameParaLoader::GetInstance()->getDayUpdateTime(hour,minute,seconds);
		m_stime = TimerInterface::GetInstance()->RegDayPointRepeatedTime(&FestivalActiveMgr::onTime,this,0,hour,minute,seconds);
}

void FestivalActiveMgr::onTime(void * obj)
{
	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	INT64PLAYERMAP::const_iterator it = players.begin();
	for(; it!=players.end(); ++it)
	{
		if(it->second.Get()==NULL)
				continue;
		if(it->second->getFestivalMgr().Get()==NULL)
			continue;
		it->second->getFestivalMgr()->reSetAttr();
	}
}

