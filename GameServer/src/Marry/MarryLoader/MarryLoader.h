/*
 * MarryLoader.h
 *
 *  Created on: 2015年3月19日
 *      Author: root
 */

#ifndef MARRYLOADER_H_
#define MARRYLOADER_H_

#include "Singleton.h"
#include "define.h"
#include "../../Horse/HorseLoader/HorseLoader.h"

using namespace CommBaseOut;
using namespace std;

struct MarryRingData
{
	MarryRingData() : m_ID(0),m_Stars(0),m_LevelLimit(0),m_Exp(0),m_ItemCounts(0),m_ConsumeMoney(0),m_BuckleItem(0),
			m_BaseExp(0),m_MinCrit(0),m_MinCritRate(0),m_MaxCrit(0),m_MaxCritRate(0)
	{

	}

	DWORD m_ID;
	WORD  m_Stars;
	WORD  m_LevelLimit;
	DWORD m_Exp;
	WORD  m_ItemCounts;
	DWORD m_ConsumeMoney;
	DWORD m_BuckleItem;
	int m_BaseExp;
	float m_MinCrit;
	float m_MinCritRate;
	float m_MaxCrit;
	float m_MaxCritRate;
	map<ECharProf,map<ECharAttrType,int64> > m_AttList;
};

struct MarryRingProbability
{
	MarryRingProbability() : m_Probability(0),m_CritTime(.0f){}

	int 			m_Probability;  //概率
	float		    m_CritTime;		//暴击倍数
};

//奖励，根据结婚总天数算奖励
struct MarryRward
{
	DWORD m_ID;
	DWORD m_Day;
	vector<DWORD> m_ItemList;
	vector<DWORD> m_NumList;
	vector<WORD>  m_FlagList;
};

//离婚数据
struct SingleDivorceData
{
	SingleDivorceData() : m_ForceDivorceGolden(0),m_ForceDivorceRebate(0){}

	DWORD m_ForceDivorceGolden;//强制离婚扣的元宝
	BYTE  m_ForceDivorceRebate;//强制离婚系统回扣,百分比该数据为0~100包含0和100
};

class MarryLoader : public Singleton<MarryLoader>
{
public:
	MarryLoader();

	~MarryLoader();

public:
	int init(string& str);

	const MarryRingData* getFileData(DWORD id);

	bool isMaxRise(DWORD id);

	const map<DWORD,MarryRingData>& getFileDataList(){return m_FileDataList;}

	DWORD getFirstID();

	//奖励ID映射成索引
	short idMappingToIndex(DWORD id);

	//获取奖励数据
	const MarryRward* getMarryRwardData(DWORD id);

	const SingleDivorceData& getSingleDivorceData(){return m_DivorceData;}

	//获取暴击对应的倍率
	float getCritTime(eCommonRateType);

	int getMaxID();

protected:
	bool loadMarryRingData(string& str);

	bool loadMarryRwardData(string& str);

	bool loadSingleData(string& str);

private:
	map<DWORD,MarryRingData> m_FileDataList;
	SingleDivorceData		 m_DivorceData;
	map<DWORD,MarryRward>	 m_RwardList;
};

#endif /* MARRYLOADER_H_ */
