/*
 * GsCountryMgr.cpp
 *
 *  Created on: 2015年3月18日
 *      Author: root
 */
#include "GsCountryMgr.h"
#include <math.h>
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"
#include "../QuickMessage.h"
#include "../EventSet/EventDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/Country/Country.pb.h"
#include "stdlib.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "playerCampShopMgr.h"
#include "../ServerEventDefine.h"
#include "./FileLoader/GameParaLoader.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/FileLoader/DropItemLoader.h"
#include "../Attribute/AttributeManage.h"
#include "../Mail/MailMessage.h"
#include "../StatisticMgr/StatisticHandle.h"

GsCountryMgr::GsCountryMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_JUANXIAN);
	DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_JOB_DAYPRIZE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_SHOPINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_BUY);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GS2WS_COUNTRY_JUANXIAN);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_WS2GS_COUNTRY_BASEINFO);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_WS2GS_COUNTRY_LIVEUP);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_WS2GS_COUNTRY_RANK_RESET);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_WS2GS_COUNTRY_ATTR_ALTER);
	
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_GETJOINAWARD); // 获取加入推荐阵营奖励

	m_kingFightPower = 0;
	m_MoneyPropID = 0;
	m_GoldenPropID = 0;
	m_GoldenRate = 0;
	m_nFirstTime = 0;
	m_nSecondTime = 0;
	m_nThirdTime = 0;
	m_nGoosId = 0;
	m_nGoodsNum = 0;
	m_pLua.OpenLua();

	loadConfig();	
	initTimer();
}

GsCountryMgr::~GsCountryMgr()
{
	m_pLua.CloseLua();
}



int GsCountryMgr::getContributeByProp(int propID)
{
		std::map<int,int>::const_iterator it=m_propContributeInfo.find(propID);
		if(it!=m_propContributeInfo.end())
			return it->second;
		return 0;
}

int GsCountryMgr::getContributeByMoney(int money)
{
		std::map<int,int>::const_iterator it=m_moneyContributeInfo.find(money);
		if(it!=m_moneyContributeInfo.end())
				return it->second;
		return 0;
}

int GsCountryMgr::getContributeByGolden(int golden)
{
		std::map<int,int>::const_iterator it=m_goldenContributeInfo.find(golden);
		if(it!=m_goldenContributeInfo.end())
				return it->second;
		return 0;
}

const CampShopInfo* GsCountryMgr::getShopInfo(int id)
{
		std::map<int,CampShopInfo>::const_iterator it=m_shopInfo.find(id);
		if(it!=	m_shopInfo.end())
				return &it->second;
		return NULL;
}

const CampGradeInfo* GsCountryMgr::getCampInfo(int lv)
{
		std::map<int,CampGradeInfo>::const_iterator it=m_CampGradeList.find(lv);
		if(it!=m_CampGradeList.end())
				return &it->second;
		return NULL;
}

const CampJobPrize* GsCountryMgr::getCampJobInfo(BYTE job)
{
		std::map<unsigned char,CampJobPrize>::const_iterator it = m_JobList.find(job);
		if(it!=m_JobList.end())
				return &it->second;
		return NULL;
}

void GsCountryMgr::GetShopInfo(std::map<int, CampShopInfo> & info)
{
	info = m_shopInfo;
}

int GsCountryMgr::getCampGrade(int id)
{
		std::map<int,int>::const_iterator it=m_campbaseInfo.find(id);
		if(it!=m_campbaseInfo.end())
			return it->second;
		return -1;
}

void GsCountryMgr::getCampAttr(int id, std::vector<std::pair<int,int> >& attrs)
{
	//get lv by county id
	int camplv = getCampGrade(id);
	if(camplv==-1)
		return;

	const CampGradeInfo* pGradeInfo=GsCountryMgr::getCampInfo(camplv);
	if(pGradeInfo==NULL)
		return;

	attrs.push_back( make_pair(eCharHPMax,pGradeInfo->HP));
	attrs.push_back( make_pair(eCharPhyAttackMax,pGradeInfo->attack));
	attrs.push_back( make_pair(eCharPhyDefence,pGradeInfo->defense));
}

void GsCountryMgr::loadConfig()
{
			loadLuaConfig();
			loadContribute();
			//陈营信息表
			loadCampInfo();
			//官职奖励表
			loadJobInfo();
			//陈营商店表
			loadShop();
			//阵营boss战
			loadBossInfo();
}

