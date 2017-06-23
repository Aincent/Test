/*
 * EquipData.h
 *
 *  Created on: 2014年4月8日
 *      Author: helh
 */

#ifndef EQUIPDATA_H_
#define EQUIPDATA_H_

#include "define.h"
#include "util.h"
#include "PropData.h"
#include <vector>
#include "../MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "../MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "../FileLoader/GoodsLoader.h"
#include "util.h"
#include "../MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "../CharDefine.h"

using namespace std;
using namespace CommBaseOut;

struct attributeBonus
{
	int HP;
	int phyAttack;
	int phyDef;
	int hitRate;
	int avoidRate;
	int downRate;
	int crackRate;
	attributeBonus() : HP(0), phyAttack(0), phyDef(0), hitRate(0), avoidRate(0), downRate(0),
		crackRate(0)
	{
	}
};
//洗练属性
struct ReGetAttr
{
	BYTE type;
	WORD value;
	int level;

	ReGetAttr():type(0),value(0),level(0)
	{

	}
};

enum e_CreateEquipEnum
{
	e_create_all =0,  //
	e_create_stone,
	e_create_attr,
};

/*
 * 装备 镶嵌改成 默认  5个空 全开 不管有没有装备
 * */

const uint Com_FillStone_HoleNum = 3;  //默认 空个数
const uint Strength_Max_Lv = 12; //默认强化最大等级为12

// 武器道具
class EquipProp : public PropBase 
{
public:
	EquipProp(int id)
	: PropBase(id),m_cUpgrade(0), m_sBattleValue(0),
											m_dwEndTime(0),m_nLegendAttrCount(0)
	{
		this->Init();
	}

	~EquipProp()
	{
		m_tempAttr.clear();
		m_attrBaptizeList.clear();
		m_sStones.clear();
	}
	enum
	{
		Type = 2,
	};

	void Init()
	{
		this->InitInlayStone();
	}

	void InitInlayStone()
	{
		m_sStones.resize(Com_FillStone_HoleNum);

		for (uint i = 0; i < (uint)Com_FillStone_HoleNum; ++i)
		{
			m_sStones[i] = 0;
		}
	}

	virtual BYTE GetType() const
	{
		return EquipProp::Type;
	}
	

	// 得到已经镶嵌的孔数
	BYTE GetHadInlayHoleNum() const
	{
		BYTE count = 0;
		for (uint i = 0; i < m_sStones.size(); ++i)
		{
			if (0 != m_sStones[i])
			{
				++count;
			}
		}

		return count;
	}


	// 镶嵌一个宝石
	int InlayStone(int stoneID,uint nPos)
	{
		if (0 >= stoneID || IsAddStoneFull() || nPos < 0 || nPos >= m_sStones.size())
		{
			return -1;
		}

		const GoodsInfo *info = 	GoodsLoader::GetInstance()->GetItemDataByID(stoneID);
		if(info==NULL)
			return -1;

		m_sStones[nPos] = stoneID;

		return nPos;
	}

	// 清除宝石
	void ClearStone()
	{
		for (uint i = 0; i < m_sStones.size(); ++i)
		{
			m_sStones[i] = 0;
		}
	}



	void	ClearStone(int pos)
	{
		m_sStones[pos] = 0;
		//从大到小 排序  //这样  就 免于  几次 循环 挨个 赋值来改变 值  达到 重新  排好位置的 效果
	//	std::sort(m_sStones.begin(), m_sStones.end(), greater<int>());
	}

	//是否镶嵌满了
	bool IsAddStoneFull()
	{
		uint count = 0;
		for (uint i = 0; i < m_sStones.size(); ++i)
		{
			if (0 != m_sStones[i])
			{
				++count;
			}
		}

		return Com_FillStone_HoleNum == count;
	}

	//这个 孔是否  有 宝石
	int IsHaveStone(uint pos)
	{
		if (0 < m_sStones[pos])
		{
			return m_sStones[pos];
		}

		return 0;
	}

	void SetEndTime(int64 dwTime)
	{
		m_dwEndTime = dwTime;
	}
	const int64 GetEndTime() const
	{
		return m_dwEndTime;
	}

	virtual void SetServerInfo(const PlayerInfo::ItemInfo *info)
	{
		if(m_id < 0)
		{
			m_id = -1;
		}

		m_count = info->flag() & 0xff;
		m_cUpgrade = (info->flag() >> 8) & 0xff;
		SetBind(info->flag() >> 16);

		m_attrBaptizeList.resize(info->newattr_size());
		if (info->newattr_size() == info->refineattrlv_size())
		{
			for (int i = 0; i < info->newattr_size(); ++i)
			{
					ReGetAttr attr;
					attr.type = info->newattr(i) >> 14;
					attr.value = info->newattr(i) & 0x3fff;
					attr.level = info->refineattrlv(i);
					m_attrBaptizeList[i] = attr;
			}
		}

		for (int j = 0; j < info->stoneid_size(); ++j)
		{
			int pos = info->inlaypos(j);
			m_sStones[pos] = info->stoneid(j);
		}
	}

