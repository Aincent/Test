
#include "ChristmasManage.h"
#include "Probability/Probability.h"
#include "FileLoader/FestivalUIFile.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeCreature.h"


ChristmasManage::ChristmasManage(Player& player) : m_Owner(player)
{

}

ChristmasManage::~ChristmasManage()
{
	Release();
}

void ChristmasManage::ReInit()
{

}

void ChristmasManage::Release()
{
	m_EggData.m_EggBreakInfo.Init();
	m_LoginData.m_LoginDay.Init();
	m_LoginData.m_RewardData.Init();
	m_LimitData.m_ReflushTime = 0;
	m_LimitData.m_DataList.clear();
}

void ChristmasManage::initFromDB(ChristmasDB::ChristmasAllData& dbData)
{
	if(FestivalUIFile::GetInstance()->isEggTime())
	{
		initEggData(*dbData.mutable_breakeggdata());
	}

	if(FestivalUIFile::GetInstance()->isLoginTime())
	{
		initLoginData(*dbData.mutable_logindata());
	}

	if(FestivalUIFile::GetInstance()->isLimitBuyTime())
	{
		initLimitData(*dbData.mutable_limitdata());
	}

	playerLogin();

	m_Owner.SetInitPartDataFlag(eChristmasInfo);

	return ;
}

void ChristmasManage::getDBdata(ChristmasDB::ChristmasAllData& dbData)
{
	if(FestivalUIFile::GetInstance()->isEggTime())
	{
		dbData.mutable_breakeggdata()->set_breakinfo(getBitData(m_EggData.m_EggBreakInfo));
	}

	if(FestivalUIFile::GetInstance()->isLoginTime())
	{
		dbData.mutable_logindata()->set_loginday(getBitData(m_LoginData.m_LoginDay));
		dbData.mutable_logindata()->set_rewardinfo(getBitData(m_LoginData.m_RewardData));
	}

	if(FestivalUIFile::GetInstance()->isLimitBuyTime())
	{
		ChristmasDB::LimitDataAll* tLimit = dbData.mutable_limitdata();
		tLimit->set_reflushtime(m_LimitData.m_ReflushTime);

		for(size_t i = 0;i < m_LimitData.m_DataList.size();++i)
		{
			ChristmasDB::SingleLimitDB* tLimitSingle = tLimit->add_limitlist();

			tLimitSingle->set_id(m_LimitData.m_DataList[i].m_ID);
			tLimitSingle->set_counts(m_LimitData.m_DataList[i].m_LimitNum);
		}
	}

	return ;
}

ServerRet ChristmasManage::reflushEggTime()
{
	ServerRet tError = eRetSuccess;

	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	short tCounts = 0;
	for(short i = 0;i < tSingle.m_EggCounts;++i)
	{
		if(m_EggData.m_EggBreakInfo.GetBit(i))
		{
			continue;
		}

		++tCounts;
	}

	if(tCounts >= tSingle.m_EggCounts)
	{
		return eChristmasNotReflushEgg;
	}

	if(tSingle.m_ReflushEggGolden > (DWORD)m_Owner.getAttManage()->getValue<int>(eCharGolden))
	{
		return eNotEnogGolden;
	}

	if(tSingle.m_ReflushEggGolden > 0)
	{
		vector<int> tAttAlter;
		m_Owner.ChangeGolden(tSingle.m_ReflushEggGolden,false);
		tAttAlter.push_back(eCharGolden);
		m_Owner.SynCharAttribute(tAttAlter);
	}

	setBitData(0,m_EggData.m_EggBreakInfo);

	m_Owner.SetDataFlag(eChristmasInfo);

	m_Owner.SetDataFlag(eBaseInfo);

	return tError;
}