void GsCountryMgr::loadLuaConfig()
{
		int tableCount = 0;
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
			return ;
		}

		string strConfig = strFileFullPath + "Data/country.lua";
		if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
		{
				return;
		}

		//阵营霸主争夺的战力
		m_pLua.SetLuaFunction("GetPower");
		if(m_pLua.Call(0, 1))
		{
				return;
		}
		m_kingFightPower = m_pLua.GetInt();

		//铜钱、元宝id及捐献的资产计算倍数
		m_pLua.SetLuaFunction("GetMoneyPropInfo");
		if(m_pLua.Call(0, 3))
		{
				return;
		}
		m_GoldenRate = m_pLua.GetInt();
		m_GoldenPropID = m_pLua.GetInt();
		m_MoneyPropID = m_pLua.GetInt();


		m_pLua.SetLuaFunction("GetFlushBoss");
		if(m_pLua.Call(0, 4))
		{
				return;
		}

		m_nFlushConTime = m_pLua.GetInt();
		m_nThirdTime = m_pLua.GetInt();
		m_nSecondTime = m_pLua.GetInt();
		m_nFirstTime = m_pLua.GetInt();

		m_pLua.SetLuaFunction("GetCampAwardGoods");
		if(m_pLua.Call(0, 4))
		{
				return;
		}

		m_nGoodsNum = m_pLua.GetInt();
		m_nGoosId = m_pLua.GetInt();

		m_pLua.SetLuaFunction("GetCampAward");
		if(m_pLua.Call(0, 1))
		{
						return;
		}

		tableCount = m_pLua.GetTableFieldCount();

		for(int i = 1 ; i <= tableCount ; i++)
		{
			if (m_pLua.GetFieldTable(i) != 0)
			{
				return;
			}

			m_CampGoods[m_pLua.GetFieldInt("id")] = m_pLua.GetFieldInt("num");
			m_pLua.CloseTable();
		}
		m_pLua.CloseTable();

		m_pLua.SetLuaFunction("GetKillBossScore");
		if(m_pLua.Call(0, 1))
		{
						return;
		}

		tableCount = m_pLua.GetTableFieldCount();

		for(int i = 1 ; i <= tableCount ; i++)
		{
			if (m_pLua.GetFieldTable(i) != 0)
			{
				return;
			}

			m_KillBossScore[m_pLua.GetFieldInt("boss")] = m_pLua.GetFieldInt("score");
			m_pLua.CloseTable();
		}
		m_pLua.CloseTable();

		m_pLua.SetLuaFunction("GetBossNum");
		if(m_pLua.Call(0, 1))
		{
						return;
		}

		tableCount = m_pLua.GetTableFieldCount();

		for(int i = 1 ; i <= tableCount ; i++)
		{
			if (m_pLua.GetFieldTable(i) != 0)
			{
				return;
			}

			m_BossNum[m_pLua.GetFieldInt("boss")] = m_pLua.GetFieldInt("seq");
			m_pLua.CloseTable();
		}
		m_pLua.CloseTable();

		m_pLua.SetLuaFunction("GetJoinRcndAward");
		if (m_pLua.Call(0, 1))
		{
			return;
		}

		tableCount = m_pLua.GetTableFieldCount();
		for (int i = 1; i <= tableCount; ++i)
		{
			if (0 != m_pLua.GetFieldTable(i))
			{
				return;
			}

			m_joinRcndAward.idList.push_back(m_pLua.GetFieldInt("ID"));
			m_joinRcndAward.numList.push_back(m_pLua.GetFieldInt("num"));
			m_joinRcndAward.isBind.push_back(m_pLua.GetFieldInt("isBind"));

			m_pLua.CloseTable();
		}

		m_pLua.CloseTable();

		m_pLua.SetLuaFunction("GetShowCntyText");
		if (m_pLua.Call(0, 1))
		{
			return;
		}

		tableCount = m_pLua.GetTableFieldCount();

		m_showCntyText[eCountryID_sui] = m_pLua.GetFieldInt(eCountryID_sui);
		m_showCntyText[eCountryID_tang] = m_pLua.GetFieldInt(eCountryID_tang);

		m_pLua.CloseTable();
}

void GsCountryMgr::loadContribute()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return ;
		}
		string strFile = strFileFullPath + "Data/CampContributionTypeData.csv";
		CSVLoader loader;
		if(!loader.OpenFromFile(strFile.c_str()))
		{
				return;
		}

		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
			int count = 0;

			//忽略字段
			loader.GetInt(i, count++);
			loader.GetInt(i, count++);
			//道具id
			int propID = loader.GetInt(i, count++);
			//名称
			loader.GetString(i, count++);

			//数量
			std::string strpk = loader.GetString(i, count++);
			if(GameParaLoader::GetInstance()->getExchangeMoneyItemId()==propID)
			{
					StrArgPkg countPkg("|", strpk);
					for(size_t n=0; n<countPkg.size(); ++n)
					{
							m_moneyContributeInfo[atoi(getArg(countPkg,n).c_str())] = 0;
					}
					std::string  strpk= loader.GetString(i,count++);
					StrArgPkg strContribute("|",strpk);
					std::map<int,int>::iterator it=m_moneyContributeInfo.begin();
					for(size_t n=0; it!=m_moneyContributeInfo.end() && n<strContribute.size(); ++n,++it)
					{
							it->second = atoi(getArg(strContribute,n).c_str());
					}
			}
			else if(GameParaLoader::GetInstance()->getExchangeGoldenItemId()==propID)
			{
					StrArgPkg countPkg("|", strpk);
					for(size_t n=0; n<countPkg.size(); ++n)
					{
						m_goldenContributeInfo[atoi(getArg(countPkg,n).c_str())] = 0;
					}
					std::string  strpk= loader.GetString(i,count++);
					StrArgPkg strContribute("|",strpk);
					std::map<int,int>::iterator it=m_goldenContributeInfo.begin();
					for(size_t n=0; it!=m_goldenContributeInfo.end() && n<strContribute.size(); ++n,++it)
					{
							it->second = atoi(getArg(strContribute,n).c_str());
					}
			}
			else
			{
					m_propContributeInfo[propID] = loader.GetInt(i,count++);
			}
		}

}

