/*
 * NewYearActivity.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: root
 */


#include "NewYearActivity.h"
#include "FileLoader/FestivalUIFile.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeCreature.h"


NewYearActivity::NewYearActivity(Player& owner) :
m_Owner(owner)
{
	ReInit();
}

NewYearActivity::~NewYearActivity()
{
	Release();
}

void NewYearActivity::ReInit()
{
	m_HorseReceive = false;
	m_ContiReceive = false;
	m_LoginDay.Init();
	m_RewardData.Init();
}

void NewYearActivity::Release()
{
	m_HorseReceive = false;
	m_ContiReceive = false;
	m_LoginDay.Release();
	m_RewardData.Release();
}


void NewYearActivity::initFromDB(ChristmasDB::ChristmasAllData& dbData)
{
	if(FestivalUIFile::GetInstance()->isNewYearTime(eActivityUILogin))
	{
		setBitData(0,m_LoginDay);

		setBitData(0,m_RewardData);

		playerLogin();
	}

	ChristmasDB::NewYearPro* tDB = dbData.mutable_newyeardata();
	if(!FestivalUIFile::GetInstance()->isNewYearTime(tDB->inittime()))
	{
		return ;
	}

	setBitData(tDB->loginday(),m_LoginDay);

	setBitData(tDB->loginreward(),m_RewardData);

	m_HorseReceive = tDB->horsereceive();

	m_ContiReceive = tDB->contireceive();
	
	playerLogin();

	return ;
}

void NewYearActivity::getToDB(ChristmasDB::ChristmasAllData& dbData)
{
	if(!FestivalUIFile::GetInstance()->isNewYearTime(eActivityUILogin))
	{
		return ;
	}

	ChristmasDB::NewYearPro* tDB = dbData.mutable_newyeardata();

	tDB->set_inittime(CUtil::GetNowSecond());

	tDB->set_loginday(getBitData(m_LoginDay));

	tDB->set_loginreward(getBitData(m_RewardData));

	tDB->set_horsereceive(m_HorseReceive);

	tDB->set_contireceive(m_ContiReceive);

	return ;
}

void NewYearActivity::playerLogin()
{
	if(!FestivalUIFile::GetInstance()->isNewYearTime(eActivityUILogin))
	{
		return ;
	}

	int tLoginData = getBitData(m_LoginDay);

	struct tm tempTm;
	time_t tempCurrentTime = 0;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);

	tLoginData = tLoginData | 1 << tempTm.tm_mday;

	setBitData(tLoginData,m_LoginDay);

	m_Owner.SetDataFlag(eChristmasInfo);

	return ;
}

void NewYearActivity::clientGetLoginData(ServerReturn::ServerFourInt& retrunData)
{
	retrunData.set_retf(getBitData(m_RewardData));
	retrunData.set_rets(FestivalUIFile::GetInstance()->getNewYearDays());
	retrunData.set_rett(getLoginDay());
	retrunData.set_retfo(m_ContiReceive);

	return ;
}

ServerRet NewYearActivity::receiveLoginReward(int index)
{
	if(index == 1)
	{
		return loginReward();
	}
	else
	{
		return continueReward();
	}

	return eRetSuccess;
}

ServerRet NewYearActivity::buyHorseReward(int& skinID)
{
	if(m_HorseReceive)
	{
		return eNewYearHaveBuySkin;
	}

	eFestivalUIType tFestiType = FestivalUIFile::GetInstance()->getCurrentUIType(eActivityUILogin);
	if(tFestiType == eFestivalUINULL)
	{
		return eSystemError;
	}

	const NewYearHorse* tData = FestivalUIFile::GetInstance()->getHorseData(tFestiType);
	if(tData == NULL)
	{
		return eSystemError;
	}

	if((int)tData->m_Golden > m_Owner.getAttManage()->getValue<int>(eCharGolden))
	{
		return eNotEnoughGold;
	}

	if(tData->m_Golden > 0)
	{
		vector<int> tAttAlter;
		m_Owner.ChangeGolden(tData->m_Golden,false);
		tAttAlter.push_back(eCharGolden);
		m_Owner.SynCharAttribute(tAttAlter);
	}

	const GoodsInfo* tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(tData->m_ItemID);
	if(tempInfo == NULL)
	{
		return eSystemError;
	}

	skinID = tempInfo->buffID;

	vector<int> tPos;
	if(m_Owner.GetContainer(ePackageType)->AddItem(tData->m_ItemID,tData->m_Num,tPos,tData->m_Bind) != eRetSuccess)
	{
		return ePackageFull;
	}

	m_Owner.SetDataFlag(eChristmasInfo);

	m_HorseReceive = true;

	return eRetSuccess;
}