void ChristmasManage::breakEgg(int index,ChristmasActivity::BreakEggReturn& returnMsg)
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	//index下标从0开始
	if(index >= tSingle.m_EggCounts)
	{
		returnMsg.set_error(eSystemError);
		return ;
	}

	if(index < 0 && index != -1)
	{
		returnMsg.set_error(eSystemError);
		return ;
	}

	DWORD tBuckleMoney = 0;
	short tCounts      = getNotBreakCounts();
	//-1表示全砸
	if(index != -1)
	{
		if(m_EggData.m_EggBreakInfo.GetBit(index))
		{
			returnMsg.set_error(eChristmasThisEggHaveBreak);
			return ;
		}

		tBuckleMoney = tSingle.m_BreakMoney;
	}
	else
	{
		if(tCounts <= 0)
		{
			returnMsg.set_error(eChristmasAllEggHaveBreak);
			return ;
		}

		tBuckleMoney = tSingle.m_BreakMoney * tCounts;
	}

	if(tBuckleMoney > (DWORD)m_Owner.getAttManage()->getValue<int>(eCharGolden))
	{
		returnMsg.set_error(eNotEnogGolden);
		return ;
	}

	if(m_Owner.GetContainer(ePackageType)->GetFreeSize() < (index == -1 ? tSingle.m_EggCounts : 1))
	{
		returnMsg.set_error(ePackageFull);
		return ;
	}

	const vector<BreakEggFileData>& tEggData = FestivalUIFile::GetInstance()->getEggData();

	vector<int> tAttAlter;

	if(tBuckleMoney > 0)
	{
		m_Owner.ChangeGolden(tBuckleMoney,false);
		tAttAlter.push_back(eCharGolden);
	}

	if(index == -1)
	{
		for(short i= 0;i < tSingle.m_EggCounts;++i)
		{
			if(m_EggData.m_EggBreakInfo.GetBit(i))
			{
				continue;
			}

			ProbabilityVector<BreakEggFileData> tProbability(tEggData);
			int tIndex = tProbability.calculation();

			if(tIndex == INVALID_INDEX)
			{
				continue ;
			}

			vector<int> tPos;
			m_Owner.GetContainer(ePackageType)->AddItem(tEggData[tIndex].m_ItemID,tEggData[tIndex].m_Counts,tPos,tEggData[tIndex].m_BindType,0);

			ChristmasActivity::OneItemData* tOneItem = returnMsg.add_itemlist();

			tOneItem->set_itemid(tEggData[tIndex].m_ItemID);
			tOneItem->set_bindtype(tEggData[tIndex].m_BindType);
			tOneItem->set_num(tEggData[tIndex].m_Counts);
		}

		for(short i = 0;i < tSingle.m_EggCounts;++i)
		{
			m_EggData.m_EggBreakInfo.SetBit(i,true);
		}
	}
	else
	{
		m_EggData.m_EggBreakInfo.SetBit(index,true);

		ProbabilityVector<BreakEggFileData> tProbability(tEggData);
		int tIndex = tProbability.calculation();
		vector<int> tPos;
		m_Owner.GetContainer(ePackageType)->AddItem(tEggData[tIndex].m_ItemID,tEggData[tIndex].m_Counts,tPos,tEggData[tIndex].m_BindType,0);

		ChristmasActivity::OneItemData* tOneItem = returnMsg.add_itemlist();

		tOneItem->set_itemid(tEggData[tIndex].m_ItemID);
		tOneItem->set_bindtype(tEggData[tIndex].m_BindType);
		tOneItem->set_num(tEggData[tIndex].m_Counts);
	}

	if(tAttAlter.size() > 0)
	{
		m_Owner.SynCharAttribute(tAttAlter);
	}

	m_Owner.SetDataFlag(eChristmasInfo);

	m_Owner.SetDataFlag(eBaseInfo);

	returnMsg.set_error(eRetSuccess);

	returnMsg.set_index(index);

	if(getNotBreakCounts() <= 0)
	{
		setBitData(0,m_EggData.m_EggBreakInfo);
	}

	return ;
}

void ChristmasManage::clientGetEggData(ChristmasActivity::BreakEggMsg& returnMsg)
{
	returnMsg.set_breakinfo(getBitData(m_EggData.m_EggBreakInfo));

	return ;
}

void ChristmasManage::playerLogin()
{
	if(!FestivalUIFile::GetInstance()->isLoginTime())
	{
		return ;
	}

	int tLoginData = getBitData(m_LoginData.m_LoginDay);

	struct tm tempTm;
	time_t tempCurrentTime = 0;

	time(&tempCurrentTime);
	localtime_r(&tempCurrentTime,&tempTm);

	tLoginData = tLoginData | 1 << tempTm.tm_mday;

	setBitData(tLoginData,m_LoginData.m_LoginDay);

	m_Owner.SetDataFlag(eChristmasInfo);

	return ;
}

void ChristmasManage::clientGetLoginData(ChristmasActivity::ChristmasLoginMsg& returnMsg)
{
	returnMsg.set_loginday(getLoginDay());

	returnMsg.set_rewardinfo(getBitData(m_LoginData.m_RewardData));

	return ;
}

ServerRet ChristmasManage::receiveLoginReward(int index)
{
	int tDay = getLoginDay();
	//索引从0开始
	if(index + 1 > tDay)
	{
		return eCHristmasRewardConditionNotEnough;
	}

	if(m_LoginData.m_RewardData.GetBit(index))
	{
		return eChristmasRewardHaveReceive;
	}

	const LoginFileData* tData = FestivalUIFile::GetInstance()->getLoginByID(index + 1);
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

	m_LoginData.m_RewardData.SetBit(index,true);

	m_Owner.SetDataFlag(eChristmasInfo);

	return eRetSuccess;
}