//陈营信息表
void GsCountryMgr::loadCampInfo()
{
			std::string strFileFullPath;
			if(CUtil::GetModulePath(strFileFullPath) != 0)
			{
					return ;
			}
			string strFile = strFileFullPath + "Data/CamplvTypeData.csv";
			CSVLoader loader;
			if(!loader.OpenFromFile(strFile.c_str()))
			{
					return;
			}

			//ID	 阵营等级	铜钱	天机令	地煞令	玄真令	黄宇令	生命	物攻	法攻	物防	法防	战斗力
			for(unsigned int i=0; i<loader.GetRowCount(); ++i)
			{
				CampGradeInfo info;
				int count = 0;
				//id
				loader.GetInt(i, count++);

				info.lv = loader.GetInt(i, count++);
				info.money = loader.GetInt(i, count++);

				for(int n=0; n<4; ++n)
				{
						std::string strProp = loader.GetString(i, count++);
						StrArgPkg strPkg(":", strProp);
						if(strPkg.size()!=2)
						{
								return;
						}
						info.props.push_back( std::make_pair( atoi(getArg(strPkg,1).c_str()),atoi(getArg(strPkg,0).c_str())) );
				}

				info.HP = loader.GetInt(i,count++);
				info.attack = loader.GetInt(i,count++);
				info.magicAttack = loader.GetInt(i,count++);
				info.defense = loader.GetInt(i,count++);
				info.magicDefense = loader.GetInt(i,count++);
				info.power = loader.GetInt(i,count++);
				m_CampGradeList[info.lv] = info;
			}
}
//官职奖励表
void GsCountryMgr::loadJobInfo()
{
				std::string strFileFullPath;
				if(CUtil::GetModulePath(strFileFullPath) != 0)
				{
						return ;
				}
				string strFile = strFileFullPath + "Data/CampofficeTypeData.csv";
				CSVLoader loader;
				if(!loader.OpenFromFile(strFile.c_str()))
				{
						return;
				}

				//官职ID	官职排名	官职名称	官职人数	属性加成	礼包名称	礼包ID
				for(unsigned int i=0; i<loader.GetRowCount(); ++i)
				{
					CampJobPrize info;
					int count = 0;
					//id
					info.jobID = loader.GetInt(i, count++);
					loader.GetInt(i,count++);
					loader.GetString(i,count++);
					info.jobNums = loader.GetInt(i, count++);
					std::string strAttrRate = loader.GetString(i, count++);
					StrArgPkg strPkg("%", strAttrRate);
					if(strPkg.size()!=1)
					{
							return;
					}
					info.attriRate = atof(getArg(strPkg,0).c_str())*0.01f;
					loader.GetString(i,count++);
					info.propID = loader.GetInt(i,count++);
					m_JobList[info.jobID] = info;
				}
}

//陈营商店表
void GsCountryMgr::loadShop()
{
			std::string strFileFullPath;
			if(CUtil::GetModulePath(strFileFullPath) != 0)
			{
				return ;
			}
			string strFile = strFileFullPath + "Data/CampstoreTypeData.csv";
			CSVLoader loader;
			if(!loader.OpenFromFile(strFile.c_str()))
			{
					return;
			}

			//排序	道具ID	今日可购买数量	消耗个人阵营贡献	阵营等级
			for(unsigned int i=0; i<loader.GetRowCount(); ++i)
			{
						CampShopInfo info;
						int count = 0;

						info.index = loader.GetInt(i,count++);
						info.propId = loader.GetInt(i, count++);
						info.nums = loader.GetInt(i,count++);
						info.contribute = loader.GetInt(i,count++);
						info.campLv = loader.GetInt(i, count++);
						count++;
						info.nBindType = loader.GetInt(i, count++);
						m_shopInfo[info.propId] = info;
			}
}

void GsCountryMgr::loadBossInfo()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return ;
	}
	string strFile = strFileFullPath + "Data/CampmapTypeData.csv";
	CSVLoader loader;
	if(!loader.OpenFromFile(strFile.c_str()))
	{
			return;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int d = 0;
		int a = 0;
		int count = 0;

		BossMap bossMap;
		bossMap.nId = loader.GetInt(i,count++);
		bossMap.nXPos = loader.GetInt(i,count++);
		bossMap.nYPos = loader.GetInt(i,count++);
		bossMap.nRadius = loader.GetInt(i,count++);
		bossMap.nXPos1 = loader.GetInt(i,count++);
		bossMap.nYPos1 = loader.GetInt(i,count++);
		bossMap.nXPos2 = loader.GetInt(i,count++);
		bossMap.nYPos2 = loader.GetInt(i,count++);
		bossMap.nBossId = loader.GetInt(i,count++);
		bossMap.nRate = loader.GetInt(i,count++);
		bossMap.nItemId = loader.GetInt(i,count++);
		bossMap.nItemNum = loader.GetInt(i,count++);

		if(bossMap.nId == 0)
		{
			continue;
		}

		if(bossMap.nId  <= 2)
		{
			for(int i = bossMap.nXPos - bossMap.nRadius; i <= bossMap.nXPos + bossMap.nRadius; i++)
			{
				for(int j = bossMap.nYPos - bossMap.nRadius; j <=  bossMap.nYPos + bossMap.nRadius; j++)
				{
					d = (int)sqrt((i-bossMap.nXPos)*(i-bossMap.nXPos)+(j-bossMap.nYPos)*(j-bossMap.nYPos));
					if(d <= bossMap.nRadius)
					{
						CountryBossPos pos;
						pos.nXPos = i;
						pos.nYPos = j;
						bossMap.vPos.push_back(pos);
					}
				}
			}
		}
		else
		{
			int c =  (int)sqrt((bossMap.nXPos1 - bossMap.nXPos)*(bossMap.nXPos1 - bossMap.nXPos) - (bossMap.nYPos2 - bossMap.nYPos) * (bossMap.nYPos2 - bossMap.nYPos));
			a = (bossMap.nXPos1 - bossMap.nXPos)*2;

			for(int i = bossMap.nXPos - (bossMap.nXPos1 - bossMap.nXPos); i < bossMap.nXPos1; i++)
			{
				for(int j = bossMap.nYPos - bossMap.nYPos2; j < bossMap.nYPos + bossMap.nYPos2; j++)
				{
					d = (int)sqrt((i-(bossMap.nXPos - c))*(i-(bossMap.nXPos -c))+(j-bossMap.nYPos)*(j-bossMap.nYPos)) + (int)sqrt((i-(bossMap.nXPos + c))*(i-(bossMap.nXPos + c))+(j-bossMap.nYPos)*(j-bossMap.nYPos));
					if(d <= a)
					{
						CountryBossPos pos;
						pos.nXPos = i;
						pos.nYPos = j;
						bossMap.vPos.push_back(pos);
					}
				}
			}
		}

		m_campBoss[bossMap.nId] = bossMap;
	}
}

