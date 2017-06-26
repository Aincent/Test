/*
 * MeridianLoader.h
 *
 *  Created on: 2014年6月5日
 *      Author: root
 */

#ifndef MERIDIANLOADER_H_
#define MERIDIANLOADER_H_

#include <map>
#include "CSVLoader.h"
#include "CharDefine.h"

using namespace CommBaseOut;
using namespace std;

typedef struct sMeridianStruct
{
	//获得经验
	int i_exp;
	//附近玩家获得经验
	int i_senexp;
	//消耗真气
	int i_uping;
	//消耗铜钱
	int i_upmoney;
	// 消耗元宝
	int i_upgolden;
	//打通几率
	int i_pty;

	//类型
	int i_type;

	map<ECharProf,map<ECharAttrType,int64> > m_AttList;

	int m_LevelLimit;

	sMeridianStruct():i_exp(0),i_senexp(0),i_uping(0),i_upmoney(0), i_upgolden(0)
	,i_pty(0),i_type(0),m_LevelLimit(0)
	{
	}
}MeridianStruct;

typedef struct sMeridianBase
{
	// 特殊冲穴次数
	int m_throughAcpCnt;
	// 特殊打通穴位折扣
	int m_throughAcpDisc;
}MeridiansBaseInfo;

enum ePoentType
{
	ePtMin = 1,
	ePtBig = 2,
};

class MeridianLoader
{
public:
	~MeridianLoader();

	static MeridianLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new MeridianLoader();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	int Init(string & path);

	int InitMeridianLoader(string & file);

	int InitMeridianBase(string& file);

	MeridianStruct * GetMeridianStruct(int id)
	{
		map<int , MeridianStruct>::iterator it = m_meridianStruct.find(id);
		if(it == m_meridianStruct.end())
		{
			return 0;
		}

		return &it->second;
	}

	int GetMaxMeridian()
	{
		return m_meridianStruct.size();
	}

	// 获取特殊冲穴次数
	int GetThroughAcpCnt();

	// 获取特殊打通穴位折扣
	int GetThroughAcpDisc();

	const map<int, MeridianStruct>& getFileData(){return m_meridianStruct;}

private:
	MeridianLoader();

private:
	static MeridianLoader * m_instance;
	map<int, MeridianStruct> m_meridianStruct;
	MeridiansBaseInfo m_baseInfo;
};

#endif /* MERIDIANLOADER_H_ */
