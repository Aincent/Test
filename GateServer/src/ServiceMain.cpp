/*
 * ServiceMain.cpp
 *
 *  Created on: 2013��9��12��
 *      Author: helh
 */

#include "ServiceMain.h"
#include <signal.h>
#include "./Log/Logger.h"
#include "./Network/Message_Facade.h"
#include "util.h"
#include "./Network/NetWorkConfig.h"
#include "ServerManager.h"
#include "./CharactorMgr/CharactorLogin.h"
#include "./CharactorMgr/CharSynch.h"
#include "./CharactorMgr/CharactorInfo.h"
//#include "./QuestMgr/QuestManager.h"
#include "./CharBattle/CharBattle.h"
#include "./CharactorMgr/MapLoader/MapConfig.h"
#include "./GoodsMgr/GoodsManager.h"
#include "./Ranking/RankingManager.h"
#include "./FriendMgr/FriendManager.h"
#include "./TradeMgr/TradeMgr.h"
#include "./TradeMgr/ExchangeMgr.h"
#include "./Chat/ChatManager.h"
#include "./CommonMessage/CommonMessage.h"
#include "./InstanceMap/InstanceMap.h"
#include "./Mail/MailManager.h"
#include "./Monitor/MonitorManager.h"
#include "./GT_MSOpreation/MSOpreationMessage.h"


CServiceMain::CServiceMain()
{
}

CServiceMain::~CServiceMain()
{
}

int CServiceMain::Init()
{
	std::string strFileFullPath;
	std::string strFullPath;

	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	strFullPath = strFileFullPath + "Config/GateServerConfig.lua";

	if(m_conf.LoadConfig(strFullPath))
	{
		return -1;
	}

	return 0;
}

int CServiceMain::InitFile()
{
	std::string strFileFullPath;

	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	if(CMapConfig::GetInstance()->LoadMapInfo(strFileFullPath))
	{
		return -1;
	}

	return 0;
}

void CServiceMain::InitInstance()
{
	CharactorLogin::GetInstance();
	CharSynch::GetInstance();
	//QuestMgr::GetInstance();
	GoodsManager::GetInstance();
	CharactorInfo::GetInstance();
	CharBattleMgr::GetInstance();
	FriendManager::GetInstance();
	TradeMgr::GetInstance();
	RankingMgr::GetInstance();
	ExchangeMgr::GetInstance();
	ChatManager::GetInstance();
	CommonMessage::GetInstance();
	InstanceMap::GetInstance();
	MailManager::GetInstance();
	MonitorManager::GetInstance();

	MSopreationManager::GetInstance();
}

void CServiceMain::Start()
{
	if(Init())
	{
		cerr << " init config error " << endl;

		return ;
	}

	if(CLoggerMT::GetInstance()->Init(m_conf.GetLogConf().fileLen, m_conf.GetLogConf().info, m_conf.GetLogConf().debug,
			m_conf.GetLogConf().warning, m_conf.GetLogConf().error, m_conf.GetLogConf().fatal, m_conf.GetLogConf().display,
			m_conf.GetLogConf().filePath, m_conf.GetLogConf().module, m_conf.GetLogConf().base) != 0)
	{
		cerr << " init log error " << endl;

		return ;
	}

	if(InitFile() != 0)
	{
		return ;
	}

	if(Message_Facade::Init(ServerConHandler::GetInstance(), m_conf.GetBlockThread(), m_conf.GetIOThread()))
	{
		LOG_ERROR(FILEINFO, " init net error ");

		return;
	}

	ServerConHandler::GetInstance()->RegisterMsg();
	ServerConHandler::GetInstance()->SetServerID(m_conf.GetServerID());
	ServerConHandler::GetInstance()->SetIP(m_conf.GetFirstAccIP());
	ServerConHandler::GetInstance()->SetPort(m_conf.GetFirstAccPort());
	vector<AcceptorConfig> acc = m_conf.GetAccCof();
	vector<AcceptorConfig>::iterator itAcc = acc.begin();
	for(; itAcc!=acc.end(); ++itAcc)
	{
		AcceptorConfig accConf = *itAcc;
		Message_Facade::AddAcceptConfig(accConf);
	}

	vector<ConnectionConfig> con = m_conf.GetConnCof();
	vector<ConnectionConfig>::iterator itCon = con.begin();
	for(; itCon!=con.end(); ++itCon)
	{
		ConnectionConfig connConf = *itCon;
		Message_Facade::AddConnectionConfig(connConf);
	}

	InitInstance();

	if(Message_Facade::Run())
	{
		LOG_ERROR(FILEINFO, " run net error ");

		return;
	}

	Message_Facade::Wait();
	Message_Facade::Stop();
	UnInit();
}

void CServiceMain::UnInit()
{
	CharactorLogin::GetInstance()->DestroyInstance();
	CharSynch::GetInstance()->DestroyInstance();
	//QuestMgr::GetInstance()->DestroyInstance();
	GoodsManager::GetInstance()->DestroyInstance();
	CharactorInfo::GetInstance()->DestroyInstance();
	CharBattleMgr::GetInstance()->DestroyInstance();
	CMapConfig::GetInstance()->DestroyInstance();

	RankingMgr::GetInstance()->DestroyInstance();
	FriendManager::GetInstance()->DestroyInstance();
	TradeMgr::GetInstance()->DestroyInstance();
	ExchangeMgr::GetInstance()->DestroyInstance();
	ChatManager::GetInstance()->DestroyInstance();
	CommonMessage::GetInstance()->DestroyInstance();
	InstanceMap::GetInstance()->DestroyInstance();
	MailManager::GetInstance()->DestroyInstance();
	MonitorManager::GetInstance()->DestroyInstance();

	MSopreationManager::GetInstance()->DestroyInstance();

	ServerConHandler::GetInstance()->DestoryInstance();
	CLoggerMT::GetInstance()->DestoryInstance();
}

void CServiceMain::Stop(int signal)
{
	Message_Facade::UnWait();
}