void GsCountryMgr::GetRandomPos(int64& nXPos,int64& nYPos,DWORD country)
{
	map<int,BossMap>::iterator itr = m_campBoss.find(country);
	if(itr != m_campBoss.end())
	{
		int temp = rand()%itr->second.vPos.size();
		nXPos = itr->second.vPos[temp].nXPos;
		nYPos = itr->second.vPos[temp].nYPos;
	}
}

void GsCountryMgr::GetRandomPos(int mapId,vector<CountryBossPos>& vec)
{
	map<int,BossMap>::iterator itr = m_campBoss.find(mapId);
	if(itr != m_campBoss.end())
	{
		vec = itr->second.vPos;
	}
}


bool GsCountryMgr::IsSafeZone(int64 nXPos,int64 nYPos,DWORD country)
{
	int d = 0;
	map<int,BossMap>::iterator itr = m_campBoss.find(country);
	if(itr != m_campBoss.end())
	{
		d = (int)sqrt((nXPos-itr->second.nXPos)*(nXPos-itr->second.nXPos)+(nYPos-itr->second.nYPos)*(nYPos-itr->second.nYPos));
		if(d <= itr->second.nRadius)
		{
			return false;
		}
	}

	return true;
}

//初始化定时器
	void GsCountryMgr::initTimer()
	{
		if(m_stime.IsValid())
		{
				m_stime.SetNull();
		}
		int hour=0,minute=0,seconds=0;
		GameParaLoader::GetInstance()->getDayUpdateTime(hour,minute,seconds);
		m_stime = TimerInterface::GetInstance()->RegDayPointRepeatedTime(&GsCountryMgr::onTime,this,0,hour,minute,seconds);
	}

	void GsCountryMgr::onTime(void * obj)
	{
				const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
				INT64PLAYERMAP::const_iterator it = players.begin();
				for(; it!=players.end(); ++it)
				{
						if(it->second.Get()==NULL)
							continue;
						if(it->second->getCampShopMgr().Get()==NULL)
							continue;
						it->second->getCampShopMgr()->reSetAttr();
						std::vector<int> updateList;
						updateList.push_back(eDayInfoUpade_CampShop);
						it->second->SendDayUpdate(updateList);
				}
	}

void GsCountryMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_WS2GS_COUNTRY_LIVEUP);
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_WS2GS_COUNTRY_BASEINFO);
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_WS2GS_COUNTRY_RANK_RESET);
		DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_WS2GS_COUNTRY_ATTR_ALTER);
		DEF_SWITCH_TRY_DISPATCH_END
}

void GsCountryMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN

		//DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_TACK_OFF);
		DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_GETJOINAWARD); // 获取加入推荐阵营奖励

		DEF_SWITCH_TRY_DISPATCH_END
}

void GsCountryMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
		DEF_SWITCH_TRY_DISPATCH_BEGIN

		DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GS2WS_COUNTRY_JUANXIAN);

		DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryMgr,MSG_WS2GS_COUNTRY_LIVEUP)
{
	CountryPackage::WSNoticeGSCountryLiveUp req;
	int len=0;
	char *content = message->GetBuffer(len);
	req.ParseFromArray(content,len);

	const CampGradeInfo* pinfo=getCampInfo(req.level());
	if(pinfo==NULL) return;

	std::map<int,int>::iterator itCountry=m_campbaseInfo.find(req.country());
	if(itCountry!=m_campbaseInfo.end())
	{
			itCountry->second = req.level();
	}

	std::vector<pair<int,int> > attrs;
	getCampAttr(req.country(),attrs);

	const INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayer();
	INT64PLAYERMAP::const_iterator it = players.begin();
	for(; it!=players.end(); ++it)
	{
		const Smart_Ptr<Player> &player = it->second;
		if(player.Get()==NULL) continue;
		if(player->getAttManage()->getValue<DWORD>(eCharCountry)!= (DWORD)req.country()) continue;
		eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttCamp);
		player->removeAttr(0,tempValueType);
		player->addAttr(0,attrs,true,tempValueType);

		std::vector<int> type;
		type.push_back(eFightPower);
		player->SynCharAttribute(type);

		//设置脏数据
		player->SetDataFlag(eBattleInfo);
	}

}

DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryMgr,MSG_WS2GS_COUNTRY_BASEINFO)
{
		CountryPackage::WStoGsCountryInfo ret;
		int len = 0;
		char * content = message->GetBuffer(len);
		ret.ParseFromArray(content,len);
		for(int i=0; i<ret.country_size(); ++i)
		{
				const CountryPackage::CountryGrade& grade = ret.country(i);
				m_campbaseInfo[grade.country()] = grade.level();
		}
}

DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryMgr,MSG_WS2GS_COUNTRY_RANK_RESET)
{
	//得到所有在线玩家
	CountryPackage::WeekUpdate req;
	int len=0;
	char* con = message->GetBuffer(len);
	req.ParseFromArray(con,len);
	INT64PLAYERMAP& players = PlayerMessage::GetInstance()->getAllPlayerRef();
	INT64PLAYERMAP::iterator it = players.begin();
	for(; it!=players.end(); ++it)
	{
		if(it->second.Get()==NULL)
			continue;
		//重置周贡献度
		it->second->setWeekContrituion(0);
		//重置官职
		bool isNewJob = false;
		for(int i=0; i<req.list_size(); ++i)
		{
			if(it->second->GetID()==req.list(i).id())
			{
				it->second->CountryJobAttrRemove();
				it->second->getAttManage()->setValue(eCountryJob,req.list(i).job());
				it->second->CountryJobAttrAdd();

				isNewJob = true;
				break;
			}
		}
		if(!isNewJob)
		{
			if(it->second->getAttManage()->getValue<WORD>(eCountryJob)>0)
			{
				it->second->CountryJobAttrRemove();
			}
			it->second->getAttManage()->setValue(eCountryJob,0);
		}
		//设置脏数据
		it->second->SetDataFlag(eBattleInfo);
		it->second->SetDataFlag(eBaseInfo);

		//通知客户端
		it->second->SynCharAttribute(eCountryJob,it->second->getAttManage()->getValue<WORD>(eCountryJob));
	}	
}

//属性改变
DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryMgr,MSG_WS2GS_COUNTRY_ATTR_ALTER)
{
	CountryPackage::WStoGSAttrChange ret;
	int len=0;
	char* content = message->GetBuffer(len);
	ret.ParseFromArray(content,len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,ret.charid());
	if(player.Get()==NULL)
		return;

	switch(ret.attrid())
	{
			case eCountryAttrType_contryid:
			{
				player->getAttManage()->setValue(eCharCountry,ret.value());

				std::vector<pair<int,int> > attrs;
				getCampAttr(ret.value(),attrs);
				eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttCamp);
				player->removeAttr(0,tempValueType);
				player->addAttr(0,attrs,true,tempValueType);

				std::vector<int> type;
				type.push_back(eFightPower);
				player->SynCharAttribute(type);

				OneParmaEventArgs tempArgs(player);
				player->FireEvent(PLAYER_JOIN_CAMP,tempArgs);
			}
			break;
		default:
			break;
	}
	//设置脏数据
	player->SetDataFlag(eBaseInfo);

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_JUANXIAN)
{
	  CountryPackage::ClientReqContribute req;
	  CountryPackage::AckClientReqContribute toClient;
		req.ParseFromString(content);

		if(player->getAttManage()->getValue<DWORD>(eCharCountry) <=  0)
		{
				toClient.set_success(1);
				REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GS_COUNTRY_JUANXIAN);
				return;
		}

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();

		//物品 金钱是否足够
		Smart_Ptr<ArticleBaseMgr> pack = player->GetPackage();
		int contribute = 0;
		bool isMoney = false;
		bool isGolden = false;
		bool isCostGoods = false;
		if(req.id()==GameParaLoader::GetInstance()->getExchangeMoneyItemId())//金钱
		{
			if(req.num() > player->getAttManage()->getValue<int>(eCharMoney)) return;
			contribute = getContributeByMoney(req.num());
			if(contribute==0) return;
			isMoney = true;

//				StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,req.num(),player);
//				StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,player);
		}
		else if(req.id()==GameParaLoader::GetInstance()->getExchangeGoldenItemId())//元宝
		{
			if(req.num() > player->getAttManage()->getValue<int>(eCharGolden)) return;
			contribute = getContributeByGolden(req.num());
			if(contribute==0) return;
			isGolden = true;
//			StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,req.num(),player);
//			StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,player);

			// 统计元宝花费
//				StatisticMgr::GetInstance()->StatisticPlayerGetItems(player, eStatic_AboutGolden, eStaMinor_AboutGolden_CountryDonate,1, req.num());
		}
		else
		{
			int nums = pack->GetGoodsNum(req.id());
			if(req.num()>nums) return;
			contribute = getContributeByProp(req.id())*req.num();
			if(contribute==0) return;
			isCostGoods = true;
				//扣物品
//				pack->DeleteGoodsById(req.id(), req.num(), true);

//				StatisticMgr::SetItemCost(costItemInfo,req.id(), req.num());
		}

		BEHAVIOR_MONI(player, BehaviorType::Main::eCountry, BehaviorType::Minor::eCountry_Contribute, MONITOR);

		if (isMoney)
		{
			//扣钱
			player->MoneyChange(req.num(),false);
		}

		if (isGolden)
		{
			player->ChangeGolden(req.num(),false);
		}
		if (isCostGoods)
		{
			pack->DeleteGoodsById(req.id(), req.num(), true);
		}