void ChristmasManage::clientGetLimitData(ChristmasActivity::ChristmasLimitBuyInfo& returnMsg)
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();
	DWORD64 tCurrentTime = CUtil::GetNowSecond();
	if(tCurrentTime >= m_LimitData.m_ReflushTime)
	{
		reflushLimitData();
		m_LimitData.m_ReflushTime = tCurrentTime + tSingle.m_LimitReflushTime;
	}

	returnMsg.set_reflushtime(m_LimitData.m_ReflushTime);

	for(size_t i = 0;i < m_LimitData.m_DataList.size();++i)
	{
		ChristmasActivity::SingleLimitData* tpSingleData = returnMsg.add_datalist();

		tpSingleData->set_id(m_LimitData.m_DataList[i].m_ID);
		tpSingleData->set_counts(m_LimitData.m_DataList[i].m_LimitNum);
	}

	m_Owner.SetDataFlag(eChristmasInfo);

	m_Owner.SetDataFlag(eBaseInfo);

	return ;
}

ServerRet ChristmasManage::buyLimitItem(DWORD id)
{
	LimitSingleData* tData = getLimitDataByID(id);
	if(tData == NULL)
	{
		return eSystemError;
	}

	if(tData->m_LimitNum <= 0)
	{
		return eChristmasBuyCountsNotEnough;
	}

	const LimitBuyFileData* tLimitData = FestivalUIFile::GetInstance()->getLimitBuyByID(id);
	if(tLimitData == NULL)
	{
		return eSystemError;
	}

	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	if(tSingle.m_LimitReflushGolden > (DWORD)m_Owner.getAttManage()->getValue<int>(eCharGolden))
	{
		return eNotEnogGolden;
	}

	vector<int> tPos;
	if(m_Owner.GetContainer(ePackageType)->AddItem(tLimitData->m_ItemID,tLimitData->m_Num,tPos,tLimitData->m_BindType,0) != 0)
	{
		return ePackageFull;
	}

	if(tSingle.m_LimitReflushGolden > 0)
	{
		vector<int> tAttAlter;

		m_Owner.ChangeGolden(tSingle.m_LimitReflushGolden,false);
		tAttAlter.push_back(eCharGolden);
		m_Owner.SynCharAttribute(tAttAlter);
	}

	--tData->m_LimitNum;

	m_Owner.SetDataFlag(eChristmasInfo);

	return eRetSuccess;
}

void ChristmasManage::clientReflushLimit(ChristmasActivity::LimitBuyReturn& returnMsg)
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();

	if(tSingle.m_LimitReflushGolden > (DWORD)m_Owner.getAttManage()->getValue<int>(eCharGolden))
	{
		returnMsg.set_error(eNotEnogGolden);
		return ;
	}

	if(tSingle.m_LimitReflushGolden > 0)
	{
		vector<int> tAttAlter;
		m_Owner.ChangeGolden(tSingle.m_LimitReflushGolden,false);
		tAttAlter.push_back(eCharGolden);
		m_Owner.SynCharAttribute(tAttAlter);
	}

	reflushLimitData();

	returnMsg.set_error(eRetSuccess);

	for(size_t i = 0;i < m_LimitData.m_DataList.size();++i)
	{
		ChristmasActivity::SingleLimitData* tSingleData = returnMsg.add_datalist();

		tSingleData->set_id(m_LimitData.m_DataList[i].m_ID);
		tSingleData->set_counts(m_LimitData.m_DataList[i].m_LimitNum);
	}

	m_Owner.SetDataFlag(eChristmasInfo);

	m_Owner.SetDataFlag(eBaseInfo);

	return ;
}

ServerRet ChristmasManage::clientExchangeItem(DWORD id)
{
	const ExchangeFileData* tData = FestivalUIFile::GetInstance()->getExchangeFileByID(id);
	if(tData == NULL)
	{
		return eSystemError;
	}

	DWORD tItemCounts = m_Owner.GetContainer(ePackageType)->FindPropCount(tData->m_ItemID);
	if(tData->m_Num > tItemCounts)
	{
		return eChristmasItemOntEnough;
	}

	if(tData->m_Golden > (DWORD)m_Owner.getAttManage()->getValue<int>(eCharGolden))
	{
		return eNotEnogGolden;
	}

	vector<int> tPos;
	vector<DWORD> tStrengthLv;
	tStrengthLv.resize(tData->m_ItemList.size(),0);

	if(m_Owner.GetContainer(ePackageType)->AddItem(tData->m_ItemList,tData->m_NumList,tPos,tData->m_BindList,tStrengthLv) != 0)
	{
		return ePackageFull;
	}

	if(tData->m_Golden > 0)
	{
		vector<int> tAttAlter;
		m_Owner.ChangeGolden(tData->m_Golden,false);
		tAttAlter.push_back(eCharGolden);
		m_Owner.SynCharAttribute(tAttAlter);

		m_Owner.SetDataFlag(eBaseInfo);
	}

	BaseArgs tempArgs;
	tempArgs.GoodsID = tData->m_ItemID;
	tempArgs.Num     = tData->m_Num;
	m_Owner.GetContainer(ePackageType)->DeleteGoods(&tempArgs,true);

	return eRetSuccess;
}

