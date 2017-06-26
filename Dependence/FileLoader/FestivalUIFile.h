/*
 * FestivalUIFile.h
 *
 *  Created on: 16/12/2015
 *      Author: root
 */

#ifndef CHRISTMASFILE_H_
#define CHRISTMASFILE_H_

#include "define.h"
#include "Singleton.h"

using namespace std;
using namespace CommBaseOut;

struct DateFormat
{
	DateFormat() : m_Month(0),m_Day(0),m_Hour(0),m_Minute(0){}
	short m_Month;
	short m_Day;
	short m_Hour;
	short m_Minute;
};

struct BreakEggFileData
{
	BreakEggFileData() : m_ID(0),m_ItemID(0),m_Counts(0),m_BindType(1),m_Probability(0){}
	DWORD m_ID;
	DWORD m_ItemID;
	DWORD m_Counts;
	WORD  m_BindType;
	int   m_Probability;
};

struct TestData
{
	TestData() : m_ID(0),m_Counts(0),m_ItemID(0),m_Probability(0){}
	DWORD m_ID;
	DWORD m_Counts;
	DWORD m_ItemID;
	int	  m_Probability;
};

enum eChristmasModuleType
{
	eChristmasModuleNull     = 0,
	eChristmasModuleLogin    = 1,
	eChristmasModuleSnow     = 2,
	eChristmasModuleExchange = 3,
	eChristmasModuleEgg		 = 4,
	eChristmasModuleLimit	 = 5,
	eChristmasModuleMax 	 = 6,
};

struct ChristmasTime
{
	DateFormat m_Start;
	DateFormat m_End;
};

struct ChristmasSingleData
{
	ChristmasSingleData() : m_ReflushEggGolden(0),m_EggCounts(0),m_BreakMoney(0),m_EggReflushTime(0),m_LimitCounts(4),m_LimitReflushTime(0),
			m_LimitReflushGolden(0){}
	ChristmasTime m_Time[eChristmasModuleMax];
	DWORD	   m_ReflushEggGolden;
	short	   m_EggCounts;
	DWORD	   m_BreakMoney;
	DWORD	   m_EggReflushTime;
	WORD	   m_LimitCounts;
	DWORD	   m_LimitReflushTime;
	DWORD	   m_LimitReflushGolden;
};

struct LoginFileData
{
	LoginFileData() : m_Day(0){}
	void reset()
	{
		m_Day = 0;
		m_ItemList.clear();
		m_NumList.clear();
		m_BindList.clear();
	}
	short 		  m_Day;
	vector<DWORD> m_ItemList;
	vector<DWORD> m_NumList;
	vector<WORD>  m_BindList;
};

struct LimitBuyFileData
{
	LimitBuyFileData() : m_ID(0),m_ItemID(0),m_Num(0),m_Golden(0),m_Probability(0),m_BindType(0),m_LimitNum(0){}
	DWORD m_ID;
	DWORD m_ItemID;
	DWORD m_Num;
	DWORD m_Golden;
	int   m_Probability;
	WORD  m_BindType;
	WORD  m_LimitNum;
};

struct ExchangeFileData
{
	ExchangeFileData() : m_ID(0),m_ItemID(0),m_Num(0),m_Golden(0){}
	void reset()
	{
		m_ID 	 = 0;
		m_ItemID = 0;
		m_Num 	 = 0;
		m_Golden = 0;
		m_ItemList.clear();
		m_NumList.clear();
		m_BindList.clear();
	}

	DWORD m_ID;
	DWORD m_ItemID;
	DWORD m_Num;
	DWORD m_Golden;
	vector<DWORD> m_ItemList;
	vector<DWORD> m_NumList;
	vector<WORD>  m_BindList;
};

enum eFestivalUIType
{
	eFestivalUINULL    = 0,
	eFestivalUINewYear = 1,
	eFestivalUILantern = 2,
	eFestivalUILove	   = 3,
};

enum eActivityUIType
{
	eActivityUINull  = 0,
	eActivityUILogin = 1,
	eActivityUILimit = 2,
	eActivityUIFight = 3,
};