//		StatisticInfo::GotItemInfo * gotCountryContriInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotCountryContriInfo,eCurCountryContribute,contribute);

//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Country,eStaMinor_Country_CountryDonateEvent,&costAndGotInfo);
		//增加贡献度 周贡献度 当前贡献度
		player->getAttManage()->addValue(eCountryContri,contribute);
		player->getAttManage()->addValue(eCurCountryContribute,contribute);
		player->setWeekContrituion(player->getWeekContrituion()+contribute);

		if (player->GetMyself())
		{
			PlayerCampContribute tempArgs(player->GetMyself(),contribute);
			player->FireEvent(PLAYER_CAMP_CONTRIBUTE,tempArgs);
		}

		//捐献是否计算霸主争夺
		int power = player->getAttManage()->getValue<int>(eFightPower);
		if(power>=m_kingFightPower || player->getKingRankFlag())
		{
				player->setKingRankFlag(1);
		}

		//设置脏数据
		player->SetDataFlag(eBaseInfo);

		//同步客户端
		player->SynCharAttribute(eCountryContri,player->getAttManage()->getValue<int>(eCountryContri));
		player->SynCharAttribute(eCurCountryContribute,player->getAttManage()->getValue<int>(eCurCountryContribute));

		//通知世界服
		CountryPackage::GSReqWSContribute gstows;
		gstows.set_char_(player->GetID());
		gstows.set_contry(player->getAttManage()->getValue<DWORD>(eCharCountry));
		gstows.set_contribute(contribute);
		if(isMoney||isGolden)
		{
			//元宝要按一定比例转换成资金
				int64 financial= req.num();
				if(isGolden)
					financial = financial*getGoldenRate();
				gstows.set_id(0);
				gstows.set_num(0);
				gstows.set_money(financial);
		}
		else
		{
				gstows.set_id(req.id());
				gstows.set_num(req.num());
				gstows.set_money(0);
		}


		if (player->getCampShopMgr())
		{
			int count = player->getCampShopMgr()->IsHaveCanBuy();
			player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_CountryShopCanBuy), count);
		}

		

		Smart_Ptr<SvrItem> worldSvr;
		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GS2WS_COUNTRY_JUANXIAN, &gstows, worldSvr.Get(), Request);
		messRet->SetAct(new requestAct(message));
		Message_Facade::Send(messRet);

}


DEF_MSG_ACK_DEFINE_FUN(GsCountryMgr, MSG_REQ_GS2WS_COUNTRY_JUANXIAN)
{
		   //请求超时
			if(message->GetErrno() == eReqTimeOut)
			{
				LOG_WARNING(FILEINFO, "gameserver request wdserver get shop and ack timeout");
				return;
			}

			CountryPackage::AckClientReqContribute ret;
			int len = 0;
			char * content = message->GetBuffer(len);
			ret.ParseFromArray(content,len);

			REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &ret, MSG_REQ_C2GS_COUNTRY_JUANXIAN);
}