void ChristmasManage::gmSetLoginDay(DWORD day)
{
	int tLoginData = getBitData(m_LoginData.m_LoginDay);

	tLoginData = tLoginData | 1 << (day % 32);

	setBitData(tLoginData,m_LoginData.m_LoginDay);

	m_Owner.SetDataFlag(eChristmasInfo);

	return ;
}

void ChristmasManage::initEggData(ChristmasDB::BreakEggDB& dbData)
{
	setBitData(dbData.breakinfo(),m_EggData.m_EggBreakInfo);

	return ;
}

void ChristmasManage::initLoginData(ChristmasDB::ChristmasLoginDB& dbData)
{
	setBitData(dbData.loginday(),m_LoginData.m_LoginDay);

	setBitData(dbData.rewardinfo(),m_LoginData.m_RewardData);

	return ;
}

void ChristmasManage::initLimitData(ChristmasDB::LimitDataAll& dbData)
{
	m_LimitData.m_ReflushTime = dbData.reflushtime();

	for(int i = 0;i < dbData.limitlist_size();++i)
	{
		ChristmasDB::SingleLimitDB* tData = dbData.mutable_limitlist(i);
		LimitSingleData tLimitSingle;

		tLimitSingle.m_ID		= tData->id();
		tLimitSingle.m_LimitNum = tData->counts();

		m_LimitData.m_DataList.push_back(tLimitSingle);
	}

	return ;
}

void ChristmasManage::setBitData(int data,CBitArray& bitData)
{
	unsigned char signinBuf[sizeof(int)] = {0};

	CUtil::SafeMemmove(signinBuf,sizeof(int),&data,sizeof(int));

	bitData.SetData(signinBuf,sizeof(int));

	return ;
}

int ChristmasManage::getBitData(CBitArray& bitData)
{
	int tResult = 0;
	int tSize   = 0;

	const unsigned char* tBuffer = bitData.GetData(tSize);

	CUtil::SafeMemmove(&tResult,sizeof(int),tBuffer,tSize);

	return tResult;
}

short ChristmasManage::getNotBreakCounts()
{
	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();
	short tResult = 0;
	for(short i = 0;i < tSingle.m_EggCounts;++i)
	{
		if(m_EggData.m_EggBreakInfo.GetBit(i))
		{
			continue;
		}

		++tResult;
	}

	return tResult;
}

int ChristmasManage::getLoginDay()
{
	int tResult = 0;
	for(unsigned int i = 0;i < sizeof(int) * 8;++i)
	{
		if(!m_LoginData.m_LoginDay.GetBit(i))
		{
			continue;
		}

		++tResult;
	}

	return tResult;
}

void ChristmasManage::reflushLimitData()
{
	m_LimitData.m_DataList.clear();

	const ChristmasSingleData& tSingle = FestivalUIFile::GetInstance()->getSingleData();
	map<DWORD,LimitBuyFileData> tData  = FestivalUIFile::GetInstance()->getLimitBuyData();

	map<DWORD,LimitBuyFileData>::iterator tIt;
	LimitSingleData tResultData;

	for(short i = 0;i < tSingle.m_LimitCounts;++i)
	{
		ProbabilityMap<LimitBuyFileData,DWORD> tHelper(tData);

		DWORD tKey = tHelper.calculation();
		tIt = tData.find(tKey);

		if(tIt == tData.end())
		{
			continue ;
		}

		tResultData.m_ID   		= tKey;
		tResultData.m_LimitNum  = tIt->second.m_LimitNum;
		m_LimitData.m_DataList.push_back(tResultData);
		tData.erase(tIt);
	}

	m_Owner.SetDataFlag(eChristmasInfo);

	return ;
}

LimitSingleData* ChristmasManage::getLimitDataByID(DWORD id)
{
	for(size_t i = 0;i < m_LimitData.m_DataList.size();++i)
	{
		if(m_LimitData.m_DataList[i].m_ID != id)
		{
			continue;
		}

		return &m_LimitData.m_DataList[i];
	}

	return NULL;
}





