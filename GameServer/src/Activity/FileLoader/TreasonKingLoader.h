/*
 * TreasonKingLoader.h
 *
 *  Created on: 2014年11月24日
 *      Author: root
 */

#ifndef TREASONKINGLOADER_H_
#define TREASONKINGLOADER_H_

#include "define.h"
#include "Singleton.h"
#include "Lua/LuaInterface.h"

using namespace CommBaseOut;
using namespace std;

struct QuantityArticle
{
	DWORD nArticle;
	DWORD nNum;

	QuantityArticle():nArticle(0),nNum(0)
	{
	}
};


struct Reward
{
	DWORD nId;
	vector<QuantityArticle> vQa;

	Reward():nId(0)
	{
		vQa.clear();
	}
};

struct Probability
{
	DWORD nProLow;
	DWORD nProHigh;

	Probability():nProLow(0),nProHigh(0)
	{
	}
};

class TreasonKingLoader : public Singleton<TreasonKingLoader>
{
public:
	TreasonKingLoader();
	~TreasonKingLoader();

	int Init(string &path);
	int InitProbability(string &file);
	int InitLua(string & file);

	int GetOneGold()
	{
		return m_nOneGold;
	}

	int GetGoGolden()
	{
		return m_nGoGolden;
	}

	int GetDigNum()
	{
		return m_nDigNum;
	}

	int GetLevelsGold(DWORD nLevels)
	{
		if(nLevels >= m_vLevelsGold.size())
			return 0;
		return m_vLevelsGold[nLevels];
	}

	int GetOneGold(DWORD nLevels)
	{
		std::map<int,int>::iterator it = m_mapDigGoldCost.find(nLevels);
		if(it!=m_mapDigGoldCost.end())
			return it->second;

		return 0;
	}

	void GetReward(DWORD nLevels, QuantityArticle& qa);

	int GetLevelsMapId(int nLevels);

	int GetMaxLevelMapID();

	int GetMaxLevel();

	int getDigBindType() const {
		return m_digBindType;
	}

	int getGoRewardBindType() const {
		return m_goRewardBindType;
	}

	const map<int, int>& getMapDigGoldCost() const {
		return m_mapDigGoldCost;
	}

	const map<int, int>& getMLevelsMap() const {
		return m_mLevelsMap;
	}

	const map<int, Reward>& getMReward() const {
		return m_mReward;
	}

	int getNDigNum() const {
		return m_nDigNum;
	}

	int getNGoGolden() const {
		return m_nGoGolden;
	}

	int getNOneGold() const {
		return m_nOneGold;
	}

	const CLuaInterface& getPLua() const {
		return m_pLua;
	}

	const vector<int>& getVLevelsGold() const {
		return m_vLevelsGold;
	}

	const vector<QuantityArticle>& getVQa() const {
		return m_vQA;
	}

private:
	//单次挖掘消耗基数
	int m_nOneGold;
	//挖宝出来的物品的绑定类型
	int m_digBindType;
	//单次进入元宝消耗基数
	int m_nGoGolden;
	//每一层的奖励物品的绑定类型
	int m_goRewardBindType;
	int m_nDigNum;
	CLuaInterface m_pLua;
	map<int,Reward> m_mReward;
	vector<int> m_vLevelsGold;
	//每层的挖宝消耗
	map<int,int> m_mapDigGoldCost;
	vector<QuantityArticle> m_vQA;
	map<int,int> m_mLevelsMap;
	int m_maxLevelMapID;
};


#endif /* TREASONKINGLOADER_H_ */
