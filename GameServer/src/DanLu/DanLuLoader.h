/*
 * DanLuLoader.h
 *
 *  Created on: 2014年8月7日
 *      Author: root
 */

#ifndef DANLULOADER_H_
#define DANLULOADER_H_

#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"
#include "Probability/Probability.h"

using namespace std;
using namespace CommBaseOut;
namespace DanLuInfo
{
	typedef struct sProduceItem
	{
		int m_itemID;
		int m_itemCount;
		sProduceItem() : m_itemID(0), m_itemCount(0)
		{	}
		void Clear()
		{
			m_itemID = 0;
			m_itemCount = 0;
		}
		bool IsInvalid()
		{
			return (0 >= m_itemID || 0 >= m_itemCount);
		}
	}ProduceItem;

	typedef struct sProduce
	{
		ProduceItem m_produceItem;
		int m_Probability;
	}Produce;
}
//炉练作用值
typedef struct sDanLuFunInfo
{
	//加的血量
	int blood;
	//加的物理攻击
	int phyatk;
	//加的物理防御
	int phydefend;
	//命中
	int m_Hit;
	//闪避
	int m_Dodge;
	//加的暴机
	int crackrate;
	//加的抗暴
	int crackdefend;
	int m_LevelLimit;
	sDanLuFunInfo():blood(0),phyatk(0),phydefend(0),m_Hit(0),m_Dodge(0),crackrate(0),crackdefend(0),m_LevelLimit(0)
	{

	}
}DanLuFunInfo;

typedef struct sLianLuInfo
{
	//练炉ID
	int id;
	//品质
	int quality;
	//星级
	int start;
	//升到下一ID所需的经验
	int needexp;
	// 升级所需数量
	int upNeedNum;
	//职业--------炉练作用值
	map<int, DanLuFunInfo> m_danluFun;
	//相应
	vector<DanLuInfo::Produce> m_produceItem;
	// vector<pair<int, pair<int, int> > > m_produceItem;
	//所需铜钱
	int m_NeedMoney;
	int m_LevelLimit;
	int m_ItemID;
	int m_BaseExp;
	float m_MinCrit;
	float m_MinCritRate;
	float m_MaxCrit;
	float m_MaxCritRate;

	sLianLuInfo():id(0),quality(0),start(0),needexp(0), upNeedNum(0),m_NeedMoney(0),m_LevelLimit(0),m_ItemID(0),
			m_BaseExp(0),m_MinCrit(0),m_MinCritRate(0),m_MaxCrit(0),m_MaxCritRate(0)
	{

	}
}LianLuInfo;

struct LianEquipParm
{
	//等级 系数
	vector<int> m_lvParm;
	//品质系数
	vector<int> m_quareParm;
	//需要达到的 点数值
	int pointCeil;
//	vector<int> needPointVec;
	// 奖励堆叠数
	int awardStack;

	LianEquipParm() : pointCeil(0), awardStack(0)
	{
		m_lvParm.clear();
		m_quareParm.clear();
	}
};

struct DanLuReclaim
{
	int nId;
	int nType;
	int nQuality;
	int nStar;
	int nItemId;
	int nItemNum;

	DanLuReclaim():nId(0),nType(0),nQuality(0),nStar(0),nItemId(0),nItemNum(0)
	{

	}
};

enum DanLuReclaimType
{
	eReclaim_Strengthen = 1,
	eReclaim_Purification = 2,
	eReclaim_Max,
};

class DanLuLoader : public Singleton<DanLuLoader>
{
public:
	DanLuLoader():m_maxDanluID(0)
	{

	}

	~DanLuLoader()
	{
		m_allDanLu.clear();
	}

	int InitFile(const string &file);

	int InitDanLu(const string &file);

	int InitDanEquip(const string &file);

	int InitDanLuReclaim(const string &file);

	void GetLianLuInfo(int id,	LianLuInfo&info)
	{
		map<int, LianLuInfo>::iterator it = m_allDanLu.find(id);
		if (it != m_allDanLu.end())
		{
			info = it->second;
		}
	}

	LianLuInfo * GetLianLuPtr(int id)
	{
		map<int, LianLuInfo>::iterator it = m_allDanLu.find(id);
		if (it != m_allDanLu.end())
		{
			return  &it->second;
		}
		return NULL;
	}



	int GetLianLuMaxLv()
	{
		return m_maxDanluID;
	}

	bool IsMaxDanLuID(const int & id)
	{
		if (id == m_maxDanluID)
			return true;

		return false;
	}

	LianEquipParm * GetLianEquipParmPtr(){ return &m_lianEquipParm; }

	//物品使用等级   ------ 品质
	int CalExchangePoint(int useLv, int quality);

	int CalExchangeExp(int useLv, int quality);

	int GetLianEquipCeiling();

	const map<int, LianLuInfo>& getFileData(){return m_allDanLu;}

	int getMaxID();

	const DanLuReclaim* GetDanLuReclaim(int nType,int nQuality,int nStar = 0);
private:
	//键 练炉ID
	map<int, LianLuInfo> m_allDanLu;

	int m_maxDanluID;

	LianEquipParm m_lianEquipParm;
	vector<DanLuReclaim> m_DanLuReclaim;
};


#endif /* DANLULOADER_H_ */