void NewYearActivity::gmSetContinueDay(int day)
{
	struct tm tempTm;
	time_t tempCurrentTime = 0;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);
	int tDay = tempTm.tm_mday;

	while(day > 0 && tDay > 0)
	{
		m_LoginDay.SetBit(tDay,true);
		--day;
		--tDay;
	}

	return ;
}

void NewYearActivity::setBitData(int data,CBitArray& bitData)
{
	unsigned char signinBuf[sizeof(int)] = {0};

	CUtil::SafeMemmove(signinBuf,sizeof(int),&data,sizeof(int));

	bitData.SetData(signinBuf,sizeof(int));

	return ;
}

int NewYearActivity::getBitData(CBitArray& bitData)
{
	int tResult = 0;
	int tSize   = 0;

	const unsigned char* tBuffer = bitData.GetData(tSize);

	CUtil::SafeMemmove(&tResult,sizeof(int),tBuffer,tSize);

	return tResult;
}

int NewYearActivity::changeLoginDay()
{
	const ChristmasTime* tData = FestivalUIFile::GetInstance()->getNewYLoginTime();
	if(tData == NULL)
	{
		return 0;
	}

	struct tm tempTm;
	time_t tempCurrentTime = 0;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);

	int tResult	  = 0;
	int tCurIndex = 0;
	++tempTm.tm_mon;

	for(int i = tData->m_Start.m_Month;i <= tempTm.tm_mon;++i)
	{
		int tCurDay = 0;
		if(i == tempTm.tm_mon)
		{
			tCurDay = tData->m_Start.m_Day;
		}
		else
		{
			tCurDay = getMonthDays(i);
		}

		if(m_LoginDay.GetBit(tCurDay))
		{
			tResult |= 1 << tCurIndex;
		}

		++tCurIndex;
	}

	return tResult ;
}

int NewYearActivity::getMonthDays(int day)
{
	day = day % 31;

	if(day == 1 || day == 3 || day == 5 || day == 7 || day == 8 || day == 10 || day == 12)
	{
		return 31;
	}

	if(day == 4 || day == 6 || day == 9 || day == 11)
	{
		return 30;
	}

	if(day == 2)
	{
		return 29;
	}

	return 30;
}

int NewYearActivity::getLoginDay()
{
	int tResult = 0;
	for(unsigned int i = 0;i < sizeof(int) * 8;++i)
	{
		if(!m_LoginDay.GetBit(i))
		{
			continue;
		}

		++tResult;
	}

	return tResult;
}

ServerRet NewYearActivity::loginReward()
{
	int tIndex = FestivalUIFile::GetInstance()->getNewYearDays() - 1;
	
	if(m_RewardData.GetBit(tIndex))
	{
		return eNewYearLoginHaveReceive;
	}

	const OneLoginReward* tData = FestivalUIFile::GetInstance()->getNewYearLoginData(tIndex + 1,eLoginRewardDay);
	if(tData == NULL)
	{
		return eSystemError;
	}

	vector<DWORD> tStrengthLv;
	vector<int> tPos;

	tStrengthLv.resize(tData->m_ItemList.size(),0);
	if(m_Owner.GetContainer(ePackageType)->AddItem(tData->m_ItemList,tData->m_NumList,tPos,tData->m_BindList,tStrengthLv) != eRetSuccess)
	{
		return ePackageFull;
	}

	m_RewardData.SetBit(tIndex,true);

	m_Owner.SetDataFlag(eChristmasInfo);

	return eRetSuccess;
}

ServerRet NewYearActivity::continueReward()
{
	if(m_ContiReceive)
	{
		return eNewYearContinueHaveRecieve;
	}

	const OneLoginReward* tData = FestivalUIFile::GetInstance()->getNewYearLoginData(getLoginDay(),eLoginRewardConti);
	if(tData == NULL)
	{
		return eSystemError;
	}

	vector<DWORD> tStrengthLv;
	vector<int> tPos;

	tStrengthLv.resize(tData->m_ItemList.size(),0);
	if(m_Owner.GetContainer(ePackageType)->AddItem(tData->m_ItemList,tData->m_NumList,tPos,tData->m_BindList,tStrengthLv) != eRetSuccess)
	{
		return ePackageFull;
	}

	m_ContiReceive = true;

	m_Owner.SetDataFlag(eChristmasInfo);

	return eRetSuccess;
}