//官职奖励
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_JOB_DAYPRIZE)
{
		CountryPackage::AckClientReqJobPrize  ret;

		ret.set_seccess(0);
		if(player->getFitchFlag()>0)
				return;

		const CampJobPrize* pJobinfo=getCampJobInfo(player->getAttManage()->getValue<WORD>(eCountryJob));
		if(pJobinfo==NULL) return;

		//处理物品
		const GoodsInfo *ginfo = GoodsLoader::GetInstance()->GetItemDataByID(pJobinfo->propID);
		if(ginfo==NULL) return;


		BEHAVIOR_MONI(player, BehaviorType::Main::eCountry, BehaviorType::Minor::eCountry_GetJobAward, MONITOR);;

		vector<int> vecLock;
		int err = player->GetContainer(ePackageType)->IsBagEnought(pJobinfo->propID, 1, eGetBind);
		if (0 == err)
		{
			player->GetContainer(ePackageType)->AddItem(pJobinfo->propID, 1, vecLock, eGetBind, ginfo->equipStrengthLv,true);

			//更新领取标识
			player->setFitchFlag(1);
			//设置脏数据
			player->SetDataFlag(eBaseInfo);
			//同步到世界服
			player->SynchCharAttributeToWS(eCountryFitchPrize,1);

			player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Country, eUIHLSmall_Country_CountryJobReward), 1, false);

		}
		else
		{
			ret.set_seccess(err);
		}
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,pJobinfo->propID,1);
//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Country,eStaMinor_Country_JobReward,&costAndGotInfo);

		

		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_COUNTRY_JOB_DAYPRIZE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_SHOPINFO)
{
		CountryPackage::AckClientReqShopInfo  ret;
		Smart_Ptr<PlayerCampShopMgr> pShop = player->getCampShopMgr();
		std::map<int,CampShopInfo>::const_iterator it=m_shopInfo.begin();
		for(; it!=	m_shopInfo.end(); ++it)
		{
				CountryPackage::CountryShopItem* pItem = ret.add_itemlist();
				pItem->set_id(it->first);

				int num = pShop->getPropAttr(it->first);
				if(num==-1)
					num = 0;

				if(num>it->second.nums)
					num = it->second.nums;

				pItem->set_count(num);
		}
		ret.set_contribute(player->getAttManage()->getValue<int>(eCurCountryContribute));
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_COUNTRY_SHOPINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryMgr,MSG_REQ_C2GS_COUNTRY_BUY)
{
		CountryPackage::ClientReqBuyShopItem req;
		CountryPackage::AckClientReqBuyShopItem ret;

		req.ParseFromString(content);
		const CampShopInfo* pShop = getShopInfo(req.id());
		if(pShop==NULL) return;

		const GoodsInfo *ginfo = GoodsLoader::GetInstance()->GetItemDataByID(req.id());
		if(ginfo==NULL) return;

		int contribute = pShop->contribute*req.count();
		if(player->getAttManage()->getValue<int>(eCurCountryContribute)<contribute)
			return;

		//限购
		int nums = player->getCampShopMgr()->getPropAttr(req.id());
		if(nums+req.count() > pShop->nums)
			return;
		if(getCampGrade(player->getAttManage()->getValue<DWORD>(eCharCountry)) < pShop->campLv)
			return;

		BEHAVIOR_MONI(player, BehaviorType::Main::eCountry, BehaviorType::Minor::eCountry_BuyCntyShop, MONITOR);;

		int err = player->GetContainer(ePackageType)->IsBagEnought(req.id(), req.count(), pShop->nBindType);
		if (0 == err)
		{
			vector<int> vecLock;
			player->GetContainer(ePackageType)->AddItem(req.id(),req.count(),vecLock,pShop->nBindType,ginfo->equipStrengthLv,true);

			//限购属性
			player->getCampShopMgr()->setPropAttr(req.id(),nums+req.count());

			//扣贡献度
			player->ChangeCountryContribution(contribute,false,true);

			ret.set_id(req.id());
			ret.set_count(nums+req.count());
			ret.set_contribute(player->getAttManage()->getValue<int>(eCurCountryContribute));
			ret.set_success(0);

			//设置脏数据
			player->SetDataFlag(eBaseInfo);
			player->SetDataFlag(eCampShopInfo);

		}
		else
		{
			ret.set_success(err);
		}


//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotItemInfo,req.id(),req.count());
//
//		StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costInfo,eCurCountryContribute,contribute,player);

//		StatisticMgr::SetItemCostAfter(costInfo,eCurCountryContribute,player);

//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Country,eStaMinor_Country_MallBuyEvent,&costAndGotInfo);

		//同步属性到世界服
		//player->SynchCharAttributeToWS(eCurCountryContribute,player->getCurContribute());

		

		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_COUNTRY_BUY);

}

DEF_MSG_REQUEST_DEFINE_FUN(GsCountryMgr, MSG_REQ_WS2GM_GETJOINAWARD)
{
	int64 charID = message->GetMessageTime();
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, charID);

	if (!player)
	{
		return;
	}

	CountryPackage::AckClientReqJionCountry ret;
	int len =  0;
	char* buffer = message->GetBuffer(len);

	ret.ParseFromArray(buffer, len);


	if (player->GetContainer(ePackageType))
	{
		const JoinRcndAward& award = GetJoinRcndAward();

		int isEnought = player->GetContainer(ePackageType)->IsBagEnought(award.idList, award.numList, award.isBind);
		if (0 != isEnought)
		{ // 背包体积不足
			vector<int64> getAward;
			for(uint i = 0; i < award.idList.size(); i++)
			{
				int64 info = award.numList[i];
				info = info << 32;	//物品数量
				info = info | award.idList[i];
				getAward.push_back(info);
			}

			// 发送邮件
			MailMessageMgr::GetInstance()->SendSysMail(getAward, player->GetName());
			ret.set_getawarderr(eJoinCtryAwardNotGet);
		}
		else
		{
			vector<int> pos;
			vector<DWORD> strengthLv;
			strengthLv.resize(award.idList.size(), 0);
			player->GetContainer(ePackageType)->AddItem(award.idList, award.numList, pos, award.isBind, strengthLv);
			ret.set_getawarderr(0);
		}
	}
	else
	{
		ret.set_getawarderr(eJoinCtryAwardNotGet);
		LOG_ERROR(FILEINFO, "Player Get Join Country Recommend Award, Package Container Null");
	}

	Safe_Smart_Ptr<Message> msg = build_message(message, &ret, Ack);
	Message_Facade::Send(msg);
}

int GsCountryMgr::GetNextBossTime(int num)
{
	if(num == 0)
	{
		return m_nFirstTime;
	}
	else if(num == 1)
	{
		return m_nSecondTime;
	}
	else if(num == 2)
	{
		return m_nThirdTime;
	}
	else
	{
		return 0;
	}

	return 0;
}

int GsCountryMgr::GetNextBossBornTime(int num)
{
	int ret = 0;
	if(num == 0)
	{
		ret = m_nFirstTime;
	}
	else if(num == 1)
	{
		ret = m_nSecondTime - m_nFirstTime ;
	}
	else if(num == 2)
	{
		ret =  m_nThirdTime - m_nSecondTime;
	}
	else
	{
		ret =  0;
	}

	return ret;
}

void GsCountryMgr::GetBossMap(map<int,BossMap>& bossMap)
{
	bossMap = m_campBoss;
}