	virtual void SetClientInfo(CharLogin::ClientItemInfo *info)
	{
		info->set_id(m_id);
		if(m_id <= 0)
		{
			info->set_id(-1);
		}

		info->set_flag(((m_flag & ePropBind) << 16) | (m_cUpgrade << 8) | (m_count & 0xff));

		for(uint i = 0; i < m_attrBaptizeList.size(); ++i)
		{
				info->add_newattr((m_attrBaptizeList[i].type << 14) | (m_attrBaptizeList[i].value & 0x3fff));
				info->add_refineattrlv(m_attrBaptizeList[i].level);
		}

		for (uint i = 0; i < m_sStones.size(); ++i)
		{
			if (m_sStones[i] > 0)
			{
				info->add_inlaypos(i);
				info->add_stoneid(m_sStones[i]);
			}
		}
	}

	virtual void SetInfo(PlayerInfo::ItemInfo *info)
	{
		info->set_id(m_id);
		if(m_id < 0)
		{
			info->set_id(-1);
			m_id = -1;
		}

		info->set_flag(((m_flag & ePropBind) << 16) | (m_cUpgrade << 8) | (m_count & 0xff));
		for(uint i=0; i<m_attrBaptizeList.size(); ++i)
		{
				info->add_newattr((m_attrBaptizeList[i].type << 14) | (m_attrBaptizeList[i].value & 0x3fff));
				info->add_refineattrlv(m_attrBaptizeList[i].level);
		}

		info->set_endtime(m_dwEndTime);

		for (uint i = 0; i < m_sStones.size(); ++i)
		{
			info->add_inlaypos(i);
			info->add_stoneid(m_sStones[i]);
		}
	}

	virtual void CreateItem(GoodsInfo &info)
	{
		if(eGetBind == info.bindTyle)
		{
			SetBind();
		}
	}

	virtual bool IsSame(int id, int flag)
	{
		return false;
	}



	attributeBonus GetAttrBonus()
	{
		attributeBonus att;
		for(uint i = 0 ; i < m_attrBaptizeList.size() ; i++)
		{
			switch(m_attrBaptizeList[i].type)
			{
			case eCharHP:
					{
						att.HP += m_attrBaptizeList[i].value;
						break;
					}
				case eCharPhyAttackMax:
					{
						att.phyAttack += m_attrBaptizeList[i].value;
						break;
					}
				case eCharPhyDefence:
					{
						att.phyDef += m_attrBaptizeList[i].value;
						break;
					}
				case eCharHitRate:
					{
					att.hitRate += m_attrBaptizeList[i].value;
						break;
					}
				case eCharAvoidRate:
					{
						att.avoidRate += m_attrBaptizeList[i].value;
						break;
					}
				case eCrackDownRate:
					{
						att.downRate += m_attrBaptizeList[i].value;
						break;
					}
				case eAvoidCrackRate:
					{
						att.crackRate += m_attrBaptizeList[i].value;
						break;
					}
				default:
					{
						break;
					}
			}
		}
		return att;
	}
//


	int GetUpgrade()
	{
		return m_cUpgrade ;
	}

	void SetUpgrade(int grade)
	{
		m_cUpgrade = grade;
	}

	void AddUpgrade()
	{
		if(m_cUpgrade  >= Strength_Max_Lv)
		{
			return;
		}

		m_cUpgrade = m_cUpgrade + 1;
	}

	void DescribeUpgrade()
	{
		if(m_cUpgrade == 0)
		{
			return;
		}
		m_cUpgrade = m_cUpgrade - 1;
	}

//	vector<int>  GetStones() {		return m_sStones;	}

	vector<int>  GetStones() {		return m_sStones;	}

	void HaveStone_FunPos(const int &need_hole,vector<int> &vec)
	{
		if(need_hole >(int)m_sStones.size())
			return ;


		for (uint i = 0; i < m_sStones.size(); ++i)
		{
			if (m_sStones[i] > 0)
			{
				vec.push_back(m_sStones[i]);
			}

			if (need_hole <= (int)vec.size())
			{
				return;
			}
		}

	}

	void SetStones(const vector<int> sStones)
	{
		m_sStones = sStones;
	}

	int GetLegendAttrCount(void)
	{
		return m_nLegendAttrCount;
	}