struct NewYearLimitData
{
	NewYearLimitData() : m_ID(0),m_ItemID(0),m_Counts(0),m_Flag(0),m_Type(eFestivalUINULL),m_Golden(0),m_PersentDiscount(0),m_AllLimits(0),m_OneLimits(0),m_Probability(0){}
	DWORD m_ID;
	DWORD m_ItemID;
	DWORD m_Counts;
	WORD  m_Flag;
	eFestivalUIType m_Type;
	DWORD m_Golden;
	WORD  m_PersentDiscount;
	DWORD m_AllLimits;
	DWORD m_OneLimits;
	int   m_Probability;
};

struct NewYearTime
{
	map<eActivityUIType,ChristmasTime> m_Datas;
};

enum eLoginRewardType
{
	eLoginRewardNull  = 0,
	eLoginRewardDay   = 1,
	eLoginRewardConti = 2,
};

struct OneLoginReward
{
	OneLoginReward() : m_ID(0),m_Day(0){}
	DWORD 			 m_ID;
	WORD		     m_Day;
	vector<DWORD>    m_ItemList;
	vector<DWORD>    m_NumList;
	vector<WORD>     m_BindList;
};

struct NewYearLoginReward
{
	map<eLoginRewardType,map<WORD,OneLoginReward> > m_Datas;
};

struct NewYearHorse
{
	NewYearHorse() : m_ItemID(0),m_Num(0),m_Bind(0),m_Golden(0),m_Reflushs(0){}
	DWORD      m_ItemID;
	DWORD      m_Num;
	WORD       m_Bind;
	DWORD	   m_Golden;
	WORD	   m_Reflushs;
	DateFormat m_ReflushTime;
};

enum eNewYearStage
{
	eNewYearNull 	 = 0,
	eNewYearNotStart = 1,
	eNewYearStarting = 2,
};

class FestivalUIFile : public Singleton<FestivalUIFile>
{
public:
	FestivalUIFile();

	~FestivalUIFile();

public:
	int init(string& str);

	int initEggFile(string& str);

	int initLoginFile(string& str);

	int initLimitBuyFile(string& str);

	int initExchangeFile(string& str);

	int initSingleFile(string& str);

	const vector<BreakEggFileData>& getEggData(){return m_EggList;}

	const ChristmasSingleData& getSingleData(){return m_SingleData;}

	const LoginFileData* getLoginByID(short day);

	const map<DWORD,LimitBuyFileData>& getLimitBuyData(){return m_LimitBuyList;}

	const LimitBuyFileData* getLimitBuyByID(DWORD id);

	const map<DWORD,ExchangeFileData>& getExchangeData(){return m_ExchangeList;}

	const ExchangeFileData* getExchangeFileByID(DWORD id);

	bool isEggTime();

	bool isLoginTime();

	bool isLimitBuyTime();

	bool isExchangeTime();

	void gmProbabilityTest(int counts);

	bool isInTime(const DateFormat& beginTime,const DateFormat& endTime);

	int initNewYear(string& str);

	const map<eFestivalUIType,NewYearTime>& getNewYearTime(){return m_NewYearTimes;}

	const map<DWORD,NewYearLimitData>* getLimitDatas(eFestivalUIType type);

	const NewYearLimitData* getLimitData(DWORD id);

	const NewYearHorse* getHorseData(eFestivalUIType type);

	const NewYearTime* getNewYearTime(eFestivalUIType type);

	bool isNewYearTime(eActivityUIType type);

	short getNewYearDays();

	const ChristmasTime* getNewYLoginTime();

	const OneLoginReward* getNewYearLoginData(int day,eLoginRewardType type);

	bool isNewYearTime(DWORD64 dbtime);

	int initNewYLimit(string& str);

	int initNewYTime(string& str);

	int initNewYLogin(string& str);
	
	int initNewYHorse(string& str);

	void initTime(DateFormat& time,const string& str);

	eFestivalUIType getCurrentUIType(eActivityUIType type);

private:
	vector<BreakEggFileData>     			m_EggList;
	ChristmasSingleData		     			m_SingleData;
	map<short,LoginFileData>     			m_LoginList;
	map<DWORD,LimitBuyFileData>  			m_LimitBuyList;
	map<DWORD,ExchangeFileData>  			m_ExchangeList;
	map<eFestivalUIType,map<DWORD,NewYearLimitData> > m_NewYearLimitList;
	map<eFestivalUIType,NewYearTime> 		m_NewYearTimes;
	map<eFestivalUIType,NewYearLoginReward> m_NewYearLogins;
	map<eFestivalUIType,NewYearHorse>		m_NewYearHorses;
};


#endif /* CHRISTMASFILE_H_ */