int GsCountryMgr::GetFlushConTime()
{
	return m_nFlushConTime;
}

int GsCountryMgr::GetFlushExp(int lv,int rate)
{
	m_pLua.SetLuaFunction("GetFlushExp");
	m_pLua.SetInt(lv);
	m_pLua.SetInt(rate);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetFlushCamp(int count)
{
	m_pLua.SetLuaFunction("GetFlushBoss");
	m_pLua.SetInt(count);
	if(m_pLua.Call(1, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetFlushCon()
{
	m_pLua.SetLuaFunction("GetFlushCon");
	if(m_pLua.Call(0, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetSystemScore()
{
	m_pLua.SetLuaFunction("GetSystemScore");
	if(m_pLua.Call(0, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetPersionScore()
{
	m_pLua.SetLuaFunction("GetPersionScore");
	if(m_pLua.Call(0, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetKillEnemyScore()
{
	m_pLua.SetLuaFunction("GetKillEnemyScore");
	if(m_pLua.Call(0, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetKillBossScore(int nId)
{
	map<int,int>::iterator itr = m_KillBossScore.find(nId);
	if(itr != m_KillBossScore.end())
	{
		return itr->second;
	}

	return 0;
}

int GsCountryMgr::GetBossNum(int nId)
{
	map<int,int>::iterator itr = m_BossNum.find(nId);
	if(itr != m_BossNum.end())
	{
		return itr->second;
	}

	return 0;
}

int GsCountryMgr::GetFlushNewsTime()
{
	m_pLua.SetLuaFunction("GetFlushNewsTime");
	if(m_pLua.Call(0, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetEndKillExp(int lv,int count)
{
	m_pLua.SetLuaFunction("GetEndKillExp");
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetEndKillCredit(int lv,int count)
{
	m_pLua.SetLuaFunction("GetEndKillcredit");
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetEndKillCon(int lv,int count)
{
	m_pLua.SetLuaFunction("GetEndKillCon");
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetHurtRankingExp(int lv, int rank,int count)
{
	m_pLua.SetLuaFunction("GetHurtRankingExp");
	m_pLua.SetInt(rank);
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(3, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetHurtRankingCredit(int lv, int rank,int count)
{
	m_pLua.SetLuaFunction("GetHurtRankingcredit");
	m_pLua.SetInt(rank);
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(3, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetHurtRankingCon(int lv, int rank, int count)
{
	m_pLua.SetLuaFunction("GetHurtRankingCon");
	m_pLua.SetInt(rank);
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(3, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

void GsCountryMgr::GetBossPower(CountryBossPackage::BossHarmSettle& info,int64 nBossId,int nRank)
{
	int nNum = 0;

	if(nRank == 1)
	{
		nNum = 3;
	}
	else if(1 < nRank && nRank < 4)
	{
		nNum = 2;
	}
	else if(nRank >= 4 && nRank <= 10)
	{
		nNum = 1;
	}
	else
	{
		nNum = 0;
	}

	MonsterInfo tempMOnsterInfo;
	MonsterLoader::GetInstance()->GetMonsterInfo(nBossId, tempMOnsterInfo);

	if(tempMOnsterInfo.fallID.size() <= 0)
		return ;

	for(int i = 0; i < nNum;++i)
	{
		vector<DropStaticInfo> tempItem;
		DropItemLoader::GetInstance()->GetDropItem(tempMOnsterInfo.fallID,tempItem);

		for(size_t j = 0;j < tempItem.size();++j)
		{
			CountryBossPackage::BossHarmItem *pItem = info.add_item();
			pItem->set_id(tempItem[j].m_ItemID);
			pItem->set_num(tempItem[j].m_Counts);
		}
	}

	return ;
}

int GsCountryMgr::GetCampAwardExp(int lv,int count)
{
	m_pLua.SetLuaFunction("GetCampAwardExp");
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetCampAwardMoney(int lv,int count)
{
	m_pLua.SetLuaFunction("GetCampAwardMoney");
	m_pLua.SetInt(lv);
	m_pLua.SetInt(count);
	if(m_pLua.Call(2, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetCampWinAwardExp(int lv)
{
	m_pLua.SetLuaFunction("GetCampWinAwardExp");
	m_pLua.SetInt(lv);
	if(m_pLua.Call(1, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetCampWinAwardMoney(int lv)
{
	m_pLua.SetLuaFunction("GetCampWinAwardMoney");
	m_pLua.SetInt(lv);
	if(m_pLua.Call(1, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

int GsCountryMgr::GetCampMoney()
{
	m_pLua.SetLuaFunction("GetCampMoney");
	if(m_pLua.Call(0, 1))
	{
			return 0;
	}

	return m_pLua.GetInt();
}

// 获取加入推荐阵营奖励
const JoinRcndAward& GsCountryMgr::GetJoinRcndAward() const
{
	return m_joinRcndAward;
}

const int GsCountryMgr::GetShowCntyText(eCountryID ID) const
{
	map<eCountryID, int>::const_iterator it = m_showCntyText.find(ID);
	if (m_showCntyText.end() != it)
	{
		return it->second;
	}

	return -1;
}
const int GsCountryMgr::GetShowCntyText(DWORD ID) const
{
	eCountryID _ID = static_cast<eCountryID>(ID);

	return GetShowCntyText(_ID);
}