	void SetLegendAttrCount(int count)
	{
		if(count < 0)
			return;
		m_nLegendAttrCount = count;
	}

	void IncreaseLegendAttrCount(void)
	{
		m_nLegendAttrCount++;
	}
	// －－－－－－－－－－－附加属性已不用    目前用洗练属性 －－－－－－－－－－－－－－－－－－－
		void GetAttrBaptizeList(vector<ReGetAttr>& attrBaptizeList)
		{
			attrBaptizeList.resize(m_attrBaptizeList.size());
			for (uint i = 0; i < attrBaptizeList.size(); ++i)
			{
				attrBaptizeList[i] = m_attrBaptizeList[i];
			}
		}

		void SetAttrBaptizeList(vector<ReGetAttr>& attrBaptizeList)
		{
			m_attrBaptizeList.resize(attrBaptizeList.size());
			for (uint i = 0; i < m_attrBaptizeList.size(); ++i)
			{
				m_attrBaptizeList[i] = attrBaptizeList[i];
			}
		}

		bool AffirmRefine(vector<int> lockPos)
		{
			if (0 >= m_tempAttr.size())
				return false;

			vector<ReGetAttr> destAttr; // 目标属性
			map<int, ReGetAttr> destFilterAttr; // 目标属性过滤
			map<int, ReGetAttr> srcFilterAttr = m_tempAttr; // 源属性过滤
			destAttr.resize(m_tempAttr.size());

			for (uint destIndex = 0; destIndex < m_attrBaptizeList.size(); ++destIndex)
			{
				bool flag = false; // 没锁定
				for (uint j = 0; j < lockPos.size(); ++j)
				{
					if (lockPos[j] == (int)destIndex)
					{	// 有锁定
						flag = true;
						break;
					}
				}

				map<int, ReGetAttr>::iterator it = srcFilterAttr.begin();
				for ( ; it != srcFilterAttr.end(); ++it)
				{
					if (it->second.type == m_attrBaptizeList[destIndex].type)
					{	// 有相同类型
						break;
					}
				}

				if (!flag)
				{ // 无锁定
					if (it == srcFilterAttr.end())
					{	// 没有相同类型 目标属性过滤无相同类型与无锁定类型
						destFilterAttr[destIndex] = m_attrBaptizeList[destIndex];
					}
					else
					{	// 有相同内新属性,赋值给目标属性
						destAttr[destIndex] = it->second;
					}
				}
				else
				{
					destAttr[destIndex] = m_attrBaptizeList[destIndex];
				}

				if (it != srcFilterAttr.end())
				{ // 源属性过滤掉有相同类型 相同类型属性已经赋值给目标属性
					srcFilterAttr.erase(it);
				}
			}

			map<int, ReGetAttr>::iterator it = srcFilterAttr.begin();
			for( ; it != srcFilterAttr.end(); ++it)
			{
				destAttr[it->first] = it->second;
			}

			for (map<int, ReGetAttr>::iterator itr = destFilterAttr.begin(); itr != destFilterAttr.end(); ++itr)
			{
				destAttr[itr->first] = itr->second;
			}

			SetAttrBaptizeList(destAttr);
			map<int, ReGetAttr> temp;
			SetTempAttr(temp);

			return true;
		}

		void SetTempAttr(map<int, ReGetAttr>& tempAttr)
		{
			m_tempAttr.clear();
			for (map<int, ReGetAttr>::iterator it = tempAttr.begin(); it != tempAttr.end(); ++it)
			{
				m_tempAttr[it->first] = it->second;
			}
		}

		void GetTempAttr(map<int, ReGetAttr>& tempAttr)
		{
			tempAttr.clear();

			for (map<int, ReGetAttr>::iterator it = m_tempAttr.begin(); it != m_tempAttr.end(); ++it)
			{
				tempAttr[it->first] = it->second;
			}
		}

		bool IsHaveRefine()
		{
			return m_tempAttr.size()?true:false;
		}

		const vector<ReGetAttr>& GetAttrBaptizeList(){return m_attrBaptizeList;}

		void Relasses()
		{
			ClearStone();
			m_attrBaptizeList.clear();
			m_cUpgrade = 0;
		}

protected:

	BYTE m_cUpgrade; // 升级数

	vector<int> m_sStones;  // 宝石链 key: (pair<位置, 镶嵌类型>)  value: 镶嵌宝石的ID


	WORD m_sBattleValue;	// 战斗力


	vector<ReGetAttr> m_attrBaptizeList; // 洗练属性

	int64	m_dwEndTime;		// 结束时间


	map<int, ReGetAttr> m_tempAttr; // 临时洗练属性

	int m_nLegendAttrCount;
};



#endif /* EQUIPDATA_H_ */
